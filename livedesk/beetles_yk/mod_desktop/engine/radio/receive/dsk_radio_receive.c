/*
**************************************************************************************************************
*											        ePDK
*						            the Easy Portable/Player Develop Kits
*									           LARK app sample
*
*						        	(c) Copyright 2006-2007, TERRY, China
*											 All Rights Reserved
*
* File    : dsk_radio.c
* By      : terry
* Version : V1.00
* time    : 2009-12-01
**************************************************************************************************************
*/

#include "..\\dsk_radio_i.h"
#include "list_file.h"

__pCBK_t volatile  cb_search_success  = NULL;
void   * volatile  cb_ctx_search_success  = NULL;

typedef struct
{
	//ES_FILE 			*fm_drv;
	__u8 				thread_id;
	__u32				cur_freq;
	__u32         		rec_search_mode;
	__u32				search_flag;/*1,searching,2,search over*/	
	__u32 				start_freq; 
	__play_list_type_e	play_list_type;	
	dsk_radio_ch_list_t search_result;
	dsk_radio_ch_list_t favorite;
}dsk_radio_rcv_t;

static dsk_radio_rcv_t *dsk_radio_rcv = NULL;

void __autosearch_thread(void *p_arg)
{
	dsk_radio_rcv_t *dsk_radio_rcv;
	
	__s32 result;
	
	__s32 channel_no=0;
	
	dsk_radio_rcv = (dsk_radio_rcv_t *)p_arg;
	
	
	
	while(1)
	{
		if(esKRNL_TDelReq(EXEC_prioself) == OS_TASK_DEL_REQ)
        {     
            esKRNL_TDel(EXEC_prioself);

            break;
        }
        if(dsk_radio_rcv->search_flag == 0x01)
        {
        	dsk_radio_rcv->cur_freq = dsk_radio_rcv->start_freq;
        	result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, DRV_FM_AUTO_SEARCH|DRV_FM_SEARCH_UP, (void *)(dsk_radio_rcv->start_freq+LARK_SEARCH_STEP_US));
	        if(result == -1)
	        {
	        	dsk_radio_rcv->search_flag = 0x02;
	        }
	        else
	        {
	        	
	            dsk_radio_rcv->search_result.freq_list[channel_no].freq = result;
	            dsk_radio_rcv->cur_freq = result;
	            channel_no++;
	            dsk_radio_rcv->search_result.valid_num++;
	            
	            dsk_radio_rcv->start_freq = result;
				if(cb_search_success != NULL)
	            {
	            	esKRNL_CallBack(cb_search_success, cb_ctx_search_success);
				}
	            eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_MUTE, DRV_FM_VOICE_ON, 0);	           
	            if(dsk_radio_rcv->search_result.valid_num >= MAX_CHANNEL_NUM )
	            {
	            	dsk_radio_rcv->search_flag = 0x02;
	            }
	        }
        }
        if(dsk_radio_rcv->search_flag == 0x02)
        {
        	if(dsk_radio_rcv->search_result.valid_num>0)
		    {
		    	
		        result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)(dsk_radio_rcv->search_result.freq_list[0].freq));
		        
		        dsk_radio_rcv->cur_freq = dsk_radio_rcv->search_result.freq_list[0].freq;
		        channel_no = 0;
		    }
		    /*否则播放最小频率*/
		    else
		    {
		    	
		        result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)dsk_radio_rcv->start_freq);
		        
		        dsk_radio_rcv->cur_freq = dsk_radio_rcv->start_freq;
		    }
		    //write_channel_list_to_file("d:\\apps\\fm.ini",&dsk_radio_rcv->search_result);
		    dsk_radio_rcv->search_flag = 0x00;
        }
        esKRNL_TimeDly(4);
	}
}

