/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                            csi drv test Module
*
*                                    (c) Copyright 2011-2015, cele.ma China
*                                             All Rights Reserved
*
* File    : test_csi.c
* By      : kingvan
* Version : v1.0
* Date    :2012-9-20 11:05:24  	
*********************************************************************************************************
*/
#include "ePDK.h"


//#include "main.h"
//#include "uart_in_data.h"

//#define DEBUG
//#define USER_CONFIG

#define put_wvalue(n,c) (*((volatile __u32 *)(n))  = (c))   /* word output */


#define DISPLAY_MODE_LCD 0
#define DISPLAY_MODE_TV  1
#define DISPLAY_MODE_VGA 2
#define DISPLAY_MODE DISPLAY_MODE_LCD//


__u32  lcd_width=0;
__u32  lcd_height=0;

__u32  disp_fb_x_size=0;
__u32  disp_fb_y_size=0;

__u32  disp_src_x_size=0;
__u32  disp_src_y_size=0;

__u32  TIME_OUT_EN=1;//0:disable

__u32 scaler_base=0x01e00000;

ES_FILE  *fp_disp = (ES_FILE *)0;
ES_FILE  *fp_csi  = (ES_FILE *)0;
ES_FILE  *fp_raw_bin = (ES_FILE *)0;//save frame data to file


__disp_pixel_mod_t disp_mode;
//    DISP_MOD_INTERLEAVED        =0x1,   //interleaved,1个地址
//    DISP_MOD_NON_MB_PLANAR      =0x0,   //无宏块平面模式,3个地址,RGB/YUV每个channel分别存放
//    DISP_MOD_NON_MB_UV_COMBINED =0x2,   //无宏块UV打包模式,2个地址,Y和UV分别存放
//    DISP_MOD_MB_PLANAR          =0x4,   //宏块平面模式,3个地址,RGB/YUV每个channel分别存放
//    DISP_MOD_MB_UV_COMBINED     =0x6,   //宏块UV打包模式 ,2个地址,Y和UV分别存放

__disp_pixel_seq_t disp_px_seq;
////for interleave argb8888
//    DISP_SEQ_ARGB   =0x0,//A在高位
//    DISP_SEQ_BGRA   =0x2,
//
////for nterleaved yuv422
//    DISP_SEQ_UYVY   =0x3,
//    DISP_SEQ_YUYV   =0x4,
//    DISP_SEQ_VYUY   =0x5,
//    DISP_SEQ_YVYU   =0x6,
//
////for interleaved yuv444
//    DISP_SEQ_AYUV   =0x7,
//    DISP_SEQ_VUYA   =0x8,
//
////for uv_combined yuv420
//    DISP_SEQ_UVUV   =0x9,
//    DISP_SEQ_VUVU   =0xa,

__disp_pixel_fmt_t disp_px_fmt;
//    DISP_FORMAT_1BPP        =0x0,
//    DISP_FORMAT_2BPP        =0x1,
//    DISP_FORMAT_4BPP        =0x2,
//    DISP_FORMAT_8BPP        =0x3,
//    DISP_FORMAT_RGB655      =0x4,
//    DISP_FORMAT_RGB565      =0x5,
//    DISP_FORMAT_RGB556      =0x6,
//    DISP_FORMAT_ARGB1555    =0x7,
//    DISP_FORMAT_RGBA5551    =0x8,
//    DISP_FORMAT_RGB888      =0x9,
//    DISP_FORMAT_ARGB8888    =0xa,
//
//    DISP_FORMAT_YUV444      =0xb,
//    DISP_FORMAT_YUV422      =0xc,
//    DISP_FORMAT_YUV420      =0xd,
//    DISP_FORMAT_YUV411      =0xe,
//    DISP_FORMAT_CSIRGB      =0xf,


//const __u32 step_size=2;
//__u8 iic_reg[reg_step];
//void input_iic_data();

//__u8 asc2bin(__u8 data);

__u32 frame_count=0;
__u32 CNT_MAX=5;
__u32 file_writing=0;
__u32 wr_size;

__u8  TIME_OUT_MAX=0;

char file_path[]="e:\\";
char file_number[3]="000";

char file_name[];

//__u32 *frame_buffer_ptr;
__u8 *fb_ptr;//ref to frame buffer addr

__u8 reg_data[]=
{
0x12,0x34,0x56,0x78,
0x9a,0xbc,0xde,0xf0,
0xfe,0xdc,0xba,0x98,
0x76,0x54,0x32,0x10,
//0x12345678,
//0x9abcdef0,
//0xfedcba98,
//0x76543210,
};



