/***************************************
*	MPEG LAYI II III audio infor 
*	support Xing /ID3V2 /ID3V1/VBRI tag 
*	
*   2009/9/14  修正 GETTAG_VBRI 输入
*	date:  2008/10/10  author: fufo
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"	

typedef enum {
	MPEG1 =  0,
	MPEG2 =  1,
	MPEG25 = 2
} MPEGVersion;

typedef struct _FrameHeader {
    MPEGVersion ver;	/* version ID */
    int layer;			/* layer index (1, 2, or 3) */
    int crc;			/* CRC flag: 0 = disabled, 1 = enabled */
    int brIdx;			/* bitrate index (0 - 15) */
    int srIdx;			/* sample rate index (0 - 2) */
    int paddingBit;		/* padding flag: 0 = no padding, 1 = single pad byte */
    int privateBit;		/* unused */
    int sMode;			/* mono/stereo mode */
    int CRCWord;		/* CRC word (16 bits, 0 if crc not enabled) */

	int framesize;	
	int samprate;
	int bitrate;
	int framesamplecount;

} mp3FrameHeader;

static const int samplerateTab[3][3] = {
  {44100, 48000, 32000},		/* MPEG-1 */
  {22050, 24000, 16000},		/* MPEG-2 */
  {11025, 12000,  8000},		/* MPEG-2.5 */
};
static const char codecInfo[9][13]=
{
	"MPEG1.0 LAY1",
	"MPEG1.0 LAY2",
	"MPEG1.0 LAY3",
	"MPEG2.0 LAY1",
	"MPEG2.0 LAY2",
	"MPEG2.0 LAY3",
	"MPEG2.5 LAY1",
	"MPEG2.5 LAY2",
	"MPEG2.5 LAY3"
};

/* indexing = [version][layer][bitrate index]
 * bitrate (kbps) of frame
 *   - bitrate index == 0 is "free" mode (bitrate determined on the fly by
 *       counting bits between successive sync words)
 */
static const short bitrateTab[3][3][15] = {
	{
		/* MPEG-1 */
		{  0, 32, 64, 96,128,160,192,224,256,288,320,352,384,416,448}, /* Layer 1 */
		{  0, 32, 48, 56, 64, 80, 96,112,128,160,192,224,256,320,384}, /* Layer 2 */
		{  0, 32, 40, 48, 56, 64, 80, 96,112,128,160,192,224,256,320}, /* Layer 3 */
	},
	{
		/* MPEG-2 */
		{  0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256}, /* Layer 1 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 2 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 3 */
	},
	{
		/* MPEG-2.5 */
		{  0, 32, 48, 56, 64, 80, 96,112,128,144,160,176,192,224,256}, /* Layer 1 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 2 */
		{  0,  8, 16, 24, 32, 40, 48, 56, 64, 80, 96,112,128,144,160}, /* Layer 3 */
	},
};

/* indexing = [version][layer]
 * number of samples in one frame (per channel)
 */
static const short samplesPerFrameTab[3][3] = {
	{384, 1152, 1152 }, /* MPEG1 */
	{384, 1152,  576 }, /* MPEG2 */
	{384, 1152,  576 }, /* MPEG2.5 */
};


static int UnpackFrameHeader(mp3FrameHeader *fh, char *buf);
static int	Mp3HeadSync(__audio_file_info_t *AIF,mp3FrameHeader *frameinfo);

