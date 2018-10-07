/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*
*                                (c) Copyright 2006-2007, lszhang China
*                                               All Rights Reserved
*
* File   : GetAudio_format.c
* Version: V1.0
* By     : lszhang	2008-12-19 10:02:07 
* 程序说明：
* 1、不能用全局变量，如果需要，定义结构体，包含所有全局变量，通过指针传递这些变量。
* 2、内部函数必须在函数定义前添加static。
* 3、如果别的文件需要引用本函数，则其函数名定义必须以GetInfo_为开始的字符串。
* 4、数组如果太大必须用malloc开空间，防止堆栈溢出。
*********************************************************************************************************
*/

//#include <string.h>
#include "AuInParser.h"
#include "sw_audio_format.h"
#include "GetAudio_format.h"

#define SUCCESS 1
#define ERROR   0
#define AUDIOPOSTFIXNUM 25


typedef struct __AUDIO_FORMAT
{
	const char				pChar[10];
	__sw_audio_format_e     ulFormat;
}__audio_format_t;


static const __audio_format_t audio_format_postfix[AUDIOPOSTFIXNUM]=
{
    
	".AAC", 	SW_AUDIO_FORMAT_AAC,	        	

	".AC3", 	SW_AUDIO_FORMAT_AC3,
	".MLP", 	SW_AUDIO_FORMAT_AC3,
	        		
	".APL", 	SW_AUDIO_FORMAT_APE,	
	".MAC", 	SW_AUDIO_FORMAT_APE,	
	".APE", 	SW_AUDIO_FORMAT_APE,
	        		
	".DTS", 	SW_AUDIO_FORMAT_DTS,

	".FLAC",	SW_AUDIO_FORMAT_FLAC,
	       
	".MP3", 	SW_AUDIO_FORMAT_MP3,       
	".MP2", 	SW_AUDIO_FORMAT_MP3,           
	".MP1", 	SW_AUDIO_FORMAT_MP3,
	        	
	".OGG", 	SW_AUDIO_FORMAT_OGG,
	 
	".RA",  	SW_AUDIO_FORMAT_RA,
	//".RM",  	SW_AUDIO_FORMAT_RA,
	//".RMVB",	SW_AUDIO_FORMAT_RA,

	".WAV", 	SW_AUDIO_FORMAT_WAV,
	        	
	".WMA", 	SW_AUDIO_FORMAT_WMA,

	//".ASF", 	SW_AUDIO_FORMAT_WMA,
	".AMR",     SW_AUDIO_FORMAT_AMR,
	".AWB",     SW_AUDIO_FORMAT_AMR,

	".OMG",     SW_AUDIO_FORMAT_ATRC,
	".OMA",     SW_AUDIO_FORMAT_ATRC,
	".AA3",     SW_AUDIO_FORMAT_ATRC,

	".CAF",     SW_AUDIO_FORMAT_CAF,

	".MID",     SW_AUDIO_FORMAT_MID,
	".KMP3",     SW_AUDIO_FORMAT_MP3,//..kkk
	".KMID",     SW_AUDIO_FORMAT_MID,//..kkk
	".A_T", 	SW_AUDIO_FORMAT_,
  
};
static const __sw_audio_format_e AudioFormatOrder[]=
{
    SW_AUDIO_FORMAT_WMA,
    SW_AUDIO_FORMAT_APE,
    SW_AUDIO_FORMAT_FLAC,
    SW_AUDIO_FORMAT_OGG,
    SW_AUDIO_FORMAT_RA,
    SW_AUDIO_FORMAT_WAV,
    SW_AUDIO_FORMAT_AMR,
    SW_AUDIO_FORMAT_ATRC,
    SW_AUDIO_FORMAT_MID,
    SW_AUDIO_FORMAT_CAF,

    SW_AUDIO_FORMAT_MP3,
    SW_AUDIO_FORMAT_DTS,
    SW_AUDIO_FORMAT_AC3,
    SW_AUDIO_FORMAT_AAC,
    
};