__u8 asc2bin(__u8 data)
{
    //__u8 ret_d;
    if (data>0x2f&&data<0x3a) data=(data-0x30) & 0x0f;
    	else if (data>0x40&&data<0x47) data=(data-0x37) & 0x0f;
    	else if (data>0x60&&data<0x67) data=(data-0x57) & 0x0f;
    	//else if (data==0x0d || data==0x0a ||data==0x20)data=0x00;//CR //space
      else data=0xff;//other ascii

    return data;
}

__csi_frame_t *p_display_buf;
__u32 frame_id = 0;
static __s32 capture_on_times = -1;


__s32 test_video_in_main(__s32 dev_index)
{
	__u32 dev_sel=0;
	__u32 mod_num=0;
	__u32 i = 0;
	__u32 count_loop = 0;
	__u32 count_loop1 = 0;

	__u32 screen_width = 0;
	__u32 screen_height = 0;
	__u32 source_width = 0;
	__u32 source_height = 0;

	__s32 time_out=0;
	__s32 frame_rate=0;
	__s32 frame_period_ms=0;

	__u32 csi_src_size=0;

	__csi_mode_all_t csi_mode_all[2];
	__csi_mode_t csi_mode[2];
	__csi_frame_queue_t FrameQueue;

	__disp_layer_info_t layer_para = {0};
	__disp_video_fb_t video_fb;
	__u32 arg[3];
	__u32 sel = 0;
	ES_FILE * disphd;

	__hdle hlay;


	RECT rect;

 start:
    __inf("csi hello!\n");

    __inf("please select csi channel(0/1): ");

    dev_sel=dev_index;
    dev_sel=(dev_sel&0x7f-0x30)&0x0f;
	__inf("%d\n",dev_sel);

	if (dev_sel==0)
	{
	    __u8				mid;
	    __mp * hDrv;
		#if 1
		esDEV_Plugin("\\drv\\csi.drv", 0, 0, 1);
		#else
		mid = esMODS_MInstall("f:\\AW1663_test\\csi_test\\csi.drv", 0);
		__inf("install csi_2ch.drv mid=%d\n",mid);
		hDrv = esMODS_MOpen(mid , 0);
		__inf("open csi_2ch.drv hDrv=%x\n",hDrv);
		esMODS_MIoctrl(hDrv, DRV_CMD_PLUGIN, 0, 0);
		#endif
		esKRNL_TimeDly(1);
	  	fp_csi = eLIBs_fopen("b:\\USER\\CSI", "r+");
	  	__inf("open b:\\USER\\CSI fp=%x\n",fp_csi);
		
  	}
	else if(dev_sel==1)
	{
		#if 1
		esDEV_Plugin("\\drv\\tvd.drv", 0, 0, 1);
		#else
		mid = esMODS_MInstall("f:\\AW1663_test\\csi_test\\csi.drv", 0);
		__inf("install csi_2ch.drv mid=%d\n",mid);
		hDrv = esMODS_MOpen(mid , 0);
		__inf("open csi_2ch.drv hDrv=%x\n",hDrv);
		esMODS_MIoctrl(hDrv, DRV_CMD_PLUGIN, 0, 0);
		#endif
		fp_csi = eLIBs_fopen("b:\\USER\\TVD", "r+");
  	}
	__inf("csi driver plugin success!\n");

	if(fp_csi == NULL)
	{
		__err("csi driver open failed\n");
		return EPDK_FAIL;
	}	
	else 
	{
		__inf("csi driver open success! fp=%x.\n",fp_csi);
    }

	//先初始化
    eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_INIT, 0, 0);
    //获取所有模式
    eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_GET_MODE_ALL, 0, &csi_mode_all[dev_sel]);
    for (i = 0; i<csi_mode_all[dev_sel].number; i++){
        __inf("#########################\n");
        __inf("### mode %d ###\n", i);
        __inf("# size: %dx%d\n", 				csi_mode_all[dev_sel].csi_mode[i].size.width, csi_mode_all[dev_sel].csi_mode[i].size.height);
        __inf("# frame_rate: %d\n", 		csi_mode_all[dev_sel].csi_mode[i].frame_rate);
        __inf("# color_format: %d\n", 	csi_mode_all[dev_sel].csi_mode[i].color_format);
        __inf("# frame_period: %d\n", 	csi_mode_all[dev_sel].csi_mode[i].frame_period);
        __inf("# component_seq: %d\n", 	csi_mode_all[dev_sel].csi_mode[i].component_seq);
        __inf("# store_mode: %d\n", 		csi_mode_all[dev_sel].csi_mode[i].store_mode);
        __inf("# color_space: %d\n", 		csi_mode_all[dev_sel].csi_mode[i].color_space);
        __inf("#########################\n");
    }
    //设置模式
