/***************************************
*	Windows Medio audio infor 
*	
*
*	date:  2008/10/10  author: fufo
*	copyright 2100 AllWinner.LTD
*   2008-12-03 change play during for sendduration not playduration
*   2009-1-7 15:26:46 add wma Professional lossless
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"


	
typedef struct	_WMAOBJECT_STRUCT
{
	unsigned int	UID[4];
	int	size[2];
}_WMAOBJECT;//WMAObjectframe;

static const unsigned int	WMAHEADOBJ[7][4]=
{
	{0x3026b275,0x8e66cf11,0xa6d900aa,0x0062ce6c},//  header_object UUID
    //{0x409e69f8,0x4d5bcf11,0xa8fd0080,0x5f5c442b},//audio object
    //0x818d0d47, 0x43, 0x4451, 0x92, 0x37, 0xda, 0xef, 0xbc, 0x48, 0x4f, 0x50);
    {0xa1dcab8c,0x92514443,0xbcefda37,0x504f48bc},//CLSID_CAsfPropertiesObjectV1
	// 0xb7dc0791, 0xa9b7, 0x11cf, 0x8e, 0xe6, 0x0, 0xc0, 0xc, 0x20, 0x53, 0x65);
	{0x9107dcb7,0xb7a9cf11,0x8ee600c0,0x0c205365},//CLSID_CAsfPropertiesObjectV2		
	{0x3326b275,0x8e66cf11,0xa6d900aa,0x0062ce6c},  //content object
	{0xa1dcab8c,0x47a9cf11,0x8ee400c0,0x0c205365}, //Properties object
	
    //0xf8699e40,0x5b4d,0x11cf,0xa8,0xfd,0x00,0x80,0x5f,0x5c,0x44,0x2b  //CLSID_AsfXStreamTypeAcmAudio
	{0x409e69f8,0x4d5bcf11,0xa8fd0080,0x5f5c442b },
	{0x40a4d0d2,0x07e3d211,0x97f000a0,0xc95ea850 } //wma extended object
};
static const char WMAExtPicture[0x16] ={0x57,0x00 ,0x4d,0x00,0x2f,0x00,0x50,0x00,0x69,0x00,0x63,0x00,0x74,0x00,0x75,0x00,0x72,0x00,0x65,0x00,0x00,0x00};
static const char WMAExtAlbumTitle[0x1c] ={'W',0x00 ,'M',0x00,'/',0x00,'A',0x00,'l',0x00,'b',0x00,'u',0x00,'m',0x00,'T',0x00,'i',0x00,'t',0x00,'l',0x00,'e',0x00,0x00,0x00};
static const char WMAExtYear[0x10]  ={'W',0x00 ,'M',0x00,'/',0x00,'Y',0x00,'e',0x00,'a',0x00,'r',0x00,0x00,0x00};
static const char WMAExtGenre[0x12] ={'W',0x00 ,'M',0x00,'/',0x00,'G',0x00,'e',0x00,'n',0x00,'r',0x00,'e',0x00,0x00,0x00};
//WM/PICTURE
static const char WMAExtImag[4][32] ={
	{'i',0x00 ,'m',0x00,'a',0x00,'g',0x00,'e',0x00,'/',0x00,'b',0x00,'m',0x00,'p',0x00,0x00,0x00,0x00,0x00},
	{'i',0x00 ,'m',0x00,'a',0x00,'g',0x00,'e',0x00,'/',0x00,'j',0x00,'p',0x00,'e',0x00,'g',0x00,0x00,0x00,0x00,0x00},
	{'i',0x00 ,'m',0x00,'a',0x00,'g',0x00,'e',0x00,'/',0x00,'g',0x00,'i',0x00,'f',0x00,0x00,0x00,0x00,0x00},
	{'i',0x00 ,'m',0x00,'a',0x00,'g',0x00,'e',0x00,'/',0x00,'p',0x00,'n',0x00,'g',0x00,0x00,0x00,0x00,0x00},
};
static const short WMAExtImagLen[4] ={22-4,24-4,22-4,22-4};

static const char WMAcodecInfo[4][31]=
{	
	"Window Media Audio-Standard",
	"Window Media Audio-Professional",
	"Window Media Audio-lossless",
	"unknow format"
};
static int	ReadObject(__audio_file_info_t *AIF,_WMAOBJECT *WMAObjectframe);

int GetInfo_WMA(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{	
	int Ret = 0;
	int	Object=0;
	int i,k;
	__int64 filelength;
	int formatIndex  = 0;
	_WMAOBJECT WMAObjectframe;
	//set font type=unicode
	AIF->ulCharEncode = 1;
	filelength = GetInfo_BsInit(bs,AIF);
	if(filelength <= 0) return ERROR;
	ReadObject(AIF,&WMAObjectframe);  //GUID guid;
	if(GetInfo_UidComPare(WMAObjectframe.UID,WMAHEADOBJ[0],4) )
	{
		int objnumber;
		objnumber = (int)GetInfo_GetBsInByteLI(4,AIF);  //DWORD number_headers;
		GetInfo_SkipBsInByte(2,AIF);
		for(k=0;k<objnumber;k++)
		{	
			if(!ReadObject(AIF,&WMAObjectframe))
			{
				if ((AIF->ulDuration != 0) && AIF->ulbufflag)
				{
					Ret = 1;
				}
				goto WMA_error;
			}
			if((GetInfo_UidComPare(WMAObjectframe.UID,WMAHEADOBJ[1],4))|(GetInfo_UidComPare(WMAObjectframe.UID,WMAHEADOBJ[2],4)))
			{
				unsigned int nChannels,nSamplesPerSec,/*nBlockAlign,*/wBitsPerSample,nAvgBytesPerSec;
				unsigned int streamType[4],cbTypeSpecific;
				Object++;
				streamType[0] = GetInfo_GetBsInByte(4,AIF);
				streamType[1] = GetInfo_GetBsInByte(4,AIF);
				streamType[2] = GetInfo_GetBsInByte(4,AIF);
				streamType[3] = GetInfo_GetBsInByte(4,AIF);
				WMAObjectframe.size[0] -=(16);
				if(!GetInfo_UidComPare(streamType,WMAHEADOBJ[5],4) )
				{
					goto audiobjectexit;
				}
				GetInfo_SkipBsInByte(16+8,AIF);
				WMAObjectframe.size[0] -= (16+8);
				
				cbTypeSpecific = GetInfo_GetBsInByteLI(4,AIF);
				GetInfo_SkipBsInByte(10,AIF);
				WMAObjectframe.size[0] -= (10+4);

				if(cbTypeSpecific>14)
				{
	#define WAVE_FORMAT_MSAUDIO1  0x0160
	#define WAVE_FORMAT_WMAUDIO2  0x0161
	#define WAVE_FORMAT_WMAUDIO3  0x0162
	#define WAVE_FORMAT_WMAUDIO4  0x0163
    #define WAVE_FORMAT_WMAMPEG3  0x0055
				short FORMAT_WMAUDI;
				FORMAT_WMAUDI = GetInfo_GetBsInByteLI(2,AIF);
				//GetInfo_GetBsInByteLI(2,AIF);
				switch(FORMAT_WMAUDI)
				{
				case WAVE_FORMAT_MSAUDIO1:
				case WAVE_FORMAT_WMAUDIO2:
						formatIndex = 0;
						break;
				case WAVE_FORMAT_WMAUDIO3:
						formatIndex = 1;
						break;
				case WAVE_FORMAT_WMAUDIO4:
						formatIndex = 2;
						break;
				case WAVE_FORMAT_WMAMPEG3:
					        AIF->ulFormat= SW_AUDIO_FORMAT_MP3 ;
				default:
						formatIndex = 3;
						break;


				}
				nChannels =  GetInfo_GetBsInByteLI(2,AIF);
				nSamplesPerSec = GetInfo_GetBsInByteLI(4,AIF);
				nAvgBytesPerSec = GetInfo_GetBsInByteLI(4,AIF);
				//nBlockAlign = GetInfo_GetBsInByteLI(2,AIF);
				GetInfo_GetBsInByteLI(2,AIF);
				wBitsPerSample = 16;


				AIF->ulSampleRate = nSamplesPerSec;
				AIF->ulBitRate = nAvgBytesPerSec*8;
				AIF->ulBitsSample = wBitsPerSample;
				AIF->ulChannels = nChannels;

				WMAObjectframe.size[0] -=14;
				}
