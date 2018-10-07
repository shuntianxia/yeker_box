/***************************************
*	DOBLY AC3 
*	
*
*	date:  2008/10/10  author: lszhang
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"
/* AAC file format */
enum {
	AAC_FF_Unknown = 0,		/* should be 0 on init */

	AAC_FF_ADTS = 1,
	AAC_FF_ADIF = 2,
	AAC_FF_RAW  = 3,
	AAC_FF_LATM = 4

};
static const int sampRateTab[12] = {
    96000, 88200, 64000, 48000, 44100, 32000, 
	24000, 22050, 16000, 12000, 11025,  8000
};


#define AACINFOLENGTH 15
static const char AACcodecInfo[6][AACINFOLENGTH]=
{
	"AAC Unknown",
	"AAC ADTS",
	"AAC ADIF",
	"AAC RAW",
	"AAC LATM",
	"AAC Unspport"

};
	
/* channel mapping (table 1.6.3.4) (-1 = unknown, so need to determine mapping based on rules in 8.5.1) */
static const int channelMapTab[8] = {
	-1, 1, 2, 3, 4, 5, 6, 8
};
//static unsigned char readBuf[2*768*2*2];

#define	SYNCWORDH			0xff
#define	SYNCWORDL			0xf0

#define	SYNCWORDL_LATM		0xE0
#define	SYNCWORDL_H			0x56

#define ADIF_COPYID_SIZE    10
#define CHAN_ELEM_IS_CPE(x)		(((x) & 0x10) >> 4)  /* bit 4 = SCE/CPE flag */
typedef struct _ADTSHeader {
	/* fixed */
	unsigned char id;                             /* MPEG bit - should be 1 */
	unsigned char layer;                          /* MPEG layer - should be 0 */
	unsigned char protectBit;                     /* 0 = CRC word follows, 1 = no CRC word */
	unsigned char profile;                        /* 0 = main, 1 = LC, 2 = SSR, 3 = reserved */
	unsigned char sampRateIdx;                    /* sample rate index range = [0, 11] */
	unsigned char privateBit;                     /* ignore */
	unsigned char channelConfig;                  /* 0 = implicit, >0 = use default table */
	unsigned char origCopy;                       /* 0 = copy, 1 = original */
	unsigned char home;                           /* ignore */

	/* variable */
	unsigned char copyBit;                        /* 1 bit of the 72-bit copyright ID (transmitted as 1 bit per frame) */
	unsigned char copyStart;                      /* 1 = this bit starts the 72-bit ID, 0 = it does not */
	int           frameLength;                    /* length of frame */
	int           bufferFull;                     /* number of 32-bit words left in enc buffer, 0x7FF = VBR */
	unsigned char numRawDataBlocks;               /* number of raw data blocks in frame */

	/* CRC */
	int           crcCheckWord;                   /* 16-bit CRC check word (present if protectBit == 0) */
} ADTSHeader;

typedef struct _ADIFHeader {
	unsigned char copyBit;                        /* 0 = no copyright ID, 1 = 72-bit copyright ID follows immediately */
	unsigned char origCopy;                       /* 0 = copy, 1 = original */
	unsigned char home;                           /* ignore */
	unsigned char bsType;                         /* bitstream type: 0 = CBR, 1 = VBR */
	int           bitRate;                        /* bitRate: CBR = bits/sec, VBR = peak bits/frame, 0 = unknown */
	unsigned char numPCE;                         /* number of program config elements (max = 16) */
	int           bufferFull;                     /* bits left in bit reservoir */
	unsigned char copyID[ADIF_COPYID_SIZE];	      /* optional 72-bit copyright ID */
} ADIFHeader;
#define IS_ADIF(p)		((p)[0] == 'A' && (p)[1] == 'D' && (p)[2] == 'I' && (p)[3] == 'F')
//int	AC3_FRAME(__audio_file_info_t *AIF,int filelen);

typedef struct _BitStreamInfo {
	unsigned char *bytePtr;
	unsigned int iCache;
	int cachedBits;
	int nBytes;
} BitStreamInfo;
#define AAC_NUM_PROFILES	3
#define AAC_PROFILE_MP		0
#define AAC_PROFILE_LC		1
#define AAC_PROFILE_SSR		2
#define ADTS_HEADER_BYTES	7
#define NUM_SAMPLE_RATES	12
#define NUM_DEF_CHAN_MAPS	8
#define NUM_ELEMENTS		8
#define MAX_NUM_PCE_ADIF	16

