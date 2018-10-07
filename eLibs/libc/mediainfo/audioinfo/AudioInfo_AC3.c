/***************************************
*	DOBLY AC3 
*	
*
*	date:  2008/10/10  author: fufo
*	copyright 2100 AllWinner.LTD
*   2008-12-03  修正正确时的返回值
**************************************/
#define		SUPPORTDNETAC3
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"

#define AC3_HEADER_SIZE 7	
#define		SYNC_WD			0x0b77		/* AC-3 frame sync word */
#define		SYNC_WD2		0x770b
#define		NFSCOD			3			/* # defined sample rates */
#define		NDATARATE		38			/* # defined data rates */
#define 	NACMOD			8			/* # audio coding modes */
#define		NREMATBNDS		4	
#define		NCHANS			6			/* max # channels */
#define		NFCHANS			5			/* max # full bw channels */
#define		NPCMCHANS		6			/* max # output channels */
#define		NOUTWORDS		(3840 / 2)		/* max # words per frame			*/
#define	LEFT				0		/* left channel */
#define	CNTR				1		/* center channel */
#define	RGHT				2		/* right channel */
#define	LSUR				3		/* left surround channel */
#define	RSUR				4		/* right surround channel */
#define	LFE					5		/* low frequency effects channel */
#define	MSUR				3		/* mono surround channel */
#define	NONE				-1		/* channel not in use */
enum { MODE11, MODE10, MODE20, MODE30,	/* audio coding mode */
	MODE21, MODE31, MODE22, MODE32 };
//static const short chantab[NACMOD][NPCMCHANS] = 
//{	{LEFT,	RGHT,	LFE,	NONE,	NONE,	NONE},		/* 1+1 */
//	{CNTR,	LFE,	NONE,	NONE,	NONE,	NONE},		/* 1/0 */
//	{LEFT,	RGHT,	LFE,	NONE,	NONE,	NONE},		/* 2/0 */
//	{LEFT,	CNTR,	RGHT,	LFE,	NONE,	NONE},		/* 3/0 */
//	{LEFT,	RGHT,	MSUR,	LFE,	NONE,	NONE},		/* 2/1 */
//	{LEFT,	CNTR,	RGHT,	MSUR,	LFE,	NONE},		/* 3/1 */
//	{LEFT,	RGHT,	LSUR,	RSUR,	LFE,	NONE},		/* 2/2 */
//	{LEFT,	CNTR,	RGHT,	LSUR,	RSUR,	LFE}	};	/* 3/2 */

