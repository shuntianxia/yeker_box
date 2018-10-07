/***************************************
*	Windows Medio audio infor
*
*
*	date:  2008/10/29  author: lszhang
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"

#define COMPRESSION_LEVEL_FAST          1000
#define COMPRESSION_LEVEL_NORMAL        2000
#define COMPRESSION_LEVEL_HIGH          3000
#define COMPRESSION_LEVEL_EXTRA_HIGH    4000
#define COMPRESSION_LEVEL_INSANE        5000

#define MAC_FORMAT_FLAG_8_BIT                 1    // is 8-bit [OBSOLETE]
#define MAC_FORMAT_FLAG_24_BIT                8    // is 24-bit [OBSOLETE]
static const char apecodecInfo[1][3]=
{	
	"APE"
};
static const char apeformat[7][10]=
{
	"Fast",
	"Normal",
	"High",
	"Extra_High",
	"Insane",
	"Version:",
	"Mode:"
};
int GetInfo_APE(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	//int Ret = 0;
	//int	Object=0;
	//int i,k;
	//int		VBRFrames;
	__int64		filelength=0;
	//int		BsVal;
	int		ID3Len = 0;
	//unsigned int retVal;
	unsigned int nGoalID;
	unsigned int nReadID;
	int nFormatFlags;
	int nVersion;
	int nCompressionLevel;
	int nTotalFrames;
	int nBlocksPerFrame;
    int nFinalFrameBlocks;
	int nTotalBlocks;
	int nLengthMS;

	int nScanBytes;
	unsigned char cTemp;
	filelength = GetInfo_BsInit(bs,AIF);
	if(filelength<=0) return ERROR;

 	if((!AIF->ulbufflag)&&(AIF->length<=0))
	{
		GetInfo_APETAG(AIF);
	}  
	ID3Len=GetInfo_GETID3_V2(AIF);
	//GetInfo_GETID3_V2(AIF);
// scan until we hit the APE_DESCRIPTOR, the end of the file, or 1 MB later
    nGoalID = ('M' << 24) | ('A' << 16) | ('C' << 8) | (' ');
    nReadID = GetInfo_GetBsInByte(4,AIF);
    nScanBytes = 0;
    //while ((nGoalID != nReadID)  && (nScanBytes < (1024 * 1024)))
	while ((nGoalID != nReadID)  && (nScanBytes < (1024 * 10)))
    {
        cTemp = GetInfo_GetBsInByte(1,AIF);
		if(GetInfo_return<1)
			goto APE_error;//no data
        nReadID = ((unsigned int) cTemp) | (nReadID << 8);
        //nJunkBytes++;
        nScanBytes++;
    }
    ID3Len +=nScanBytes;
    if (nGoalID != nReadID)
        goto APE_error;
    //get version
    nVersion = GetInfo_GetBsInByteLI(2,AIF);
	 if ( nVersion >= 3980)
    {
        // current header format
        GetInfo_SkipBsInByte(52-6,AIF);//skip ape_descriptor;
		nCompressionLevel       = GetInfo_GetBsInByteLI(2,AIF);
		nFormatFlags			= GetInfo_GetBsInByteLI(2,AIF);
		nBlocksPerFrame         = GetInfo_GetBsInByteLI(4,AIF);
		nFinalFrameBlocks       = GetInfo_GetBsInByteLI(4,AIF);
		nTotalFrames			= GetInfo_GetBsInByteLI(4,AIF);
		AIF->ulBitsSample       = GetInfo_GetBsInByteLI(2,AIF);
		AIF->ulChannels			= GetInfo_GetBsInByteLI(2,AIF);
		AIF->ulSampleRate		= GetInfo_GetBsInByteLI(4,AIF);
		nTotalBlocks            = (nTotalFrames == 0) ? 0 : ((nTotalFrames -  1) * nBlocksPerFrame) + nFinalFrameBlocks;
		nLengthMS				= (int)((double)(nTotalBlocks)*1000 / (double)(AIF->ulSampleRate));
		AIF->ulDuration			= nLengthMS;
		AIF->ulBitRate			=  (int)(((double)(filelength) * (double)(8)) /((double)(nTotalBlocks) / (double)(AIF->ulSampleRate)) );

    }
    else
    {
        // legacy support
        nCompressionLevel       = GetInfo_GetBsInByteLI(2,AIF);
		nFormatFlags			= GetInfo_GetBsInByteLI(2,AIF);
		AIF->ulChannels			= GetInfo_GetBsInByteLI(2,AIF);
		AIF->ulSampleRate		= GetInfo_GetBsInByteLI(4,AIF);
		GetInfo_SkipBsInByte(8,AIF);		//nHeaderBytes and nTerminatingBytes
		nTotalFrames			= GetInfo_GetBsInByteLI(4,AIF);
		nFinalFrameBlocks		= GetInfo_GetBsInByteLI(4,AIF);
        PRINT("Channels:%x SampleRate:%x TotalFrames:%x FrameBlocks:%x",
            AIF->ulChannels,AIF->ulSampleRate,nTotalFrames,nFinalFrameBlocks);
        #if 1 //add by yefl
        if (nFormatFlags & 4) {
            GetInfo_SkipBsInByte(4,AIF); 
            PRINT("SKIP 4 BYTE\n");
        }
        GetInfo_SkipBsInByte(4,AIF); //skip seek_table_length 
        #endif

        AIF->ulBitsSample		= (nFormatFlags & MAC_FORMAT_FLAG_8_BIT) ? 8 : ((nFormatFlags & MAC_FORMAT_FLAG_24_BIT) ? 24 : 16);
        nBlocksPerFrame			= ((nVersion >= 3900) || ((nVersion >= 3800) && (nCompressionLevel == COMPRESSION_LEVEL_EXTRA_HIGH))) ? 73728 : 9216;
        if ((nVersion >= 3950)) nBlocksPerFrame = 73728 * 4;
		nTotalBlocks            = (nTotalFrames == 0) ? 0 : ((nTotalFrames -  1) * nBlocksPerFrame) + nFinalFrameBlocks;
        nLengthMS				= (int)((double)(nTotalBlocks)*1000 / (double)(AIF->ulSampleRate));
		AIF->ulDuration			= nLengthMS;
		AIF->ulBitRate			=  (int)(((double)(filelength) * (double)(8)) /((double)(nTotalBlocks) / (double)(AIF->ulSampleRate)) );
    }
	if(AIF->InforBufLeftLength>=3)
	{
		AIF->ulAudio_name_sz = 3;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
		memcpy(AIF->ulAudio_name,apecodecInfo[0],AIF->ulAudio_name_sz);
		
	}
	if(AIF->InforBufLeftLength>=42)
	{
		int length = 0;
		__s8 *ApeInforBuf = AIF->InforBuf;
		//nversion 9+5
		*ApeInforBuf++ = ' ';
		memcpy(ApeInforBuf,apeformat[5],8);
		ApeInforBuf           += 8;
		length                += 9;
		if((nVersion/10000) > 0)
		{
			*ApeInforBuf++ = 0x30 + nVersion/10000;
			length++;
		}
		*ApeInforBuf++ = 0x30 + (nVersion%10000)/1000;
		*ApeInforBuf++ = '.';
		*ApeInforBuf++ = 0x30 + (nVersion%1000)/100;
		*ApeInforBuf++ = 0x30 + (nVersion%100)/10;
		length          += 4;
		
		//nCompressionLevel 18+10
		*ApeInforBuf++ = ' ';
		memcpy(ApeInforBuf,apeformat[6],5);
		ApeInforBuf             += 5;
		length                  += 6;
		if (nCompressionLevel == COMPRESSION_LEVEL_FAST)
        {
            
           memcpy(ApeInforBuf,apeformat[0],4);
           length += 4; 
        }
        else if (nCompressionLevel == COMPRESSION_LEVEL_NORMAL)
        {
            memcpy(ApeInforBuf,apeformat[1],6); 
           length += 6;
        }
        else if (nCompressionLevel == COMPRESSION_LEVEL_HIGH)
        {
            memcpy(ApeInforBuf,apeformat[2],4); 
           length += 4;
        }
        else if (nCompressionLevel == COMPRESSION_LEVEL_EXTRA_HIGH)
        {
            memcpy(ApeInforBuf,apeformat[3],10); 
            length += 10;
        }
        /*else if (nCompressionLevel == COMPRESSION_LEVEL_INSANE)
        {
	    	memcpy(ApeInforBuf,apeformat[4],6); 
           length +=6;
	    }*/
	    else
	    {
	    	goto APE_error;
	    }
	    AIF->ulAudio_name_sz += length;
	    AIF->InforBuf +=length;
		AIF->InforBufLeftLength -=length;
	}
    GetInfo_BsEndRelease(AIF);
    return 1;
APE_error:
	  GetInfo_BsEndRelease(AIF);
	  return ERROR;

}

