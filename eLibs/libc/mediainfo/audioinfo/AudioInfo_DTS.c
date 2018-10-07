/***************************************
	MPEG LAYI II III audio infor parser
	support Xing /ID3V2 /ID3V1/VBRI tag 
	//2009-3-5 Z增加 DTS - HD 检测 
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"

#define HEADER_SIZE 14

typedef struct dca_state_s {

    /* Bitstream handling */
    __u32 * buffer_start;
    __u32 bits_left;
    __u32 current_word;
    int      word_mode;         /* 16/14 bits word format (1 -> 16, 0 -> 14) */
    int      bigendian_mode;    /* endianness (1 -> be, 0 -> le) */
 
}dca_state_t;
//typedef struct dca_state_s dca_state_t;


static const int dca_sample_rates[] =
{
    0, 8000, 16000, 32000, 0, 0, 11025, 22050, 44100, 0, 0,
    12000, 24000, 48000, 96000, 192000
};

static const int dca_bit_rates[] =
{
    32000, 56000, 64000, 96000, 112000, 128000,
    192000, 224000, 256000, 320000, 384000,
    448000, 512000, 576000, 640000, 768000,
    896000, 1024000, 1152000, 1280000, 1344000,
    1408000, 1411200, 1472000, 1536000, 1920000,
    2048000, 3072000, 3840000, 1/*open*/, 2/*variable*/, 3/*lossless*/
};

static const short dca_channels[] =
{
    1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 6, 6, 7, 8, 8
};
/*
static const short dca_bits_per_sample[] =
{
    16, 16, 20, 20, 0, 24, 24
};*/

static const char dtscodecInfo[2][26]=
{	
	"DTS-Digital Theatre System",
		"DTS-HD"
};
#if 0
static int GetInfo_DTSOLD(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	int ret=1;
	unsigned int (*GBs)(int ByteLen,__audio_file_info_t *AIF)=NULL;	
	unsigned int data,data2,framesize,ACA,NumberofPCM,sample_rate,bitrate,lfe;
	__int64 TempData;
	__int64 filelength;
	int dtsflag = 0;	
	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<=0) return ERROR;

	data = GetInfo_GetBsInByte(4,AIF);
	if(data == 0x7ffe8001)
	{
		GBs = GetInfo_GetBsInByte;

	}else if(data == 0xfe7f0180)
	{	
		GBs = GetInfo_GetBsInByteLI;
	}else
	{	
		goto DTS_error;
	}
	data = GBs(2,AIF);
	data2 = GBs(2,AIF);
	NumberofPCM = (data>>2)&0x7f ;
	NumberofPCM += 1;
	framesize = ((data2 >>4)&0xfff) | ((data&0x3)<<12);
	framesize += 1;
	if(framesize<96||framesize>16384)
		goto DTS_error;
	data = GBs(2,AIF); 
	ACA = ((data2&0XF)<<2)|((data>>14)&0X3); //Audio channel arrangement
	if(ACA > 63) goto DTS_error;
	ACA = dca_channels[ACA];
	sample_rate = (data>>10)&0Xf;
	sample_rate = dca_sample_rates[sample_rate];
	if(sample_rate == 0)
		goto DTS_error;
	bitrate = (data>>5)&0X1f;
	bitrate = dca_bit_rates[bitrate];
	data = GBs(1,AIF); 
	lfe = (data>>1)&0x3;
	if((lfe==1)||(lfe==0x2))
		ACA += 1;

	GetInfo_SkipBsInByte(framesize-11,AIF);
	data = GBs(4,AIF);
	if(data == 0x64582025) dtsflag =1;
	

	AIF->ulSampleRate = sample_rate;
	AIF->ulChannels = ACA;
	AIF->ulBitRate = bitrate;//framesize*8*AIF->ulSampleRate/256*6;
	AIF->ulBitsSample = 16;
	//TempData = (__int64)(filelength/(framesize))*(NumberofPCM/8)*256*1000;
	//AIF->ulDuration = (__u32)(TempData/AIF->ulSampleRate);
	TempData = (__int64)filelength*8*1000;
	AIF->ulDuration = (__u32)(TempData/bitrate);
	if(AIF->InforBufLeftLength>=26)
	{
		AIF->ulAudio_name_sz = 26;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
		memcpy(AIF->ulAudio_name,dtscodecInfo[dtsflag],AIF->ulAudio_name_sz);
		
	}
	if(dtsflag == 1) AIF->ulDuration =0;

  GetInfo_BsEndRelease(AIF);
	return ret;
DTS_error:
	GetInfo_BsEndRelease(AIF);
	return ERROR;
}	

#endif

