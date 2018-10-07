
#ifndef		__BSHEAD
#define		__BSHEAD
#include "osPlatform.h"
#include "AuInParser.h"
#define SUCCESS 1
#define ERROR   0
#define		READFILELEN 4096  //2048
#define     LEFTFILELEN 100
#define ERRORFAMENUM 10
typedef	struct _bs
{
	char	*bsbufptr;
	int		bsbufLen;
	char    *BsBuf;   
	//char	BsBuf[4*1024+10];
	FILE	*ReadFile;
}BsInfo;
extern int GetInfo_return;
int GetInfo_GetFileLocation(__audio_file_info_t *AIF);
unsigned	int	GetInfo_SkipBsInByte(int	ByteLen,__audio_file_info_t *AIF);
unsigned	int	GetInfo_GetBsInByteLI(int	ByteLen,__audio_file_info_t *AIF);//Little-endian   ByteLen<=4
unsigned	int	GetInfo_GetBsInByte(int	ByteLen,__audio_file_info_t *AIF);//Big-endian ByteLen<=4
unsigned	int	GetInfo_ShowBs(int skipLen,int ByteLen,__audio_file_info_t *AIF);//show 从当前位置偏远的byte
int	GetInfo_StringLen(char *StringPtr);
int	GetInfo_ExtractI4( char *buf);//Read 4byte and don't modify bs ptr
__int64	GetInfo_BsInit(FILE *fileptr,__audio_file_info_t *AIF);  //init bs buffer infor ,and return file length
int	GetInfo_BsEndRelease(__audio_file_info_t *AIF);//release AIF->ulBSINFO;
int	GetInfo_UidComPare(unsigned int *Sour1,const unsigned int *Sour2,int length);  //compare int type data buffer ,if equal return 1 else 0
int	GetInfo_CHARComPare(unsigned char *Sour1,const unsigned char *Sour2,int length);


#endif
