#include "ePDK.h"

#define	DE_INTERLACE_MODULE_ENABLE	0

static __u32  lcd_width=0;
static __u32  lcd_height=0;

static __u32  disp_fb_x_size=0;
static __u32  disp_fb_y_size=0;

static __u32  disp_src_x_size=0;
static __u32  disp_src_y_size=0;


static ES_FILE  *fp_disp = (ES_FILE *)0;
static ES_FILE  *fp_tvd  = (ES_FILE *)0;

#if DE_INTERLACE_MODULE_ENABLE
static ES_FILE  *fp_di = (ES_FILE *)0;
#endif

static __disp_pixel_mod_t disp_mode;
static __disp_pixel_seq_t disp_px_seq;
static __disp_pixel_fmt_t disp_px_fmt;


static __u8 *fb_ptr;//ref to frame buffer addr

static __csi_frame_t *p_display_buf;
static __u32 frame_id = 0;
static __s32 capture_on_times = -1;


/*******************************************************************************
Function name: generate_frame_id
Description:
    1。只用低8bit，因为display_info的top_index是__u8类型
Parameters:

Return:

Time: 2010/7/13
*******************************************************************************/
static __s32 generate_frame_id(__s32 index)
{
    __s32 prefix = capture_on_times%16;
    __s32 frame_id;
    frame_id = (prefix<<4 | index);
    return frame_id;
}

static __s32 CalFrmBufSize(__rectsz_t *pfrm_size, __u32 str_mode, 
                __pixel_yuvfmt_t clr_fmt, __u32 *pbuf_size)
{
    __u32   height, width;
    __u32   size0, size1, size2;
    
    if (str_mode != YUV_MOD_NON_MB_PLANAR && 
            str_mode != YUV_MOD_UV_NON_MB_COMBINED)
    {
        __err("fatal err: %d--not supported store mod!\n",str_mode);
        str_mode = YUV_MOD_NON_MB_PLANAR;
        return EPDK_FAIL;
    }
    width = pfrm_size->width ;
    height = pfrm_size->height;
    size0 = width * height;
    size1 = size2 = 0;
    
    switch (clr_fmt)
    {
        case PIXEL_YUV422:
        {
            if (str_mode == YUV_MOD_NON_MB_PLANAR)
            {
                size1 = size2 = size0/2;
            }
            else
            {
                size1 = size0;
            }
            break;
        }
        case PIXEL_YUV420:
        {
            if (str_mode == YUV_MOD_NON_MB_PLANAR)
            {
                size1 = size2 = size0/4;
            }
            else
            {
                size1 = size0/2;
            }
            break;
        }
        default:
        {
            __wrn("fatal err: not supported store mod!\n");
            break;
        }
    }
    pbuf_size[0] = size0 ;
    pbuf_size[1] = size1 ;
    pbuf_size[2] = size2 ;
    return EPDK_OK;
}