static int	DTSheaderSYNC(__audio_file_info_t *AIF,__int64 filelen);		
int GetInfo_DTS(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	int ret=1;
	__int64 filelength;

	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<14) return ERROR;
	ret=DTSheaderSYNC(AIF,filelength);
    GetInfo_BsEndRelease(AIF);
	return ret;

}

static int dca_syncinfo (dca_state_t * state, __u8 * buf, int * flags,
                  int * sample_rate, int * bit_rate, int * frame_length);

static int	DTSheaderSYNC(__audio_file_info_t *AIF,__int64 filelen)
{
	//unsigned	int syncWord=0; 
	int		CheckFileLen=10*1024;//10k范围内查找头
	int		length;
	dca_state_t state;
	int flags,sample_rate,bitrate,frame_length,ACA,lfe,data,dtsflag=0;
	__u8 buf[HEADER_SIZE+10];
	__u8 *bufptr;
	__int64 TempData;
	int i=0;
	if(CheckFileLen>filelen) CheckFileLen = CheckFileLen;
	if(CheckFileLen<HEADER_SIZE)
		return 0;
	memset(&state,0,sizeof(dca_state_t));

	for(i=0;i<HEADER_SIZE;i++)
	{
		buf[i]=GetInfo_GetBsInByte(1,AIF);
	}

//	readlength=HEADER_SIZE;//fread(buf,1,HEADER_SIZE,in_file);
	CheckFileLen -=HEADER_SIZE;
	length = 0;
	while(!length)
	{
		length = dca_syncinfo (&state, buf, &flags, &sample_rate,&bitrate, &frame_length);
		if((length<96)||(length > 16384))
			length = 0;  //invalid frame length
		if (!length) 
		{
			for (bufptr = buf; bufptr < buf + HEADER_SIZE-1; bufptr++)
				bufptr[0] = bufptr[1];
			buf[HEADER_SIZE-1]=GetInfo_GetBsInByte(1,AIF);
			if(GetInfo_return<1)
				return 0;//no data;
			CheckFileLen -=1;
			if(CheckFileLen<0)
				return 0;		
			continue;
		}
	}
	ACA = flags&0x7f;
	ACA = dca_channels[ACA];
	lfe = flags&0x80;
	if(lfe)
		ACA += 1;
	frame_length = length;
	GetInfo_SkipBsInByte(frame_length-HEADER_SIZE,AIF);
	data = GetInfo_GetBsInByte(4,AIF);
	if((data == 0x64582025)||(data == 0x25205864)) dtsflag =1;
	

	AIF->ulSampleRate = sample_rate;
	AIF->ulChannels = ACA;
	AIF->ulBitRate = bitrate;//framesize*8*AIF->ulSampleRate/256*6;
	AIF->ulBitsSample = 16;
	TempData = (__int64)filelen*8*1000;
	AIF->ulDuration = (__u32)(TempData/bitrate);
	if(AIF->InforBufLeftLength>=26)
	{
		AIF->ulAudio_name_sz = 26;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
		memcpy(AIF->ulAudio_name,dtscodecInfo[dtsflag],AIF->ulAudio_name_sz);
		
	}
	if(dtsflag == 1) AIF->ulDuration =0;
	
	return 1;

}







#   define swable32(x)\
((((__u32)x) >> 16) | (((__u32)x) << 16))

#   define swab32(x)\
((((__u8*)&x)[0] << 24) | (((__u8*)&x)[1] << 16) |  \
 (((__u8*)&x)[2] << 8)  | (((__u8*)&x)[3]))

static void bitstream_fill_current (dca_state_t * state)
{
    __u32 tmp;

    tmp = *(state->buffer_start++);

    if (state->bigendian_mode)
        state->current_word = swab32 (tmp);
    else
        state->current_word = swable32 (tmp);

    if (!state->word_mode)
    {
        state->current_word = (state->current_word & 0x00003FFF) |
            ((state->current_word & 0x3FFF0000 ) >> 2);
    }
}

/*
 * The fast paths for _get is in the
 * bitstream.h header file so it can be inlined.
 *
 * The "bottom half" of this routine is suffixed _bh
 *
 * -ah
 */

static __u32 dca_bitstream_get_bh (dca_state_t * state, __u32 num_bits)
{
    __u32 result;

    num_bits -= state->bits_left;

    result = ((state->current_word << (32 - state->bits_left)) >>
	      (32 - state->bits_left));

    if ( !state->word_mode && num_bits > 28 ) {
        bitstream_fill_current (state);
	result = (result << 28) | state->current_word;
	num_bits -= 28;
    }

    bitstream_fill_current (state);

    if ( state->word_mode )
    {
        if (num_bits != 0)
	    result = (result << num_bits) |
	             (state->current_word >> (32 - num_bits));

	state->bits_left = 32 - num_bits;
    }
    else
    {
        if (num_bits != 0)
	    result = (result << num_bits) |
	             (state->current_word >> (28 - num_bits));

	state->bits_left = 28 - num_bits;
    }

    return result;
}

