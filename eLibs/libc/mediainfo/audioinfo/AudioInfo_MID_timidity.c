/***************************************
*	Windows Medio audio infor 
*	
*
*	date:  2009-11-24 15:14:19  author: lszhang
*	copyright 2100 AllWinner.LTD
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"

static const char midcodecInfo[1][3]=
{
	"MID"	
};
	
//static int midi_port_number = 0;
//static int divisions = 0;
typedef struct __mid_struct
{
    int smf_at_time_max;
    int smf_at_time_min;
    int sample_correction;
    int sample_increment;
    int tempotime[512][2] ;
    int tempotime_num;    
}mid_struct;
//static int smf_at_time_max = 0;
//static int smf_at_time_min = 0x7fffffff;
//static int sample_correction = 0;
//static int sample_increment = 0;
//static int tempotime[512][2] ;
//static int tempotime_num = 0;
#define MERGE_CHANNEL_PORT(ch) ((int)(ch) | (midi_port_number << 4))
static int getvl(__audio_file_info_t *AIF);
static void compute_sample_increment(int tempo, int divisions,mid_struct* mid_data);
static int read_smf_track(__audio_file_info_t *AIF, int trackno, int rewindp,mid_struct*mid_data);
int GetInfo_MID(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{	
	int Ret = 0;
	__int64	filelength = 0;
	char magic[4];
	int len;
	int i,k;
	int dt,samples_to_do= 0;
	__int64	sample_cum=0;
	int divisions = 0;
	mid_struct* mid_data = NULL;
	
	short format,tracks,divisions_tmp;

	//memset(tempotime,0,512*2*sizeof(int));
	//tempotime_num = 0;
	//smf_at_time_max = 0;
	//smf_at_time_min = 0x7fffffff;
	
	mid_data = (mid_struct*)malloc(sizeof(mid_struct));
	if(!mid_data)
	        return ERROR;
	filelength = GetInfo_BsInit(bs,AIF);
	if(filelength <=0) 
	{
	    free(mid_data);
	    mid_data = NULL;
	    return ERROR;
	}
	    
	
	memset(mid_data,0,sizeof(mid_struct));
	mid_data->smf_at_time_min = 0x7fffffff;
	    
	magic[0] = GetInfo_GetBsInByte(1,AIF);
	magic[1] = GetInfo_GetBsInByte(1,AIF);
	magic[2] = GetInfo_GetBsInByte(1,AIF);
	magic[3] = GetInfo_GetBsInByte(1,AIF);
	if(memcmp(magic, "MThd", 4) == 0)//read smf file
    {
		len = GetInfo_GetBsInByte(4,AIF);
		format = GetInfo_GetBsInByte(2,AIF);
		tracks = GetInfo_GetBsInByte(2,AIF);
		divisions_tmp = GetInfo_GetBsInByte(2,AIF);
		 if(divisions_tmp < 0)
    {
	/* SMPTE time -- totally untested. Got a MIDI file that uses this? */
	divisions=
	    (int)(-(divisions_tmp / 256)) * (int)(divisions_tmp & 0xFF);
    }
    else
	divisions = (int)divisions_tmp;
    mid_data->tempotime[0][0] = 500000;
	mid_data->tempotime[0][1] = 0;
	mid_data->tempotime_num++;
    compute_sample_increment(500000, divisions,mid_data);
		if(len > 6)
		{
			GetInfo_SkipBsInByte(len - 6,AIF);
		}
		if(format < 0 || format > 2)
		{
			goto midExit;//Unknown MIDI file format
		}
		switch(format)
		{
		case 0:
    		if(read_smf_track(AIF, 0, 1,mid_data)<0)
    		{
    			
    			goto midExit;//return 1;
    		}
    		break;

		case 1:
    		for(i = 0; i < tracks; i++)
    		{
    			if(read_smf_track(AIF, i, 1,mid_data)<0)
    			{
    			
    			goto midExit;//return 1;
    			}
    		}
    		break;

		case 2: /* We simply play the tracks sequentially */
    		for(i = 0; i < tracks; i++)
    		{
    			if(read_smf_track(AIF, i, 0,mid_data)<0)
    			{
    				goto midExit;//return 1;
    			}
    		}
    		break;
		}
		
	Ret = SUCCESS;
    }
    else
    {
    	goto midExit;	
    }
	for(i=0;i<mid_data->tempotime_num;i++)
	{
		for(k=1;k<mid_data->tempotime_num-i;k++)
		{
			if(mid_data->tempotime[k-1][1]>mid_data->tempotime[k][1])
			{
				dt = mid_data->tempotime[k-1][0];
				mid_data->tempotime[k-1][0] = mid_data->tempotime[k][0];
				mid_data->tempotime[k][0] = dt;
				dt = mid_data->tempotime[k-1][1];
				mid_data->tempotime[k-1][1] = mid_data->tempotime[k][1];
				mid_data->tempotime[k][1] = dt;

			}
		}
	}
    for(i=0;i< mid_data->tempotime_num;i++)
	{
		
		if(mid_data->tempotime[i][1]<=mid_data->smf_at_time_min)
		{
			compute_sample_increment(mid_data->tempotime[i][0], divisions,mid_data);
			mid_data->tempotime[i][1] = mid_data->smf_at_time_min;
			continue;
		}
		dt = mid_data->tempotime[i][1] - mid_data->tempotime[i-1][1];
		samples_to_do += mid_data->sample_increment * dt;
	    sample_cum += ((__int64)(mid_data->sample_correction) * dt);
		samples_to_do += sample_cum>>16;
		sample_cum &= 0xffff;
		compute_sample_increment(mid_data->tempotime[i][0], divisions,mid_data);
	}
		dt = mid_data->smf_at_time_max - mid_data->tempotime[mid_data->tempotime_num-1][1];
		samples_to_do += mid_data->sample_increment * dt;
	    sample_cum += ((__int64)(mid_data->sample_correction) * dt);
		samples_to_do += sample_cum>>16;
		sample_cum &= 0xffff;
 
	AIF->ulChannels = 2;
	AIF->ulBitsSample = 16;
	AIF->ulSampleRate = 44100;
	AIF->ulDuration = (__int64)samples_to_do*1000/AIF->ulSampleRate + 2000;
	AIF->ulBitRate = (__int64)filelength*8*1000/AIF->ulDuration; 
	if(AIF->InforBufLeftLength>=3)
	{
		AIF->ulAudio_name_sz = 3;
		AIF->ulAudio_name = AIF->InforBuf;
		AIF->InforBuf +=AIF->ulAudio_name_sz;
		AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;	
		memcpy(AIF->ulAudio_name,midcodecInfo[0],AIF->ulAudio_name_sz);
		
	}
