/***************************************
*	DOBLY AC3 
*	
*
*	date:  2008/10/10  author: fufo
*	copyright 2100 AllWinner.LTD
*   2008-12-03  修正正确时的返回值
**************************************/

#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"
	
#define AMR_MAGIC_NUMBER "#!AMR\n"
#define AMRWB_MAGIC_NUMBER "#!AMR-WB\n"

static const char AMRcodecInfo[2][7]=
{	
	"AMR",
	"AMR-WB"
};
static const int block_size1[16]={ 12, 13, 15, 17, 19, 20, 26, 31, 5, 0, 0, 0, 0, 0, 0, 0 };
static const int  block_size2[16]= {18, 24, 33, 37, 41, 47, 51, 59, 61, 6, 6, 0, 0, 0, 1, 1};
int GetInfo_AMR(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	int ret=0;
	__int64 filelength;
	char Buff[10];
	int dec_mode;
	int i;
	int codeCC = 0;
	int framesize = 0;
	__int64 TempData = 0;
	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<10) return ERROR;
	

	for(i=0;i<10;i++)
	{
		Buff[i]=(char)GetInfo_GetBsInByte(1,AIF);
	}
	if (!strncmp( Buff, AMR_MAGIC_NUMBER, strlen( AMR_MAGIC_NUMBER ) ) )
	{
		ret = 1;
		codeCC = 0;
		dec_mode = (Buff[6] >> 3) & 0x000F;
		framesize = block_size1[dec_mode]+1;

	}else if(!strncmp( Buff, AMRWB_MAGIC_NUMBER, strlen( AMRWB_MAGIC_NUMBER ) ) )
	{
		codeCC = 1;
		ret = 1;
		dec_mode = (Buff[9] >> 3) & 0x000F;
		framesize = block_size2[dec_mode];
	}
	//if(ret == 0) return ERROR;
	if(ret == 0) goto AMR_error;
	AIF->ulSampleRate = 8000;
	AIF->ulChannels = 1;
	AIF->ulBitRate = framesize*50*8;//
	AIF->ulBitsSample = 16;

	TempData = (__int64)filelength*8*1000;
	AIF->ulDuration = (__u32)(TempData/AIF->ulBitRate);
	if(AIF->InforBufLeftLength>=7)
	{
	
		AIF->ulAudio_name_sz = 7;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
		memcpy(AIF->ulAudio_name,AMRcodecInfo[codeCC],AIF->ulAudio_name_sz);
		
	}

AMR_error:
	GetInfo_BsEndRelease(AIF);
	return ret;
}