#define MAX_NUM_FCE			15
#define MAX_NUM_SCE			15
#define MAX_NUM_BCE			15
#define MAX_NUM_LCE			 3
#define MAX_NUM_ADE			 7
#define MAX_NUM_CCE			15
/* sizeof(ProgConfigElement) = 82 bytes (if KEEP_PCE_COMMENTS not defined) */
typedef struct _ProgConfigElement {
	unsigned char elemInstTag;                    /* element instance tag */
	unsigned char profile;                        /* 0 = main, 1 = LC, 2 = SSR, 3 = reserved */
	unsigned char sampRateIdx;                    /* sample rate index range = [0, 11] */
	unsigned char numFCE;                         /* number of front channel elements (max = 15) */
	unsigned char numSCE;                         /* number of side channel elements (max = 15) */
	unsigned char numBCE;                         /* number of back channel elements (max = 15) */
	unsigned char numLCE;                         /* number of LFE channel elements (max = 3) */
	unsigned char numADE;                         /* number of associated data elements (max = 7) */
	unsigned char numCCE;                         /* number of valid channel coupling elements (max = 15) */
	unsigned char monoMixdown;                    /* mono mixdown: bit 4 = present flag, bits 3-0 = element number */
	unsigned char stereoMixdown;                  /* stereo mixdown: bit 4 = present flag, bits 3-0 = element number */
	unsigned char matrixMixdown;                  /* matrix mixdown: bit 4 = present flag, bit 3 = unused, 
	                                                                 bits 2-1 = index, bit 0 = pseudo-surround enable */
	unsigned char fce[MAX_NUM_FCE];               /* front element channel pair: bit 4 = SCE/CPE flag, bits 3-0 = inst tag */
	unsigned char sce[MAX_NUM_SCE];               /* side element channel pair: bit 4 = SCE/CPE flag, bits 3-0 = inst tag */
	unsigned char bce[MAX_NUM_BCE];               /* back element channel pair: bit 4 = SCE/CPE flag, bits 3-0 = inst tag */
	unsigned char lce[MAX_NUM_LCE];               /* instance tag for LFE elements */
	unsigned char ade[MAX_NUM_ADE];               /* instance tag for ADE elements */
	unsigned char cce[MAX_NUM_BCE];               /* channel coupling elements: bit 4 = switching flag, bits 3-0 = inst tag */

#ifdef KEEP_PCE_COMMENTS
	/* make this optional - if not enabled, decoder will just skip comments */
	unsigned char commentBytes;
	unsigned char commentField[MAX_COMMENT_BYTES];
#endif

} ProgConfigElement;

/**************************************************************************************
 * Function:    SetBitstreamPointer
 *
 * Description: initialize bitstream reader
 *
 * Inputs:      pointer to BitStreamInfo struct
 *              number of bytes in bitstream
 *              pointer to byte-aligned buffer of data to read from
 *
 * Outputs:     initialized bitstream info struct
 *
 * Return:      none
 **************************************************************************************/
static void SetBitstreamPointer(BitStreamInfo *bsi, int nBytes, unsigned char *buf)
{
	/* init bitstream */
	bsi->bytePtr = buf;
	bsi->iCache = 0;		/* 4-byte unsigned int */
	bsi->cachedBits = 0;	/* i.e. zero bits in cache */
	bsi->nBytes = nBytes;
}

/**************************************************************************************
 * Function:    RefillBitstreamCache
 *
 * Description: read new data from bitstream buffer into 32-bit cache
 *
 * Inputs:      pointer to initialized BitStreamInfo struct
 *
 * Outputs:     updated bitstream info struct
 *
 * Return:      none
 *
 * Notes:       only call when iCache is completely drained (resets bitOffset to 0)
 *              always loads 4 new bytes except when bsi->nBytes < 4 (end of buffer)
 *              stores data as big-endian in cache, regardless of machine endian-ness
 **************************************************************************************/
static __inline void RefillBitstreamCache(BitStreamInfo *bsi)
{
	int nBytes = bsi->nBytes;

	/* optimize for common case, independent of machine endian-ness */
	if (nBytes >= 4) {
		bsi->iCache  = (*bsi->bytePtr++) << 24;
		bsi->iCache |= (*bsi->bytePtr++) << 16;
		bsi->iCache |= (*bsi->bytePtr++) <<  8;
		bsi->iCache |= (*bsi->bytePtr++);
		bsi->cachedBits = 32;
		bsi->nBytes -= 4;
	} else {
		bsi->iCache = 0;
		while (nBytes--) {
			bsi->iCache |= (*bsi->bytePtr++);
			bsi->iCache <<= 8;
		}
		bsi->iCache <<= ((3 - bsi->nBytes)*8);
		bsi->cachedBits = 8*bsi->nBytes;
		bsi->nBytes = 0;
	}
}
/**************************************************************************************
 * Function:    GetBits
 *
 * Description: get bits from bitstream, advance bitstream pointer
 *
 * Inputs:      pointer to initialized BitStreamInfo struct
 *              number of bits to get from bitstream
 *
 * Outputs:     updated bitstream info struct
 *
 * Return:      the next nBits bits of data from bitstream buffer
 *
 * Notes:       nBits must be in range [0, 31], nBits outside this range masked by 0x1f
 *              for speed, does not indicate error if you overrun bit buffer 
 *              if nBits == 0, returns 0
 **************************************************************************************/