__s32 test_tvin_with_di()
{
	__u32 mod_num=0;
	__u32 i = 0;

	__u32 screen_width = 0;
	__u32 screen_height = 0;
	__u32 source_width = 0;
	__u32 source_height = 0;

	__s32 frame_rate=0;
	__s32 frame_period_ms=0;

	__u32 tvin_src_size=0;

	__tvd_mode_all_t tvd_mode_all;
	__tvd_mode_t tvd_mode;
	__tvd_frame_queue_t FrameQueue;

	__disp_layer_info_t layer_para = {0};
	__disp_video_fb_t video_fb;
	__u32 arg[3];
	__u32 sel = 0;
	ES_FILE * disphd;
	__u32	csi_src_size ;
	__hdle hlay;
	__u32 	buf_size[3] ;

	RECT rect;
#if DE_INTERLACE_MODULE_ENABLE
	DI_Fb_Addr_t DI_Fb_Addr ;
	DI_Source_Config_t DI_Source_Config;
	__u32		di_format_support ;
	__u32		IsFisrtFrm ;
#endif

 start:
    __inf("hello tvin test!\n");

	esDEV_Plugin("\\drv\\tvd.drv", 0, 0, 1);
	esKRNL_TimeDly(10);
  	fp_tvd= eLIBs_fopen("b:\\USER\\TVD", "r+");
  	__inf("open b:\\USER\\TVD fp=%x\n",fp_tvd);
		
#if DE_INTERLACE_MODULE_ENABLE
	esDEV_Plugin("\\drv\\di.drv", 0, 0, 1);
	esKRNL_TimeDly(10);
	fp_di = eLIBs_fopen("b:\\USER\\DI", "r+");
	__inf("di driver plugin success!\n");
	if(fp_di == NULL)
	{
		__err("di driver open failed\n");
		return EPDK_FAIL;
	}	
	else 
	{
		__inf("di driver open success! fp_di=%x.\n",fp_di);
    }
#endif

	//先初始化
    eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_INIT, 0, 0);

    //获取所有模式
    eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_GET_MODE_ALL, 0, &tvd_mode_all);
    for (i = 0; i<tvd_mode_all.number; i++){
        __inf("#########################\n");
        __inf("### mode %d ###\n", i);
        __inf("# size: %dx%d\n", 				tvd_mode_all.tvd_mode[i].size.width, tvd_mode_all.tvd_mode[i].size.height);
        __inf("# frame_rate: %d\n", 		tvd_mode_all.tvd_mode[i].frame_rate);
        __inf("# color_format: %d\n", 	tvd_mode_all.tvd_mode[i].color_format);
        __inf("# frame_period: %d\n", 	tvd_mode_all.tvd_mode[i].frame_period);
        __inf("# component_seq: %d\n", 	tvd_mode_all.tvd_mode[i].component_seq);
        __inf("# store_mode: %d\n", 		tvd_mode_all.tvd_mode[i].store_mode);
        __inf("# color_space: %d\n", 		tvd_mode_all.tvd_mode[i].color_space);
        __inf("#########################\n");
    }
    //设置模式
