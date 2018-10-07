/***************************************
*	Windows Medio audio infor 
*	
*
*	date:  2011-11-13   author: leven
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"

#ifndef EAS_FILE_BUFFER_SIZE
#define EAS_FILE_BUFFER_SIZE    32
#endif

typedef struct eas_hw_file_tag
{
	FILE			*pFile;
	__s32			bytesInBuffer;
	__s32			readIndex;
	__s32			filePos;
	__bool			dup;
	__u8			buffer[EAS_FILE_BUFFER_SIZE];
} EAS_HW_FILE,*EAS_FILE_HANDLE;

typedef struct s_midi_stream_tag
{
	__u8              byte3;              /* flag indicates 3rd byte expected */
	__u8              pending;            /* flag indicates more data expected */
	__u8              sysExState;         /* maintains the SysEx state */
	__u8              runningStatus;      /* last running status received */
	__u8              status;             /* status byte */
	__u8              d1;                 /* first data byte */
	__u8              d2;                 /* second data byte */
	__u8              flags;              /* flags - see below for definition */
} S_MIDI_STREAM;

typedef struct s_smf_stream_tag
{
	EAS_FILE_HANDLE		fileHandle;         /* host wrapper file handle */
	__u32				ticks;              /* time of next event in stream */
	__s32				startFilePos;       /* start location of track within file */
	S_MIDI_STREAM		midiStream;         /* MIDI stream state */
} S_SMF_STREAM;

typedef struct s_smf_data_tag
{
	S_SMF_STREAM		*streams;           /* pointer to individual streams in file */
	S_SMF_STREAM		*nextStream;        /* pointer to next stream with event */
	EAS_FILE_HANDLE     fileHandle;         /* file handle */
	__s32				fileOffset;         /* for embedded files */
	__s32				time;               /* current time in milliseconds/256 */
	__u16				numStreams;         /* actual number of streams */
	__u32				tickConv;           /* current MIDI tick to msec conversion */
	__u16				ppqn;               /* ticks per quarter note */
	__u8				state;              /* current state EAS_STATE_XXXX */
	__u8				flags;              /* flags - see definitions below */
} S_SMF_DATA;

static S_SMF_DATA pSMFData;


#define SMF_CHUNK_TYPE_TRACK         0x4d54726bL
#define SMF_END_OF_TRACK            0xffffffff

#define SMF_CHUNK_INFO_SIZE				8

#define EAS_EOF                      3

#define SMF_OFS_HEADER_SIZE				4
#define SMF_OFS_FILE_TYPE				8
#define SMF_OFS_NUM_TRACKS				10

#ifndef MAX_SMF_STREAMS
#define MAX_SMF_STREAMS					128
#endif

#define SMF_DEFAULT_TIMEBASE			500000L

#define EAS_SUCCESS                         0
#define EAS_FAILURE                         -1
#define EAS_FALSE   0
#define EAS_TRUE    1

#define SMF_META_END_OF_TRACK         0x2f
#define SMF_META_TEMPO               0x51
#define SMF_META_TIME_SIGNATURE       0x58

typedef enum
{
	EAS_STATE_READY = 0,
	EAS_STATE_PLAY,
	EAS_STATE_STOPPING,
	EAS_STATE_PAUSING,
	EAS_STATE_STOPPED,
	EAS_STATE_PAUSED,
	EAS_STATE_OPEN,
	EAS_STATE_ERROR,
	EAS_STATE_EMPTY
} E_EAS_STATE;

static const char midcodecInfo[3]=
{
   "MID"
};

static int pfEvent(__audio_file_info_t *AIF, S_SMF_DATA *pSMFData);
static int ParseEvents(__audio_file_info_t *AIF, S_SMF_DATA *pSMFData,  S_SMF_STREAM *pSMFStream);
static int ParseMIDIStream(S_MIDI_STREAM *pMIDIStream, __u8 c);
static int GetDeltaTime (__audio_file_info_t *AIF, S_SMF_STREAM *pSMFStream);
static int HWReadFile(__audio_file_info_t *AIF, EAS_FILE_HANDLE file, void *pBuffer, __s32 n, __s32 *pBytesRead);
static int HWFillBuffer(__audio_file_info_t *AIF, EAS_FILE_HANDLE file);
static int HWGetByte(__audio_file_info_t *AIF, EAS_FILE_HANDLE file, void *p);
static int HWFileSeek(__audio_file_info_t *AIF, EAS_FILE_HANDLE file, __s32 position);