static unsigned int GetBits(BitStreamInfo *bsi, int nBits)
{
	unsigned int data, lowBits;

	nBits &= 0x1f;							/* nBits mod 32 to avoid unpredictable results like >> by negative amount */
	data = bsi->iCache >> (31 - nBits);		/* unsigned >> so zero-extend */
	data >>= 1;								/* do as >> 31, >> 1 so that nBits = 0 works okay (returns 0) */
	bsi->iCache <<= nBits;					/* left-justify cache */
	bsi->cachedBits -= nBits;				/* how many bits have we drawn from the cache so far */

	/* if we cross an int boundary, refill the cache */
	if (bsi->cachedBits < 0) {
		lowBits = -bsi->cachedBits;
		RefillBitstreamCache(bsi);
		data |= bsi->iCache >> (32 - lowBits);		/* get the low-order bits */
	
		bsi->cachedBits -= lowBits;			/* how many bits have we drawn from the cache so far */
		bsi->iCache <<= lowBits;			/* left-justify cache */
	}

	return data;
}
/**************************************************************************************
 * Function:    AACFindSyncWord
 *
 * Description: locate the next byte-alinged sync word in the raw AAC stream
 *
 * Inputs:      buffer to search for sync word
 *              max number of bytes to search in buffer
 *
 * Outputs:     none
 *
 * Return:      offset to first sync word (bytes from start of buf)
 *              -1 if sync not found after searching nBytes
 **************************************************************************************/

static int AACFindSyncWord(__audio_file_info_t *AIF)
{
	int i=0;
  BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	/* find byte-aligned syncword (12 bits = 0xFFF) */
	for(i=0;i<0x1000;i++)
	{
		//if ( (BSINFO->bsbufptr[0] & SYNCWORDH) == SYNCWORDH && (BSINFO->bsbufptr[1] & SYNCWORDL) == SYNCWORDL )
        if((GetInfo_ShowBs(0,2,AIF)&0xfff0)==0xfff0)		
		return i;		
		//if(BSINFO->bsbufLen<0)return i-2;
		if(GetInfo_return<2) return -1;
		GetInfo_GetBsInByte(1,AIF);
		

	}
	
	
	return -1;
}
static int AACFindSyncWord_LATM(__audio_file_info_t *AIF)
{
	int i=0;
  BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	/* find byte-aligned syncword (12 bits = 0xFFF) */
    for(i=0;i<0x1000;i++)
	{
		//if ( (BSINFO->bsbufptr[0] & SYNCWORDH) == SYNCWORDH && (BSINFO->bsbufptr[1] & SYNCWORDL) == SYNCWORDL )
        if((GetInfo_ShowBs(0,2,AIF)&0xffE0)==0x56E0)		
		return i;		
		//if(BSINFO->bsbufLen<0)return i-2;
		if(GetInfo_return<2) return -1;
		GetInfo_GetBsInByte(1,AIF);
		

	}
	
	
	return -1;
}