__s32 dsk_radio_rcv_open(void)
{	
	if(h_radio == NULL)
	{
		__msg("radio not open!\n");
		return EPDK_FAIL;
	}
	
	if(dsk_radio_rcv)
	{		
		return EPDK_OK;
	}

	dsk_radio_send_close();
		
	dsk_radio_rcv = (dsk_radio_rcv_t *)esMEMS_Malloc(0,sizeof(dsk_radio_rcv_t));
	if(dsk_radio_rcv == NULL)
	{
		__msg("dsk_radio_rcv malloc error!\n");
		return EPDK_FAIL;
	}
	eLIBs_memset(dsk_radio_rcv, 0, sizeof(dsk_radio_rcv_t));
	//h_radio->fm_drv = fm_drv;
	
	eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_INIT, 0, 0);
	
	dsk_radio_rcv->thread_id = esKRNL_TCreate(__autosearch_thread, (void *)dsk_radio_rcv, 0x400,KRNL_priolevel3);

	return EPDK_OK;
}

__bool dsk_radio_rcv_is_open(void)
{
	if(dsk_radio_rcv)
	{
		return EPDK_TRUE;
	}
	else
	{
		return EPDK_FALSE;
	}
}

__s32 dsk_radio_rcv_set_freq_play(__s32 freq)
{
	__s32 result = 0;
	__drv_fm_area_freq_t freq_range = h_radio->freq_range;

	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(dsk_radio_rcv->search_flag == 0x01 || dsk_radio_rcv->search_flag == 0x02)
	{
		return -1;
	}
	/*当参数大于波段对应的最大频率时,频率设为波段的最大频率*/
    if(freq < freq_range.fm_area_min_freq )
    {
        freq = freq_range.fm_area_min_freq;
    }
    /*当参数小于波段对应的最小频率时,频率设为波段的最小频率*/
    else if(freq > freq_range.fm_area_max_freq)
    {
        freq = freq_range.fm_area_max_freq;
    }
    result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)freq);
    if(result == 0)
    {
    	dsk_radio_rcv->cur_freq = freq;
    }
    
	return result;
}


__s32 dsk_radio_rcv_auto_search(void)
{
	__s32 result;
	//dsk_radio_rcv_t *dsk_radio_rcv = (dsk_radio_rcv_t *)handle;
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(dsk_radio_rcv->rec_search_mode == DRV_FM_SEARCH_UP)
	{
		result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, 
					DRV_FM_AUTO_SEARCH|dsk_radio_rcv->rec_search_mode, (void *)(dsk_radio_rcv->cur_freq+LARK_SEARCH_STEP_US));
	}
    else
	{
		result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_SEARCH, 
						DRV_FM_AUTO_SEARCH|dsk_radio_rcv->rec_search_mode, (void *)(dsk_radio_rcv->cur_freq-LARK_SEARCH_STEP_US));
	}

    if(result == -1)
    {
        __log(" Error:find a channel from fm driver failed \n ");
    }
    else
    {
    	dsk_radio_rcv->cur_freq = result;
    }
    eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_MUTE, DRV_FM_VOICE_ON, 0);
    
    return result;
}

__s32 dsk_radio_rcv_get_cur_freq(__u32 *freq)
{
	__u32 *inner_freq;
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	inner_freq = freq;
	*inner_freq = dsk_radio_rcv->cur_freq;
	return 0;
}

__s32 dsk_radio_rcv_set_search_mode(__u32 search_mode)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(search_mode == DSK_RADIO_REC_SEARCH_UP)
    {
        dsk_radio_rcv->rec_search_mode = DRV_FM_SEARCH_UP;
    }
    else
    {
        dsk_radio_rcv->rec_search_mode = DRV_FM_SEARCH_DOWN;
    }
    
    return 0;	
}

__s32 dsk_radio_rcv_search_all(void)
{
	__u32 i;
	__u32 start_freq = h_radio->freq_range.fm_area_min_freq;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(	dsk_radio_rcv->search_flag)
		return 0;
	dsk_radio_rcv->search_result.valid_num = 0;
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		eLIBs_memset(dsk_radio_rcv->search_result.freq_list[i].name, 0, CHANNEL_NAME_LEN);
		dsk_radio_rcv->search_result.freq_list[i].freq = 0;
	}
	dsk_radio_rcv->start_freq = start_freq;
	dsk_radio_rcv->search_flag = 0x01;
	return 0;
}


