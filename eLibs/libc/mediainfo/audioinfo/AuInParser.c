/**
	audio parser:  
	input :file name
	output:total time,
**/

//for test
//in AudioInfor,only malloc AudioInfor.InforBuf pointer (buffer>=1024bytes)


#include "osPlatform.h"
#include "AuInParser.h"
#include "GetAudio_format.h"

#define		INFLEN	1024

static  __audio_file_info_t	AudioInfor;       
static	__s8	infor[INFLEN]; 

int main(int argc, char* argv[])
{
	int ret;
	int audio_format;
	//argv[1] = "dukou.ogg";
	ret = GetAudioFormat(argv[1],&audio_format);
	if(ret!=1)return 0;
	
	memset(&AudioInfor, 0, sizeof(AudioInfor));
	memset(&infor, 0,INFLEN);
	AudioInfor.InforBufLeftLength = INFLEN;
	AudioInfor.InforBuf = infor;

    ret = 	GetAudioInfo(argv[1],&AudioInfor);
	return ret;

}