static int GetNextFrame(__audio_file_info_t *AIF,int *readlength,INT64 *filelength)
{
	   int nBytes;
	   int retVal=-1;
	   //int ret=-1;
	   ADTSHeader            fhADTS;
	   BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
			nBytes = AACFindSyncWord(AIF);
			if (nBytes < 0)
				return -1;
		/*
		readPtr +=nBytes+7;
		bytesLeft -=nBytes+7;
		AACBKU.fileread +=nBytes+7;
		*/
		 *readlength += nBytes;
		fhADTS.frameLength = ((int)(GetInfo_ShowBs(3,1,AIF)&0x3)<<11)|((int)(GetInfo_ShowBs(4,1,AIF)&0xff)<<3)|((GetInfo_ShowBs(5,1,AIF)>>5)&0x07);
		GetInfo_SkipBsInByte(fhADTS.frameLength,AIF);
		*readlength += fhADTS.frameLength;
		if(*readlength >= *filelength)
			{
				*readlength =*filelength;
				return -1;
			}
		if(BSINFO->bsbufLen<10)GetInfo_GetBsInByte(0,AIF);
		while(retVal<0)
		{
			retVal = 0;
			
			nBytes = AACFindSyncWord(AIF);
			if (nBytes < 0)
				return -1;
			*readlength += nBytes;
			if(*readlength >= *filelength)
			{
				*readlength = *filelength;
				return -1;
			}
			fhADTS.layer =            (GetInfo_ShowBs(1,1,AIF)>>1)&0x03;//GetBits(&bsi, 2);
			//fhADTS.protectBit =       GetBits(&bsi, 1);
			fhADTS.profile =          (GetInfo_ShowBs(2,1,AIF)>>6)&0x03;//GetBits(&bsi, 2);
			fhADTS.sampRateIdx =      (GetInfo_ShowBs(2,1,AIF)>>2)&0x0f;//GetBits(&bsi, 4);
			//fhADTS.privateBit =       GetBits(&bsi, 1);
			fhADTS.channelConfig =    ((GetInfo_ShowBs(2,1,AIF)<<2)&0x04)|((GetInfo_ShowBs(3,1,AIF)>>6)&0x03);//GetBits(&bsi, 3);
			if(GetInfo_return<1)
			return -1;
			/* check validity of header */
	   		 if (fhADTS.layer != 0 /*|| fhADTS.profile != 1 */||
				fhADTS.sampRateIdx >= 12 || fhADTS.channelConfig >= 8)
			{
				GetInfo_GetBsInByte(1,AIF);
				*readlength +=1;
				retVal = -1;
  	     	 }
			
		}
	

		return 0;
		
		
}
static int GetNextFrame_LATM(__audio_file_info_t *AIF,int *readlength,INT64 *filelength)
{
	   int nBytes;
	   //int retVal=-1;
	   //int ret=-1;
	   ADTSHeader            fhADTS;
	   BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
			nBytes = AACFindSyncWord_LATM(AIF);
			if (nBytes < 0)
				return -1;
		/*
		readPtr +=nBytes+7;
		bytesLeft -=nBytes+7;
		AACBKU.fileread +=nBytes+7;
		*/
		 *readlength += nBytes;
		fhADTS.frameLength  = ((GetInfo_ShowBs(1,1,AIF)&0x1f)<<8)|(GetInfo_ShowBs(2,1,AIF)&0xff);
		if(GetInfo_return<1)
			return -1;
		GetInfo_SkipBsInByte(fhADTS.frameLength,AIF);
		*readlength += fhADTS.frameLength;
		if(*readlength >= *filelength)
			{
				*readlength =*filelength;
				return -1;
			}
		if(BSINFO->bsbufLen<10)GetInfo_GetBsInByte(0,AIF);
		
			
			nBytes = AACFindSyncWord_LATM(AIF);
			if (nBytes < 0)
				return -1;
			*readlength += nBytes;
			if(*readlength >= *filelength)
			{
				*readlength = *filelength;
				return -1;
			}


		return 0;
		
		
}

/**************************************************************************************
 * Function:    DecodeProgramConfigElement
 *
 * Description: decode one PCE
 *
 * Inputs:      BitStreamInfo struct pointing to start of PCE (14496-3, table 4.4.2) 
 *
 * Outputs:     filled-in ProgConfigElement struct
 *              updated BitStreamInfo struct
 *
 * Return:      0 if successful, error code (< 0) if error
 *
 * Notes:       #define KEEP_PCE_COMMENTS to save the comment field of the PCE
 *                (otherwise we just skip it in the bitstream, to save memory)
 **************************************************************************************/
