/***************************************
*	Windows Medio audio infor 
*	
*
*	date:  2008/10/10  author: fufo
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"


#define wave_format_g723_adpcm				0x0014 /* antex electronics corporation */ 
#define wave_format_antex_adpcme			0x0033 /* antex electronics corporation */ 
#define wave_format_g721_adpcm				0x0040 /* antex electronics corporation */ 
#define wave_format_aptx					0x0025 /* audio processing technology */ 
#define wave_format_audiofile_af36			0x0024 /* audiofile, inc. */ 
#define wave_format_audiofile_af10			0x0026 /* audiofile, inc. */ 
#define wave_format_control_res_vqlpc		0x0034 /* control resources limited */ 
#define wave_format_control_res_cr10		0x0037 /* control resources limited */ 
#define wave_format_creative_adpcm			0x0200 /* creative labs, inc */ 
#define wave_format_dolby_ac2				0x0030 /* dolby laboratories */ 
#define wave_format_dspgroup_truespeech		0x0022 /* dsp group, inc */ 
#define wave_format_digistd					0x0015 /* dsp solutions, inc. */ 
#define wave_format_digifix					0x0016 /* dsp solutions, inc. */ 
#define wave_format_digireal				0x0035 /* dsp solutions, inc. */ 
#define wave_format_digiadpcm				0x0036 /* dsp solutions, inc. */ 
#define wave_format_echosc1					0x0023 /* echo speech corporation */ 
#define wave_format_fm_towns_snd			0x0300 /* fujitsu corp. */ 
#define wave_format_ibm_cvsd				0x0005 /* ibm corporation */ 
#define wave_format_oligsm					0x1000 /* ing c. olivetti & c., s.p.a. */ 
#define wave_format_oliadpcm				0x1001 /* ing c. olivetti & c., s.p.a. */ 
#define wave_format_olicelp					0x1002 /* ing c. olivetti & c., s.p.a. */ 
#define wave_format_olisbc					0x1003 /* ing c. olivetti & c., s.p.a. */ 
#define wave_format_oliopr					0x1004 /* ing c. olivetti & c., s.p.a. */ 
#define wave_format_ima_adpcm				(wave_form_dvi_adpcm) /* intel corporation */ 
#define wave_format_dvi_adpcm				0x0011 /* intel corporation */ 
#define wave_format_unknown					0x0000 /* microsoft corporation */ 
#define wave_format_pcm						0x0001 /* microsoft corporation */ 
#define wave_format_adpcm					0x0002 /* microsoft corporation */ 
#define wave_format_alaw					0x0006 /* microsoft corporation */ 
#define wave_format_mulaw					0x0007 /* microsoft corporation */ 
#define wave_format_gsm610					0x0031 /* microsoft corporation */ 
#define wave_format_mpeg					0x0050 /* microsoft corporation */ 
#define wave_format_nms_vbxadpcm			0x0038 /* natural microsystems */ 
#define wave_format_oki_adpcm				0x0010 /* oki */ 
#define wave_format_sierra_adpcm			0x0013 /* sierra semiconductor corp */ 
#define wave_format_sonarc					0x0021 /* speech compression */ 
#define wave_format_mediaspace_adpcm		0x0012 /* videologic */ 
#define wave_format_yamaha_adpcm			0x0020 /* yamaha corporation of america */ 
#define WAVE_FORMAT_MPEGLAYER3				0x0055 /* MPEG Layer 3 */
#define WAVE_FORMAT_EXTENSIBLE				0xFFFE /* SubFormat */

enum{
	WAV_UNKNOWN = 0,
	WAV_PCM,
	WAV_DVI_ADPCM,
	WAV_ALAW,
	WAV_MULAW,
	WAV_RIFX,
	WAV_ADPCM,
	WAV_GSM610,
	WAV_DTS,
	WAV_MP3,
	WAV_UNSPPORT
};
#define WAVCODEINFOLENGTH 15
static const char wavcodecInfo[][WAVCODEINFOLENGTH]=
{
	"WAV Unknown",	
	"WAV PCM",
	"WAV DVI_ADPCM",
	"WAV ALaw",
	"WAV MULaw",
	"WAV RIFX",
	"WAV ADPCM",
	"WAV GSM610",
	"WAV DTS",
	"WAV MP3",
	"WAV Unspport"
};
	