csi_mode_sel:

    __inf("please input tvd mode(0~%d): ",tvd_mode_all.number-1);

    mod_num=0;//__getc();
    mod_num=(mod_num&0x7f-0x30)&0x0f;
	__inf("%d\n",mod_num);
    mod_num=(mod_num<0 || mod_num>tvd_mode_all.number-1)?0:mod_num;
    __inf("select tvd mode [%d]\n", mod_num);
    //mod_num=2;
    eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_SET_MODE, 0, &(tvd_mode_all.tvd_mode[mod_num]));
    //取得当前模式
    eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_GET_MODE, 0, &tvd_mode);
    __inf("color_fmt=%d:%dx%d@%d\n",tvd_mode.color_format, tvd_mode.size.width, tvd_mode.size.height, tvd_mode.frame_rate);

    FrameQueue.num = 0x6; // single buffer;

    csi_src_size = tvd_mode.size.width*tvd_mode.size.height;//for YUV422(/420)_UV_CB
    __inf("csi_src_size=%dkByte\n",csi_src_size/1024);

		CalFrmBufSize(&tvd_mode.size,tvd_mode.store_mode,tvd_mode.color_format,buf_size);
    eLIBs_printf("buf_size[0]=%d buf_size[1]=%d buf_size[2]=%d \n",buf_size[0],buf_size[1],buf_size[2]);
	
    for (i=0;i<FrameQueue.num;i++)
  	{
	    //address 0
	    if(buf_size[0])
	    {
		    if((FrameQueue.tvd_frame_array[i].addr[0] = (__u32)esMEMS_Palloc(((buf_size[0]  + 1023)>>10), MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE))==NULL)
		    {
		    	__err("Failed to malloc!\n");
		    }
		    eLIBs_memset((__u8 *)FrameQueue.tvd_frame_array[i].addr[0], 0, csi_src_size);
		    eLIBs_CleanFlushDCacheRegion((void *)FrameQueue.tvd_frame_array[i].addr[0], csi_src_size);
	    }

	    //address 1
	    if(buf_size[1])
	    {
		    if((FrameQueue.tvd_frame_array[i].addr[1] = (__u32)esMEMS_Palloc(((buf_size[1]  + 1023)>>10), MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE))==NULL)
		    {
		    	__err("Failed to malloc!\n");
		    }
		    eLIBs_memset((__u8 *)FrameQueue.tvd_frame_array[i].addr[1], 0, csi_src_size);
		    eLIBs_CleanFlushDCacheRegion((void *)FrameQueue.tvd_frame_array[i].addr[1], csi_src_size);
	    }
	    //address 2
	    if(buf_size[2])
	    {
		    if((FrameQueue.tvd_frame_array[i].addr[2] = (__u32)esMEMS_Palloc(((buf_size[2]  + 1023)>>10), MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE))==NULL)
		    {
		    	__err("Failed to malloc!\n");
		    }
		    eLIBs_memset((__u8 *)FrameQueue.tvd_frame_array[i].addr[2], 0, csi_src_size);
		    eLIBs_CleanFlushDCacheRegion((void *)FrameQueue.tvd_frame_array[i].addr[2], csi_src_size);
	    }
	    FrameQueue.tvd_frame_array[i].frame_id = generate_frame_id(i);
	    __msg("request frame id %d = %x\n",i,FrameQueue.tvd_frame_array[i].frame_id );
	    FrameQueue.tvd_frame_array[i].bProgressiveSrc = 0; //bProgressiveSrc;  值由csidrv决定，这里随便赋值
	    FrameQueue.tvd_frame_array[i].bTopFieldFirst = 0;   //bTopFieldFirst;
	    FrameQueue.tvd_frame_array[i].eAspectRatio = 0;     //eAspectRatio;
	    FrameQueue.tvd_frame_array[i].color_format = tvd_mode.color_format;
	    FrameQueue.tvd_frame_array[i].src_rect.width = FrameQueue.tvd_frame_array[i].dst_rect.width = tvd_mode.size.width;
	    FrameQueue.tvd_frame_array[i].src_rect.height = FrameQueue.tvd_frame_array[i].dst_rect.height = tvd_mode.size.height;
	    FrameQueue.tvd_frame_array[i].frame_rate = tvd_mode.frame_rate;
	    FrameQueue.tvd_frame_array[i].uPts = 0;   //pts;
	    FrameQueue.tvd_frame_array[i].bPtsValid = 1;
	    FrameQueue.tvd_frame_array[i].component_seq = tvd_mode.component_seq;;
	    FrameQueue.tvd_frame_array[i].store_mode = tvd_mode.store_mode;
	    __msg("buffer%d:%x,%x,%x\n",0, FrameQueue.tvd_frame_array[i].addr[0],FrameQueue.tvd_frame_array[i].addr[1],FrameQueue.tvd_frame_array[i].addr[2]);


	}
	eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_SET_FRAME_QUEUE, 0, &FrameQueue);

    frame_period_ms=tvd_mode.frame_period / 1000;
//=====================================================
    switch (tvd_mode.color_format){
        case PIXEL_YUV422:{
        	disp_px_fmt=DISP_FORMAT_YUV422;
        	__inf("DISP_FORMAT_YUV422\n");
        	break;
        }
        case PIXEL_YUV420:{
        	disp_px_fmt=DISP_FORMAT_YUV420;
        	__inf("DISP_FORMAT_YUV420\n");
        	break;
        }
        case PIXEL_YUV444:{
        	disp_px_fmt=DISP_FORMAT_YUV444;
        	__inf("DISP_FORMAT_YUV444\n");
        	break;
        }
        case PIXEL_CSIRGB:{
        	disp_px_fmt=DISP_FORMAT_CSIRGB;
        	__inf("DISP_FORMAT_CSIRGB\n");
        	break;
        }
        case PIXEL_OTHERFMT:{
        	disp_px_fmt=DISP_FORMAT_YUV422;
        	__inf("DISP_FORMAT_(OTHERFMT)YUV422\n");
        	break;
        }
    }