audiobjectexit:
				GetInfo_SkipBsInByte(WMAObjectframe.size[0],AIF);
			}
			else if(GetInfo_UidComPare(WMAObjectframe.UID,WMAHEADOBJ[3],4))
			{
				char data;
				Object++;
				AIF->ultitle_sz = GetInfo_GetBsInByteLI(2,AIF);  //WORD title_len; 
				AIF->ulauthor_sz = GetInfo_GetBsInByteLI(2,AIF);  //WORD author_len; 
				AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
				AIF->ulauthorCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;

				GetInfo_SkipBsInByte(6,AIF);// copyright_len;  description_len;  rating_len;

///read title
				AIF->ultitle = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ultitle_sz)
					goto WMA_error;
				AIF->InforBufLeftLength -=AIF->ultitle_sz;
				for(i=0;i<AIF->ultitle_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					*AIF->InforBuf++= data;
				}
///read author
				AIF->ulauthor = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ulauthor_sz)
					goto WMA_error;
				AIF->InforBufLeftLength -=AIF->ulauthor_sz;
				for(i=0;i<AIF->ulauthor_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					*AIF->InforBuf++= data;
				}
				GetInfo_SkipBsInByte(WMAObjectframe.size[0]-10-AIF->ultitle_sz-AIF->ulauthor_sz,AIF);
			}
			else if (GetInfo_UidComPare(WMAObjectframe.UID,WMAHEADOBJ[4],4))
			{	
				unsigned int play_durationL,play_durationH;
				unsigned int Preoll_L,Preoll_H;

				uint64_t duration,Preoll;

				Object++;
				GetInfo_SkipBsInByte(48-8,AIF);
				play_durationL = GetInfo_GetBsInByteLI(4,AIF);//play duration
				play_durationH = GetInfo_GetBsInByteLI(4,AIF);

				GetInfo_SkipBsInByte(8,AIF);//send duration
				Preoll_L = GetInfo_GetBsInByteLI(4,AIF);//preroll
				Preoll_H = GetInfo_GetBsInByteLI(4,AIF);

				duration = (uint64_t)play_durationH<<32|(play_durationL&0xffffffff);
				Preoll = (uint64_t)Preoll_H<<32|(Preoll_L&0xffffffff);
				Preoll *= 10000;
				duration -= Preoll;
				AIF->ulDuration = (unsigned int)(duration/10000);
				GetInfo_SkipBsInByte(WMAObjectframe.size[0]-(48+8+8),AIF);

			}else if (GetInfo_UidComPare(WMAObjectframe.UID,WMAHEADOBJ[6],4))//extended object
			{	
				struct 
				{
					int Size1;
					char  Name[50];
					int flag;
					int Size2;
				}ExSection;
				int ExNo = GetInfo_GetBsInByteLI(2,AIF);
				int i2 ,j;
				int	ExReadLen = WMAObjectframe.size[0] /*- 16 - 8*/- 2;

				for(i2=0;i2<ExNo/*AIF->ulauthor_sz*/;i2++)
				{
					if(ExReadLen <= 0)
						break;
					ExSection.Size1 = GetInfo_GetBsInByteLI(2,AIF);
					for(j=0;j<ExSection.Size1;j++)
					{
						if(j<50)
							ExSection.Name[j]= (char )GetInfo_GetBsInByte(1,AIF);
						else
							GetInfo_GetBsInByte(1,AIF);
					}
					ExSection.flag = GetInfo_GetBsInByteLI(2,AIF);
					ExSection.Size2 = GetInfo_GetBsInByteLI(2,AIF);
					ExReadLen = ExReadLen - 6 - ExSection.Size1 - ExSection.Size2;
					if((ExSection.Size1 == 0x16) && GetInfo_CHARComPare((unsigned char*)(ExSection.Name),(const unsigned char*)WMAExtPicture,0x16))
					{
						__id3_image_info_t image;
						AIF->ulAPic_sz = sizeof(__id3_image_info_t);
						AIF->ulAPicCharEncode = (__a_audio_fonttype_e) GetInfo_GetBsInByte(1,AIF);
						memset(&image,0,sizeof(__id3_image_info_t));
						image.length = GetInfo_GetBsInByteLI(4,AIF);
						ExSection.Size2 -= 5;
						ExSection.Size1 = 0;
						while(ExSection.Size2)
						{
							short tempx = GetInfo_ShowBs(0,2,AIF);
							if(tempx ==0)
							{
								break;
							}
							if(ExSection.Size1<24)
								ExSection.Name[ExSection.Size1++]=GetInfo_GetBsInByte(1,AIF);
							else
								GetInfo_GetBsInByte(1,AIF);
							ExSection.Size2 -= 1;
						}

						while(ExSection.Size2)
						{
							char tempx = GetInfo_ShowBs(0,1,AIF);
							if(tempx !=0)
								break;
							if(ExSection.Size1<24)
								ExSection.Name[ExSection.Size1++]=GetInfo_GetBsInByte(1,AIF);
							else
								GetInfo_GetBsInByte(1,AIF);
							ExSection.Size2 -= 1;

						}


						for(j=0;j<4;j++)
						{	
							if(GetInfo_CHARComPare((unsigned char *)(ExSection.Name),(const unsigned char*)(WMAExtImag[j]),WMAExtImagLen[j]))
							{
								break;

							}

						}
					image.img_format = (__w_img_format_t)j;

					image.FileLocation = GetInfo_GetFileLocation(AIF);
					//lszhang I do not know what it is,jump ,
					image.FileLocation += ExSection.Size2 - image.length;
					if(image.FileLocation < 0)image.FileLocation = 0;
					AIF->InforBufLeftLength -= 4-((int)(AIF->InforBuf)&0x3);
					AIF->InforBuf += 4-((int)(AIF->InforBuf)&0x3);
					AIF->ulAPic = (__id3_image_info_t*)(AIF->InforBuf);
					if(AIF->InforBufLeftLength<AIF->ulAPic_sz)
						break;				
					AIF->InforBufLeftLength -=AIF->ulAPic_sz;
					memcpy(AIF->ulAPic,&image,AIF->ulAPic_sz);
					AIF->InforBuf += AIF->ulAPic_sz;
					GetInfo_SkipBsInByte(ExSection.Size2,AIF);


					}
					else if((ExSection.Size1 == 0x1c) && GetInfo_CHARComPare((unsigned char*)(ExSection.Name),(const unsigned char*)WMAExtAlbumTitle,0x1c))
					{
						AIF->ulAlbum_sz = ExSection.Size2;  //WORD Album_len;
						AIF->ulAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_16LE; 
						AIF->ulAlbum = AIF->InforBuf;
						if(AIF->InforBufLeftLength<AIF->ulAlbum_sz)
							goto WMA_error;
						AIF->InforBufLeftLength -=AIF->ulAlbum_sz;
						for(j=0;j<AIF->ulAlbum_sz;j++)
						{
							*AIF->InforBuf++= (char )GetInfo_GetBsInByte(1,AIF);
						}
						
					}
					else if((ExSection.Size1 == 0x10) && GetInfo_CHARComPare((unsigned char*)(ExSection.Name),(const unsigned char*)WMAExtYear,0x10))
					{
						AIF->ulYear_sz = ExSection.Size2;  //WORD Year_len;
						AIF->ulYearCharEncode = A_AUDIO_FONTTYPE_UTF_16LE; 
						AIF->ulYear = AIF->InforBuf;
						if(AIF->InforBufLeftLength<AIF->ulYear_sz)
							goto WMA_error;
						AIF->InforBufLeftLength -=AIF->ulYear_sz;
						for(j=0;j<AIF->ulYear_sz;j++)
						{
							*AIF->InforBuf++= (char )GetInfo_GetBsInByte(1,AIF);
						}
						
					}
					else if((ExSection.Size1 == 0x12) && GetInfo_CHARComPare((unsigned char*)(ExSection.Name),(const unsigned char*)WMAExtGenre,0x12))
					{
						AIF->ulGenre_sz = ExSection.Size2;  //WORD Genre_len;
						AIF->ulGenreCharEncode = A_AUDIO_FONTTYPE_UTF_16LE; 
						AIF->ulGenre = AIF->InforBuf;
						if(AIF->InforBufLeftLength<AIF->ulGenre_sz)
							goto WMA_error;
						AIF->InforBufLeftLength -=AIF->ulGenre_sz;
						for(j=0;j<AIF->ulGenre_sz;j++)
						{
							*AIF->InforBuf++= (char )GetInfo_GetBsInByte(1,AIF);
						}
						
					}
					else
					{
						GetInfo_SkipBsInByte(ExSection.Size2,AIF);
						ExSection.Size2 -= ExSection.Size2;
					}
					

				}
				GetInfo_SkipBsInByte(ExReadLen,AIF);

			}else
			{
				GetInfo_SkipBsInByte(WMAObjectframe.size[0],AIF);
			}
			
		//	if(Object>=3)
			//	break;
		}
		Ret = 1;
		
		if(AIF->InforBufLeftLength>=31)
		{
			AIF->ulAudio_name_sz = 31;
			AIF->ulAudio_name = AIF->InforBuf;
			AIF->InforBuf +=AIF->ulAudio_name_sz;
			AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
			memcpy(AIF->ulAudio_name,WMAcodecInfo[formatIndex],AIF->ulAudio_name_sz);
			
		}
		
		
	}
	else
		Ret = 0;
		GetInfo_BsEndRelease(AIF);
	return Ret;
WMA_error:
	 GetInfo_BsEndRelease(AIF);	
	 return Ret;
}

static int	ReadObject(__audio_file_info_t *AIF,_WMAOBJECT *WMAObjectframe)
{
		WMAObjectframe->UID[0] = GetInfo_GetBsInByte(4,AIF);  //GUID guid;
		WMAObjectframe->UID[1] = GetInfo_GetBsInByte(4,AIF);
		WMAObjectframe->UID[2] = GetInfo_GetBsInByte(4,AIF);
		WMAObjectframe->UID[3] = GetInfo_GetBsInByte(4,AIF);

		WMAObjectframe->size[0] = GetInfo_GetBsInByteLI(4,AIF);  //QWORD size;
		WMAObjectframe->size[1] = GetInfo_GetBsInByteLI(4,AIF);
		WMAObjectframe->size[0] -=24;
		if(GetInfo_return<4)
		{
			return 0;
		}
		else
		{
			return 1;
		}

}