midExit:
	
	if(mid_data)
	{
	    free(mid_data);
	    mid_data = NULL;
	}
	GetInfo_BsEndRelease(AIF);
	return Ret;
	
}
static int read_smf_track(__audio_file_info_t *AIF, int trackno, int rewindp,mid_struct* mid_data)
{
	char tmp[4];
	int len;
	int i,me,type;
	int a,b,c;
	int smf_at_time = 0;
	int tempo = 0;
	int next_pos,pos;
	int lastchan, laststatus;
	int midi_port_number = 0;
	tmp[0]  = GetInfo_GetBsInByte(1,AIF);
	tmp[1]  = GetInfo_GetBsInByte(1,AIF);
	tmp[2]  = GetInfo_GetBsInByte(1,AIF);
	tmp[3]  = GetInfo_GetBsInByte(1,AIF);
	len = GetInfo_GetBsInByte(4,AIF);
	next_pos = GetInfo_GetFileLocation(AIF) + len;
	if(strncmp(tmp, "MTrk", 4))
	{
		return -2;
	}
	lastchan = laststatus = 0;
	for(;;)
	{
		len = getvl(AIF);
		if(len<0)return -1;
		smf_at_time += len;
		i = GetInfo_GetBsInByte(1,AIF);
		me = (unsigned char)i;
		if(me == 0xF0 || me == 0xF7) /* SysEx event */
		{
			len = getvl(AIF);
			if(len<0)return -1;
			GetInfo_SkipBsInByte(len ,AIF);
			
		}
		else if(me == 0xFF) /* Meta event */
		{
		   
			type = GetInfo_GetBsInByte(1,AIF);
			len = getvl(AIF);
			if(len<0)return -1;
			
			switch(type)
			{
				case 0x2F:
					pos = GetInfo_GetFileLocation(AIF);

					if(pos < next_pos)
						GetInfo_SkipBsInByte(next_pos - pos ,AIF);
					return 0;
				case 0x51: /* Tempo */
			 
					a = GetInfo_GetBsInByte(1,AIF);
					b = GetInfo_GetBsInByte(1,AIF);
					c = GetInfo_GetBsInByte(1,AIF);
					tempo = c + b * 256 + a * 65536;
					mid_data->tempotime[mid_data->tempotime_num][0] = tempo;
					mid_data->tempotime[mid_data->tempotime_num][1] = smf_at_time;
					mid_data->tempotime_num++;
					break;
				case 0x21: /* MIDI port number */
        		    if(len == 1)
        		    {
        			midi_port_number = GetInfo_GetBsInByte(1,AIF);
        			midi_port_number &= 0xF;
        		    }
        		    else
        			GetInfo_SkipBsInByte(len ,AIF);
					break;
				default:
					GetInfo_SkipBsInByte(len ,AIF);
			}
		}
		else
		{
		    a = me;
    	    if(a & 0x80) /* status byte */
    	    {
    		lastchan = MERGE_CHANNEL_PORT(a & 0x0F);
    		laststatus = (a >> 4) & 0x07;
    		if(laststatus != 7)
    		    a = GetInfo_GetBsInByte(1,AIF) & 0x7F;

    	    }
    	    switch(laststatus)
    	    {
    	        case 0: /* Note off */
    	            GetInfo_GetBsInByte(1,AIF);
					if(mid_data->smf_at_time_max < smf_at_time)
						mid_data->smf_at_time_max = smf_at_time;
    	            break;
        	    case 1: /* Note on */
        
            		b = GetInfo_GetBsInByte(1,AIF) & 0x7F;
            
            		if(b)
            		{
            		    if(smf_at_time<mid_data->smf_at_time_min)
            		        mid_data->smf_at_time_min =smf_at_time;
            		}
            		else /* b == 0 means Note Off */
            		{
            		    ;
            		}
					if(mid_data->smf_at_time_max < smf_at_time)
							mid_data->smf_at_time_max = smf_at_time;
            		break;
            	case 2: /* Key Pressure */
            	    b = GetInfo_GetBsInByte(1,AIF) & 0x7F;
					if(mid_data->smf_at_time_max < smf_at_time)
							mid_data->smf_at_time_max = smf_at_time;
            	    break;
            
            	case 3: /* Control change */
            		 
            	    b = GetInfo_GetBsInByte(1,AIF);
					if(mid_data->smf_at_time_max < smf_at_time)
							mid_data->smf_at_time_max = smf_at_time;
            	    break;
            
            	case 4: /* Program change */
					if(mid_data->smf_at_time_max < smf_at_time)
							mid_data->smf_at_time_max = smf_at_time;
            	    break;
            
            	case 5: /* Channel pressure */
					if(mid_data->smf_at_time_max < smf_at_time)
							mid_data->smf_at_time_max = smf_at_time;
            	    break;
            
            	case 6: /* Pitch wheel */
            	    b = GetInfo_GetBsInByte(1,AIF) & 0x7F;
					if(mid_data->smf_at_time_max < smf_at_time)
						mid_data->	smf_at_time_max = smf_at_time;
            	    break;
            
            	default: /* case 7: */
            	/* Ignore this event */
            	switch(lastchan & 0xF)
            	{
            	  case 2: /* Sys Com Song Position Pntr */
                	   GetInfo_GetBsInByte(2,AIF);
                	    break;
            
            	  case 3: /* Sys Com Song Select(Song #) */
            	    GetInfo_GetBsInByte(1,AIF);
            	    break;
            
            	  case 6: /* Sys Com tune request */
            	    break;
            	  case 8: /* Sys real time timing clock */
            	    break;
            	  case 10: /* Sys real time start */
            	    break;
            	  case 11: /* Sys real time continue */
            	    break;
            	  case 12: /* Sys real time stop */
            	    break;
            	  case 14: /* Sys real time active sensing */
            	    break;
            	  default: /* 1, 4, 5, 9, 13 */
            	    break;
            	}
    	    }
		}
		    
	}

}
static void compute_sample_increment(int tempo, int divisions,mid_struct* mid_data)
{
  double a;
  a = (double) (tempo) * (double) (44100) * (65536.0/1000000.0) /
    (double)(divisions);

  mid_data->sample_correction = (int)(a) & 0xFFFF;
  mid_data->sample_increment = (int)(a) >> 16;
}
static int getvl(__audio_file_info_t *AIF)
{
    int l;
    int c;

    
    l = 0;

    /* 1 */

	c = GetInfo_GetBsInByte(1,AIF);

    if(!(c & 0x80)) return l | c;
    l = (l | (c & 0x7f)) << 7;

    /* 2 */
	c =GetInfo_GetBsInByte(1,AIF);
    if(!(c & 0x80)) return l | c;
    l = (l | (c & 0x7f)) << 7;

    /* 3 */
	c = GetInfo_GetBsInByte(1,AIF);
    if(!(c & 0x80)) return l | c;
    l = (l | (c & 0x7f)) << 7;

    /* 4 */

	c = GetInfo_GetBsInByte(1,AIF);

    if(!(c & 0x80)) return l | c;


    return -2;

}