typedef struct WAVE_FORMAT_STRUCT
{
	short wFormatTag;
	short wChannels;
	__u32 dwSamplesPerSec;
	__u32 dwAvgBytesPerSec;
	short wBlockAlign;
	short wBitsPerSample;
	short wSamplePerBlock;//for adpcm or have SamplePerBlock
}WAVE_FORMAT;//WAVEFORMAT;



typedef struct	_WAVOBJECT_STRUCT
{
	__u32	UID;
	__s32	Dsize;
}_WAVOBJECT;//WAVObject;

#define RIFX    0X52494658
#define	RIFF	0X52494646
#define	WAVE	0X57415645
#define	fmt		0x666d7420
#define	fact	0x66616374
#define	data	0x64617461	
static int	WAVReadObject(__audio_file_info_t *AIF,_WAVOBJECT *WAVObject);

int GetInfo_WAV(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{	
	__u32	Durationadpcm;  
	int Ret = 0;
//	int ID3Len;//cky add 
	//int readlength=0;//cky add 
	__int64	filelength = 0;
	int i ;
	int	TempUid;
	int	Findallflag = 0;
	int wavcode_info = 0;
	int InforBufLeftLengthCopy = AIF->InforBufLeftLength;
	WAVE_FORMAT WAVEFORMAT;
	_WAVOBJECT WAVObject;
	filelength = GetInfo_BsInit(bs,AIF);
	if(filelength <=0) return ERROR;
#if 0//cky add 
	ID3Len=GetInfo_GETID3_V2(AIF);//if it have id3.
	//readlength = ID3Len;
#endif
	WAVReadObject(AIF,&WAVObject);  //GUID guid;
	memset(&WAVEFORMAT,0,sizeof(WAVEFORMAT));
	if(WAVObject.UID ==RIFF)
	{	
		TempUid = GetInfo_GetBsInByte(4,AIF);
		if(TempUid == WAVE)
		{
			while(1)
			{
				if(!WAVReadObject(AIF,&WAVObject))
				{
					Ret = 0;
					break;
				}
				switch(WAVObject.UID)
				{
				case fmt:
					{
						Ret =1;
						WAVEFORMAT.wFormatTag = GetInfo_GetBsInByteLI(2,AIF);
						{
							switch(WAVEFORMAT.wFormatTag)
							{
								case wave_format_unknown:
									{
									   wavcode_info = WAV_UNKNOWN;
									   break;
									}
								case wave_format_pcm:
								case (short)WAVE_FORMAT_EXTENSIBLE:
									{
									   wavcode_info = WAV_PCM;
									   break;
									}
								case wave_format_adpcm:
									{
									   wavcode_info = WAV_ADPCM;
									   break;
									}
								case wave_format_dvi_adpcm:
									{
									   wavcode_info = WAV_DVI_ADPCM;
									   break;
									}
								case wave_format_alaw:
									{
									   wavcode_info = WAV_ALAW;
									   break;
									}
								case wave_format_mulaw:
									{
									   wavcode_info = WAV_MULAW;
									   break;
									}
								case wave_format_gsm610:
									{
									   wavcode_info = WAV_GSM610;
									   break;
									}
								case WAVE_FORMAT_MPEGLAYER3:
									{
										wavcode_info = WAV_MP3;
										AIF->ulFormat = SW_AUDIO_FORMAT_MP3;
										break;
									}
								default:
									{
									   wavcode_info = WAV_UNSPPORT;
									   break;
									}
								
							}
						}
						WAVEFORMAT.wChannels = GetInfo_GetBsInByteLI(2,AIF);
						WAVEFORMAT.dwSamplesPerSec = GetInfo_GetBsInByteLI(4,AIF);
						WAVEFORMAT.dwAvgBytesPerSec = GetInfo_GetBsInByteLI(4,AIF);
						WAVEFORMAT.wBlockAlign = GetInfo_GetBsInByteLI(2,AIF);
						WAVEFORMAT.wBitsPerSample = GetInfo_GetBsInByteLI(2,AIF);
						AIF->ulChannels = WAVEFORMAT.wChannels;
						AIF->ulBitRate = WAVEFORMAT.dwAvgBytesPerSec*8;
						AIF->ulBitsSample = WAVEFORMAT.wBitsPerSample;//16;
						AIF->ulSampleRate = WAVEFORMAT.dwSamplesPerSec;
						
						WAVObject.Dsize -= 16;
						if(WAVObject.Dsize>=4)
						{
							GetInfo_SkipBsInByte(2,AIF);
							WAVEFORMAT.wSamplePerBlock = GetInfo_GetBsInByteLI(2,AIF);
							WAVObject.Dsize -= 4;
						}
						Findallflag++;
						if(Findallflag>=2)
							goto wavExit;
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);
						break;
					}
				case fact:
					{
						int datafactsize = GetInfo_GetBsInByteLI(4,AIF);
						if(WAVEFORMAT.dwSamplesPerSec)
						{
							Durationadpcm = (int)((__int64)datafactsize*1000/WAVEFORMAT.dwSamplesPerSec);
						}
						WAVObject.Dsize -=4;
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);
						break;
					}
				case data:
					{
						unsigned char ptr[6] = {0};
						Ret =1;
						//if dvi_adpcm wSamplePerBlock count out
						if((WAVEFORMAT.wFormatTag == wave_format_dvi_adpcm)&&(WAVEFORMAT.wChannels))
						{
							WAVEFORMAT.wSamplePerBlock = ((WAVEFORMAT.wBlockAlign-4* WAVEFORMAT.wChannels)*2+ WAVEFORMAT.wChannels)/ WAVEFORMAT.wChannels;
						}
						//if not fact
						if(AIF->ulDuration==0)
						{
							if(  ( (WAVEFORMAT.wFormatTag == wave_format_pcm)||(WAVEFORMAT.wFormatTag == (short)WAVE_FORMAT_EXTENSIBLE)||(WAVEFORMAT.wFormatTag == wave_format_alaw)||(WAVEFORMAT.wFormatTag == wave_format_mulaw) )
										&& (WAVEFORMAT.dwSamplesPerSec) )
							{
								AIF->ulDuration = (int)((__int64)(WAVObject.Dsize/(WAVEFORMAT.wChannels*WAVEFORMAT.wBitsPerSample/8))*1000/WAVEFORMAT.dwSamplesPerSec);
							}
							else if(WAVEFORMAT.wSamplePerBlock)
							{
								AIF->ulDuration = (int)((__int64)WAVObject.Dsize*1000*WAVEFORMAT.wSamplePerBlock/((__int64)WAVEFORMAT.wBlockAlign*WAVEFORMAT.dwSamplesPerSec));
							}
							else
							{
								if(WAVEFORMAT.dwAvgBytesPerSec)
								{
									AIF->ulDuration = (int)((__int64)WAVObject.Dsize*1000/WAVEFORMAT.dwAvgBytesPerSec);
								}
							}
						}
						if(abs(AIF->ulDuration-Durationadpcm)<=(int)((__int64)WAVEFORMAT.wSamplePerBlock*1000/AIF->ulSampleRate))
						{
							AIF->ulDuration=Durationadpcm;
						}
						Findallflag++;
						//other music check dts...
						for(i = 0; i < WAVObject.Dsize;i++)
						{
							if(GetInfo_ShowBs(0,1,AIF))
								break;
							GetInfo_SkipBsInByte(1,AIF);

						}
						ptr[0] = GetInfo_ShowBs(0,1,AIF);
						ptr[1] = GetInfo_ShowBs(1,1,AIF);
						ptr[2] = GetInfo_ShowBs(2,1,AIF);
						ptr[3] = GetInfo_ShowBs(3,1,AIF);
						ptr[4] = GetInfo_ShowBs(4,1,AIF);
						ptr[5] = GetInfo_ShowBs(5,1,AIF);
						if ((ptr[0] == 0xff && ptr[1] == 0x1f &&
							  ptr[2] == 0x00 && ptr[3] == 0xe8 &&
							  (ptr[4] & 0xf0) == 0xf0 && ptr[5] == 0x07)
						   ||(ptr[0] == 0x1f && ptr[1] == 0xff &&
							  ptr[2] == 0xe8 && ptr[3] == 0x00 &&
							  ptr[4] == 0x07 &&(ptr[5] & 0xf0) == 0xf0)
						   ||(ptr[0] == 0xfe && ptr[1] == 0x7f &&
							  ptr[2] == 0x01 && ptr[3] == 0x80)
						   ||(ptr[0] == 0x7f && ptr[1] == 0xfe &&
							  ptr[2] == 0x80 && ptr[3] == 0x01))
        				{
        					wavcode_info = WAV_DTS;
							AIF->ulFormat = SW_AUDIO_FORMAT_DTS;
							goto wavExit;
        				}


						if(Findallflag>=2)
							goto wavExit;
						WAVObject.Dsize -= i;
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);	
						break;
						
					}
				default:
					{
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);
						break;
						
					}
					
				}
			}
			
		}
		
	}
	else if(WAVObject.UID ==RIFX)
	{	
		TempUid = GetInfo_GetBsInByte(4,AIF);
		if(TempUid == WAVE)
		{
			while(1)
			{
				if(!WAVReadObject(AIF,&WAVObject))
				{
					Ret  = 0;
					break;
				}
				WAVObject.Dsize = ((WAVObject.Dsize>>24) & 0X000000FF)
				                 |((WAVObject.Dsize>>8 ) & 0X0000FF00)
				                 |((WAVObject.Dsize<<8 ) & 0X00FF0000)
				                 |((WAVObject.Dsize<<24) & 0XFF000000);
				switch(WAVObject.UID)
				{
				case fmt:
					{
						Ret =1;
						WAVEFORMAT.wFormatTag = GetInfo_GetBsInByte(2,AIF);
						if(WAVEFORMAT.wFormatTag==1)wavcode_info = WAV_RIFX;
						WAVEFORMAT.wChannels = GetInfo_GetBsInByte(2,AIF);
						WAVEFORMAT.dwSamplesPerSec = GetInfo_GetBsInByte(4,AIF);
						WAVEFORMAT.dwAvgBytesPerSec = GetInfo_GetBsInByte(4,AIF);
						WAVEFORMAT.wBlockAlign = GetInfo_GetBsInByte(2,AIF);
						WAVEFORMAT.wBitsPerSample = GetInfo_GetBsInByte(2,AIF);
						AIF->ulChannels = WAVEFORMAT.wChannels;
						AIF->ulBitRate = WAVEFORMAT.dwAvgBytesPerSec*8;
						AIF->ulBitsSample = 16;//WAVEFORMAT.wBitsPerSample;
						AIF->ulSampleRate = WAVEFORMAT.dwSamplesPerSec;
						
						WAVObject.Dsize -= 16;
						if(WAVObject.Dsize>=4)
						{
							GetInfo_SkipBsInByte(2,AIF);
							WAVEFORMAT.wSamplePerBlock = GetInfo_GetBsInByte(2,AIF);
							WAVObject.Dsize -= 4;
						}
						Findallflag++;
						if(Findallflag>=2)
							goto wavExit;
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);
						break;
					}
				case fact:
					{
						int datafactsize = GetInfo_GetBsInByte(4,AIF);
						if(WAVEFORMAT.dwSamplesPerSec)
						{
							AIF->ulDuration = (int)((__int64)datafactsize*1000/WAVEFORMAT.dwSamplesPerSec);
						}
						WAVObject.Dsize -=4;
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);
						break;
					}
				case data:
					{
						Ret =1;
						//if dvi_adpcm wSamplePerBlock count out
						if((WAVEFORMAT.wFormatTag == wave_format_dvi_adpcm)&&(WAVEFORMAT.wChannels))
						{
							WAVEFORMAT.wSamplePerBlock = ((WAVEFORMAT.wBlockAlign-4* WAVEFORMAT.wChannels)*2+ WAVEFORMAT.wChannels)/ WAVEFORMAT.wChannels;
						}
						//if not fact
						if(AIF->ulDuration==0)
						{
						    if(  ( (WAVEFORMAT.wFormatTag == wave_format_pcm)||(WAVEFORMAT.wFormatTag == (short)WAVE_FORMAT_EXTENSIBLE)||(WAVEFORMAT.wFormatTag == wave_format_alaw)||(WAVEFORMAT.wFormatTag == wave_format_mulaw) )
										&& (WAVEFORMAT.dwSamplesPerSec) )
							{
								AIF->ulDuration = (int)((__int64)(WAVObject.Dsize/(WAVEFORMAT.wChannels*WAVEFORMAT.wBitsPerSample/8))*1000/WAVEFORMAT.dwSamplesPerSec);
							}
							else if(WAVEFORMAT.wSamplePerBlock)
							{
								AIF->ulDuration = (int)((__int64)WAVObject.Dsize*1000*WAVEFORMAT.wSamplePerBlock/((__int64)WAVEFORMAT.wBlockAlign*WAVEFORMAT.dwSamplesPerSec));
							}
							else
							{
								if(WAVEFORMAT.dwAvgBytesPerSec)
								{
									AIF->ulDuration = (int)((__int64)WAVObject.Dsize*1000/WAVEFORMAT.dwAvgBytesPerSec);
								}
							}
						}
						Findallflag++;
						if(Findallflag>=2)
							goto wavExit;	
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);	
						break;
						
					}
				default:
					{
						GetInfo_SkipBsInByte(WAVObject.Dsize,AIF);
						break;
						
					}
					
				}
			}
			
		}
		
	}