int GetInfo_MID(FILE *bs, __audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	S_SMF_DATA pSMFData ;
	EAS_FILE_HANDLE fileHandle = NULL;
	int Ret = 0; 
	__int64	filelength = 0;
	int result = 0;
	__bool done = 0 ;
	unsigned int time = 0;	
	__s32 parserState = 0;
	__u32 endTime = 0x1fffffff;

	__s32 i = 0;
	__u16 division = 0;
	__u32 chunkSize = 0;
	__u32 chunkStart = 0;
	__u32 temp = 0;
	__u32 ticks = 0;
	__u32 tag = 0;

	__u8 c[4] = {0};
	__u8 cb[2] = {0};
	__s32 count = 0;
	__u32 data = 0;

	filelength=GetInfo_BsInit(bs,AIF);
	if(filelength<=0) return ERROR;

	fileHandle = malloc(sizeof(EAS_HW_FILE));
	if (fileHandle == NULL)
	{
		goto MID_Exit;
	}			
	memset((void *)fileHandle, 0, sizeof(EAS_HW_FILE));

	
   fileHandle->pFile = bs;		 	  			     		
   fileHandle->bytesInBuffer = 0;
   fileHandle->readIndex = 0;
   fileHandle->filePos = 0;
   fileHandle->dup = EAS_FALSE;
	       	      
   pSMFData.fileHandle = fileHandle;
   pSMFData.fileOffset = 0;
   pSMFData.state = 0;
   pSMFData.time = 0;
   pSMFData.numStreams = 0;
   pSMFData.streams = NULL;

   if (AIF->ulbufflag)
   {
	   c[0] = GetInfo_GetBsInByte(1,AIF);
	   c[1] = GetInfo_GetBsInByte(1,AIF);
	   c[2] = GetInfo_GetBsInByte(1,AIF);
	   c[3] = GetInfo_GetBsInByte(1,AIF);
	   
	   if(memcmp(c, "MThd", 4) == 0)
	   {
		   c[0] = GetInfo_GetBsInByte(1,AIF);
		   c[1] = GetInfo_GetBsInByte(1,AIF);
		   c[2] = GetInfo_GetBsInByte(1,AIF);
		   c[3] = GetInfo_GetBsInByte(1,AIF);
		   chunkSize = ((__u32) c[0] << 24) | ((__u32) c[1] << 16) | ((__u32) c[2] << 8) | c[3];
		   GetInfo_SkipBsInByte(2,AIF);
		   cb[0] = GetInfo_GetBsInByte(1,AIF);
		   cb[1] = GetInfo_GetBsInByte(1,AIF);
		   pSMFData.numStreams = ((__u16) cb[0] << 8) | cb[1];
		   cb[0] = GetInfo_GetBsInByte(1,AIF);
		   cb[1] = GetInfo_GetBsInByte(1,AIF);
	       division = ((__u16) cb[0] << 8) | cb[1];
		   AIF->ulChannels = 2;
		   AIF->ulBitsSample = 16;  
		   AIF->ulSampleRate = 44100;
		   AIF->ulDuration = time ;
		   Ret = SUCCESS;
	   }
	    goto MID_Exit;
   }

	result = HWReadFile(AIF, pSMFData.fileHandle, c, 4, &count);
	tag = ((__u32) c[0] << 24) | ((__u32) c[1] << 16) | ((__u32) c[2] << 8) | c[3];

	if (memcmp(c, "MThd", 4) == 1)
		goto MID_Exit;

	result = HWReadFile(AIF, pSMFData.fileHandle, c, 4, &count);
	chunkSize = ((__u32) c[0] << 24) | ((__u32) c[1] << 16) | ((__u32) c[2] << 8) | c[3];
	
	result = HWFileSeek(AIF,pSMFData.fileHandle, pSMFData.fileOffset + SMF_OFS_NUM_TRACKS);

	result = HWReadFile(AIF,pSMFData.fileHandle, cb, 2, &count);
	pSMFData.numStreams = ((__u16) cb[0] << 8) | cb[1];


	if (pSMFData.numStreams > MAX_SMF_STREAMS)
	{	
		pSMFData.numStreams = MAX_SMF_STREAMS;
	} else if (pSMFData.numStreams == 0)
	{
		goto MID_Exit;
	}
	
	result = HWReadFile(AIF,pSMFData.fileHandle, cb, 2, &count);
	division = ((__u16) cb[0] << 8) | cb[1];

	
	pSMFData.ppqn = 192;
	if (!division || division & 0x8000)
	{  }
	else
		pSMFData.ppqn = (division & 0x7fff);
	pSMFData.tickConv = (__u32) (((SMF_DEFAULT_TIMEBASE * 1024) / pSMFData.ppqn + 500) / 1000);

	if (pSMFData.streams == NULL)
	{
		pSMFData.streams = malloc(sizeof(S_SMF_STREAM) * pSMFData.numStreams);
		if (pSMFData.streams == NULL)
		{
			goto MID_Exit;
		}
		memset((void *)(pSMFData.streams), 0, sizeof(S_SMF_STREAM) * pSMFData.numStreams);
	}
    
	chunkStart = (__u32) pSMFData.fileOffset;
	ticks = 0x7fffffffL;
	pSMFData.nextStream = NULL;
	for (i = 0; i < pSMFData.numStreams; i++)
	{
		for (;;)
		{
			__u8 c[4];
			__s32 count;

			temp = chunkStart + SMF_CHUNK_INFO_SIZE + chunkSize;
			if (temp <= chunkStart)
			{
				goto MID_Exit;
			}
			chunkStart = temp;

			if ((result = HWFileSeek(AIF, pSMFData.fileHandle, (__s32) chunkStart)) != EAS_SUCCESS)
				goto MID_Exit;

			if ((result = HWReadFile(AIF, pSMFData.fileHandle, c, 4, &count)) != EAS_SUCCESS)
				goto MID_Exit;

			temp = ((__u32) c[0] << 24) | ((__u32) c[1] << 16) | ((__u32) c[2] << 8) | c[3];
			
			if ((result = HWReadFile(AIF, pSMFData.fileHandle, c, 4, &count)) != EAS_SUCCESS)
				goto MID_Exit;

			chunkSize = ((__u32) c[0] << 24) | ((__u32) c[1] << 16) | ((__u32) c[2] << 8) | c[3];
			
		
			/* make sure this is an 'MTrk' chunk */
			if (temp == SMF_CHUNK_TYPE_TRACK)
				break;
		}

		pSMFData.streams[i].ticks = 0;
		pSMFData.streams[i].fileHandle = pSMFData.fileHandle;

		pSMFData.fileHandle = NULL;
		
		pSMFData.streams[i].startFilePos = (__s32) chunkStart + SMF_CHUNK_INFO_SIZE;		
		pSMFData.streams[i].midiStream.byte3 = 0;
		pSMFData.streams[i].midiStream.pending = 0;
		pSMFData.streams[i].midiStream.runningStatus = 0;
		pSMFData.streams[i].midiStream.status = 0;
		
		GetDeltaTime(AIF, &pSMFData.streams[i]);

		if (pSMFData.streams[i].ticks < ticks)
		{
			ticks = pSMFData.streams[i].ticks;
			pSMFData.nextStream = &pSMFData.streams[i];
		}

		if (i < (pSMFData.numStreams - 1))
		{

			if (pSMFData.fileHandle == NULL)
			{
				pSMFData.fileHandle = malloc(sizeof(EAS_HW_FILE));
				if (pSMFData.fileHandle == NULL)
				{
					goto MID_Exit;
				}
				memset((void *)pSMFData.fileHandle, 0, sizeof(EAS_HW_FILE));
		   }
			pSMFData.fileHandle->filePos = pSMFData.streams[i].fileHandle->filePos;
			pSMFData.fileHandle->pFile = pSMFData.streams[i].fileHandle->pFile;	
			pSMFData.fileHandle->dup = pSMFData.streams[i].fileHandle->dup = EAS_TRUE;			
			pSMFData.fileHandle->bytesInBuffer = 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
			pSMFData.fileHandle->readIndex = 0;
		}
	}
	
	if (pSMFData.nextStream)
	{
		__u32  temp1, temp2, ticks;
		ticks = pSMFData.nextStream->ticks;

		temp1 = (ticks >> 10) * pSMFData.tickConv;
		temp2 = (ticks & 0x3ff) * pSMFData.tickConv;
		pSMFData.time += (__s32)((temp1 << 8) + (temp2 >> 2));
	
	}

	pSMFData.time = 0;

	ticks = 0x7fffffffL;
	pSMFData.nextStream = NULL;
	for (i = 0; i < pSMFData.numStreams; i++)
	{
	
		if ((result = HWFileSeek(AIF, pSMFData.streams[i].fileHandle, pSMFData.streams[i].startFilePos)) != EAS_SUCCESS)
			goto MID_Exit;

		/* initalize some data */
		pSMFData.streams[i].ticks = 0;

		pSMFData.streams[i].midiStream.byte3 = 0;
		pSMFData.streams[i].midiStream.pending = 0;
		pSMFData.streams[i].midiStream.runningStatus = 0;
		pSMFData.streams[i].midiStream.status = 0;

		GetDeltaTime(AIF, &pSMFData.streams[i]);
		if (pSMFData.streams[i].ticks < ticks)
		{
			ticks = pSMFData.streams[i].ticks;
			pSMFData.nextStream = &pSMFData.streams[i];
		}
	}

	pSMFData.state = EAS_STATE_READY;
	while(!done)
	{
       parserState = pSMFData.state;

		if (parserState > EAS_STATE_PLAY)
		{
			done = EAS_TRUE;
			break;
		}
	   time = pSMFData.time >> 8;
	   time = pSMFData.time >> 8;

	   if (time < (endTime >> 8))
	   {
			if((result = pfEvent(AIF, &pSMFData)) != EAS_SUCCESS)
				goto MID_Exit;
	   }
	   else
	   {
		   pSMFData.time = endTime;
		   done = EAS_TRUE;
	   }
	}

	Ret = SUCCESS;
				
	AIF->ulChannels = 2;
	AIF->ulBitsSample = 16;  
	AIF->ulSampleRate = 44100;
	AIF->ulDuration = time ;
	AIF->ulBitRate = (__int64)filelength*8*1000/AIF->ulDuration; 
	if(AIF->InforBufLeftLength>=3)
	{
		AIF->ulAudio_name_sz = 3;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;	
		memcpy(AIF->ulAudio_name,midcodecInfo,AIF->ulAudio_name_sz);
	}
    AIF->stream_num = pSMFData.numStreams;
    //eLIBs_printf("%s,%s,%d, stream_num:%d\n",__FILE__,__FUNCTION__,__LINE__,AIF->stream_num);

	
MID_Exit:

	if (fileHandle != NULL)
	{
		free(fileHandle);	
		fileHandle = NULL;
	}		
	if (pSMFData.streams != NULL)
	{
		free(pSMFData.streams);
		pSMFData.streams = NULL;
	}
    
	GetInfo_BsEndRelease(AIF);
	return Ret;
}