static int DecodeProgramConfigElement(ProgConfigElement *pce, BitStreamInfo *bsi)
{
	int i;

	pce->elemInstTag =   GetBits(bsi, 4);
	pce->profile =       GetBits(bsi, 2);
	pce->sampRateIdx =   GetBits(bsi, 4);
	pce->numFCE =        GetBits(bsi, 4);
	pce->numSCE =        GetBits(bsi, 4);
	pce->numBCE =        GetBits(bsi, 4);
	pce->numLCE =        GetBits(bsi, 2);
	pce->numADE =        GetBits(bsi, 3);
	pce->numCCE =        GetBits(bsi, 4);

	pce->monoMixdown = GetBits(bsi, 1) << 4;	/* present flag */
	if (pce->monoMixdown)
		pce->monoMixdown |= GetBits(bsi, 4);	/* element number */

	pce->stereoMixdown = GetBits(bsi, 1) << 4;	/* present flag */
	if (pce->stereoMixdown)
		pce->stereoMixdown  |= GetBits(bsi, 4);	/* element number */

	pce->matrixMixdown = GetBits(bsi, 1) << 4;	/* present flag */
	if (pce->matrixMixdown) {
		pce->matrixMixdown  |= GetBits(bsi, 2) << 1;	/* index */
		pce->matrixMixdown  |= GetBits(bsi, 1);			/* pseudo-surround enable */
	}

	for (i = 0; i < pce->numFCE; i++) {
		pce->fce[i]  = GetBits(bsi, 1) << 4;	/* is_cpe flag */
		pce->fce[i] |= GetBits(bsi, 4);			/* tag select */
	}

	for (i = 0; i < pce->numSCE; i++) {
		pce->sce[i]  = GetBits(bsi, 1) << 4;	/* is_cpe flag */
		pce->sce[i] |= GetBits(bsi, 4);			/* tag select */
	}

	for (i = 0; i < pce->numBCE; i++) {
		pce->bce[i]  = GetBits(bsi, 1) << 4;	/* is_cpe flag */
		pce->bce[i] |= GetBits(bsi, 4);			/* tag select */
	}

	for (i = 0; i < pce->numLCE; i++)
		pce->lce[i] = GetBits(bsi, 4);			/* tag select */

	for (i = 0; i < pce->numADE; i++)
		pce->ade[i] = GetBits(bsi, 4);			/* tag select */

	for (i = 0; i < pce->numCCE; i++) {
		pce->cce[i]  = GetBits(bsi, 1) << 4;	/* independent/dependent flag */
		pce->cce[i] |= GetBits(bsi, 4);			/* tag select */
	}

    i = bsi->cachedBits&0x07;
	if(i!=0)GetBits(bsi, i);
	//ByteAlignBitstream(bsi);

#ifdef KEEP_PCE_COMMENTS
	pce->commentBytes = GetBits(bsi, 8);
	for (i = 0; i < pce->commentBytes; i++)
		pce->commentField[i] = GetBits(bsi, 8);
#else
	/* eat comment bytes and throw away */
	i = GetBits(bsi, 8);
	while (i--)
		GetBits(bsi, 8);
#endif

	return 0;
}
/**************************************************************************************
 * Function:    GetNumChannelsADIF
 *
 * Description: get number of channels from program config elements in an ADIF file
 *
 * Inputs:      array of filled-in program config element structures
 *              number of PCE's
 *
 * Outputs:     none
 *
 * Return:      total number of channels in file
 *              -1 if error (invalid number of PCE's or unsupported mode)
 **************************************************************************************/
static int GetNumChannelsADIF(ProgConfigElement *fhPCE, int nPCE)
{
	int i, j, nChans;

	if (nPCE < 1 || nPCE > 16)
		return -1;

	nChans = 0;
	for (i = 0; i < nPCE; i++) {
		/* for now: only support LC, no channel coupling */
		if (/*fhPCE[i].profile != AAC_PROFILE_LC ||*/ fhPCE[i].numCCE > 0)
			return -1;

		/* add up number of channels in all channel elements (assume all single-channel) */
        nChans += fhPCE[i].numFCE;
        nChans += fhPCE[i].numSCE;
        nChans += fhPCE[i].numBCE;
        nChans += fhPCE[i].numLCE;

		/* add one more for every element which is a channel pair */
        for (j = 0; j < fhPCE[i].numFCE; j++) {
            if (CHAN_ELEM_IS_CPE(fhPCE[i].fce[j]))
                nChans++;
        }
        for (j = 0; j < fhPCE[i].numSCE; j++) {
            if (CHAN_ELEM_IS_CPE(fhPCE[i].sce[j]))
                nChans++;
        }
        for (j = 0; j < fhPCE[i].numBCE; j++) {
            if (CHAN_ELEM_IS_CPE(fhPCE[i].bce[j]))
                nChans++;
        }

	}

	return nChans;
}

/**************************************************************************************
 * Function:    GetSampleRateIdxADIF
 *
 * Description: get sampling rate index from program config elements in an ADIF file
 *
 * Inputs:      array of filled-in program config element structures
 *              number of PCE's
 *
 * Outputs:     none
 *
 * Return:      sample rate of file
 *              -1 if error (invalid number of PCE's or sample rate mismatch)
 **************************************************************************************/
static int GetSampleRateIdxADIF(ProgConfigElement *fhPCE, int nPCE)
{
	int i, idx;

	if (nPCE < 1 || nPCE > MAX_NUM_PCE_ADIF)
		return -1;

	/* make sure all PCE's have the same sample rate */
	idx = fhPCE[0].sampRateIdx;
	for (i = 1; i < nPCE; i++) {
		if (fhPCE[i].sampRateIdx != idx)
			return -1;
	}

	return idx;
}