wavExit:
	if(Ret)
	{
		if(AIF->InforBufLeftLength>=WAVCODEINFOLENGTH)
		{
			AIF->ulAudio_name_sz = WAVCODEINFOLENGTH;
			AIF->ulAudio_name = AIF->InforBuf;
			AIF->InforBuf +=AIF->ulAudio_name_sz;
			AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;	
			
			memcpy(AIF->ulAudio_name,wavcodecInfo[wavcode_info],AIF->ulAudio_name_sz);
			
		}
	}
	GetInfo_BsEndRelease(AIF);
	if(WAVEFORMAT.wFormatTag == WAVE_FORMAT_MPEGLAYER3)
	{
		AIF->InforBuf           -= InforBufLeftLengthCopy - AIF->InforBufLeftLength;
		AIF->InforBufLeftLength  = InforBufLeftLengthCopy;
		return GetInfo_MP3(bs,AIF);
	}
	if(AIF->ulFormat == SW_AUDIO_FORMAT_DTS)
	{
		AIF->InforBuf           -= InforBufLeftLengthCopy - AIF->InforBufLeftLength;
		AIF->InforBufLeftLength  = InforBufLeftLengthCopy;
		return GetInfo_DTS(bs,AIF);
	}
	return Ret;
	
}

static int	WAVReadObject(__audio_file_info_t *AIF,_WAVOBJECT *WAVObject)
{
		WAVObject->UID = GetInfo_GetBsInByte(4,AIF);  //GUID guid;
		WAVObject->Dsize = GetInfo_GetBsInByteLI(4,AIF);
		if(GetInfo_return<4)
		{
			return 0;
		}
		else
		{
			return 1;
		}
}