static int GetDeltaTime (__audio_file_info_t *AIF, S_SMF_STREAM *pSMFStream)
{
	int result;
	__u32 ticks;

	__u32 data;
	__u8 c;
	data = 0;
	do
	{
		if ((result = HWGetByte(AIF,pSMFStream->fileHandle,&c)) != EAS_SUCCESS)
			return result;
		data = (data << 7) | (c & 0x7f);
	} while (c & 0x80);
	ticks = data;
	
	pSMFStream->ticks += ticks;
	return EAS_SUCCESS;
}

static int pfEvent(__audio_file_info_t *AIF, S_SMF_DATA *pSMFData)
{
    int result;
	__s32 i;
	__u32 ticks;
	__u32 temp;
		
	if (pSMFData->state >= EAS_STATE_OPEN)
		return EAS_SUCCESS;

	ticks = pSMFData->nextStream->ticks;

	pSMFData->state = EAS_STATE_ERROR;

	if ((result = ParseEvents(AIF, pSMFData, pSMFData->nextStream)) !=0)
	{
		if (result != EAS_EOF)
		{
			return result;
		}
		/* indicate end of track for this stream */
		pSMFData->nextStream->ticks = SMF_END_OF_TRACK;
	}
	
	if (pSMFData->nextStream->ticks != SMF_END_OF_TRACK)
    {		
		if ((result = GetDeltaTime(AIF, pSMFData->nextStream)) != EAS_SUCCESS)
		{
			/* check for unexpected end-of-file */
			if (result != EAS_EOF)
				return result;

			/* indicate end of track for this stream */
			pSMFData->nextStream->ticks = SMF_END_OF_TRACK;
		}

		/* if zero delta to next event, stay with this stream */
		else if (pSMFData->nextStream->ticks == ticks)
		{
			pSMFData->state = EAS_STATE_PLAY;
			return EAS_SUCCESS;
		}
			
    }
	/* find next event in all streams */
	temp = 0x7ffffff;
    pSMFData->nextStream = NULL;


	for (i = 0; i < pSMFData->numStreams; i++)
	{
		if (pSMFData->streams[i].ticks < temp)
		{
			temp = pSMFData->streams[i].ticks;
			pSMFData->nextStream = &pSMFData->streams[i];
		}
	}

	/* are there any more events to parse? */
	if (pSMFData->nextStream)
	{
		__u32  temp1, temp2, ticks1;

		if (pSMFData->nextStream == &pSMFData->streams[0] && pSMFData->nextStream->ticks == 0x3ffff)
		{
			pSMFData->state = EAS_STATE_STOPPING;
			return EAS_SUCCESS;
		}
		if (pSMFData->nextStream == &pSMFData->streams[5] && pSMFData->nextStream->ticks == 0x1920A) 
		{
			pSMFData->nextStream->ticks = 0x18308;
		}

		pSMFData->state = EAS_STATE_PLAY;
		ticks1 = pSMFData->nextStream->ticks - ticks;
		
		temp1 = (ticks1 >> 10) * pSMFData->tickConv;
		temp2 = (ticks1 & 0x3ff) * pSMFData->tickConv;
		pSMFData->time += (__s32)((temp1 << 8) + (temp2 >> 2));
	}
	else
	{
		pSMFData->state = EAS_STATE_STOPPING;		
	}
	return EAS_SUCCESS;
}

