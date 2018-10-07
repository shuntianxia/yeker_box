/***************************************
*	Windows Medio audio infor 
*	
*
*	date:  2009/06/01  author: lszhang
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"

#define EA3_HEADER_SIZE 96

enum {
    OMA_CODECID_ATRAC3  = 0,
    OMA_CODECID_ATRAC3P = 1,
};
#define ATRACCODEINFOLENGTH 8
static const char atracformat[2][ATRACCODEINFOLENGTH] =
{
	"ATRAC3",
	"ATRAC3P"
};

int GetInfo_ATRAC(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{	
//	int Ret = 0;
	__int64	filelength = 0;
	int		ID3Len = 0;
	//int nScanBytes = 0;
	unsigned char ea3flag[] = {'e', 'a', '3', 3, 0};
	unsigned char EA3FLAG[3] ={'E', 'A', '3'};
	unsigned char buf[EA3_HEADER_SIZE];
	int i;
	int ea3_taglen,EA3_pos;
	short eid;
	int codec_tag,channel_id,codec_params,samplerate,framesize;
	unsigned short srate_tab[6] = {320,441,480,882,960,0};
	
	filelength = GetInfo_BsInit(bs,AIF);
	if(filelength <=0) return ERROR;
	
	ID3Len=GetInfo_GETID3_V2(AIF);
	for(i=0;i<10;i++)
	{
		buf[i] = GetInfo_GetBsInByte(1,AIF);
	}

	if (memcmp(buf, ea3flag,5))
	   goto ATRAC_error;

    ea3_taglen = ((buf[6] & 0x7f) << 21) | ((buf[7] & 0x7f) << 14) | ((buf[8] & 0x7f) << 7) | (buf[9] & 0x7f);

    EA3_pos = ea3_taglen + 10;
    if (buf[5] & 0x10)
        EA3_pos += 10;

    
	GetInfo_SkipBsInByte(EA3_pos - 10,AIF);
	
   for(i=0;i<6;i++)
	{
		buf[i] = GetInfo_GetBsInByte(1,AIF);
	}

	if (memcmp(buf, EA3FLAG,3) || buf[4] != 0 || buf[5] != EA3_HEADER_SIZE) {
        goto ATRAC_error;
    }

    //eid = AV_RB16(&buf[6]);
	eid = GetInfo_GetBsInByte(2,AIF);
    if (eid != -1 && eid != -128) {
        goto ATRAC_error;
    }
	GetInfo_SkipBsInByte(32-8,AIF);
	//codec_tag = buf[32];
	codec_tag  = GetInfo_GetBsInByte(1,AIF);

    //codec_params = AV_RB24(&buf[33]);
    codec_params = GetInfo_GetBsInByte(3,AIF);

    channel_id = (codec_params >> 10) & 7;
    samplerate = srate_tab[(codec_params >> 13) & 7]*100;
	switch (codec_tag) {
        case OMA_CODECID_ATRAC3:
            if (samplerate != 44100)
                ;//av_log(s, AV_LOG_ERROR, "Unsupported sample rate, send sample file to developers: %d\n", samplerate);

            framesize = (codec_params & 0x3FF) * 8;
            //jsflag = (codec_params >> 17) & 1; /* get stereo coding mode, 1 for joint-stereo */
            channel_id = 2;

            break;
        case OMA_CODECID_ATRAC3P:
            framesize = ((codec_params & 0x3FF) * 8) + 8;
            //av_log(s, AV_LOG_ERROR, "Unsupported codec ATRAC3+!\n");
            break;
        default:
            //av_log(s, AV_LOG_ERROR, "Unsupported codec %d!\n",buf[32]);
            goto ATRAC_error;
            //break;
    }
    AIF->ulBitsSample = 16;
    AIF->ulChannels   = channel_id;
    AIF->ulSampleRate = samplerate;
    AIF->ulBitRate    = samplerate * framesize * 8 / (1024 *(codec_tag+1));
    AIF->ulDuration   = (unsigned int)((double)(filelength - EA3_pos - EA3_HEADER_SIZE - ID3Len)* 8 *1000/(double)(AIF->ulBitRate));
    if(AIF->InforBufLeftLength>=ATRACCODEINFOLENGTH)
		{
			AIF->ulAudio_name_sz = ATRACCODEINFOLENGTH;
			AIF->ulAudio_name = AIF->InforBuf;
			AIF->InforBuf +=AIF->ulAudio_name_sz;
			AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;	
			
			memcpy(AIF->ulAudio_name,atracformat[codec_tag],AIF->ulAudio_name_sz);
			
		}
	
	  GetInfo_BsEndRelease(AIF);
    return SUCCESS;
ATRAC_error:
	  GetInfo_BsEndRelease(AIF);
	  return ERROR;
	
	
}