static void InitmemetInfo(__audio_file_info_t *AIF)
{
	__s8* pInfor = 0;
	__s32 ulInforLength = 0;
	if(!AIF)
	{
		return ;
	}
	pInfor = AIF->InforBuf;
	ulInforLength = AIF->InforBufLeftLength;
	memset(AIF->InforBuf, 0,AIF->InforBufLeftLength);
	memset(AIF, 0, sizeof(__audio_file_info_t)); 
	AIF->InforBuf = pInfor;
	AIF->InforBufLeftLength = ulInforLength; 
}
static void ProtestInfo(__audio_file_info_t *AIF)
{
	if(AIF->ulAudio_name_sz<0)
	{
		AIF->ulAudio_name_sz = 0;
	}
	if(AIF->ulGenre_sz<0)
	{
		AIF->ulGenre_sz = 0;
	}
	if(AIF->ultitle_sz<0)
	{
		AIF->ultitle_sz = 0;
	}
	if(AIF->ulauthor_sz<0)
	{
		AIF->ulauthor_sz = 0;
	}
	if(AIF->ulAlbum_sz<0)
	{
		AIF->ulAlbum_sz = 0;
	}
	if(AIF->ulYear_sz<0)
	{
		AIF->ulYear_sz = 0;
	}
}
static int GetInfoFunction(FILE *Bitstream,__audio_file_info_t *AIF,int audio_format)
{
	int ret = ERROR;
	switch(audio_format)
	{

	case SW_AUDIO_FORMAT_AAC:
		ret = GetInfo_AAC(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_AC3:
		ret = GetInfo_AC3(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_APE:
		ret = GetInfo_APE(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_DTS:
		ret = GetInfo_DTS(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_FLAC:
		ret = GetInfo_FLAC(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_MP3:
		ret = GetInfo_MP3(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_OGG:
		ret = GetInfo_OGG(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_RA:
		ret = GetInfo_RA(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_WAV:
		ret = GetInfo_WAV(Bitstream,AIF);
		break;	
	case SW_AUDIO_FORMAT_WMA:
		ret = GetInfo_WMA(Bitstream,AIF);
		break;
    case SW_AUDIO_FORMAT_AMR:
		ret = GetInfo_AMR(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_ATRC:
		ret = GetInfo_ATRAC(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_MID:
		ret = GetInfo_MID(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_CAF:
		ret = GetInfo_CAF(Bitstream,AIF);
		break;
	case SW_AUDIO_FORMAT_:
		AIF->ulSampleRate = 44100;
		AIF->ulChannels   = 2;
		AIF->ulBitRate    = 1411200;
		AIF->ulBitsSample = 16;
		AIF->ulDuration   = 80249;
		
		ret = SUCCESS;
		break;
   
	default:
		ret =ERROR;	
	}
	return ret;
}
      
int GetAudioFormat(const char *pFilename,int *A_Audio_Format)
{
	int ret = ERROR;
	int i;
	char filefix[6];
	const char * pExtension = &pFilename[strlen(pFilename)]; 
	*A_Audio_Format = SW_AUDIO_FORMAT_UNKNOWN;

	if(strlen(pFilename)==0)
	{
		return ret;
	}
	
  while ((pExtension > pFilename) && (*pExtension != '.'))
        pExtension--;
	
  if(strlen(pExtension)>5)return ret;
  strcpy(filefix,pExtension);
  //strupr(filefix);
  for(i= 0;i<strlen(pExtension);i++)
  {
  	if((filefix[i]>96) && (filefix[i]<123))
  	  filefix[i] -=32;
  }
        
  for(i=0;i<AUDIOPOSTFIXNUM;i++)
  {
  	if (strcmp(filefix, audio_format_postfix[i].pChar) == 0)
  		{
  			*A_Audio_Format = audio_format_postfix[i].ulFormat;
  			ret = SUCCESS;
  			break;
  		}
  }
    return ret;
    
    	
}
#define TESTFILETYPE //是否文件类型判断不对时，不根据后缀智能判断文件类型
int GetAudioInfo(const char *pFilename,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	FILE *Bitstream=NULL;
	int audio_format;
	int ret=0;
	InitmemetInfo(AIF);
	if(strlen(pFilename)==0)
	{
		return ret;
	} 
	Bitstream=fopen(pFilename,"rb");
	if(!Bitstream)
	{
		//PRINT("Could not open %s for read.\n", pFilename);
		return 0;
	}
	ret = GetAudioFormat(pFilename,&audio_format);
	if(ret!=SUCCESS)	return ERROR;
	AIF->ulFormat = ( __sw_audio_format_e)audio_format;
    AIF->stream_num = 1;//默认一个音轨数，目前只有midi有做音轨数提取
    ret = GetInfoFunction(Bitstream,AIF,audio_format);
	if(Bitstream)
	{
		fclose(Bitstream);
		Bitstream=NULL;
	}
	//for bug
#ifdef TESTFILETYPE
	if(ret==ERROR)
	{
		int i;
		for(i=0; (i<SW_AUDIO_FORMAT_CAF)&&(ret==ERROR);i++)
		{
			audio_format = AudioFormatOrder[i];
			Bitstream=fopen(pFilename,"rb");
			if(!Bitstream)
			{
				//PRINT("Could not open %s for read.\n", pFilename);
				return 0;
			}
			InitmemetInfo(AIF);		
			AIF->ulFormat = ( __sw_audio_format_e)audio_format;
			ret = GetInfoFunction(Bitstream,AIF,audio_format);
			if(Bitstream)
			{
				fclose(Bitstream);
				Bitstream=NULL;
			}
		}

	}
#endif
	ProtestInfo(AIF);
  return ret;

}
//*pFilename must get right audio_format suffix
int GetAudioDataInfo(const char *pFilename,__audio_file_info_t *AIF,__s8* buf,__s32 datalen)  //return 1 :succed 0 :fail
{
	FILE *Bitstream=NULL;
	int audio_format;
	int ret=ERROR;
	
	if(pFilename)
	{
		ret = GetAudioFormat(pFilename,&audio_format);
	}
	if(ret==ERROR)
	{
		__s8* bufdata = buf;
		__s32 buflen = datalen;
		do{
			
			ret = GetInfo_GETID3_Len(bufdata,ret);
			bufdata +=ret;
			buflen -=ret;
		}while(ret&&(buflen>0));
		if(buflen>0)
		{
			ret =GetAudioFormatData(bufdata,buflen,&audio_format);
		}
		if(ret == ERROR)
		{
			return ret;
		}

	}
	InitmemetInfo(AIF);	
	AIF->ulFormat = ( __sw_audio_format_e)audio_format;
	AIF->ulbufflag = 1;
	AIF->ulbuf = (char*)buf;
	AIF->ulbuf_length = datalen;
	if(datalen<=0)return ERROR;
	
    ret = GetInfoFunction(Bitstream,AIF,audio_format);
	
	ProtestInfo(AIF);
	
    return ret;

}
int GetAudioFileInfo(FILE *Bitstream,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{

	int audio_format;
	int ret=ERROR;
	int fileID3len = 0;
    if(Bitstream)
	{
		fseek(Bitstream,0,SEEK_SET);
		do{
			ret = fread(AIF->InforBuf,1,AIF->InforBufLeftLength,Bitstream);
			if((ret!=AIF->InforBufLeftLength)&&(ret<10))
				return ERROR;
			ret = GetInfo_GETID3_Len(AIF->InforBuf,ret);
			if(ret)
			{
				fseek(Bitstream,ret-AIF->InforBufLeftLength,SEEK_CUR);
			}
			fileID3len +=ret;
		}while(ret);
		fseek(Bitstream,0,SEEK_SET);
		ret =GetAudioFormatData((unsigned char*)AIF->InforBuf,AIF->InforBufLeftLength,&audio_format);
		//for wav dts max 8k
		if((audio_format == SW_AUDIO_FORMAT_WAV)&&(ret==SUCCESS))
		{
			unsigned char * pIndata = malloc(8*1024);
			if(pIndata)
			{
				fseek(Bitstream,fileID3len,SEEK_SET);
				ret = fread(pIndata,1,8*1024,Bitstream);
				fseek(Bitstream,0,SEEK_SET);
				ret =GetAudioFormatData((unsigned char*)pIndata,ret,&audio_format);
				free(pIndata);
			}
		}
		if(ret==SUCCESS)
		{
			InitmemetInfo(AIF);				
			AIF->ulFormat = ( __sw_audio_format_e)audio_format;
			ret = GetInfoFunction(Bitstream,AIF,audio_format);
		}


	}
	else
	{
		return ret;
	}
	//for bug
#ifdef TESTFILETYPE
	if(ret==ERROR)
	{
		int i;

		for(i=0; (i<SW_AUDIO_FORMAT_CAF)&&(ret==ERROR);i++)
		{
			audio_format = AudioFormatOrder[i];
			fseek(Bitstream,0,SEEK_SET);
			InitmemetInfo(AIF);	  			
			AIF->ulFormat = ( __sw_audio_format_e)audio_format;
			ret = GetInfoFunction(Bitstream,AIF,audio_format);
		}

	}
#endif

	ProtestInfo(AIF);
	
    return ret;

}
int GetAudioFileInfoOffset(FILE *Bitstream,int64_t offset,int64_t length,__audio_file_info_t *AIF)
{
	int audio_format;
	int ret=ERROR;
	//__s8* pInfor =AIF->InforBuf;
	//__s32 ulInforLength = AIF->InforBufLeftLength;
	int fileID3len = 0;
  if(Bitstream)
	{
		fseek(Bitstream,offset,SEEK_SET);
		do{
			ret = fread(AIF->InforBuf,1,AIF->InforBufLeftLength,Bitstream);
			if((ret!=AIF->InforBufLeftLength)&&(ret<10))
				return ERROR;
			ret = GetInfo_GETID3_Len(AIF->InforBuf,ret);
			if(ret)
			{
				fseek(Bitstream,ret-AIF->InforBufLeftLength,SEEK_CUR);
			}
			fileID3len +=ret;
		}while(ret);
		fseek(Bitstream,offset,SEEK_SET);
		ret =GetAudioFormatData((unsigned char*)AIF->InforBuf,AIF->InforBufLeftLength,&audio_format);
		//for wav dts max 8k
		if((audio_format == SW_AUDIO_FORMAT_WAV)&&(ret==SUCCESS))
		{
			int len  =8*1024;
			unsigned char * pIndata = malloc(len);
			if(pIndata)
			{
				fseek(Bitstream,offset + fileID3len,SEEK_SET);
				if((!length)&&(len>length))
				{
					len = length;
				}
				ret = fread(pIndata,1,len,Bitstream);
				fseek(Bitstream,offset,SEEK_SET);
				ret =GetAudioFormatData((unsigned char*)pIndata,ret,&audio_format);
				free(pIndata);
			}
		}
		if(ret==SUCCESS)
		{
			InitmemetInfo(AIF);
			AIF->offset = offset;
			AIF->length = length;				
			AIF->ulFormat = ( __sw_audio_format_e)audio_format;
			ret = GetInfoFunction(Bitstream,AIF,audio_format);
		}


	}
	else
	{
		return ret;
	}
	//for bug
#ifdef TESTFILETYPE
	if(ret==ERROR)
	{
		int i;

		for(i=0; (i<SW_AUDIO_FORMAT_CAF)&&(ret==ERROR);i++)
		{
			audio_format = AudioFormatOrder[i];
			fseek(Bitstream,0,SEEK_SET);
			InitmemetInfo(AIF);	  			
			AIF->ulFormat = ( __sw_audio_format_e)audio_format;
			ret = GetInfoFunction(Bitstream,AIF,audio_format);
		}

	}
#endif

	ProtestInfo(AIF);
	
    return ret;
}
#define AMR_MAGIC_NUMBER "#!AMR\n"
#define AMRWB_MAGIC_NUMBER "#!AMR-WB\n"
int GetAudioFormatData(unsigned char *ptr,int buflen,int *A_Audio_Format)
{
	int ret = ERROR;
	unsigned char ea3flag[] = {'e', 'a', '3', 3, 0};
	
	*A_Audio_Format = SW_AUDIO_FORMAT_UNKNOWN;
	
	
	if(((ptr[0] == 0x77)&&(ptr[1] == 0x0b))
		||((ptr[0] == 0x0b)&&(ptr[1] == 0x77))
		||((ptr[0] == 0x72)&&(ptr[1] == 0xf8)&&(ptr[2] == 0xbb)&&(ptr[3] == 0x6f))
		||((ptr[0] == 0xf8)&&(ptr[1] == 0x72)&&(ptr[2] == 0x6f)&&(ptr[3] == 0xbb))
		||((ptr[0] == 0x72)&&(ptr[1] == 0xf8)&&(ptr[2] == 0xba)&&(ptr[3] == 0x6f))
		||((ptr[0] == 0xf8)&&(ptr[1] == 0x72)&&(ptr[2] == 0x6f)&&(ptr[3] == 0xba)))
		//0x72f8bb6f//0xf8726fbb//0x72f8ba6f//0xf8726fba
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_AC3;
  		ret = SUCCESS;
	}
	else if ((!strncmp( ptr, AMR_MAGIC_NUMBER, strlen( AMR_MAGIC_NUMBER ) ) )
	            ||(!strncmp( ptr, AMRWB_MAGIC_NUMBER, strlen( AMRWB_MAGIC_NUMBER ) ) ))
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_AMR;
  		ret = SUCCESS;
	}
	else if((ptr[0]=='M')&&(ptr[1]=='A')&&(ptr[2]=='C')&&(ptr[3]==' '))
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_APE;
  		ret = SUCCESS;
	}
	else if (!memcmp(ptr, ea3flag,5))
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_ATRC;
  		ret = SUCCESS;
	}
	else if ((ptr[0] == 0xff && ptr[1] == 0x1f &&
              ptr[2] == 0x00 && ptr[3] == 0xe8 &&
              (ptr[4] & 0xf0) == 0xf0 && ptr[5] == 0x07)
           ||(ptr[0] == 0x1f && ptr[1] == 0xff &&
              ptr[2] == 0xe8 && ptr[3] == 0x00 &&
              ptr[4] == 0x07 && (ptr[5] & 0xf0) == 0xf0)
		   ||(ptr[0] == 0xfe && ptr[1] == 0x7f &&
			  ptr[2] == 0x01 && ptr[3] == 0x80)
		   ||(ptr[0] == 0x7f && ptr[1] == 0xfe &&
              ptr[2] == 0x80 && ptr[3] == 0x01))
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_DTS;
  		ret = SUCCESS;
	}
	else if((ptr[0] == 0x66)&&(ptr[1] == 0x4c)&&(ptr[2] == 0x61)&&(ptr[3] == 0x43))//0x664c6143
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_FLAC;
  		ret = SUCCESS;
	}
	else if(memcmp(ptr, "MThd", 4) == 0)
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_MID;
  		ret = SUCCESS;
	}
	else if(memcmp(ptr, "caff", 4) == 0)
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_CAF;
		ret = SUCCESS;
	}
	else if(((ptr[0]&0xff)==0xff)&&((ptr[1]&0xe0)==0xe0)&&((ptr[1] & (3<<1)) != 0)&&((ptr[2] & (0xf<<4)) != (0xf<<4))&&((ptr[2] & (3<<2)) != (3<<2)))
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_MP3;
  		ret = SUCCESS;
	}
	else if((ptr[0] == 0x4f)&&(ptr[1] == 0x67)&&(ptr[2] == 0x67)&&(ptr[3] == 0x53))//0x4F676753
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_OGG;
  		ret = SUCCESS;
	}
	else if(((ptr[0] == 0x2e)&&(ptr[1] == 0x52)&&(ptr[2] == 0x4d)&&(ptr[3] == 0x46))//0x2E524D46
		||((ptr[0] == 0x2e)&&(ptr[1] == 0x72)&&(ptr[2] == 0x61)&&(ptr[3] == 0xfd)))//0x2E7261FD
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_RA;
  		ret = SUCCESS;
	}
	else if(((ptr[0] == 0x52)&&(ptr[1] == 0x49)&&(ptr[2] == 0x46)&&(ptr[3] == 0x58))//0X52494658
		||((ptr[0] == 0x52)&&(ptr[1] == 0x49)&&(ptr[2] == 0x46)&&(ptr[3] == 0x46)))//0X52494646
	{
		int i =0;
		*A_Audio_Format = SW_AUDIO_FORMAT_WAV;
  		ret = SUCCESS;
		for(i=4;i<buflen;i++)
		{
            if((ptr[i] == 0x64)&&(ptr[i+1] == 0x61)&&(ptr[i+2] == 0x74)&&(ptr[i+3] == 0x61))
				break;
		}
		i+=8;
		if(i<buflen)
		{
			for(;i<buflen;i++)
			{
				if(ptr[i]!=0)
					break;
			}
		}
		if(i<buflen-5)
		{
			if ((ptr[i + 0] == 0xff && ptr[i + 1] == 0x1f &&
              ptr[i + 2] == 0x00 && ptr[i + 3] == 0xe8 &&
              (ptr[i + 4] & 0xf0) == 0xf0 && ptr[i + 5] == 0x07)
           ||(ptr[i + 0] == 0x1f && ptr[i + 1] == 0xff &&
              ptr[i + 2] == 0xe8 && ptr[i + 3] == 0x00 &&
              ptr[i + 4] == 0x07 &&(ptr[i + 5] & 0xf0) == 0xf0)
		   ||(ptr[i + 0] == 0xfe && ptr[i + 1] == 0x7f &&
			  ptr[i + 2] == 0x01 && ptr[i + 3] == 0x80)
		   ||(ptr[i + 0] == 0x7f && ptr[i + 1] == 0xfe &&
              ptr[i + 2] == 0x80 && ptr[i + 3] == 0x01))
			{
				*A_Audio_Format = SW_AUDIO_FORMAT_DTS;
  				ret = SUCCESS;
			}

		}
		//add for adpcm call  CedarA
        for(i=12; i<(buflen > 512 ? 512 : buflen); i++)
    	{
    	    if((ptr[i] == 0x66)&&(ptr[i+1] == 0x6D)&&(ptr[i+2] == 0x74)&&(ptr[i+3] == 0x20))//fmt
    			break;
    	}
        if(i<buflen-10)
        {
            unsigned int tempWavTag = ptr[i+8]|((unsigned int)(ptr[i+9])<<8);
            //ALOGV("WAV FOR tag =%d\n",tempWavTag);
            if(tempWavTag == 0x55)/* MPEG Layer 3 */
            {
                *A_Audio_Format = SW_AUDIO_FORMAT_MP3;
  				ret = SUCCESS;
            }
        }
	}
	else if((ptr[0] == 0x30)&&(ptr[1] == 0x26)&&(ptr[2] == 0xb2)&&(ptr[3] == 0x75))
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_WMA;
  		ret = SUCCESS;
	}
	else if((((ptr[0]&0xff)==0xff)&&((ptr[1]&0xf0)==0xf0)&&(((ptr[1] & (0x3<<1)) == 0x0<<1)||((ptr[1] & (0x3<<1)) == 0x3<<1))&&((ptr[2] & (0xf<<2)) < (12<<2))&&(((ptr[2]&0x01)|(ptr[3]&(0x03<<6)))!=0))
		||(((ptr[0]&0xff)==0x56)&&((ptr[1]&0xe0)==0xe0))
		||(((ptr[0]&0xff)=='A')&&((ptr[1]&0xff)=='D')&&((ptr[2]&0xff)=='I')&&((ptr[3]&0xff)=='F')))
	{
		*A_Audio_Format = SW_AUDIO_FORMAT_AAC;
  		ret = SUCCESS;
	}
	
    return ret;
}