__s32 dsk_radio_rcv_next_freq_play(void)
{
	
	__u32 freq;
	__s32 result;
	__s32 cur_band;
	__drv_fm_area_freq_t freq_range;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	cur_band	= h_radio->cur_band;
	freq_range 	= h_radio->freq_range;
	
	if(dsk_radio_rcv->search_flag == 0x01 || dsk_radio_rcv->search_flag == 0x02)
	{
		return -1;
	}
	/*当波段为日本波段时,频率的跳幅为50KHZ*/
    if(cur_band == DSK_RADIO_BAND_JAPAN)
    {
        freq = dsk_radio_rcv->cur_freq + LARK_SEARCH_STEP_JAPAN;         
    }
    /*当波段为其他波段时,频率的跳幅为100KHZ*/
    else 
    {
        freq = dsk_radio_rcv->cur_freq + LARK_SEARCH_STEP_US;
    }
    /*当下一频率的值大于波段对应的最大频率时,循环返回到最小频率处*/
    if(freq > freq_range.fm_area_max_freq)
    {
        freq = freq_range.fm_area_min_freq;
    }

    dsk_radio_rcv->cur_freq = freq;
    result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)freq);
	return result;
}

__s32 dsk_radio_rcv_pre_freq_play(void)
{
	
	__u32 freq;
	__s32 result;
	__s32 cur_band;
	__drv_fm_area_freq_t freq_range;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	cur_band	= h_radio->cur_band;
	freq_range 	= h_radio->freq_range;
	
	if(dsk_radio_rcv->search_flag == 0x01 || dsk_radio_rcv->search_flag == 0x02)
	{
		return -1;
	}
	/*当波段为日本波段时,频率的跳幅为50KHZ*/
    if(cur_band == DSK_RADIO_BAND_JAPAN)
    {
        freq = dsk_radio_rcv->cur_freq - LARK_SEARCH_STEP_JAPAN;         
    }
    /*当波段为其他波段时,频率的跳幅为100KHZ*/
    else 
    {
        freq = dsk_radio_rcv->cur_freq - LARK_SEARCH_STEP_US;
    }
    /*当下一频率的值大于波段对应的最大频率时,循环返回到最小频率处*/
    if(freq < freq_range.fm_area_min_freq)
    {
        freq = freq_range.fm_area_max_freq;
    }
    dsk_radio_rcv->cur_freq = freq;

    result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_PLAY, 0, (void *)freq);
	return result;
}

__s32 dsk_radio_rcv_set_play_list_type(__play_list_type_e play_list_type )
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}

	dsk_radio_rcv->play_list_type = play_list_type;	

	return 0;
}

__s32 dsk_radio_rcv_get_play_list_type(__s32 *play_list_type)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}

	*play_list_type = dsk_radio_rcv->play_list_type;
	
	return 0;
}

__s32 dsk_radio_rcv_get_search_result_vn(__s32 *valid_num)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	*valid_num = dsk_radio_rcv->search_result.valid_num;
	
	return 0;
}

__s32 dsk_radio_rcv_get_favorite_vn(__s32 *valid_num)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	*valid_num = dsk_radio_rcv->favorite.valid_num;
	
	return 0;
}

__s32 dsk_radio_rcv_get_search_result_chaninfo(__s32 id, dsk_radio_chan_t *chan_info)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	chan_info->freq = dsk_radio_rcv->search_result.freq_list[id].freq;
	eLIBs_strcpy(chan_info->name,dsk_radio_rcv->search_result.freq_list[id].name);
	
	return 0;
}

__s32 dsk_radio_rcv_get_favorite_chaninfo(__s32 id, dsk_radio_chan_t *chan_info)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	chan_info->freq = dsk_radio_rcv->favorite.freq_list[id].freq;
	eLIBs_strcpy(chan_info->name,dsk_radio_rcv->favorite.freq_list[id].name);
	
	return 0;
}

__s32 dsk_radio_rcv_get_favorite(char *file_name)
{
	__s32 result;
	__u32 i;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	dsk_radio_rcv->favorite.valid_num = 0;
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		eLIBs_memset(dsk_radio_rcv->favorite.freq_list[i].name,0,CHANNEL_NAME_LEN);
		dsk_radio_rcv->favorite.freq_list[i].freq = 0;
	}

	result = get_channel_list_from_file(file_name,&dsk_radio_rcv->favorite);
	
	__msg("valid num read from file is %d============\n",dsk_radio_rcv->favorite.valid_num);
	
	return result;
}

