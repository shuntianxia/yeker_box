
#ifndef	_TAGINFO_
#define	_TAGINFO_
/*popular tag information read*/
int GetInfo_Language_encoding(char*ulcomm);
int GetInfo_encoding_change(__audio_file_info_t *AIF);
int GetInfo_GETID3_Len(unsigned char *ptr,int len);
unsigned	int	GetInfo_GETID3_V2(__audio_file_info_t *AIF);
int			GetInfo_GETTAG_XING(int Mpeg12flag/*0==mpeg1  other==mpeg2/2.5*/,int channel,char *ptr);
int			GetInfo_GETTAG_VBRI(char *ptr); //VBRI tag --use for mp3
unsigned	int	GetInfo_GETID3_V1(__audio_file_info_t *AIF);
unsigned int GetInfo_oggStandardTag(__audio_file_info_t *AIF);
int GetInfo_APETAG(__audio_file_info_t *AIF);
#endif
