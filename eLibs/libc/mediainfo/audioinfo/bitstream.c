
#include "bitstream.h"
int GetInfo_return;


__int64	GetInfo_BsInit(FILE *fileptr,__audio_file_info_t *AIF)
{
	__int64 filelength=0;
	int	Flen;
	BsInfo *BSINFO =NULL;
	GetInfo_return = 0;
	BSINFO = malloc(sizeof(BsInfo));
	if(BSINFO==NULL)return -1;
	memset(BSINFO, 0,sizeof(BsInfo));
	//BSINFO->BsBuf = malloc(4096+10);
	if(!AIF->ulbufflag)
	{
		BSINFO->BsBuf = malloc(READFILELEN+LEFTFILELEN);
		if(BSINFO->BsBuf == NULL)
		{
			if(BSINFO)
			{
			free(BSINFO);
			BSINFO = NULL;
			   	
			}
			return -1;
		}
	}
	AIF->ulBSINFO = (__s8*)BSINFO;
	BSINFO->ReadFile = 	fileptr;
	if(!AIF->ulbufflag)
	{
		fseek(fileptr, AIF->offset, SEEK_END); //del offset
		//_lseeki64(fileptr, 0L, SEEK_END);
		filelength = ftell(fileptr);//_tell(fileptr);// _telli64(fileptr);//ftell(fileptr); 
		fseek(fileptr, AIF->offset, SEEK_SET);
		if(AIF->length>0)
		{
			filelength = AIF->length;
		}
		if(filelength<=0)
		{
			if(BSINFO->BsBuf)
			{
				free(BSINFO->BsBuf);
				BSINFO->BsBuf = 0;
			}
			if(BSINFO)
			{
				free(BSINFO);
				BSINFO = NULL;
			}
			return -1;
		}
		Flen = fread(BSINFO->BsBuf,1,READFILELEN,BSINFO->ReadFile);
		if(Flen>filelength)
		{
			Flen = filelength;
		}
		AIF->readlen = Flen;
		BSINFO->bsbufLen = Flen;//READFILELEN;
		BSINFO->bsbufptr = BSINFO->BsBuf;
	}
	else
	{
		filelength = AIF->ulbuf_length;
		BSINFO->BsBuf = AIF->ulbuf;
		BSINFO->bsbufLen = AIF->ulbuf_length;
		BSINFO->bsbufptr = BSINFO->BsBuf;
	}

	
	//Flen = fread(&BSINFO->BsBuf[0],1,READFILELEN,BSINFO->ReadFile);
	
	return filelength;

}
int GetInfo_GetFileLocation(__audio_file_info_t *AIF)
{
	int retVal = 0;
	BsInfo *BSINFO = (BsInfo *)AIF->ulBSINFO;
	if(!AIF->ulbufflag)
	{
		retVal = ftell(BSINFO->ReadFile);
		retVal -= BSINFO->bsbufLen;
	}
	else
	{
		retVal = AIF->ulbuf_length - BSINFO->bsbufLen;
	}
	return retVal;
}

int	GetInfo_BsEndRelease(__audio_file_info_t *AIF)
{
	
	BsInfo *BSINFO = (BsInfo *)AIF->ulBSINFO;
	
	if(BSINFO)
	{
		if(!AIF->ulbufflag)
		{		
			if(BSINFO->BsBuf)
			{
			free(BSINFO->BsBuf);
			BSINFO->BsBuf = NULL;			   	
			}
		}
		
		free(BSINFO);
		BSINFO = NULL;
		AIF->ulBSINFO = NULL;
	}

	return 0;

}