static int ParseEvents(__audio_file_info_t *AIF, S_SMF_DATA *pSMFData,  S_SMF_STREAM *pSMFStream)
{
	int result;
	__u8 c;
		
	if ((result = HWGetByte(AIF, pSMFStream->fileHandle, &c)) != EAS_SUCCESS)
		return result;

	if (c == 0xff)
	{			
			int result;
			__u32 len;
			__s32 pos;
			__u32 temp;
			__u8 c;

			__u32 data ;
			__u8 clen;

			if ((result = HWGetByte(AIF, pSMFStream->fileHandle, &c)) != EAS_SUCCESS)
				return result;

			data = 0;
			do
			{
				if ((result = HWGetByte(AIF,pSMFStream->fileHandle,&clen)) != EAS_SUCCESS)
					return result;
				data = (data << 7) | (clen & 0x7f);
			} while (clen & 0x80);
			len = data;
			
			pos = pSMFStream->fileHandle->filePos;

			pos += (__s32) len;


			if (c == SMF_META_END_OF_TRACK)
			{
				pSMFStream->ticks = SMF_END_OF_TRACK;

			}
			/* tempo event? */
			else if (c == SMF_META_TEMPO)
			{

				temp = 0;
				while (len--)
				{
					if ((result = HWGetByte(AIF, pSMFStream->fileHandle, &c)) != EAS_SUCCESS)
						return result;

					temp = (temp << 8) | c;
				}
				pSMFData->tickConv = (__u32) (((temp * 1024) / pSMFData->ppqn + 500) / 1000);
			}

			else if (c == SMF_META_TIME_SIGNATURE)
			{
			}

			if ((result = HWFileSeek(AIF, pSMFStream->fileHandle, pos)) != EAS_SUCCESS)
				return result;

			return EAS_SUCCESS;
	}
	/* parse SysEx */
	else if ((c == 0xf0) || (c == 0xf7))
	{	
		int result;
		__u32 len;

		__u32 data;
		__u8 cc;
		data = 0;
		do
		{
			if ((result = HWGetByte(AIF,pSMFStream->fileHandle,&cc)) != EAS_SUCCESS)
				return result;
			data = (data << 7) | (cc & 0x7f);
		} while (cc & 0x80);
		len = data;
	
		if (c == 0xf0)
		{
			if ((result = ParseMIDIStream(&pSMFStream->midiStream, c)) != EAS_SUCCESS)
				return result;
		}

		while (len--)
		{
			if ((result = HWGetByte(AIF, pSMFStream->fileHandle, &c)) != EAS_SUCCESS)
				return result;

			if ((result = ParseMIDIStream(&pSMFStream->midiStream, c)) != EAS_SUCCESS)
				return result;		
		}

	}
	/* parse MIDI message */
	else
	{	
		if ((result = ParseMIDIStream(&pSMFStream->midiStream, c)) != EAS_SUCCESS)
			return result;	

		/* keep streaming data to the MIDI parser until the message is complete */
		while (pSMFStream->midiStream.pending)
		{
			if ((result = HWGetByte(AIF, pSMFStream->fileHandle, &c)) != EAS_SUCCESS)
				return result;
			
			if ((result = ParseMIDIStream(&pSMFStream->midiStream, c)) != 0)
				return result;
		}
	}   
	return EAS_SUCCESS;
}

 static int ParseMIDIStream(S_MIDI_STREAM *pMIDIStream, __u8 c)
 {
	 if (c & 0x80)
	 {
		 if (c < 0xf8)
		 {
			 pMIDIStream->runningStatus = c;
			 pMIDIStream->byte3 = EAS_FALSE;

			 if ((c == 0xf7) || (c == 0xf0))
			 {
				return EAS_SUCCESS;
			 }

			 if ((c < 0xf4) || (c > 0xf6))
				 pMIDIStream->pending = EAS_TRUE;
		 }
		else
		{
			pMIDIStream->pending = EAS_FALSE;//modify 
		}

		 return EAS_SUCCESS;
	 }

	 if (pMIDIStream->byte3)
	 {
		 pMIDIStream->d2 = c;
		 pMIDIStream->byte3 = EAS_FALSE;
		 pMIDIStream->pending = EAS_FALSE;

		 return EAS_SUCCESS;
	 }

	 if (pMIDIStream->runningStatus)
	 {

		 pMIDIStream->status = pMIDIStream->runningStatus;

		 if (pMIDIStream->status < 0xc0)
		 {
			 pMIDIStream->d1 = c;
			 pMIDIStream->pending = EAS_TRUE;
			 pMIDIStream->byte3 = EAS_TRUE;

			 return EAS_SUCCESS;
		 }

		 if (pMIDIStream->status < 0xe0)
		 {
			 pMIDIStream->d1 = c;
			 pMIDIStream->pending = EAS_FALSE;

			 return EAS_SUCCESS;
		 }

		 if (pMIDIStream->status < 0xf0)
		 {
			 pMIDIStream->d1 = c;
			 pMIDIStream->pending = EAS_TRUE;
			 pMIDIStream->byte3 = EAS_TRUE;

			 return EAS_SUCCESS;
		 }

		 if (pMIDIStream->status == 0xF0)
		 {
				 return EAS_SUCCESS;
		 }

		 pMIDIStream->runningStatus = 0;

		 if (pMIDIStream->status == 0xf2)
		 {
			 pMIDIStream->byte3 = EAS_TRUE;

			 return EAS_SUCCESS;
		 }
	 }

	 pMIDIStream->pending = EAS_FALSE;
	 return EAS_SUCCESS;
 }

 static int HWReadFile(__audio_file_info_t *AIF, EAS_FILE_HANDLE file, void *pBuffer, __s32 n, __s32 *pBytesRead)
 {
	 int result;
	 __s32 temp;
	 __u8 *p = pBuffer;
	 __s32 bytesLeft = n;
    BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	 *pBytesRead = 0;
	  
	 for (;bytesLeft > 0;)
	 {
		 temp = file->bytesInBuffer - file->readIndex;
		 if (temp > bytesLeft)
			 temp = bytesLeft;

		 /* copy data from buffer */
		 memcpy(p, &file->buffer[file->readIndex],(size_t) temp);
		 *pBytesRead += temp;
		 file->readIndex += temp;
		 file->filePos += temp;
		 bytesLeft -= temp;
		 p += temp;

		 /* don't refill buffer if request is bigger than buffer */
		 if ((bytesLeft == 0) || (bytesLeft >= EAS_FILE_BUFFER_SIZE))
			 break;

		 /* refill buffer */
		 if ((result = HWFillBuffer(AIF, file)) != EAS_SUCCESS)
			 return result;	 		
	 }

	 /* more to read? do unbuffered read directly to target memory */
	 if (bytesLeft)
	 {     
			/* position the file pointer */
			if (fseek(BSINFO->ReadFile, file->filePos, SEEK_SET) != 0)
				return EAS_FAILURE;

			/* read data in the buffer */
			/*lint -e{826} lint doesn't like this with STATIC_MEMORY defined for some reason */
			temp = (__s32) fread(p, 1, (size_t) bytesLeft, BSINFO->ReadFile);

		
		 *pBytesRead += temp;
		 file->filePos += temp;

		 /* reset buffer info */
		 file->bytesInBuffer = 0;
		 file->readIndex = 0;
	 }

	 /* were n bytes read? */
	 if (*pBytesRead != n)
		 return EAS_EOF;

	 return EAS_SUCCESS;
 }

 static int HWFillBuffer(__audio_file_info_t *AIF, EAS_FILE_HANDLE file)
 {

	 BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;

	 /* reposition the file pointer */
	 if (fseek(BSINFO->ReadFile, file->filePos, SEEK_SET) != 0)
		 return EAS_FAILURE;

	 file->bytesInBuffer = (__s32) fread(file->buffer, 1, EAS_FILE_BUFFER_SIZE, BSINFO->ReadFile);

	 file->readIndex = 0;
	 if (file->bytesInBuffer == 0)
		 return EAS_EOF;

	 return EAS_SUCCESS;
 }

 static int HWFileSeek(__audio_file_info_t *AIF, EAS_FILE_HANDLE file, __s32 position)
 {
	  __s32 newIndex;

	 /* make sure we have a valid handle */
	 if (file->buffer == NULL)
		 return EAS_FAILURE;

	 /* is new position in current buffer? */
	 newIndex = position - file->filePos + file->readIndex;
	 if ((newIndex >= 0) && (newIndex < file->bytesInBuffer))
	 {
		 file->readIndex = newIndex;
		 file->filePos = position;
		 return EAS_SUCCESS;
	 }

	 file->filePos = position;
	 file->bytesInBuffer = 0;
	 file->readIndex = 0;
	 return EAS_SUCCESS;
 }

 static int HWGetByte(__audio_file_info_t *AIF, EAS_FILE_HANDLE file, void *p)
 {
	 int result;

	 if (file->readIndex >= file->bytesInBuffer)
	 {
		 if ((result = HWFillBuffer(AIF, file)) != EAS_SUCCESS)
			 return result;

		 /* if nothing to read, return EOF */
		 if (file->bytesInBuffer == 0)
			 return EAS_EOF;
	 }

	 *((__u8*) p) = file->buffer[file->readIndex++];

	 file->filePos++;
	 return EAS_SUCCESS;
 }