static int UnpackADIFHeader(__audio_file_info_t *AIF,INT64 *filelength)
{
	int ret;
	int i;
	int ulChannels,ulSampleRate;
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	ADIFHeader fhADIF;
	BitStreamInfo bsi;
	ProgConfigElement pce[16];
	SetBitstreamPointer(&bsi, BSINFO->bsbufLen-4, (unsigned char*)(BSINFO->bsbufptr+4));
	if(GetBits(&bsi, 1))
		{
			for (i = 0; i < 9; i++)
			fhADIF.copyID[i] = GetBits(&bsi, 8);
		}
	fhADIF.origCopy = GetBits(&bsi, 1);
	fhADIF.home =     GetBits(&bsi, 1);
	fhADIF.bsType =   GetBits(&bsi, 1);
	fhADIF.bitRate =  GetBits(&bsi, 23);
	fhADIF.numPCE =   GetBits(&bsi, 4) + 1;	/* add 1 (so range = [1, 16]) */
	if (fhADIF.bsType == 0)
		fhADIF.bufferFull = GetBits(&bsi, 20);
	/* parse all program config elements */
	for (i = 0; i < fhADIF.numPCE; i++)
		DecodeProgramConfigElement(pce + i, &bsi);

	/* byte align */
	//ByteAlignBitstream(&bsi);
	ret = bsi.cachedBits&0x07;
	if(ret!=0)GetBits(&bsi, ret);

	/* update codec info */
//	AIF->ulChannels = GetNumChannelsADIF(pce, fhADIF.numPCE);
//	AIF->ulSampleRate = GetSampleRateIdxADIF(pce, fhADIF.numPCE);
	ulChannels = GetNumChannelsADIF(pce, fhADIF.numPCE);
	ulSampleRate = GetSampleRateIdxADIF(pce, fhADIF.numPCE);

	/* check validity of header */
	//if (AIF->ulChannels < 0 || AIF->ulSampleRate < 0 || AIF->ulSampleRate >= 12)
	if (ulChannels < 0 || ulSampleRate < 0 || ulSampleRate >= 12)
		return ERROR;
	AIF->ulChannels  = ulChannels;
	AIF->ulSampleRate = sampRateTab[ulSampleRate];
	if(fhADIF.bitRate!=0)
	{
		AIF->ulBitRate =fhADIF.bitRate;
		AIF->ulDuration = (int)(((double)(*filelength) * (double)(8))*1000 /((double)AIF->ulBitRate) ); 
    
	}
	return SUCCESS;
}
/**************************************************************************************
 * Function:    GetInfo_AAC
 *
 * Description: get AAC file infomation channels,samprate,and so on
 *
 * Inputs:		FILE *bs
 *					 AAC file pointer
 *			   __audio_file_info_t *AIF
 *					__audio_file_info_t struct pointer
 *
 * Outputs:     none
 *
 * Return:      0 success 
 *              -1 error file
 **************************************************************************************/
