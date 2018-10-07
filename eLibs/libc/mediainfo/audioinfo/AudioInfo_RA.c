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

#define RACODECNUM 13
#define RACODECLEN 19

static const char RAcodec[RACODECNUM][RACODECLEN]=
{
	"RealAudio-Unknow",
	"RealAudio-RM-Cook",
	"RealAudio-RM-Sipro",
	"RealAudio-RM-Atric",
	"RealAudio-RM-AAC",
	"RealAudio-RM-AC3",
	"RealAudio-RA-14_4",
	"RealAudio-RA-28_8",
	"RealAudio-RA-Cook",
	"RealAudio-RA-Sipro",
	"RealAudio-RA-Atric",
	"RealAudio-RA-AAC",
	"RealAudio-RA-AC3"
	
};


typedef	struct ra_substream_hdr_struct
{
    __u16      usRAFormatVersion;     /* 3, 4, or 5 */
    __u16      usRAFormatRevision;    /* should be 0 */
    __u16      usHeaderBytes;         /* size of raheader info */
    __u16      usFlavorIndex;         /* compression type */
    __u32      ulGranularity;         /* size of one block of encoded data */
    __u32      ulTotalBytes;          /* total bytes of ra data */
    __u32      ulBytesPerMin;         /* data rate of encoded and interleaved data */
    __u32      ulBytesPerMin2;        /* data rate of interleaved or non-interleaved data */
    __u32      ulInterleaveFactor;    /* number of blocks per superblock */
    __u32      ulInterleaveBlockSize; /* size of each interleave block */
    __u32      ulCodecFrameSize;      /* size of each audio frame */
    __u32      ulUserData;            /* extra field for user data */
    __u32      ulSampleRate;          /* sample rate of decoded audio */
    __u32      ulActualSampleRate;    /* sample rate of decoded audio */
    __u32      ulSampleSize;          /* bits per sample in decoded audio */
    __u32      ulChannels;            /* number of audio channels in decoded audio */
    __u32      ulInterleaverID;       /* interleaver 4cc */
    __u32      ulCodecID;             /* codec 4cc */
    __s8        bIsInterleaved;        /* 1 if file has been interleaved */
    __s8        bCopyByte;             /* copy enable byte, if 1 allow copies (SelectiveRecord) */
    __s8        ucStreamType;          /* i.e. LIVE_STREAM, FILE_STREAM */
    __s8        ucScatterType;         /* the interleave pattern type 0==cyclic,1==pattern */
    __u32      ulNumCodecFrames;      /* number of codec frames in a superblock */
    __u32*     pulInterleavePattern;  /* the pattern of interleave if not cyclic */
    __u32      ulOpaqueDataSize;      /* size of the codec specific data */
    __s8*       pOpaqueData;           /* codec specific data */
    __u32	    dBlockDuration;        /* Duration in ms of audio "block" */
    __u32      ulLastSentEndTime;     /* Ending time of last sent audio frame */
    __s8*       pFragBuffer;           /* Intermediate buffer for reconstructing VBR packets */
    __u32      ulFragBufferSize;      /* Size of intermediate buffer */
    __u32      ulFragBufferAUSize;    /* Size of AU being reconstructed */
    __u32      ulFragBufferOffset;    /* Current offset within AU */
    __u32      ulFragBufferTime;      /* Timestamp of AU being reconstructed */
    __u32      ulSuperBlockSize;      /* ulInterleaveBlockSize * ulInterleaveFactor */
    __u32      ulSuperBlockTime;      /* dBlockDuration * ulInterleaveFactor */
    __u32      ulKeyTime;             /* Timestamp of keyframe packet */
    __s8*       pIBuffer;              /* Buffer holding interleaved blocks    */
    __s8*       pDBuffer;              /* Buffer holding de-interleaved blocks */
    __u32*     pIPresentFlags;        /* number of UINT32s: ulInterleaveBlockSize */
    __u32*     pDPresentFlags;        /* number of UINT32s: ulInterleaveBlockSize */
    __u32      ulBlockCount;          /* number of blocks currently in superblock */
    __u32*     pulGENRPattern;        /* Interleave pattern for GENR interleaver */
    __u32*     pulGENRBlockNum;
    __u32*     pulGENRBlockOffset;

} ra_substream_hdr;
	
typedef struct	_RaOBJECT_struct
{
	__u32	ObjectUID;
	int		size;
	__u16  ObjectVersion;  
}_RaOBJECT;