//==================================================
    switch (tvd_mode.component_seq){
        case YUV_SEQ_UYVY:{
        	disp_px_seq=DISP_SEQ_UYVY;
        	__inf("DISP_SEQ_UYVY\n");
        	break;
        }
        case YUV_SEQ_YUYV:{
        	disp_px_seq=DISP_SEQ_YUYV;
        	__inf("DISP_SEQ_YUYV\n");
        	break;
        }
        case YUV_SEQ_VYUY:{
        	disp_px_seq=DISP_SEQ_VYUY;
        	__inf("DISP_SEQ_VYUY\n");
        	break;
        }
        case YUV_SEQ_YVYU:{
        	disp_px_seq=DISP_SEQ_YVYU;
        	__inf("DISP_SEQ_YVYU\n");
        	break;
        }
        case YUV_SEQ_UVUV:{
        	disp_px_seq=DISP_SEQ_UVUV;
        	__inf("DISP_SEQ_UVUV\n");
        	break;
        }
        case YUV_SEQ_VUVU:{
        	disp_px_seq=DISP_SEQ_VUVU;
        	__inf("DISP_SEQ_VUVU\n");
        	break;
        }
        case YUV_SEQ_AYUV:{
        	disp_px_seq=DISP_SEQ_AYUV;
        	__inf("DISP_SEQ_AYUV\n");
        	break;
        }
        case YUV_SEQ_VUYA:{
        	disp_px_seq=DISP_SEQ_VUYA;
        	__inf("DISP_SEQ_VUYA\n");
        	break;
        }
    }

//===========================
    switch(tvd_mode.store_mode){
        case YUV_MOD_INTERLEAVED:{
        	disp_mode=DISP_MOD_INTERLEAVED;
        	__inf("DISP_MOD_INTERLEAVED\n");
        	break;
        }
        case YUV_MOD_UV_NON_MB_COMBINED:{
        	disp_mode=DISP_MOD_NON_MB_UV_COMBINED;
        	__inf("DISP_MOD_NON_MB_UV_COMBINED\n");
        	break;
        }
        case YUV_MOD_NON_MB_PLANAR:{
        	disp_mode=DISP_MOD_NON_MB_PLANAR;
        	__inf("DISP_MOD_NON_MB_PLANAR\n");
        	break;
        }
    }


    disp_fb_x_size=tvd_mode.size.width;
    disp_fb_y_size=tvd_mode.size.height;

    disp_src_x_size=tvd_mode.size.width;
    disp_src_y_size=tvd_mode.size.height;

    {
		__u32 lcd_sel=0;
		__inf("######################################\n");
		__inf("#please select LCD size(0/1/2...):\n");
		__inf("#0: lcd size=800*480\n");
		__inf("#1: lcd size=400*280\n");
		__inf("#2: lcd size=320*240\n");
		__inf("######################################\n");
		lcd_sel=0;//__getc();
		lcd_sel=(lcd_sel&0x7f-0x30)&0x0f;
		__inf("%d\n",lcd_sel);

		switch(lcd_sel)
		{
			case 0:{
			lcd_width=800;
			lcd_height=480;
			__inf("select %d, lcd size=800*480\n",lcd_sel);
			break;
			}
			case 1:{
			lcd_width=400;
			lcd_height=280;
			__inf("select %d, lcd size=400*280\n",lcd_sel);
			break;
			}
			case 2:{
			lcd_width=400;
			lcd_height=280;
			__inf("select %d, lcd size=320*240\n",lcd_sel);
			break;
			}
			default:{
			lcd_width=800;
			lcd_height=480;
			__inf("default select %d, lcd size=800*480\n",0);
			break;
			}
		}
    }