/**
*
*	read	n byte from bs buffer n<=4
*/
unsigned	int	GetInfo_GetBsInByte(int	ByteLen,__audio_file_info_t *AIF)
{
	unsigned	int	RetVal;
	char		data;
	int			i;
	int			readLen;
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	if(!AIF->ulbufflag)
	{
		if((BSINFO->bsbufLen<ByteLen)||(BSINFO->bsbufLen<LEFTFILELEN))  //
		{
			memcpy(BSINFO->BsBuf,BSINFO->bsbufptr,BSINFO->bsbufLen);
			readLen = fread(&BSINFO->BsBuf[BSINFO->bsbufLen],1,READFILELEN,BSINFO->ReadFile);
			if(AIF->length>0)
			{
				if(readLen+AIF->readlen> AIF->length)
				{
					readLen = AIF->length - AIF->readlen;
				}
			}
			AIF->readlen += readLen;
			BSINFO->bsbufLen += readLen;//READFILELEN;
			BSINFO->bsbufptr = BSINFO->BsBuf;
		}
	}
	
	RetVal = 0;
	if(ByteLen > BSINFO->bsbufLen)
		ByteLen = BSINFO->bsbufLen;
	for	(i=0;i<ByteLen;i++)
	{	
		data = *BSINFO->bsbufptr++;
		BSINFO->bsbufLen-- ;
		RetVal = (RetVal<<8)|(data&0xff);
	}
	GetInfo_return = ByteLen;
	return	RetVal;
}

unsigned	int	GetInfo_GetBsInByteLI(int	ByteLen,__audio_file_info_t *AIF)
{
	unsigned	int	RetVal;
	char		data;
	int			i;
	int			readLen;
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	if(!AIF->ulbufflag)
	{
		if((BSINFO->bsbufLen<ByteLen)||(BSINFO->bsbufLen<LEFTFILELEN))  //
		{
			memcpy(BSINFO->BsBuf,BSINFO->bsbufptr,BSINFO->bsbufLen);
			readLen = fread(&BSINFO->BsBuf[BSINFO->bsbufLen],1,READFILELEN,BSINFO->ReadFile);
			if(AIF->length>0)
			{
				if(readLen+AIF->readlen> AIF->length)
				{
					readLen = AIF->length - AIF->readlen;
				}
			}
			AIF->readlen += readLen;
			BSINFO->bsbufLen += readLen;//READFILELEN;
			BSINFO->bsbufptr = BSINFO->BsBuf;
		}
	}
	
	RetVal = 0;
	if(ByteLen > BSINFO->bsbufLen)
		ByteLen = BSINFO->bsbufLen;
	for	(i=0;i<ByteLen;i++)
	{	
		data = *BSINFO->bsbufptr++;
		BSINFO->bsbufLen-- ;
		RetVal = RetVal|((data&0xff)<<(i*8));
	}
	GetInfo_return = ByteLen;
	return	RetVal;
}

/**
*
*	skip	n byte from bs 
*/
unsigned	int	GetInfo_SkipBsInByte(int	ByteLen,__audio_file_info_t *AIF)
{
	int	Flen;
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	if (ByteLen<0)
		return 0;
	if(BSINFO->bsbufLen>=ByteLen)
	{
		BSINFO->bsbufLen -=	ByteLen;
		BSINFO->bsbufptr	+=	ByteLen;
	}
	else
	{
		if(AIF->ulbufflag)
		{
			BSINFO->bsbufLen =	0;
			return 0;
		}
		ByteLen -= BSINFO->bsbufLen;
		fseek(BSINFO->ReadFile,ByteLen,SEEK_CUR);
		AIF->readlen += ByteLen;
		if(AIF->readlen>AIF->length)
		{
			AIF->readlen = AIF->length;
		}
		else if(AIF->readlen<0)
		{
			fseek(BSINFO->ReadFile,AIF->offset,SEEK_SET);
			AIF->readlen = 0;
		}
		Flen = fread(&BSINFO->BsBuf[0],1,READFILELEN,BSINFO->ReadFile);
		if(AIF->length>0)
		{
			if(Flen+AIF->readlen> AIF->length)
			{
				Flen = AIF->length - AIF->readlen;
			}
		}
		AIF->readlen += Flen;
		BSINFO->bsbufLen = Flen;//READFILELEN;
		BSINFO->bsbufptr = BSINFO->BsBuf;
	}

	return	1;
}


