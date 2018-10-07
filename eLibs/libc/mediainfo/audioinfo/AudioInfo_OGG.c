/***************************************
*	MPEG LAYI II III audio infor 
*	support Xing /ID3V2 /ID3V1/VBRI tag 
*
*	date:  2008/10/10  author: fufo
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"	

static const char oggcodecInfo[1][10]=
{
	"OGG Vorbis"	

};
static int GetOGG_head(__audio_file_info_t *AIF)
{
	int ret =0;
	int i = 0;
	unsigned int BsVal= 0;
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	BsVal = GetInfo_ExtractI4(BSINFO->bsbufptr);
	while(BsVal!=0x4F676753)//"oggs"
	{
		
		GetInfo_GetBsInByte(1,AIF);
		if(GetInfo_return<1)
		{
			ret = -1;
			break;
		}
		ret++;
		BsVal = GetInfo_ExtractI4(BSINFO->bsbufptr);
		i++;
		if(i>4*1024)
		{
			ret = -1;
			break;
		}
	
	}
	return ret;

}

static unsigned char* memchr_info(unsigned char *buf,char data,int length)
{
	int ret = 0;
	unsigned char *ptr=buf;
	while(length>0)	
	{
		if(*ptr == data)
		{
			ret =1;
			break;
		}
		
		*ptr++;
		length--;
		
	}
	if(!ret) 
		ptr = (unsigned char *)0;
	return ptr;
		
}

int GetInfo_OGG(FILE *bs,__audio_file_info_t *AIF)
{
	int	retVal;
	//int		VBRFrames;
	__int64		filelength=0;
	int		ID3Len;
	//char buffer[6];
	int packtype;
	int segments;
	int test;
	unsigned char *bufferdata = malloc(4*1024*4);
	unsigned char *buffer=bufferdata;
	int i,k=0,flag=0;
	int head[4]={0,0,0};
	int pack;
	unsigned char *pstore;
	int ulstore;
	int length = 1024;
	unsigned char *now = bufferdata;
	unsigned char *next;
	int  samplelength;
	int  samplestart = 0;
	unsigned int serialno = 0;
	unsigned int lastserialno = 0;
	BsInfo      *BSINFO;
	if(!bufferdata)return ERROR;
	AIF->ulBitsSample       = 16;
	
	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<=0)
	{ 
		if(bufferdata)
		{
			free(bufferdata);
		  	bufferdata = 0;
		}
		return ERROR;
	}
	BSINFO = (BsInfo *)AIF->ulBSINFO;
	ID3Len=GetInfo_GETID3_V2(AIF);
	
	if(BSINFO->bsbufLen<100)GetInfo_GetBsInByte(0,AIF);//do nothing ,for have enough bytes
	for(pack=0;pack<2;pack++)
	{
		buffer=bufferdata;
		
		while(k<=pack)
		{
			head[pack]  = 0;
			retVal = GetOGG_head(AIF);
			if(retVal<0)goto OGG_error;
			samplestart=0;
			for(i=6;i>=0;--i)
				samplestart= samplestart<<8 | GetInfo_ShowBs(6+i,1,AIF);
			for(i=3;i>=0;--i)
				serialno= serialno<<8 | GetInfo_ShowBs(14+i,1,AIF);
			
			GetInfo_SkipBsInByte(26,AIF);
			
			segments = GetInfo_GetBsInByte(1,AIF);
			retVal = 0;
			for(i=0;i<segments;i++)
			{
				retVal = GetInfo_GetBsInByte(1,AIF);
				head[k] +=retVal;
				
				if(retVal<255)
				{
					k++;
					if(k>2) k=2;
					
				}
			}
			for(i=0;i<head[pack];i++)
			{
				if(buffer<bufferdata+4*1024*4-1)
					*buffer++ = GetInfo_GetBsInByte(1,AIF);
				else
					GetInfo_GetBsInByte(1,AIF);
			}
			
		}
		ulstore = BSINFO->bsbufLen;
		BSINFO->bsbufLen = buffer-bufferdata;//4*1024;
		if(BSINFO->bsbufLen<READFILELEN)
			BSINFO->bsbufLen +=LEFTFILELEN;//for not read file data;
		pstore = (unsigned char*)BSINFO->bsbufptr;
		BSINFO->bsbufptr = (char*)bufferdata;
		//GetInfo_SkipBsInByte(segments);
		
		packtype = GetInfo_GetBsInByte(1,AIF);
		test =GetInfo_GetBsInByte(4,AIF);
		if (test!=0x766f7262)goto OGG_error;
		test =GetInfo_GetBsInByte(2,AIF);
		if(test!=0x6973)goto OGG_error;
		
		switch(packtype){
		case 0x01: /* least significant *bit* is read first */
			GetInfo_GetBsInByte(4,AIF);//version
			AIF->ulChannels =GetInfo_GetBsInByte(1,AIF);
			AIF->ulSampleRate = GetInfo_GetBsInByteLI(4,AIF);
			GetInfo_GetBsInByteLI(4,AIF);//bitrate_upper
			AIF->ulBitRate = GetInfo_GetBsInByteLI(4,AIF);//normainal
			GetInfo_GetBsInByteLI(4,AIF);//lower
			GetInfo_GetBsInByteLI(2,AIF);//for last bytes
			BSINFO->bsbufLen = ulstore;
			BSINFO->bsbufptr = (char*)pstore;
			break;
		case 0x03://comment
			GetInfo_oggStandardTag(AIF);
			BSINFO->bsbufLen = ulstore;
			BSINFO->bsbufptr = (char*)pstore;
			break;
		default:
			goto OGG_error;
		}
	}
	if(AIF->ulBitRate)
	{
		AIF->ulDuration = (int)(((double)(filelength-ID3Len) * (double)(8))*1000 /((double)AIF->ulBitRate) ); 
	}
	
    
	
	if((!AIF->ulbufflag)&&(AIF->length<=0))
	{

		test =1024;
		flag = 0;
		fseek(bs,-test,SEEK_END);
		fread(bufferdata,1,1024,bs);
		now = bufferdata;
		length = 1024;
		while(test<filelength)
		{		
			next=memchr_info(now, 'O', length);
			//next=strchr(now, 'O', length);
			//next=memchr(now, 'O', length);
			if(next)
			{
				retVal = GetInfo_ExtractI4((char *)next);
				if(retVal!=0x4F676753)
				{
					length -=next-now+1;
					now =next+1;
				}
				else
				{
					flag = 1;
					pstore= next;
					length -=next-now+1;
					now =next+1;
					
				}
				
			}
			else
			{
				if(flag)break;
				test +=1000;
				fseek(bs,-test,SEEK_END);
				fread(bufferdata,1,1024,bs);
				length =1024;
				now = bufferdata;
				
			}
			
			
		}
		samplelength = 0;
		for(i=6;i>=0;--i)
			samplelength= samplelength<<8 | pstore[6+i];
		for(i=3;i>=0;--i)
				lastserialno= lastserialno<<8 | pstore[14+i];
		if(lastserialno==serialno)
		{
			AIF->ulDuration = (int)((double)(samplelength - samplestart)*(double)1000/(double)AIF->ulSampleRate);
			AIF->ulBitRate  = (int)(((double)(filelength-ID3Len) * (double)(8)*AIF->ulSampleRate) /((double)(samplelength - samplestart)));
		}
		else
		{
			AIF->ulDuration = 0;
		}
		//if(ID3Len==0)GetInfo_GETID3_V1(AIF);
		fseek(bs, -128, SEEK_END); 
		BSINFO->bsbufLen = 0;
		GetInfo_GETID3_V1(AIF);
	
	}
	if(AIF->InforBufLeftLength>=10)
	{
		AIF->ulAudio_name_sz = 10;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;	
		memcpy(AIF->ulAudio_name,oggcodecInfo[0],AIF->ulAudio_name_sz);
		
	}
	if((!AIF->ulbufflag)&&(AIF->length<=0))
	{

		fseek(bs, -128, SEEK_END); 
		BSINFO->bsbufLen = 0;
		GetInfo_GETID3_V1(AIF);
	
	}
  GetInfo_BsEndRelease(AIF);
  if(bufferdata)
  {
	  free(bufferdata);
	  bufferdata = 0;
  }
	return SUCCESS;
	
OGG_error:
  GetInfo_BsEndRelease(AIF);
  if(bufferdata)
  {
	  free(bufferdata);
	  bufferdata = 0;
  }
	return ERROR;
}
	