//=====================================
	//	__inf("wait for 1s...\n");
    //esKRNL_TimeDly(100);


    disphd = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(!disphd)
    {
        __wrn("open display device fail!\n");
        return EPDK_FAIL;
    }

	layer_para.fb.addr[0]       = FrameQueue.tvd_frame_array[0].addr[0];
    layer_para.fb.addr[1]       = FrameQueue.tvd_frame_array[0].addr[1];
    layer_para.fb.addr[2]       = FrameQueue.tvd_frame_array[0].addr[2];
    layer_para.fb.size.width    = disp_fb_x_size;//csi_mode.size.width;
    layer_para.fb.size.height   = disp_fb_y_size;//csi_mode.size.height;
    layer_para.fb.mode          = disp_mode;//DISP_MOD_NON_MB_UV_COMBINED;
    layer_para.fb.format        = disp_px_fmt;//DISP_FORMAT_YUV422;
    layer_para.fb.br_swap       = 0;
    layer_para.fb.seq           = disp_px_seq;//DISP_SEQ_UVUV;
    layer_para.ck_enable        = 0;
    layer_para.alpha_en         = 1;
    layer_para.alpha_val        = 0xff;
    layer_para.pipe             = 0;
    layer_para.src_win.x        = 0;
    layer_para.src_win.y        = 0;
    layer_para.src_win.width    = disp_src_x_size;//csi_mode.size.width;
    layer_para.src_win.height   = disp_src_y_size;//csi_mode.size.height;
    layer_para.scn_win.x        = 0;
    layer_para.scn_win.y        = 0;
    layer_para.scn_win.width    = lcd_width;
    layer_para.scn_win.height   = lcd_height;

    //layer0
    arg[0] = DISP_LAYER_WORK_MODE_SCALER;
    arg[1] = 0;
    arg[2] = 0;
    hlay = eLIBs_fioctrl(disphd, DISP_CMD_LAYER_REQUEST, sel, (void*)arg);
    if(hlay == NULL)
    {
        __wrn("request layer0 fail\n");
    }
    else
    {
        layer_para.mode = DISP_LAYER_WORK_MODE_SCALER;
        layer_para.pipe = 0;
        arg[0] = hlay;
        arg[1] = (__u32)&layer_para;
        arg[2] = 0;
        eLIBs_fioctrl(disphd,DISP_CMD_LAYER_SET_PARA,sel,(void*)arg);

        arg[0] = hlay;
        arg[1] = 0;
        arg[2] = 0;
        eLIBs_fioctrl(disphd,DISP_CMD_LAYER_OPEN,sel,(void*)arg);
        eLIBs_fioctrl(disphd,DISP_CMD_LAYER_TOP,sel,(void*)arg);
    }

#if DE_INTERLACE_MODULE_ENABLE

	di_format_support = EPDK_TRUE ;
	if(tvd_mode.store_mode == YUV_MOD_UV_NON_MB_COMBINED)
	{

		DI_Source_Config.type.mod = DI_NON_MB_UVCOMBINE;
	}
	else if(tvd_mode.store_mode == YUV_MOD_UV_MB_COMBINED)
	{

		DI_Source_Config.type.mod = DI_MB_UVCOMBINE;
	}
	else
	{

		__wrn("tvd_mode.store_mode is %d ,it is not support by di",tvd_mode.store_mode);
		di_format_support = EPDK_FALSE ;
	}	
	
	if(di_format_support == EPDK_TRUE)
	{
		DI_Source_Config.src_width = tvd_mode.size.width;
		DI_Source_Config.src_height = tvd_mode.size.height;
		DI_Source_Config.type.ps = 0 ;

		eLIBs_fioctrl(fp_di,DI_CMD_CONFIG_SRC,0,(void*)(&DI_Source_Config));

		DI_Fb_Addr.cur_frame_buf.ch0_addr = (__u32)esMEMS_Palloc((buf_size[0]+1023)>>10,0);
		DI_Fb_Addr.cur_frame_buf.ch1_addr = (__u32)esMEMS_Palloc((buf_size[1]+1023)>>10,0);
		DI_Fb_Addr.out_frame_buf.ch0_addr = (__u32)esMEMS_Palloc((buf_size[0]+1023)>>10,0);
		DI_Fb_Addr.out_frame_buf.ch1_addr = (__u32)esMEMS_Palloc((buf_size[1]+1023)>>10,0);
		eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.cur_frame_buf.ch0_addr,buf_size[0]);
		eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.cur_frame_buf.ch1_addr,buf_size[1]);
		eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.out_frame_buf.ch0_addr,buf_size[0]);
		eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.out_frame_buf.ch1_addr,buf_size[1]);

		IsFisrtFrm = EPDK_FALSE ;
	}