csi_mode_sel:

    __inf("please input csi mode(0~%d): ",csi_mode_all[dev_sel].number-1);

    //mod_num = __getc();
    //mod_num = asc2bin(mod_num);
    mod_num=0;//__getc();
    mod_num=(mod_num&0x7f-0x30)&0x0f;
	__inf("%d\n",mod_num);
    mod_num=(mod_num<0 || mod_num>csi_mode_all[dev_sel].number-1)?0:mod_num;
    __inf("select csi mode [%d]\n", mod_num);
    //mod_num=2;
    eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_SET_MODE, 0, &(csi_mode_all[dev_sel].csi_mode[mod_num]));
    //取得当前模式
    eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_GET_MODE, 0, &csi_mode[dev_sel]);
    __inf("color_fmt=%d:%dx%d@%d\n",csi_mode[dev_sel].color_format, csi_mode[dev_sel].size.width, csi_mode[dev_sel].size.height, csi_mode[dev_sel].frame_rate);

    FrameQueue.num = 0x6; // single buffer;

    csi_src_size=csi_mode[dev_sel].size.width*csi_mode[dev_sel].size.height;//for YUV422(/420)_UV_CB
    __inf("csi_src_size=%dkByte\n",csi_src_size/1024);

    for (i=0;i<FrameQueue.num;i++)
  	{
	    //address 0
	    if((FrameQueue.csi_frame_array[i].addr[0] = (__u32)esMEMS_Palloc(((csi_src_size + 1023)>>10), MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE))==NULL)
	    {
	    	__err("Failed to malloc!\n");
	    }
	    eLIBs_memset((__u8 *)FrameQueue.csi_frame_array[i].addr[0], 0, csi_src_size);
	    eLIBs_CleanFlushDCacheRegion((void *)FrameQueue.csi_frame_array[i].addr[0], csi_src_size);

	    //address 1
	    if((FrameQueue.csi_frame_array[i].addr[1] = (__u32)esMEMS_Palloc(((csi_src_size + 1023)>>10), MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE))==NULL)
	    {
	    	__err("Failed to malloc!\n");
	    }
	    eLIBs_memset((__u8 *)FrameQueue.csi_frame_array[i].addr[1], 0, csi_src_size);
	    eLIBs_CleanFlushDCacheRegion((void *)FrameQueue.csi_frame_array[i].addr[1], csi_src_size);
		#if 0
	    //address 2
	    if((FrameQueue.csi_frame_array[i].addr[2] = (__u32)esMEMS_Palloc(((csi_src_size + 1023)>>10), MEMS_PALLOC_MODE_BND_NONE | MEMS_PALLOC_MODE_BNK_NONE))==NULL)
	    {
	    	__err("Failed to malloc!\n");
	    }
	    eLIBs_memset((__u8 *)FrameQueue.csi_frame_array[i].addr[2], 0, csi_src_size);
	    eLIBs_CleanFlushDCacheRegion((void *)FrameQueue.csi_frame_array[i].addr[2], csi_src_size);
		#endif

	    FrameQueue.csi_frame_array[i].frame_id = generate_frame_id(i);
	    __msg("request frame id %d = %x\n",i,FrameQueue.csi_frame_array[i].frame_id );
	    FrameQueue.csi_frame_array[i].bProgressiveSrc = 0; //bProgressiveSrc;  值由csidrv决定，这里随便赋值
	    FrameQueue.csi_frame_array[i].bTopFieldFirst = 0;   //bTopFieldFirst;
	    FrameQueue.csi_frame_array[i].eAspectRatio = 0;     //eAspectRatio;
	    FrameQueue.csi_frame_array[i].color_format = csi_mode[dev_sel].color_format;
	    FrameQueue.csi_frame_array[i].src_rect.width = FrameQueue.csi_frame_array[i].dst_rect.width = csi_mode[dev_sel].size.width;
	    FrameQueue.csi_frame_array[i].src_rect.height = FrameQueue.csi_frame_array[i].dst_rect.height = csi_mode[dev_sel].size.height;
	    FrameQueue.csi_frame_array[i].frame_rate = csi_mode[dev_sel].frame_rate;
	    FrameQueue.csi_frame_array[i].uPts = 0;   //pts;
	    FrameQueue.csi_frame_array[i].bPtsValid = 1;
	    FrameQueue.csi_frame_array[i].component_seq = csi_mode[dev_sel].component_seq;;
	    FrameQueue.csi_frame_array[i].store_mode = csi_mode[dev_sel].store_mode;
	    __msg("buffer%d:%x,%x,%x\n",0, FrameQueue.csi_frame_array[i].addr[0],FrameQueue.csi_frame_array[i].addr[1],FrameQueue.csi_frame_array[i].addr[2]);


	}
	eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_SET_FRAME_QUEUE, 0, &FrameQueue);

    frame_period_ms=csi_mode[dev_sel].frame_period / 1000;