static const int channumtable[8] = {2,1,2,3,3,4,4,5}; 
static const int eac3_blocks[4] = {
    1, 2, 3, 6
}; 
/*
static const short frmsizetab[NFSCOD][NDATARATE] =
{	{	64, 64, 80, 80, 96, 96, 112, 112, 
		128, 128, 160, 160, 192, 192, 224, 224,
		256, 256, 320, 320, 384, 384, 448, 448,
		512, 512, 640, 640, 768, 768, 896, 896,
		1024, 1024, 1152, 1152, 1280, 1280 },
	{	69, 70, 87, 88, 104, 105, 121, 122,
		139, 140, 174, 175, 208, 209, 243, 244,
		278, 279, 348, 349, 417, 418, 487, 488,
		557, 558, 696, 697, 835, 836, 975, 976,
		1114, 1115, 1253, 1254, 1393, 1394 },
	{	96, 96, 120, 120, 144, 144, 168, 168,
		192, 192, 240, 240, 288, 288, 336, 336,
		384, 384, 480, 480, 576, 576, 672, 672,
		768, 768, 960, 960, 1152, 1152, 1344, 1344,
		1536, 1536, 1728, 1728, 1920, 1920 } };
*/
static const int fstable[3]={48000,44100,32000};
static const int	bitrateTable[38]=
{
		32,	32,	40,	40,
		48,	48,	56,	56,
		64,	64,	80,	80,
		96,	96,	112,112,
		128,128,160,160,
		192,192,224,224,
		256,256,320,320,
		384,384,448,448,
		512,512,576,576,
		640,640
};
typedef enum {
    AC3_FRAME_TYPE_NONE = 0,
    AC3_FRAME_TYPE_LITAC3,
    AC3_FRAME_TYPE_LITMLP,
    AC3_FRAME_TYPE_LITTRUEHD,
    AC3_FRAME_TYPE_BIGAC3,
    AC3_FRAME_TYPE_BIGMLP,
    AC3_FRAME_TYPE_BIGTRUEHD
} AC3FrameType;
static const char AC3codecInfo[5][17]=
{	
	"Dolby Digital AC3",
	"DNET AC3",
	"EAC3",
	"MLP",
	"TRUE HD"
};
#ifdef	SUPPORTDNETAC3
static const short halfrateTable[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3};
#endif
static const unsigned char mlp_channels[32] = {
    1, 2, 3, 4, 3, 4, 5, 3, 4, 5, 4, 5, 6, 4, 5, 4,
    5, 6, 5, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
static const unsigned char thd_chancount[13] = {
//  LR    C   LFE  LRs LRvh  LRc LRrs  Cs   Ts  LRsd  LRw  Cvh  LFE2
     2,   1,   1,   2,   2,   2,   2,   1,   1,   2,   2,   1,   1
};
static int mlp_samplerate(int in)
{
    if (in == 0xF)
        return 0;

    return (in & 8 ? 44100 : 48000) << (in & 7) ;
}
static int truehd_channels(int chanmap)
{
    int channels = 0, i;

    for (i = 0; i < 13; i++)
        channels += thd_chancount[i] * ((chanmap >> i) & 1);

    return channels;
}

static int	AC3_FRAME(__audio_file_info_t *AIF,int filelen);
int GetInfo_AC3(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	int ret;
	INT64 filelength;

	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<=0) return ERROR;
	ret=AC3_FRAME(AIF,filelength);
	GetInfo_BsEndRelease(AIF);
	return ret;
}
//unsigned	int	GetInfo_GetBsInByte(int	ByteLen)
static int	AC3headerSYNC(int *syncdata,__audio_file_info_t *AIF,int filelen);
static int	AC3_FRAME(__audio_file_info_t *AIF,int filelen)
{
	int	syncword = 0,/*crc1,*/data,fscod;
	int frmsizecod,bsid,/*bsmod,*/acmod,/*cmixlev,surmixlev,dsurmod,*/lfeon/*dialnorm,compre,framesize*/;
	int ret=1;
	unsigned int (*GBs)(int ByteLen,__audio_file_info_t *AIF)=NULL;
	__int64 TempData;
#ifdef	SUPPORTDNETAC3
	short halfrate = 0;
#endif
	int bitstream_id;
	unsigned int temp = 0;
	short flag = 0;
	
	ret=AC3headerSYNC(&syncword,AIF,filelen);
	if(ret == 0)
		return 0;
	
	if((ret==AC3_FRAME_TYPE_LITAC3)||(ret==AC3_FRAME_TYPE_BIGAC3))
	{
			syncword = GetInfo_GetBsInByte(2,AIF);
		  temp = GetInfo_ShowBs(0,4,AIF);
			if(syncword ==SYNC_WD)
					GBs = GetInfo_GetBsInByte;
			else if(syncword == SYNC_WD2)
			{	
				int byte1,byte2;		
				byte1 = ( syncword << 8 ) & 0xff00;
				byte2 = ( syncword >> 8 ) & 0x00ff;
				syncword=(byte1 | byte2);
				GBs = GetInfo_GetBsInByteLI;
				temp = ((temp << 8) & 0xff00ff00)|((temp >> 8) & 0x00ff00ff);
			}else
			{
				return 0;
			}
		  
		  bitstream_id = (temp>>3) & 0x1F;
		  if(bitstream_id > 16)
		        return 0;
			if(bitstream_id <= 10) {
				/* Normal AC-3 */
				GBs(2,AIF);
				
				data = GBs(2,AIF);
				fscod = data>>14;
				frmsizecod = (data>>8)&0x3f;
				bsid = (data>>3)&0x1f;
				//bsmod = data&0x7;
				
				data = GBs(2,AIF);
				
				acmod = data>>13;
				data = (data<<3)&0xffff;
				
				if ((acmod != MODE10) && (acmod & 0x1))
				{
					//cmixlev = data>>14;
					data = (data<<2)&0xffff;
				}
				if (acmod & 0x4)
				{
					//surmixlev = data>>14;
					data = (data<<2)&0xffff;
					//surmixlev = unp2bits();
				}
				if (acmod == MODE20)
				{
					//dsurmod = data>>14;
					data = (data<<2)&0xffff;
					//dsurmod = unp2bits();
				}
				lfeon = data>>15;
				data = (data<<1)&0xffff;
				
				//dialnorm = data>>11;
				data = (data<<5)&0xffff;
				//compre = data>>15;
				data = (data<<1)&0xffff;
				
			//	framesize = frmsizetab[fscod][frmsizecod];
				/*	Set up return error code */
				
				if (syncword != SYNC_WD)
				{
					ret=0;//status = AC3I_ERR_SYNC;			/* invalid sync */
				}
				else if (fscod == 3)
				{
					ret=0;//status = AC3I_ERR_SAMPRATE;		/* invalid sample rate */
				}
				/*else if (frmsizecod >= 38)
				{
					ret=0;//status = AC3I_ERR_DATARATE;		// invalid sample rate
				}*/
				else
				{
					ret=1;//status = AC3I_ERR_NONE;			/* no error */
				}
				if(ret==0)return ret;
				AIF->ulSampleRate = fstable[fscod];
				AIF->ulChannels = channumtable[acmod] + lfeon;
				AIF->ulBitRate = bitrateTable[frmsizecod]*1000;//
				//AIF->ulBitRate = framesize*8*AIF->ulSampleRate/256*6;
				
				//TempData = (__int64)(filelen/(2*framesize))*6*256*1000;
				//AIF->ulDuration = (__u32)(TempData/AIF->ulSampleRate);
			#ifdef	SUPPORTDNETAC3
				halfrate = halfrateTable[bsid];
				AIF->ulSampleRate = AIF->ulSampleRate>>halfrate;
				AIF->ulBitRate = AIF->ulBitRate>>halfrate;//
			#endif
				flag = ((halfrate==0)?0:1);
			}
			else 
			{
		        /* Enhanced AC-3 */
		        int frame_type,frame_size,sr_code,num_blocks = 1,lfe_on,channel_mode;
		        temp = GBs(2,AIF);
		        frame_type = temp>>14;//2bits
		        if(frame_type == 3/*EAC3_FRAME_TYPE_RESERVED*/)
		            return 0;
		        //substreamid = (temp>>11)& 0x07;//3bits
		        frame_size = ((temp & 0x7ff) + 1) << 1;//11bits
		        if(frame_size < AC3_HEADER_SIZE)
		        	return 0;
		        temp = GBs(2,AIF);
		        sr_code = temp>>14;//2bits
		        if (sr_code == 3) {
		            int sr_code2 = (temp>>12) & 0x03;//2bits;
		            if(sr_code2 == 3)
		                return 0;
		            AIF->ulSampleRate = fstable[sr_code2] / 2;
		            //sr_shift = 1;
		        } else {
		            num_blocks = eac3_blocks[(temp>>12) & 0x03];
		            AIF->ulSampleRate = fstable[sr_code];
		            //sr_shift = 0;
		        }
		        channel_mode = (temp>>11)& 0x07;//3bits
		        lfe_on = (temp>>10)& 0x01;//1bits
		
		        AIF->ulBitRate = (unsigned int)(8.0 * frame_size * AIF->ulSampleRate /
		                        (num_blocks * 256.0));
		        AIF->ulChannels = channumtable[channel_mode] + lfe_on;
		        flag = 2;//for eac3 flag
		        
		   }
		   
	}//IF AC3 END
	else
	{
		  //mlp and truehd
		  int ratebits;
		  int frame_size = 0;
		  int temptrue = 0;
		  int is_vbr = 0;
		  int peak_bitrate,access_unit_size;
		  //确定类型
		  if((ret ==AC3_FRAME_TYPE_LITMLP)||(ret ==AC3_FRAME_TYPE_BIGMLP))
		  {
		  	flag = 3;
		  }
		  else if((ret ==AC3_FRAME_TYPE_LITTRUEHD)||(ret ==AC3_FRAME_TYPE_BIGTRUEHD))
		  {
		  	flag =4;
		  }
		  //确定大小端
		  if(ret&0x04)
			{
				GBs = GetInfo_GetBsInByte;		
			}
			else
			{
				GBs = GetInfo_GetBsInByteLI;
			}
			//分析头信息
			frame_size = (GBs(2,AIF) & 0xfff) * 2;
			GetInfo_SkipBsInByte(2+4,AIF);
			if(flag ==3)//mlp
			{
				 int channels_mlp;
				 GBs(1,AIF);
				 ratebits = (GBs(1,AIF)>>4)&0x0f;
				 AIF->ulSampleRate = mlp_samplerate(ratebits);
				 channels_mlp = GBs(2,AIF) & 0x1f;
				 AIF->ulChannels = mlp_channels[channels_mlp];
				 
			}
			else//truehd
			{
				int channels_thd_stream1,channels_thd_stream2;
				ratebits = (GBs(1,AIF)>>4)&0x0f;
				AIF->ulSampleRate = mlp_samplerate(ratebits);
				temptrue = GBs(3,AIF)&0x00ffffff;
				channels_thd_stream1 = (temptrue >>15)& 0x1f;
				channels_thd_stream2 = temptrue & 0x1fff;
				if (channels_thd_stream2)
					AIF->ulChannels = truehd_channels(channels_thd_stream2);
				else
					AIF->ulChannels = truehd_channels(channels_thd_stream1);
				
			}
			access_unit_size = 40 << (ratebits & 7);
			GetInfo_SkipBsInByte(6,AIF);
			temptrue = GBs(2,AIF);
			is_vbr = (temptrue>>15)&0x01;
			peak_bitrate = ((temptrue&0x7fff) * AIF->ulSampleRate + 8) >> 4;
			GetInfo_SkipBsInByte(12,AIF);
			
			if (!is_vbr) /* Stream is CBR */
				AIF->ulBitRate = peak_bitrate;
			else
			{
				//get four frame size
				int i=0;
				int frame_total = frame_size ;
				GetInfo_SkipBsInByte(frame_size-32,AIF);
				for(i=0;(i<3)&&(frame_total<filelen);i++)
				{
					frame_size = (GBs(2,AIF) & 0xfff) * 2;
					frame_total += frame_size;
					GetInfo_SkipBsInByte(frame_size-2,AIF);
				}
				
				AIF->ulBitRate = (unsigned int)(8.0 * (frame_total-28) * AIF->ulSampleRate /
		                        (access_unit_size*(i+1)));
		  }
			
	}
	AIF->ulBitsSample = 16;
	TempData = (__int64)filelen*8*1000;
	if(AIF->ulBitRate)
	{
		AIF->ulDuration = (__u32)(TempData/AIF->ulBitRate);	
	}
	if(!AIF->ulSampleRate)
	{
		return ERROR;
	}
	

	if(AIF->InforBufLeftLength>=17)
	{
		//short flag = ((halfrate==0)?0:1);
		AIF->ulAudio_name_sz = 17;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
		memcpy(AIF->ulAudio_name,AC3codecInfo[flag],AIF->ulAudio_name_sz);
		
	}

	return SUCCESS;
}