static __u32 bitstream_get (dca_state_t * state, __u32 num_bits)
{
    __u32 result;

    if (num_bits < state->bits_left) {
        result = (state->current_word << (32 - state->bits_left))
				      >> (32 - num_bits);

        state->bits_left -= num_bits;
        return result;
    }

    return dca_bitstream_get_bh (state, num_bits);
}
static void dca_bitstream_init (dca_state_t * state, __u8 * buf, int word_mode,
                         int bigendian_mode)
{
    __s32 align;

    align = (__u32)buf & 3;
    state->buffer_start = (__u32 *) ((__u32)buf - align);
    state->bits_left = 0;
    state->current_word = 0;
    state->word_mode = word_mode;
    state->bigendian_mode = bigendian_mode;
    bitstream_get (state, align * 8);
}

#define DCA_LFE 0x80
static int syncinfo (dca_state_t * state, int * flags,
                     int * sample_rate, int * bit_rate, int * frame_length)
{
    int frame_size;

    /* Sync code */
    bitstream_get (state, 32);
    /* Frame type */
    bitstream_get (state, 1);
    /* Samples deficit */
    bitstream_get (state, 5);
    /* CRC present */
    bitstream_get (state, 1);

    *frame_length = (bitstream_get (state, 7) + 1) * 32;
    frame_size = bitstream_get (state, 14) + 1;
    if (!state->word_mode) frame_size = frame_size * 8 / 14 * 2;

    /* Audio channel arrangement */
    *flags = bitstream_get (state, 6);
    if (*flags > 63)
        return 0;

    *sample_rate = bitstream_get (state, 4);
    if ((size_t)*sample_rate >= sizeof (dca_sample_rates) / sizeof (int))
        return 0;
    *sample_rate = dca_sample_rates[ *sample_rate ];
    if (!*sample_rate) return 0;

    *bit_rate = bitstream_get (state, 5);
    if ((size_t)*bit_rate >= sizeof (dca_bit_rates) / sizeof (int))
        return 0;
    *bit_rate = dca_bit_rates[ *bit_rate ];
    if (!*bit_rate) return 0;

    /* LFE */
    bitstream_get (state, 10);
    if (bitstream_get (state, 2)) *flags |= DCA_LFE;

    return frame_size;
}


static int dca_syncinfo (dca_state_t * state, __u8 * buf, int * flags,
                  int * sample_rate, int * bit_rate, int * frame_length)
{
    /*
     * Look for sync code
     */

    /* 14 bits and little endian bitstream */
    if (buf[0] == 0xff && buf[1] == 0x1f &&
        buf[2] == 0x00 && buf[3] == 0xe8 &&
        (buf[4] & 0xf0) == 0xf0 && buf[5] == 0x07)
    {
        int frame_size;
        dca_bitstream_init (state, buf, 0, 0);
        frame_size = syncinfo (state, flags, sample_rate,
                               bit_rate, frame_length);
        return frame_size;
    }

    /* 14 bits and big endian bitstream */
    if (buf[0] == 0x1f && buf[1] == 0xff &&
        buf[2] == 0xe8 && buf[3] == 0x00 &&
        buf[4] == 0x07 && (buf[5] & 0xf0) == 0xf0)
    {
        int frame_size;
        dca_bitstream_init (state, buf, 0, 1);
        frame_size = syncinfo (state, flags, sample_rate,
                               bit_rate, frame_length);
        return frame_size;
    }

    /* 16 bits and little endian bitstream */
    if (buf[0] == 0xfe && buf[1] == 0x7f &&
        buf[2] == 0x01 && buf[3] == 0x80)
    {
        int frame_size;
        dca_bitstream_init (state, buf, 1, 0);
        frame_size = syncinfo (state, flags, sample_rate,
                               bit_rate, frame_length);
        return frame_size;
    }

    /* 16 bits and big endian bitstream */
    if (buf[0] == 0x7f && buf[1] == 0xfe &&
        buf[2] == 0x80 && buf[3] == 0x01)
    {
        int frame_size;
        dca_bitstream_init (state, buf, 1, 1);
        frame_size = syncinfo (state, flags, sample_rate,
                               bit_rate, frame_length);
        return frame_size;
    }

    return 0;
}