//=====================================================
    switch (csi_mode[dev_sel].color_format){
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
    switch (csi_mode[dev_sel].component_seq){
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
    switch(csi_mode[dev_sel].store_mode){
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


    disp_fb_x_size=csi_mode[dev_sel].size.width;
    disp_fb_y_size=csi_mode[dev_sel].size.height;

    disp_src_x_size=csi_mode[dev_sel].size.width;
    disp_src_y_size=csi_mode[dev_sel].size.height;

    {
		__u32 lcd_sel=0;
		__inf("######################################\n");
		__inf("#please select LCD size(0/1/2...):\n");
		__inf("#0: lcd size=800*480\n");
		__inf("#1: lcd size=400*280\n");
		__inf("#2: lcd size=320*240\n");
		__inf("######################################\n");
		lcd_sel= 0 ;//__getc();
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

	//sel=0;
	//arg[0] = 0;
    //arg[1] = 0;
    //arg[2] = 0;
    //eLIBs_fioctrl(disphd, DISP_CMD_LCD_ON, sel, (void*)arg);

	layer_para.fb.addr[0]       = FrameQueue.csi_frame_array[0].addr[0];
    layer_para.fb.addr[1]       = FrameQueue.csi_frame_array[0].addr[1];
    layer_para.fb.addr[2]       = FrameQueue.csi_frame_array[0].addr[2];
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

#if 0
    __inf("please input TIME_OUT_MAX(0~9: no time out/5sec/10sec/15sec...)");
    TIME_OUT_MAX=__getc();
    TIME_OUT_MAX=asc2bin(TIME_OUT_MAX) *5;
	  __inf("%d\n",TIME_OUT_MAX);
    if (TIME_OUT_MAX==0) TIME_OUT_EN=0;
#endif

		time_out=0;
	  	wr_size=disp_fb_x_size*disp_fb_y_size*2;
	  //__inf("frame size=%x\n",wr_size);

		__msg("start to requst csi frame\n");

 		esKRNL_TimeDly(1);

	    //务必先开始抓取数据
	    eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_CAPTURE_ON, 0, 0);	    

	while(1)
    {
		__u32 frame_req;
		__u32 time_st ;
		static __u32 request_frame_fail_time = 0 ;
		//__inf("io set addr %x\n",p_display_buf);
		frame_req=eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_REQUEST_FRAME, 0, &p_display_buf);

		// __inf("req return %x\n",frame_req);

		if(frame_req==EPDK_OK) {

			layer_para.fb.addr[0]       = p_display_buf->addr[0];
			layer_para.fb.addr[1]       = p_display_buf->addr[1];
			layer_para.fb.addr[2]       = p_display_buf->addr[2];

	        __inf("req addr %x %x %x\n",p_display_buf->addr[0],p_display_buf->addr[1],p_display_buf->addr[2]);

	        sel    = 0;
	        arg[0] = hlay;
	        arg[1] = (__u32)&layer_para;
	        arg[2] = 0;
			time_st = esKRNL_TimeGet();
	        eLIBs_fioctrl(disphd,DISP_CMD_LAYER_SET_PARA,sel,(void*)arg);
			time_st = esKRNL_TimeGet() - time_st;
	     	//__inf("time_st:%x\n",time_st);


	        frame_req = eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_RELEASE_FRAME, p_display_buf->frame_id, NULL);

		}
		else
		{
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

    eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_CAPTURE_OFF, 0, 0);
    eLIBs_fioctrl(fp_csi, DRV_CSI_CMD_EXIT, 0, 0);//应用程序退出必须调用此步，否则内存没有释放
	eLIBs_fclose(fp_csi);
	esDEV_Plugout("\\drv\\csi.drv", 0);
	__inf("csi byebye!\n");

		//input_iic_data();
		//__inf("%x%x%x\n",iic_reg[0],iic_reg[1],iic_reg[2]);

		//while(1);

		goto start;

	return EPDK_OK;

}

/*******************************************************************************
Function name: generate_frame_id
Description:
    1。只用低8bit，因为display_info的top_index是__u8类型
Parameters:

Return:

Time: 2010/7/13
*******************************************************************************/
__s32 generate_frame_id(__s32 index)
{
    __s32 prefix = capture_on_times%16;
    __s32 frame_id;
    frame_id = (prefix<<4 | index);
    return frame_id;
}



void record_capture_on_times()
{
    capture_on_times++;
    return;
}

//	dev_index:0 for csi, 1 for tv in
__s32 test_video_in(__s32 dev_index)
{
	test_video_in_main(dev_index);
    return EPDK_OK;
}