int GetInfo_MP3(FILE *bs,__audio_file_info_t *AIF)
{
	//int	retv;
	int		VBRFrames;
	__int64		filelength=0;
	int		ID3Len = 0;
	__int64 TempData;
	int		NextFrameHead = 0;
	int		CodecIndex=0;
	mp3FrameHeader frameinfo,frameinfo2;
	BsInfo      *BSINFO;
	char     *tempBack;
	int i;
	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<=0) return ERROR;
	BSINFO = (BsInfo *)AIF->ulBSINFO;
	ID3Len=GetInfo_GETID3_V2(AIF);
	for(i=0;i<ERRORFAMENUM;i++)//while(1)
	{
		if(Mp3HeadSync(AIF,&frameinfo)==0) 
			goto MP3_error;
		NextFrameHead=GetInfo_ShowBs(frameinfo.framesize,2,AIF);
		if(GetInfo_return<2)
			goto MP3_error;
		if((NextFrameHead&0xffe0) ==0xffe0)
		{
			if(UnpackFrameHeader(&frameinfo2,BSINFO->bsbufptr + frameinfo.framesize)>0)
			{
				if(frameinfo2.srIdx==frameinfo.srIdx 
				&& frameinfo2.sMode==frameinfo.sMode 
				&& frameinfo2.layer==frameinfo.layer)
				{
					break;
				}
			}
		}
		GetInfo_GetBsInByte(1,AIF);
	}
  if(i==ERRORFAMENUM)
	{
		goto MP3_error;
	}
	AIF->ulSampleRate = frameinfo.samprate;
	AIF->ulBitRate = frameinfo.bitrate;
	AIF->ulChannels = frameinfo.sMode;
	AIF->ulBitsSample =16;
	
	CodecIndex = frameinfo.ver*3+(frameinfo.layer - 1);
	if(AIF->InforBufLeftLength>=13)
	{
		AIF->ulAudio_name_sz = 13;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
		memcpy(AIF->ulAudio_name,codecInfo[CodecIndex],AIF->ulAudio_name_sz);
	
	}
	tempBack = BSINFO->bsbufptr;
	VBRFrames=GetInfo_GETTAG_XING(frameinfo.ver,frameinfo.sMode,BSINFO->bsbufptr);
	if(VBRFrames == 0)
		VBRFrames=GetInfo_GETTAG_VBRI(tempBack+4/*BSINFO->bsbufptr-2*/);
	if(VBRFrames !=0)
	{	
		TempData = (__int64)VBRFrames * frameinfo.framesamplecount;
		AIF->ulDuration = (__u32)((TempData*1000)/frameinfo.samprate);
		AIF->ulBitRate =  (__int64)(filelength-ID3Len)*8*1000/AIF->ulDuration;
	}else
	{	
		TempData = (__int64)(filelength-ID3Len)*8*1000;
		AIF->ulDuration = (__u32)(TempData/frameinfo.bitrate);
	}
	//if((ID3Len ==0)&&(!AIF->ulbufflag))
	if((!AIF->ulbufflag)&&(AIF->length<=0))
	{

		fseek(bs, -128, SEEK_END); 
		BSINFO->bsbufLen = 0;
		if( GetInfo_GETID3_V1(AIF) == 0)
		{
			GetInfo_APETAG(AIF);
		}
		
	}
  GetInfo_BsEndRelease(AIF);
	return 1;
MP3_error:
	GetInfo_BsEndRelease(AIF);

	return ERROR;
}



static int	Mp3HeadSync(__audio_file_info_t *AIF,mp3FrameHeader *frameinfo)
{
	unsigned	int syncWord=0; 
	int		CheckFileLen=10*1024;//10k范围内查找头
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;

	while(1)
	{
		syncWord = GetInfo_ShowBs(0,2,AIF);
		//int tt=syncWord &0xffe0;
		if((syncWord&0xffe0) ==0xffe0)  /*mpeg frame head sync */
		{
			if(UnpackFrameHeader(frameinfo,BSINFO->bsbufptr)>0)
				break;
		}
		GetInfo_GetBsInByte(1,AIF);
		if(GetInfo_return<1)
			return 0;//no data;
		CheckFileLen -=1;
		if(CheckFileLen<0)
			return 0;
		
	}
	
	return 1;
}



static int UnpackFrameHeader(mp3FrameHeader *fh, char *buf)// MP3DecInfo *mp3DecInfo, unsigned char *buf)
{
	
	int verIdx;
	verIdx =         (buf[1] >> 3) & 0x03;
	if (verIdx ==0x1) 	return -1;
	fh->ver =        (MPEGVersion)( verIdx == 0 ? MPEG25 : ((verIdx & 0x01) ? MPEG1 : MPEG2) );
	fh->layer = 4 - ((buf[1] >> 1) & 0x03);     /* easy mapping of index to layer number, 4 = error */
	fh->crc =   1 - ((buf[1] >> 0) & 0x01);
	fh->brIdx =      (buf[2] >> 4) & 0x0f;
	fh->srIdx =      (buf[2] >> 2) & 0x03;
	fh->paddingBit = (buf[2] >> 1) & 0x01;
	fh->privateBit = (buf[2] >> 0) & 0x01;
	fh->sMode =      ((buf[3] >> 6) & 0x03)==3? 1: 2;      /* maps to correct enum (see definition) */    
	
	/* check parameters to avoid indexing tables with bad values */
	if (fh->srIdx == 3 || fh->layer == 4 || fh->brIdx == 15)
		return -1;
	
	if (fh->brIdx) {
		fh->bitrate = ((int)bitrateTab[fh->ver][fh->layer - 1][fh->brIdx]) * 1000;
		fh->samprate = samplerateTab[fh->ver][fh->srIdx];
		fh->framesamplecount =samplesPerFrameTab[fh->ver][fh->layer - 1];// (fh->samprate > 24000)?1152:576;

		if (fh->layer == 1)
			fh->framesize = ((12 * fh->bitrate / fh->samprate) + (fh->paddingBit ? 1 : 0)) * 4;
		else {
			unsigned int slots_per_frame;
			
			slots_per_frame = (fh->layer == 3 &&(fh->ver != MPEG1)) ?  72 : 144;
			
			fh->framesize = (slots_per_frame * fh->bitrate / fh->samprate) + (fh->paddingBit ? 1 : 0);
		}
		
	}
	else
		return -1;
	return 1;
}