int GetInfo_ExtractI4( char *buf)
{
int x;
// big endian extract

x = buf[0]&0xff;
x <<= 8;
x |= buf[1]&0xff;
x <<= 8;
x |= buf[2]&0xff;
x <<= 8;
x |= buf[3]&0xff;

return x;
}

int		GetInfo_UidComPare(unsigned int *Sour1,const unsigned int *Sour2,int length)
{	
	int	i=0;
	int	Result=0;
	for(i=0;i<length;i++)
	{
		if(Sour1[i] != Sour2[i])
			break;
	}
	if(i == length)
		Result = 1;
	return Result;
	
}

int		GetInfo_CHARComPare(unsigned char *Sour1,const unsigned char *Sour2,int length)
{	
	int	i=0;
	int	Result=0;
	for(i=0;i<length;i++)
	{
		if(Sour1[i] != Sour2[i])
			break;
	}
	if(i == length)
		Result = 1;
	return Result;
	
}

unsigned	int	GetInfo_ShowBs(int skipLen,int ByteLen,__audio_file_info_t *AIF)
{//显示从当前位置偏移skipLen 长度（byte）的byte
	//skipLen>=0,0<=ByteLen<5;
	unsigned	int	RetVal;
	char		data;
	int			i;
	char		*ptr;
	int			Flen;
  char ptrdata[16];
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
  

	if(BSINFO->bsbufLen<(skipLen+ByteLen))  //
	{
		if(AIF->ulbufflag)return 0;//no data;
		memcpy(BSINFO->BsBuf,BSINFO->bsbufptr,BSINFO->bsbufLen);
		Flen = fread(&BSINFO->BsBuf[BSINFO->bsbufLen],1,READFILELEN-BSINFO->bsbufLen,BSINFO->ReadFile);
		if(AIF->length>0)
		{
			if(Flen+AIF->readlen> AIF->length)
			{
				Flen = AIF->length - AIF->readlen;
			}
		}
		AIF->readlen += Flen;
		if(Flen==0)
		{
			GetInfo_return = -1;
			return 0xffffffff;//error
		}
		BSINFO->bsbufLen += Flen;//READFILELEN;
		BSINFO->bsbufptr = BSINFO->BsBuf;
	}
	//是否skipLen越界
	if(BSINFO->bsbufLen<(skipLen+ByteLen))
	{		
		int readlenth = 0;
		if(AIF->ulbufflag)return 0;//no data;
		Flen = skipLen - BSINFO->bsbufLen;
		if((AIF->length>0)&&(Flen+ AIF->readlen>AIF->length))
		{
			GetInfo_return = -1;
			return 0xffffffff;//error
		}
		readlenth=fseek(BSINFO->ReadFile,Flen,SEEK_CUR);
		if(readlenth)
		{
			GetInfo_return = -1;
			return 0xffffffff;//error
		}
		if(ByteLen>16)ByteLen = 16;
		readlenth = fread(ptrdata,1,ByteLen,BSINFO->ReadFile);
		if(AIF->length>0)
		{
			if(readlenth + Flen + AIF->readlen>AIF->length)
			{
				readlenth = AIF->length - AIF->readlen - Flen;
			}
		}
		if(readlenth<ByteLen)
		{
			GetInfo_return = -1;
			return 0xffffffff;//error
		}
		fseek(BSINFO->ReadFile,-Flen-readlenth,SEEK_CUR);
		ptr = ptrdata;
	}
	else
	{
		ptr=BSINFO->bsbufptr;
		ptr += skipLen;
	}
	

	RetVal = 0;
	for	(i=0;i<ByteLen;i++)
	{	
		data = *ptr++;
		RetVal = (RetVal<<8)|(data&0xff);
	}
	GetInfo_return = ByteLen;
	return	RetVal;
}

int	GetInfo_StringLen(char *StringPtr)
{
	unsigned int Len = 0;
	while(*StringPtr++!=0)
	{
		Len++;
	}
	return Len;
}