//#define TRYALL
#define	AuInfTime  60
int GetInfo_AAC(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	int ret;
	
	int ID3Len;
	int readlength=0;
    __int64 filelength;
	//unsigned char outPtr;
	int i;
	int retVal;
	int offset = 0;
	int frameOn = 0;
	int frameOn_copy =0;
	int readlength_copy = 0;
    ADTSHeader            fhADTS;	
	int nBytes;
	//int audioObjectType;
	BsInfo      *BSINFO;
	int AAC_format = AAC_FF_Unknown;
	
	//BitStreamInfo bsi;
	 filelength=GetInfo_BsInit(bs,AIF);
	 if(filelength<=0) return ERROR;
	BSINFO = (BsInfo *)AIF->ulBSINFO;

    AIF->ulBitsSample      = 16;
	ID3Len=GetInfo_GETID3_V2(AIF);//if it have id3.
	readlength = ID3Len;
	if(BSINFO->bsbufLen<100)GetInfo_GetBsInByte(0,AIF);
	/* make local copies (see "Notes" above) */
	if (IS_ADIF(BSINFO->bsbufptr)) {
			/* unpack ADIF header */
			AAC_format = AAC_FF_ADIF;
			ret = UnpackADIFHeader(AIF,&filelength);
			if(ret==ERROR)
			goto AAC_error;

		}
	else
	{
		{
			//BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
			char *buf = BSINFO->bsbufptr;
			for (i = 0; i < BSINFO->bsbufLen - 1; i++) 
			{
				if ( (buf[i+0] & SYNCWORDH) == SYNCWORDH && (buf[i+1] & SYNCWORDL) == SYNCWORDL )
				{
					AAC_format = AAC_FF_ADTS;
						break;
				}
				if ( (buf[i+0] & SYNCWORDH) == SYNCWORDL_H && (buf[i+1] & SYNCWORDL_LATM) == SYNCWORDL_LATM )
				{
					AAC_format = AAC_FF_LATM;
						break;
				}

			}
			

		}
		
		retVal = -1;
		if(AAC_format == AAC_FF_ADTS)
		{
			char *buf = BSINFO->bsbufptr;
			for(i=0;(i<ERRORFAMENUM )&&(retVal<0);i++)
			//while(retVal<0)
    		{
    			retVal = 0;
    			
    			nBytes = AACFindSyncWord(AIF);
    			if(nBytes<0)goto AAC_error;
    			readlength += nBytes;
    			if(readlength>=filelength)
    			{
    				readlength =filelength;
    				goto AAC_error;
    			}
    			fhADTS.layer =            (GetInfo_ShowBs(1,1,AIF)>>1)&0x03;//GetBits(&bsi, 2);
    			//fhADTS.protectBit =       GetBits(&bsi, 1);
    			fhADTS.profile =          (GetInfo_ShowBs(2,1,AIF)>>6)&0x03;//GetBits(&bsi, 2);
    			fhADTS.sampRateIdx =      (GetInfo_ShowBs(2,1,AIF)>>2)&0x0f;//GetBits(&bsi, 4);
    			//fhADTS.privateBit =       GetBits(&bsi, 1);
    			fhADTS.channelConfig =    ((GetInfo_ShowBs(2,1,AIF)<<2)&0x04)|((GetInfo_ShowBs(3,1,AIF)>>6)&0x03);//GetBits(&bsi, 3);
    			if(GetInfo_return<1)
					goto AAC_error;
    			/* check validity of header */
				buf = BSINFO->bsbufptr;
    			fhADTS.frameLength = ((int)(buf[3]&0x3)<<11)|((int)(buf[4]&0xff)<<3)|((buf[5]>>5)&0x07);
    			 if (((fhADTS.layer != 0 )&&(fhADTS.layer != 3 ))/*|| fhADTS.profile != 1*/ ||
    				fhADTS.sampRateIdx >= 12 || fhADTS.channelConfig >= 8 ||fhADTS.frameLength>2*1024*2)
    			{
    				GetInfo_GetBsInByte(1,AIF);
    				readlength +=1;
    				retVal = -1;
      	     	 }
	    		else
	    		{
	    			if ( (buf[fhADTS.frameLength+0] & SYNCWORDH) == SYNCWORDH && (buf[fhADTS.frameLength+1] & SYNCWORDL) == SYNCWORDL )
	    			{
	    				retVal = 0;
						break;
	    			}
	    			else
	    			{
	    				GetInfo_GetBsInByte(1,AIF);
	    				readlength +=1;
	    				retVal = -1;
	    				
	    			}
	    			
	    		}
	    		if(readlength-ID3Len>2*1024)
	    		{
	    			goto AAC_error;
	    		}    			    			
    		}
			if(i==ERRORFAMENUM)
			{
				goto AAC_error;
			}
			AIF->ulSampleRate = sampRateTab[fhADTS.sampRateIdx];
	    }
	    else//AAC_format = AAC_FF_LATM
	    {
	        char *buf = BSINFO->bsbufptr;
			for(i=0;(i<ERRORFAMENUM )&&(retVal<0);i++)
			//while(retVal<0)
    		{
    			retVal = 0;
    			
    			nBytes = AACFindSyncWord_LATM(AIF);
    			if(nBytes<0)goto AAC_error;
    			readlength += nBytes;
    			if(readlength>=filelength)
    			{
    				readlength =filelength;
    				goto AAC_error;
    			}
    			/****************************************************************/
    			nBytes = ((GetInfo_ShowBs(1,1,AIF)&0x1f)<<8)|(GetInfo_ShowBs(2,1,AIF)&0xff);
    			fhADTS.frameLength = nBytes+3;
    		    buf = BSINFO->bsbufptr;
				//GetInfo_GetBsInByte(3,AIF);
				if((GetInfo_ShowBs(0,1,AIF)&0xC0)||(fhADTS.frameLength>4096))
				{
					
					retVal = -1;

				}
				else
				{
					if ( (buf[fhADTS.frameLength+0] & SYNCWORDH) == SYNCWORDL_H && (buf[fhADTS.frameLength+1] & SYNCWORDL_LATM) == SYNCWORDL_LATM )
	    			{
	    				retVal = 0;
	    				//useSameStreamMux == 0 && audioMuxVersion==0
						fhADTS.layer =            (GetInfo_ShowBs(1+3,1,AIF))&0x07;//GetBits(&bsi,3); //3 uimsbf
						//audioObjectType = (GetInfo_ShowBs(2,1,AIF)>>3)&0x1F;//GetBits(&bsi,5); //5 bslbf
						fhADTS.sampRateIdx = 2*((GetInfo_ShowBs(2+3,1,AIF))&0x07)+ ((GetInfo_ShowBs(3+3,1,AIF)>>7)&0x01);//GetBits(&bsi,4); //4 bslbf
						if ( fhADTS.sampRateIdx==0xf )
						{
							 //GetBits(&bsi,24); //24 uimsbf
							AIF->ulSampleRate = ((GetInfo_ShowBs(3+3,1,AIF)&0x07F)<<17)
	                                             | (GetInfo_ShowBs(4+3,1,AIF) << 9)
	                                             | (GetInfo_ShowBs(5+3,1,AIF) << 1)
	                                             | ((GetInfo_ShowBs(6+3,1,AIF) >> 7)&0x01);
							fhADTS.channelConfig = ((GetInfo_ShowBs(6+3,1,AIF) >> 3)&0x0F);//GetBits(&bsi,4);
						}
						else
						{
							AIF->ulSampleRate = sampRateTab[fhADTS.sampRateIdx];
							fhADTS.channelConfig = ((GetInfo_ShowBs(3+3,1,AIF) >> 3)&0x0F);
						}
						break;
	    			}
	    			else
	    			{
	    				GetInfo_GetBsInByte(1,AIF);
	    				readlength +=1;
	    				retVal = -1;
	    				
	    			}
					


				}
				if(GetInfo_return<1)
					goto AAC_error;
				//GetInfo_SkipBsInByte(nBytes,AIF);
				//frameOn++;


    			
    			
    			/*******************************************************************/
    			  			    			
    		}
    		if(i==ERRORFAMENUM)
			{
				goto AAC_error;
			}
	        
	    }
	        
			 
		AIF->ulChannels		= channelMapTab[fhADTS.channelConfig];
		//readlength +=offset;
    	offset = readlength;
    	ret =0;
    	if(AAC_format == AAC_FF_ADTS)
    	{
        #ifndef TRYALL
        
        	ret = 10*AIF->ulSampleRate /1024;
        	for(i=0;i<ret;i++)
        	{
        		
        		retVal = GetNextFrame(AIF,&readlength,&filelength);
        		frameOn++;
        		if(retVal==-1)break;
        		
        	}
        	if(i==ret)
        	{
        		readlength_copy =readlength;
        		frameOn_copy=frameOn;
        		ret = AuInfTime*AIF->ulSampleRate /1024;
        		for(i=0;i<ret;i++)
        		{
        			retVal = GetNextFrame(AIF,&readlength,&filelength);
        			frameOn++;
        			if(retVal==-1)break;
        		}
        	}
        	if(i==ret)
        	{
        		readlength =readlength - readlength_copy+offset;
        		frameOn -= frameOn_copy;
        	}
        #else
        	
        	while(ret>=0)
        	{
        		ret = GetNextFrame(AIF,&readlength,&filelength);
        		frameOn++;
        	}
        #endif
        }
        else//LATM
        {
         #ifndef TRYALL
    
        	ret = 10*AIF->ulSampleRate /1024;
        	for(i=0;i<ret;i++)
        	{
        		
        		retVal = GetNextFrame_LATM(AIF,&readlength,&filelength);
        		frameOn++;
        		if(retVal==-1)break;
        		
        	}
        	if(i==ret)
        	{
        		readlength_copy =readlength;
        		frameOn_copy=frameOn;
        		ret = AuInfTime*AIF->ulSampleRate /1024;
        		for(i=0;i<ret;i++)
        		{
        			retVal = GetNextFrame_LATM(AIF,&readlength,&filelength);
        			frameOn++;
        			if(retVal==-1)break;
        		}
        	}
        	if(i==ret)
        	{
        		readlength =readlength - readlength_copy+offset;
        		frameOn -= frameOn_copy;
        	}
        #else
        	
        	while(ret>=0)
        	{
        		ret = GetNextFrame_LATM(AIF,&readlength,&filelength);
        		frameOn++;
        	}
        #endif
        }	
    
    	AIF->ulBitRate			= (int)((double)((double)(readlength-offset)*(double)8*(double)AIF->ulSampleRate)/(double)((double)frameOn*1024));
    	AIF->ulDuration         = (int)((double)((double)(filelength-offset)*8000) /(double)AIF->ulBitRate);//	

			 //AAC_format = AAC_FF_ADTS;

	}
	if(AIF->ulBitRate>AIF->ulSampleRate*AIF->ulChannels*16)
		goto AAC_error;//for aac not bitrate lag
    
	//if(ID3Len==0)GetInfo_GETID3_V1(AIF);
	if((!AIF->ulbufflag)&&(AIF->length<=0))
	{

		fseek(bs, -128, SEEK_END); 
		BSINFO->bsbufLen = 0;
		GetInfo_GETID3_V1(AIF);
	
	}
 
	if(AIF->InforBufLeftLength>=3)
	{
		AIF->ulAudio_name_sz = AACINFOLENGTH;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;	
		memcpy(AIF->ulAudio_name,AACcodecInfo[AAC_format],AIF->ulAudio_name_sz);
		
	}
	if(AAC_format == AAC_FF_Unknown)goto AAC_error;
	
    GetInfo_BsEndRelease(AIF);
    return SUCCESS;
AAC_error:
	GetInfo_BsEndRelease(AIF);
	return ERROR;
}