#endif

	__msg("start to requst csi frame\n");

	esKRNL_TimeDly(1);

    //务必先开始抓取数据
    eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_CAPTURE_ON, 0, 0);	    

	while(1)
    {
		__u32 frame_req;
		__u32 time_st ;
		static __u32 request_frame_fail_time = 0 ;
		//__inf("io set addr %x\n",p_display_buf);
		frame_req=eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_REQUEST_FRAME, 0, &p_display_buf);

		// __inf("req return %x\n",frame_req);

		if(frame_req==EPDK_OK) {

			#if DE_INTERLACE_MODULE_ENABLE
	        //__inf("req addr %x %x %x\n",p_display_buf->addr[0],p_display_buf->addr[1],p_display_buf->addr[2]);
			if(di_format_support == EPDK_TRUE)
			{

				if( IsFisrtFrm == EPDK_FALSE )
				{

					DI_Fb_Addr.pre_frame_buf.ch0_addr = p_display_buf->addr[0];
					DI_Fb_Addr.pre_frame_buf.ch1_addr = p_display_buf->addr[1];
					IsFisrtFrm = EPDK_TRUE ;
					continue ;
				}
				DI_Fb_Addr.cur_frame_buf.ch0_addr = p_display_buf->addr[0];
				DI_Fb_Addr.cur_frame_buf.ch1_addr = p_display_buf->addr[1];

				eLIBs_fioctrl(fp_di,DI_CMD_SET_FB_ADDR,0,(void*)(&DI_Fb_Addr));
				eLIBs_fioctrl(fp_di,DI_CMD_START,0,0);

				layer_para.fb.addr[0]       = DI_Fb_Addr.out_frame_buf.ch0_addr;
				layer_para.fb.addr[1]       = DI_Fb_Addr.out_frame_buf.ch1_addr;
			}
			else
			#endif
			{
				layer_para.fb.addr[0]       = p_display_buf->addr[0];
				layer_para.fb.addr[1]       = p_display_buf->addr[1];
				layer_para.fb.addr[2]       = p_display_buf->addr[2];
			}
	        sel = 0;
	        arg[0] = hlay;
	        arg[1] = (__u32)&layer_para;
	        arg[2] = 0;
			time_st = esKRNL_TimeGet();
	        eLIBs_fioctrl(disphd,DISP_CMD_LAYER_SET_PARA,sel,(void*)arg);
			time_st = esKRNL_TimeGet() - time_st;
	     	//__inf("time_st:%x\n",time_st);


	        frame_req = eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_RELEASE_FRAME, p_display_buf->frame_id, NULL);

		}
		else
		{
			esKRNL_TimeDly(1);
		//	__inf("request_frame_fail_time:%x\n",request_frame_fail_time++);
		}

	  }

exit_csi_test:
//退出时的操作
		arg[0] = 0;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(disphd, DISP_CMD_LCD_OFF, sel, (void*)arg);

    arg[0] = hlay;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(disphd, DISP_CMD_VIDEO_STOP, sel, (void*)arg);

    arg[0] = hlay;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(disphd, DISP_CMD_LAYER_RELEASE, sel, (void*)arg);

    eLIBs_fclose(disphd);
		//esDEV_Plugout("\\drv\\display.drv", 0);

    eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_CAPTURE_OFF, 0, 0);
    eLIBs_fioctrl(fp_tvd, DRV_CSI_CMD_EXIT, 0, 0);//应用程序退出必须调用此步，否则内存没有释放
	eLIBs_fclose(fp_tvd);
	esDEV_Plugout("\\drv\\tvd.drv", 0);
	__inf("tvd byebye!\n");

	goto start;

	return EPDK_OK;

}


