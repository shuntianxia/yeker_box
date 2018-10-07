/***************************************
*	Windows Medio audio infor 
*	
*
*	date:  2011/12/25  author: leven
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include "tagInfo.h"
#include "bitstream.h"

#define TheFrameLength 0x1000
#define MKTAG(a,b,c,d) ((a << 24) | (b << 16) | (c << 8) | (d ))

static const char midcodecInfo[1][3]=
{
	"CAF"	
};

int GetInfo_CAF(FILE *bs, __audio_file_info_t *AIF)
{
	__int64 filelength;
	__u32 framelength = 0;
	__u32 samplerate = 0;
	__u32 playlength = 0;
	__u32 thepacketbytes = 0;
	__u32 numbytes = 0;
	__u32 theframecount = 0;
	__u8 done = 1;
	__s32 theChunkSize = 0, theChunkType = 0;
	__u8 *thereadbuffer = 0;
	int Ret = 0; 

	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<=0) return ERROR;

	theChunkType = GetInfo_GetBsInByte(4, AIF);
	if (theChunkType != MKTAG( 'c', 'a', 'f', 'f' ))
	{
		goto CAF_error;
	}
	GetInfo_SkipBsInByte(4,AIF);
	while (done)
	{
		theChunkType = GetInfo_GetBsInByte(4, AIF);
		switch (theChunkType)
		{
		case MKTAG( 'd', 'e', 's', 'c' ):
			{
				GetInfo_SkipBsInByte(4,AIF);
				theChunkSize = GetInfo_GetBsInByte(4, AIF);
				GetInfo_SkipBsInByte(theChunkSize,AIF);
				break;
			}
		case MKTAG( 'k', 'u', 'k', 'i' ):
			{
				GetInfo_SkipBsInByte(8,AIF);
				framelength = GetInfo_GetBsInByte(4, AIF);
				GetInfo_GetBsInByte(1, AIF);
				AIF->ulBitsSample = GetInfo_GetBsInByte(1, AIF);
				GetInfo_SkipBsInByte(3,AIF);
				AIF->ulChannels = GetInfo_GetBsInByte(1,AIF);
				GetInfo_SkipBsInByte(10,AIF);
				AIF->ulSampleRate = GetInfo_GetBsInByte(4,AIF);
				break;
			}
		case MKTAG( 'p', 'a', 'k', 't' ):
			{
				GetInfo_SkipBsInByte(12,AIF);
				playlength = GetInfo_GetBsInByte(4,AIF);
				playlength *= TheFrameLength;			
				AIF->ulDuration =  playlength / AIF->ulSampleRate * 1000;
				Ret = 1;
				done = 0;			
				break;
			}
		default:
			goto CAF_error;		
		}
	}

	if(AIF->InforBufLeftLength>=3)
	{
		AIF->ulAudio_name_sz = 3;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;	
		memcpy(AIF->ulAudio_name,midcodecInfo[0],AIF->ulAudio_name_sz);

	}

CAF_error:

	GetInfo_BsEndRelease(AIF);
	return Ret;
}