__s32 dsk_radio_rcv_save_favorite(char *file_name)
{
	__s32 result;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	result = write_channel_list_to_file(file_name,&dsk_radio_rcv->favorite);
	
	return result;
}

__s32 dsk_radio_rcv_get_search_result(char *file_name)
{
	__s32 result;
	__u32 i;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	dsk_radio_rcv->search_result.valid_num = 0;
	for(i=0;i<MAX_CHANNEL_NUM;i++)
	{
		eLIBs_memset(dsk_radio_rcv->search_result.freq_list[i].name,0,CHANNEL_NAME_LEN);
		dsk_radio_rcv->search_result.freq_list[i].freq = 0;
	}

	result = get_channel_list_from_file(file_name,&dsk_radio_rcv->search_result);
	
	return result;
}

__s32 dsk_radio_rcv_save_search_result(char *file_name)
{
	__s32 result;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	result = write_channel_list_to_file(file_name,&dsk_radio_rcv->search_result);
	
	return result;
}

__s32 dsk_radio_rcv_add_chan_to_favorite(__u32 id,dsk_radio_chan_t *chan_info)
{
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	
	if(id >= MAX_CHANNEL_NUM)
	{
		__msg("id exceed the size of favorite!");
		return -1;
	}
	
	dsk_radio_rcv->favorite.freq_list[id].freq = chan_info->freq;
	eLIBs_strcpy(dsk_radio_rcv->favorite.freq_list[id].name,chan_info->name);
	dsk_radio_rcv->favorite.valid_num++;
	
	return 0;
}

__s32 dsk_radio_rcv_del_chan_from_favorite(__u32 id)
{	
	__s32 i = id;
	
	if(dsk_radio_rcv == NULL)
	{
		__err("radio receive not open!\n");
		return EPDK_FAIL;
	}
	if(id < 0)
	{
		__msg("id is invalid!\n");
		return -1;
	}
	if(id >= MAX_CHANNEL_NUM)
	{
		__msg("id exceed the size of favorite!\n");
		return -1;
	}
	
	if(id < dsk_radio_rcv->favorite.valid_num - 1)
	{
		for(i = id; i < dsk_radio_rcv->favorite.valid_num - 1; i++)
		{
			dsk_radio_rcv->favorite.freq_list[i].freq = dsk_radio_rcv->favorite.freq_list[i+1].freq;
			eLIBs_strcpy(dsk_radio_rcv->favorite.freq_list[i].name, dsk_radio_rcv->favorite.freq_list[i+1].name);
		}
	}
	dsk_radio_rcv->favorite.freq_list[i].freq = 0;
	eLIBs_strcpy(dsk_radio_rcv->favorite.freq_list[i].name, 0);
	
	dsk_radio_rcv->favorite.valid_num--;
	
	//__inf("---------------------\n");
	//for(i = 0; i < dsk_radio_rcv->favorite.valid_num; i++)
	//{
	//	__inf("number %d in favorite is %d\n", i, dsk_radio_rcv->favorite.freq_list[i].freq);
	//}
	//__inf("---------------------\n");
	return 0;
}


__s32 dsk_radio_rcv_close(void)
{
	__s32 result = 0;
	
	if(dsk_radio_rcv == NULL)
	{
		__msg("radio receive not open!\n");
		return EPDK_FAIL;
	}	

	if(dsk_radio_rcv->thread_id)
	{
		while(1)
		{       
			if(esKRNL_TDelReq(dsk_radio_rcv->thread_id) == OS_TASK_NOT_EXIST)
			{
			    break;
			}
			esKRNL_TimeDly(3);        
		}
	}
	
	result = eLIBs_fioctrl(h_radio->fm_drv, DRV_FM_CMD_RECVE_EXIT, 0, 0);
	
	esMEMS_Mfree(0,dsk_radio_rcv);
	dsk_radio_rcv = NULL;
	return result;
}