typedef struct rm_media_props_hdr_struct
{
//    __u32 id;                  /* unique identifier for this header */
//    __u32 size;                /* size of header in file */
//    __u16 version;             /* struct version */
    __u16 stream_num;          /* stream number */
    __u32 max_bit_rate;        /* maximum bit rate of stream */
    __u32 avg_bit_rate;        /* average bit rate of stream */
    __u32 max_pkt_size;        /* max length of packet in stream (bytes) */
    __u32 avg_pkt_size;        /* averge length of packet in stream (bytes) */
    __u32 start_time;          /* start time of stream -- clip relative */
    __u32 preroll;             /* preroll time (milliseconds) */
    __u32 duration;            /* duration of stream (milliseconds) */
    __s8   stream_name_sz;      /* length of stream_name field */
    char*  stream_name;         /* string name of stream */
    __s8   mime_type_sz;        /* length of mime_type field */
    char*  mime_type;           /* MIME type of stream */
    __u32 type_spec_sz;        /* length of type_spec field */
    __s8*  type_spec;           /* type specific data buffer */
}rm_media_props_hdr;

#define RM_HEADER_OBJECT           0x2E524D46   /* '.RMF' */
#define RM_PROPERTIES_OBJECT       0x50524F50   /* 'PROP' */
#define RM_MEDIA_PROPERTIES_OBJECT 0x4D445052   /* 'MDPR' */
#define RM_CONTENT_OBJECT          0x434F4E54   /* 'CONT' */
#define RM_DATA_OBJECT             0x44415441   /* 'DATA' */
#define RM_INDEX_OBJECT            0x494E4458   /* 'INDX' */
#define RM_MULTIHEADER_OBJECT      0x4D4C5449   /* 'MLTI' */
#define RA_HEADER_OBJECT           0x2E7261FD   /* '.ra'0xfd' */

#define DATABLOCK1	20				/* size of 14.4 input block in bytes */
#define DATABLOCK2	38				/* size of 28.8 input block in bytes */
#define DATACHUNK1	1440			/* size of 14.4 input chunk in bytes */
#define DATACHUNK2	2736			/* size of 28.8 input chunk in bytes */
#define AUDIOBLOCK	160				/* size of output block in 16-bit words (320 bytes) */
//#define AUDIOBUFFER	11520			/* size of output buffer in 16-bit words (23040 bytes) */
#define AUDIOBUFFER	12288			/* size of output buffer in 16-bit words (24576 bytes) */
#define BLOCK_5K0 296
#define DURMS_5K0 480
#define BLOCK_6K5 232
#define DURMS_6K5 288
#define BLOCK_8K5 304
#define DURMS_8K5 288
#define BLOCK_16K0 320
#define DURMS_16K0 160 

#define		SYNC_WD			0x0b77		/* AC-3 frame sync word */
#define		SYNC_WD2		0x770b
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
static const short halfrateTable[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3};

#define REALAUDIO_MIME_TYPE		 "audio/x-pn-realaudio"
#define REALAUDIO_ENCRYPTED_MIME_TYPE	 "audio/x-pn-realaudio-encrypted"

static int ra_depacki_unpack_opaque_data(int ulLen,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr);
static int ra_depacki_unpack_raformat3(int ulLen,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr);
static int ra_depacki_unpack_raformat4(int ulLen,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr);
static int ra_depacki_unpack_raformat5(int ulLen,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr);
static void	ReadObject_RA(__audio_file_info_t *AIF,_RaOBJECT *RaObjectframe);

static int ReadFunc(char *buf,int len,__audio_file_info_t *AIF);
static int GetCodec(unsigned char *buf, unsigned int *freq, unsigned int *chans, unsigned int *blocksize, unsigned short verbose,__audio_file_info_t *AIF);