static int	AC3headerSYNC(int *syncdata,__audio_file_info_t *AIF,int filelen)
{
	unsigned	short syncWord=0;
	unsigned int syncmlp = 0; 
	int		CheckFileLen =10*1024;//10k范围内查找头
	int ret = 0;
	if(CheckFileLen>filelen) CheckFileLen = CheckFileLen;

	*syncdata = 0;
	CheckFileLen -=2;
	while(1)
	{
		syncWord = GetInfo_ShowBs(0,2,AIF);
	  syncmlp  = GetInfo_ShowBs(4,4,AIF);
	  if(GetInfo_return<4)
		  return 0;//no data;
	  CheckFileLen -=2;
		if(syncWord == 0x770b )
		{
			ret = AC3_FRAME_TYPE_LITAC3;
			*syncdata = syncWord;
			break;
		}
		if(syncWord == 0x0b77 )
		{
			ret = AC3_FRAME_TYPE_BIGAC3;
			*syncdata = syncWord;
			break;
		}
		if(syncmlp == 0x72f8bb6f )
		{
			ret = AC3_FRAME_TYPE_LITMLP;
			break;
		}
		if(syncmlp == 0xf8726fbb )
		{
			ret = AC3_FRAME_TYPE_BIGMLP;
			break;
		}
		if(syncmlp == 0x72f8ba6f )
		{
			ret = AC3_FRAME_TYPE_LITTRUEHD;
			break;
		}
		if(syncmlp == 0xf8726fba )
		{
			ret = AC3_FRAME_TYPE_BIGTRUEHD;
			break;
		}
	  GetInfo_GetBsInByte(1,AIF);
	  CheckFileLen --;
	  if(CheckFileLen<0)
			return 0;
	}
	return ret;
}