int GetInfo_RA(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{	
	int Ret = 0;
	int bRet = 0;
	__int64 filelength =0;
	//int	Object=0;
	int i;
	__u32 file_version,num_headers;
	__u16 propHdr_num_streams=0;
	int AudioObjInfo = 0;
	ra_substream_hdr  ra_shdr;
	_RaOBJECT RaObjectframe;
	rm_media_props_hdr MediaObj;
	char *in;
	char *buf = malloc(DATACHUNK2);
	if(buf==NULL) return ERROR;
	in = buf;
	filelength = GetInfo_BsInit(bs,AIF);
	if(filelength<=0) 
	{
		if(buf)
		{
			free(buf);
			buf = NULL;
		}
		return ERROR;
	}
	//ReadObject_RA(AIF,&RaObjectframe);  //GUID guid;
	RaObjectframe.ObjectUID = GetInfo_GetBsInByte(4,AIF);  //GUID guid;
	RaObjectframe.size = GetInfo_GetBsInByte(4,AIF);
	
	if(RaObjectframe.ObjectUID ==RM_HEADER_OBJECT)
	{
	  	//goto RA_error;
	  RaObjectframe.ObjectVersion = GetInfo_GetBsInByte(2,AIF);
	  memset(&ra_shdr,0,sizeof(ra_substream_hdr));
	  file_version=GetInfo_GetBsInByte(4,AIF);//file_version
	  num_headers=GetInfo_GetBsInByte(4,AIF); //num_headers
      if ((RaObjectframe.ObjectVersion == 0 || RaObjectframe.ObjectVersion == 1) &&(file_version == 0 || file_version == 1))
	  {
	  	while(num_headers)
	  	{
	  		num_headers--;
	  		ReadObject_RA(AIF,&RaObjectframe);  //GUID guid;
	  		switch(RaObjectframe.ObjectUID)
	  		{
	  		case RM_PROPERTIES_OBJECT:
	  			{
	  				GetInfo_SkipBsInByte(36,AIF);
	  				propHdr_num_streams = GetInfo_GetBsInByte(2,AIF);
	  				GetInfo_GetBsInByte(2,AIF);
	  				break;
      /*    
              pInt->propHdr.id           = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.size         = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.version      = rm_unpack16(&pBuf, &ulLen);
              pInt->propHdr.max_bit_rate = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.avg_bit_rate = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.max_pkt_size = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.avg_pkt_size = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.num_pkts     = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.duration     = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.preroll      = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.index_offset = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.data_offset  = rm_unpack32(&pBuf, &ulLen);
              pInt->propHdr.num_streams  = rm_unpack16(&pBuf, &ulLen);
              pInt->propHdr.flags        = rm_unpack16(&pBuf, &ulLen);
	  */			
	  			}
	  		case RM_MEDIA_PROPERTIES_OBJECT:
	  			{	
	  				if(!AudioObjInfo)
	  				{
	  					MediaObj.stream_num = GetInfo_GetBsInByte(2,AIF);
	  					RaObjectframe.size -=12;
	  					if(MediaObj.stream_num>=propHdr_num_streams)
	  					{
	  						Ret = 0;
	  						break;
	  					}else
	  					{
	  						char StreamInfo[100];
	  						MediaObj.max_bit_rate   = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
	  						MediaObj.avg_bit_rate   = GetInfo_GetBsInByte(4,AIF);// rm_unpack32(&pBuf, &ulLen);
	  						MediaObj.max_pkt_size   = GetInfo_GetBsInByte(4,AIF);// rm_unpack32(&pBuf, &ulLen);
	  						MediaObj.avg_pkt_size   = GetInfo_GetBsInByte(4,AIF);// rm_unpack32(&pBuf, &ulLen);
	  						MediaObj.start_time     = GetInfo_GetBsInByte(4,AIF);// rm_unpack32(&pBuf, &ulLen);
	  						MediaObj.preroll        = GetInfo_GetBsInByte(4,AIF);// rm_unpack32(&pBuf, &ulLen);
	  						MediaObj.duration       = GetInfo_GetBsInByte(4,AIF);// rm_unpack32(&pBuf, &ulLen);
	  						MediaObj.stream_name_sz = GetInfo_GetBsInByte(1,AIF);// rm_unpack8(&pBuf,  &ulLen);
	  						RaObjectframe.size -= (28+1);
                RaObjectframe.size -= MediaObj.stream_name_sz;
                GetInfo_SkipBsInByte(MediaObj.stream_name_sz,AIF);
                MediaObj.mime_type_sz = GetInfo_GetBsInByte(1,AIF);
	  						for(i=0;i<MediaObj.mime_type_sz;i++)                
	                StreamInfo[i] = GetInfo_GetBsInByte(1,AIF);
	                StreamInfo[i] = 0;
					RaObjectframe.size -= (MediaObj.mime_type_sz+1);
    
	  						if(bRet)break;
	  						if (!strcmp(StreamInfo, REALAUDIO_MIME_TYPE) ||
                    !strcmp(StreamInfo, REALAUDIO_ENCRYPTED_MIME_TYPE))
                {
                      int ret;
                      //RaObjectframe.size -= (MediaObj.mime_type_sz+1);
                      MediaObj.type_spec_sz = GetInfo_GetBsInByte(4,AIF);
                      ret=ra_depacki_unpack_opaque_data(MediaObj.type_spec_sz,AIF,&ra_shdr);
                      RaObjectframe.size -=(4+MediaObj.type_spec_sz);
                      if(ret){
                      	AIF->ulAudio_name_sz = RACODECLEN;
                      	AIF->ulAudio_name = AIF->InforBuf;
                      	AIF->InforBuf += AIF->ulAudio_name_sz;
                      	if(AIF->InforBufLeftLength<AIF->ulAudio_name_sz)
	  					             AIF->ulAudio_name_sz = AIF->InforBufLeftLength;
	  				            AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;
                      	
                      	if (ra_shdr.ulCodecID == 0x636F6F6B) /* 'cook' */
                      	{
                      		memcpy(AIF->ulAudio_name,RAcodec[1],AIF->ulAudio_name_sz);
                      	}
                      	else if (ra_shdr.ulCodecID == 0x73697072) /* 'sipr' */
                      	{
                      		memcpy(AIF->ulAudio_name,RAcodec[2],AIF->ulAudio_name_sz);
                      	}
                      	else if (ra_shdr.ulCodecID == 0x61747263) /* 'atrc' */
                      	{	
                      		memcpy(AIF->ulAudio_name,RAcodec[3],AIF->ulAudio_name_sz);
                      	}
                      	else if (ra_shdr.ulCodecID == 0x72616163 || /* 'raac' */
                      		ra_shdr.ulCodecID == 0x72616370)   /* 'racp' */
                      	{
                      		memcpy(AIF->ulAudio_name,RAcodec[4],AIF->ulAudio_name_sz);
                      	}
                      	else if (ra_shdr.ulCodecID == 0x646E6574) /* 'dnet' */
                      	{
                      		memcpy(AIF->ulAudio_name,RAcodec[5],AIF->ulAudio_name_sz);
                      	}
                      	else
                      	{
                      		memcpy(AIF->ulAudio_name,RAcodec[0],AIF->ulAudio_name_sz);
                      	}
                      	AudioObjInfo = 1;
                      	AIF->ulSampleRate = ra_shdr.ulSampleRate;
                      	AIF->ulBitRate = MediaObj.avg_bit_rate;
                      	AIF->ulBitsSample = 16;
                      	AIF->ulChannels = ra_shdr.ulChannels;
                      	AIF->ulDuration = MediaObj.duration;
                      	Ret = 1;
                      }
                   bRet = 1;
                  }
	  						GetInfo_SkipBsInByte(RaObjectframe.size,AIF);
	  
	  					}
	  				}else
	  				{	
	  					RaObjectframe.size -=10;
	  					GetInfo_SkipBsInByte(RaObjectframe.size,AIF);
	  				}
	  				break;
	  			}
	  			
	  		case RM_CONTENT_OBJECT:
	  			{	
	  				char data;
	  				RaObjectframe.size -=12;
	  				AIF->ultitle_sz = GetInfo_GetBsInByte(2,AIF);
	  				AIF->ultitle = AIF->InforBuf;
	  				if(AIF->InforBufLeftLength<AIF->ultitle_sz)
	  					break;
	  				AIF->InforBufLeftLength -=AIF->ultitle_sz;
	  				RaObjectframe.size -=(AIF->ultitle_sz+2);
	  				if(RaObjectframe.size < 0)
	  					break;
	  				
	  				for(i=0;i<AIF->ultitle_sz;i++)
	  				{
	  					data= (char )GetInfo_GetBsInByte(1,AIF);
	  					*AIF->InforBuf++= data;
	  				}
	  				AIF->ulauthor_sz = GetInfo_GetBsInByte(2,AIF);
	  				AIF->ulauthor = AIF->InforBuf;
	  				if(AIF->InforBufLeftLength<AIF->ulauthor_sz)
	  					break;
	  				RaObjectframe.size -=(AIF->ulauthor_sz);
	  				if(RaObjectframe.size < 0)
	  					break;
	  				
	  				AIF->InforBufLeftLength -=AIF->ulauthor_sz;
	  				for(i=0;i<AIF->ulauthor_sz;i++)
	  				{
	  					data= (char )GetInfo_GetBsInByte(1,AIF);
	  					*AIF->InforBuf++= data;
	  				}
	  				GetInfo_SkipBsInByte(RaObjectframe.size,AIF);
	  			break;
	  			}					
	  			
	  			
	  			
	  		case RM_DATA_OBJECT:
	  			{
	  				num_headers = 0;
	  				break;
	  			}
	  		default:
	  			{
	  					RaObjectframe.size -=10;
	  					GetInfo_SkipBsInByte(RaObjectframe.size,AIF);
	  					break;
	  			}
    
	  		}
	  		
	  	}
	  }
	}
	else if (RaObjectframe.ObjectUID ==RA_HEADER_OBJECT)
	{
    unsigned int freq = 0; 
    unsigned int chans = 0;
    unsigned int blocksize = 0;
    unsigned short verbose = 1;
		unsigned int codec = 0;
			*in++ = '.';
			*in++ = 'r';
			*in++ = 'a';
			*in++ = 0xfd;
			*in++ = (RaObjectframe.size>>24) & 0xff;
			*in++ = (RaObjectframe.size>>16) & 0xff;
			*in++ = (RaObjectframe.size>>8) & 0xff;
			*in++ = (RaObjectframe.size) & 0xff; 
		
		if (buf[5]==4)
		{
      int size;

			if ((i = ReadFunc(in, 14,AIF)) <= 0) goto RA_error;
			in += i;

			size = (buf[18]<<24 | buf[19]<<16 | buf[20]<<8 | buf[21]) - 6;

			if (ReadFunc(in, size,AIF) <= 0) goto RA_error;

			codec = GetCodec(buf, &freq, &chans, &blocksize, verbose,AIF);
			//ra_depacki_unpack_opaque_data((int)(in-buf+size),AIF,&ra_shdr);
			AIF->ulSampleRate = freq;
      AIF->ulBitsSample = 16;
      AIF->ulChannels = chans;
      switch(codec){
      	case 2://28_8
        {
        	 AIF->ulDuration = (int)(((double)(filelength -(in-buf)-size) * (double)(AUDIOBLOCK*1000))/(double)(blocksize*chans*freq));
           AIF->ulBitRate  = (int)(((double)(filelength) * (double)(8*1000)) /(double)(AIF->ulDuration));
           Ret = SUCCESS;
		   break;
        }
        case 3://ac3
        {
        	int	syncword,data,fscod;
        	int frmsizecod,bsid;
        	unsigned int (*GBs)(int ByteLen,__audio_file_info_t *AIF)=NULL;
	        short halfrate;
          
	        syncword = GetInfo_GetBsInByte(2,AIF);
	        if(syncword ==SYNC_WD)
	        		GBs = GetInfo_GetBsInByte;
	        else if(syncword == SYNC_WD2)
	        {	
	        	int byte1,byte2;		
	        	byte1 = ( syncword << 8 ) & 0xff00;
	        	byte2 = ( syncword >> 8 ) & 0x00ff;
	        	syncword=(byte1 | byte2);
	        	GBs = GetInfo_GetBsInByteLI;
	        }else
	        {
	        	goto RA_error;
	        }
	        data = GBs(2,AIF);//skip 2 byte
	        
	        data = GBs(2,AIF);
	        fscod = data>>14;
	        frmsizecod = (data>>8)&0x3f;
	        bsid = (data>>3)&0x1f;
	        AIF->ulSampleRate = fstable[fscod];
	        //AIF->ulChannels = channumtable[acmod] + lfeon;
	        AIF->ulBitRate = bitrateTable[frmsizecod]*1000;//
	        
	        halfrate = halfrateTable[bsid];
	        AIF->ulSampleRate = AIF->ulSampleRate>>halfrate;
	        AIF->ulBitRate = AIF->ulBitRate>>halfrate;//
	        AIF->ulDuration = (int)(((double)(filelength) * (double)(8*1000)) /(double)(AIF->ulBitRate));
        	Ret = SUCCESS;
			break;
        }
        case 4://sipr
        {
        	 int blockduring = 0;
        	 switch(blocksize){
        	 	case BLOCK_5K0:
        	 		{
        	 			blockduring = DURMS_5K0;
        	 			break;
        	 		}
        	 	case BLOCK_6K5:
        	 		{
        	 			blockduring = DURMS_6K5;
        	 			break;
        	 		}
        	 	case BLOCK_8K5:
        	 		{
        	 			blockduring = DURMS_8K5;
        	 			break;
        	 		}
        	 	case BLOCK_16K0:
        	 		{
        	 			blockduring = DURMS_16K0;
        	 			break;
        	 		}
        	 	default:
        	 		goto RA_error;        	 		
        	 	}
        	 AIF->ulDuration = (int)(((double)(filelength -(in-buf)-size) * (double)(blockduring))/(double)(blocksize));
           AIF->ulBitRate  = (int)(((double)(blocksize) * (double)(8*1000)) /(double)(blockduring));        	 	
           Ret = SUCCESS;	 
		   break;
        }
        default:
        	goto RA_error;
      }
		}
		else if (buf[5]==3)
		{
			in = buf + 8;

			while (buf[6])
			{
				if (ReadFunc(in, 256,AIF) <= 0) goto RA_error;
			}

			if (ReadFunc(in, buf[7],AIF) <= 0) goto RA_error;

			codec = GetCodec(buf, &freq, &chans, &blocksize, verbose,AIF);
			AIF->ulSampleRate = freq;
      AIF->ulBitsSample = 16;
      AIF->ulChannels = chans;
      AIF->ulDuration = (int)(((double)(filelength -(in-buf)-buf[7]) * (double)(AUDIOBLOCK*1000))/(double)(blocksize*chans*freq));
      AIF->ulBitRate  = (int)(((double)(filelength) * (double)(8*1000)) /(double)(AIF->ulDuration));
		Ret = SUCCESS;
		}
		else
		{
			goto RA_error;
		}

	} 
	
	else
	{
		goto RA_error;
	}
	 if(buf)
	{
		free(buf);
		buf = NULL;
	}
  GetInfo_BsEndRelease(AIF);
	return Ret;
RA_error:
	if(buf)
	{
		free(buf);
		buf = NULL;
	}
	GetInfo_BsEndRelease(AIF);
	return ERROR;	
}

static void	ReadObject_RA(__audio_file_info_t *AIF,_RaOBJECT *RaObjectframe)
{
		RaObjectframe->ObjectUID = GetInfo_GetBsInByte(4,AIF);  //GUID guid;
		RaObjectframe->size = GetInfo_GetBsInByte(4,AIF);
		RaObjectframe->ObjectVersion = GetInfo_GetBsInByte(2,AIF);
}

#if 1
#define	RM_MULTIHEADER_OBJECT	0x4D4C5449
#define	RA_FORMAT_ID			0x2E7261FD
static int ra_depacki_unpack_opaque_data(int ulLen,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr)
{
		__u32 UID = 0;
		__u16 usVersion = 0;
		int	retVal = 0;
		UID = GetInfo_GetBsInByte(4,AIF);
		ulLen -=4;
		if(UID == RM_MULTIHEADER_OBJECT)
		{
			GetInfo_SkipBsInByte(ulLen,AIF);
		}else if(UID == RA_FORMAT_ID)
		{
			usVersion = GetInfo_GetBsInByte(2,AIF);
			ulLen -=2;
			switch (usVersion)
            {
			case 3:
				retVal = ra_depacki_unpack_raformat3(ulLen,AIF,ra_shdr);
				break;
			case 4:
				retVal = ra_depacki_unpack_raformat4(ulLen,AIF,ra_shdr);
				break;
			case 5:
				retVal = ra_depacki_unpack_raformat5(ulLen,AIF,ra_shdr);
				break;
            }
		}
		return retVal;
}


static int ra_depacki_unpack_raformat3(int ulLen,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr)
{
	GetInfo_SkipBsInByte(ulLen,AIF);
    return 0;
}

static int ra_depacki_unpack_raformat4(int ulLen,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr)
{
	ra_substream_hdr *pHdr = ra_shdr;
    int retVal = 0;
	
    if (ulLen >= 63 )
    {
        //__u32 ulSize = 0;
        //__u32 i      = 0;
		GetInfo_SkipBsInByte(10,AIF);
		ulLen -= 10;
        pHdr->usRAFormatVersion  = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
        pHdr->usRAFormatRevision = GetInfo_GetBsInByte(2,AIF);// rm_unpack16(&pBuf, &ulLen);
		ulLen -= 4;
        /* Sanity check */
        if (pHdr->usRAFormatVersion  == 4 &&
            pHdr->usRAFormatRevision == 0)
        {
            pHdr->usHeaderBytes         =  GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->usFlavorIndex         =  GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulGranularity         =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulTotalBytes          =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulBytesPerMin         =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulBytesPerMin2        =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulInterleaveFactor    =  GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulInterleaveBlockSize =  GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulUserData            =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulSampleRate          =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulSampleRate        >>= 16;
            pHdr->ulSampleSize          =  GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulChannels            =  GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
			if(GetInfo_GetBsInByte(1,AIF)==4)
			{
            pHdr->ulInterleaverID       =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32_from_byte_string(&pBuf, &ulLen);
			ulLen -=4;
			}
			if(GetInfo_GetBsInByte(1,AIF)==4)
			{
			pHdr->ulCodecID             =  GetInfo_GetBsInByte(4,AIF);//rm_unpack32_from_byte_string(&pBuf, &ulLen);
			ulLen -=4;
			}
			pHdr->bIsInterleaved        =  GetInfo_GetBsInByte(1,AIF);//rm_unpack8(&pBuf, &ulLen);
            pHdr->bCopyByte             =  GetInfo_GetBsInByte(1,AIF);//rm_unpack8(&pBuf, &ulLen);
            pHdr->ucStreamType          =  GetInfo_GetBsInByte(1,AIF);//rm_unpack8(&pBuf, &ulLen);
			ulLen -= 41;	
            retVal = 1;
        }
    }
	GetInfo_SkipBsInByte(ulLen,AIF);

    return retVal;
}

static int ra_depacki_unpack_raformat5(int ulLen ,__audio_file_info_t *AIF,ra_substream_hdr  *ra_shdr)
{
    int retVal = 0;
	ra_substream_hdr *pHdr = ra_shdr;

    if (ulLen >= 68)
    {
        /* Init local variables */
        //__u32 ulSize = 0;
        //__u32 i      = 0;
		GetInfo_SkipBsInByte(10,AIF);
		ulLen -= 10;
        pHdr->usRAFormatVersion  = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
        pHdr->usRAFormatRevision = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
		ulLen -=4;
        /* Sanity check */
        if (pHdr->usRAFormatVersion  == 5 &&
            pHdr->usRAFormatRevision == 0)
        {
            pHdr->usHeaderBytes         = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->usFlavorIndex         = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulGranularity         = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulTotalBytes          = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulBytesPerMin         = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulBytesPerMin2        = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulInterleaveFactor    = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulInterleaveBlockSize = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulCodecFrameSize      = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulUserData            = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulSampleRate          = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulSampleRate        >>= 16;
            pHdr->ulActualSampleRate    = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulActualSampleRate  >>= 16;
            pHdr->ulSampleSize          = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulChannels            = GetInfo_GetBsInByte(2,AIF);//rm_unpack16(&pBuf, &ulLen);
            pHdr->ulInterleaverID       = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->ulCodecID             = GetInfo_GetBsInByte(4,AIF);//rm_unpack32(&pBuf, &ulLen);
            pHdr->bIsInterleaved        = GetInfo_GetBsInByte(1,AIF);//rm_unpack8(&pBuf, &ulLen);
            pHdr->bCopyByte             = GetInfo_GetBsInByte(1,AIF);//rm_unpack8(&pBuf, &ulLen);
            pHdr->ucStreamType          = GetInfo_GetBsInByte(1,AIF);//rm_unpack8(&pBuf, &ulLen);
            pHdr->ucScatterType         = GetInfo_GetBsInByte(1,AIF);//rm_unpack8(&pBuf, &ulLen);
            /* Clear the return value */
            retVal = 1;
			ulLen -=54;
        }
    }
	GetInfo_SkipBsInByte(ulLen,AIF);
    return retVal;
}
#endif

static int ReadFunc(char *buf,int len,__audio_file_info_t *AIF)
{
	int i;
	char *pBuf = buf;
	for(i = 0;i < len;i++)
	{
		*pBuf++ =GetInfo_GetBsInByte(1,AIF);
	}
	return i;
}

static int GetCodec(unsigned char *buf, unsigned int *freq, unsigned int *chans, unsigned int *blocksize, unsigned short verbose,__audio_file_info_t *AIF)
{
	int codec = 0;

	if (buf[0]=='.' && buf[1]=='r' && buf[2]=='a' && buf[3]==0xfd)
	{
		if (buf[5]==4)
		{
			int i;

			i = buf[56] + 58;
			AIF->ulAudio_name_sz = RACODECLEN;
			AIF->ulAudio_name = AIF->InforBuf;      
      if(AIF->InforBufLeftLength<AIF->ulAudio_name_sz)
	  	   AIF->ulAudio_name_sz = AIF->InforBufLeftLength;
	    AIF->InforBuf += AIF->ulAudio_name_sz;
	    AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;
			if (buf[i]=='2' && buf[i+1]=='8' && buf[i+2]=='_' && buf[i+3]=='8')
			{ 
				codec = 2;
				memcpy(AIF->ulAudio_name,RAcodec[7],AIF->ulAudio_name_sz); 
			}
			else if (buf[i]=='c' && buf[i+1]=='o' && buf[i+2]=='o' && buf[i+3]=='k')
			{ 
				codec = 5;
				memcpy(AIF->ulAudio_name,RAcodec[8],AIF->ulAudio_name_sz); 
			}
			else if (buf[i]=='s' && buf[i+1]=='i' && buf[i+2]=='p' && buf[i+3]=='r')
			{ 
				codec = 4;
				memcpy(AIF->ulAudio_name,RAcodec[9],AIF->ulAudio_name_sz); 
			}
			else if (buf[i]=='a' && buf[i+1]=='t' && buf[i+2]=='r' && buf[i+3]=='c')
			{ 
				codec = 6;
				memcpy(AIF->ulAudio_name,RAcodec[10],AIF->ulAudio_name_sz); 
			}
			else if ( (buf[i]=='r' && buf[i+1]=='a' && buf[i+2]=='a' && buf[i+3]=='c') ||
				        (buf[i]=='r' && buf[i+1]=='a' && buf[i+2]=='c' && buf[i+3]=='p') ) 
			{ 
				codec = 7;
				memcpy(AIF->ulAudio_name,RAcodec[11],AIF->ulAudio_name_sz); 
			}
			else if (buf[i]=='d' && buf[i+1]=='n' && buf[i+2]=='e' && buf[i+3]=='t')
			{ 
				codec = 3;
				memcpy(AIF->ulAudio_name,RAcodec[12],AIF->ulAudio_name_sz); 
			}
			else                                                         
      {                                                            
      	memcpy(AIF->ulAudio_name,RAcodec[0],AIF->ulAudio_name_sz); 
      }
//			else if (buf[i]=='d' && buf[i+1]=='n' && buf[i+2]=='e' && buf[i+3]=='t') codec = 3;
//			else if (buf[i]=='s' && buf[i+1]=='i' && buf[i+2]=='p' && buf[i+3]=='r') codec = 4;
        
			*freq = (buf[48]<<8 | buf[49]);
			*chans = (buf[54]<<8 | buf[55]);
			*blocksize = (buf[24]<<24 | buf[25]<<16 | buf[26]<<8 | buf[27]);
			
      
			if (verbose)
			{
				int j;
				char *c, *s;//, *s2;
      
				s = buf + 57;
				j = buf[56] + 57;
				c = buf + j;
				j += *c + 1;
				*c = 0;
			//	s2 = c + 1;
				c = buf + j;
				*c = 0;
//				FPrintf(StdErr, "Codec:     %s/%s (v%ld)\n", (unsigned long)s, (unsigned long)s2, (unsigned long)codec);
				s = buf + 70;
				j = buf[69] + 70;
				c = buf + j;
				j += *c + 1;
				*c = 0;
//				FPrintf(StdErr, "Title:     %s\n", (unsigned long)s);
        AIF->ultitle_sz = (int)(c-s);
			  AIF->ultitle = AIF->InforBuf;
        if(AIF->InforBufLeftLength<AIF->ultitle_sz)
	  	     AIF->ultitle_sz = AIF->InforBufLeftLength;
	      AIF->InforBuf += AIF->ultitle_sz;
	      AIF->InforBufLeftLength -=AIF->ultitle_sz;
	      memcpy(AIF->ultitle,s,AIF->ultitle_sz);
				s = c + 1;
				c = buf + j;
				j += *c + 1;
				*c = 0;
//				FPrintf(StdErr, "Author:    %s\n", (unsigned long)s);
        AIF->ulauthor_sz = (int)(c-s);
			  AIF->ulauthor = AIF->InforBuf;        
        if(AIF->InforBufLeftLength<AIF->ulauthor_sz)
	  	     AIF->ulauthor_sz = AIF->InforBufLeftLength;
	      AIF->InforBuf += AIF->ulauthor_sz;
	      AIF->InforBufLeftLength -=AIF->ulauthor_sz;
	      memcpy(AIF->ulauthor,s,AIF->ulauthor_sz);
				s = c + 1;
				c = buf + j;
				*c = 0;
//				FPrintf(StdErr, "Copyright: %s\n\n", (unsigned long)s);
			}
		}
		else if (buf[5]==3)
		{
			codec = 1;
			 
			*freq = 8000;
			*chans = (buf[8]<<8 | buf[9]);
			*blocksize = (buf[10]<<24 | buf[11]<<16 | buf[12]<<8 | buf[13]);
			AIF->ulAudio_name_sz = RACODECLEN;
			AIF->ulAudio_name = AIF->InforBuf;      
      if(AIF->InforBufLeftLength<AIF->ulAudio_name_sz)
	  	   AIF->ulAudio_name_sz = AIF->InforBufLeftLength;
	    AIF->InforBuf += AIF->ulAudio_name_sz;
	    AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;
			memcpy(AIF->ulAudio_name,RAcodec[6],AIF->ulAudio_name_sz);

			if (verbose)
			{
				int j;
				char *c, *s;

//				FPrintf(StdErr, "Codec:     14_4 (v%ld)\n", (unsigned long)codec);
				s = buf + 23;
				j = buf[22] + 23;
				c = buf + j;
				j += *c + 1;
				*c = 0;
//				FPrintf(StdErr, "Title:     %s\n", (unsigned long)s);
				AIF->ultitle_sz = (int)(c-s);
			  AIF->ultitle = AIF->InforBuf;
        if(AIF->InforBufLeftLength<AIF->ultitle_sz)
	  	     AIF->ultitle_sz = AIF->InforBufLeftLength;
	      AIF->InforBuf += AIF->ultitle_sz;
	      AIF->InforBufLeftLength -=AIF->ultitle_sz;
		    memcpy(AIF->ultitle,s,AIF->ultitle_sz);
				s = c + 1;
				c = buf + j;
				j += *c + 1;
				*c = 0;
//				FPrintf(StdErr, "Author:    %s\n", (unsigned long)s);
				AIF->ulauthor_sz = (int)(c-s);
			  AIF->ulauthor = AIF->InforBuf;        
        if(AIF->InforBufLeftLength<AIF->ulauthor_sz)
	  	     AIF->ulauthor_sz = AIF->InforBufLeftLength;
	      AIF->InforBuf += AIF->ulauthor_sz;
	      AIF->InforBufLeftLength -=AIF->ulauthor_sz;
	      memcpy(AIF->ulauthor,s,AIF->ulauthor_sz);
				s = c + 1;
				c = buf + j;
				*c = 0;
//				FPrintf(StdErr, "Copyright: %s\n\n", (unsigned long)s);
			}
		}
	}

	return codec;
}
