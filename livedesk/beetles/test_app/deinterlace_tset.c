
#include "epdk.h"

#define OUTPUT_MB_UV_COMBINE_SIZE_W	720	//( (720+31)/32)*32
#define OUTPUT_MB_UV_COMBINE_SIZE_H	480
#define OUTPUT_MON_MB_UV_COMBINE_SIZE_W	720
#define OUTPUT_MON_MB_UV_COMBINE_SIZE_H	576
#define	SOURCE_FILE_NUM		5

#define	 TEST_IN_SDCARD_ENABLE	0
#if TEST_IN_SDCARD_ENABLE
#define  TEST_SOURCE_PATH	"f:\\AW1663_test\\di_test\\"
#else
#define  TEST_SOURCE_PATH	"D:\\AW1663_test\\di_test\\"
#endif
#define  MONMB_SOURCE_PATH	TEST_SOURCE_PATH"nonmb\\"
#define  MB_SOURCE_PATH		TEST_SOURCE_PATH"mb\\"

#define LAYER_USE_SCLAER_MODE	1

#define		ALIGN_32BYTE(value)	(((value+31)>>5)<<5)
__u8 mid;
__mp * hDrv;
ES_FILE* fp_di ;

int de_interlace_test_nonmb(void)
{
	DI_Fb_Addr_t DI_Fb_Addr ;
	DI_Source_Config_t DI_Source_Config;
	ES_FILE *input_file_y[SOURCE_FILE_NUM],*input_file_c[SOURCE_FILE_NUM];
	__u32 i ;
	__s32 ifile_size_y,ifile_size_uv ;
#if TEST_IN_SDCARD_ENABLE
	ES_FILE *output_file_y[SOURCE_FILE_NUM],*output_file_c[SOURCE_FILE_NUM];
#else
	ES_FILE      * p_disp;
	__s32		width, height;
	__disp_layer_info_t layer_para;
    __u32 arg[3];
	__hdle layer = NULL;
#endif

	input_file_y[0] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_y0.bin", "rb");
	input_file_c[0] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_uv0.bin", "rb");

	input_file_y[1] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_y1.bin", "rb");
	input_file_c[1] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_uv1.bin", "rb");

	input_file_y[2] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_y2.bin", "rb");
	input_file_c[2] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_uv2.bin", "rb");

	input_file_y[3] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_y3.bin", "rb");
	input_file_c[3] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_uv3.bin", "rb");

	input_file_y[4] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_y4.bin", "rb");
	input_file_c[4] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_src_720_576_uv4.bin", "rb");

#if TEST_IN_SDCARD_ENABLE
	output_file_y[0] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_y0.bin", "wb");
	output_file_c[0] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_uv0.bin", "wb");
	output_file_y[1] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_y1.bin", "wb");
	output_file_c[1] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_uv1.bin", "wb");
	output_file_y[2] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_y2.bin", "wb");
	output_file_c[2] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_uv2.bin", "wb");
	output_file_y[3] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_y3.bin", "wb");
	output_file_c[3] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_uv3.bin", "wb");
	output_file_y[4] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_y4.bin", "wb");
	output_file_c[4] = eLIBs_fopen(MONMB_SOURCE_PATH"di_test_dst_720_576_uv4.bin", "wb");
#else
	/*open display driver handle*/
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__wrn("open display device fail!\n");
		return EPDK_FAIL;
	}	

	width  = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_WIDTH, 0, 0);
	height = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_HEIGHT, 0, 0);

	__msg("width = %d, height = %d\n", width, height);
#if LAYER_USE_SCLAER_MODE
    arg[0] = DISP_LAYER_WORK_MODE_SCALER;
#else
    arg[0] = DISP_LAYER_WORK_MODE_NORMAL;
#endif
	arg[1] = 0;
    arg[2] = 0;
    layer = eLIBs_fioctrl(p_disp, DISP_CMD_LAYER_REQUEST, 0, (void*)arg);

#if LAYER_USE_SCLAER_MODE
    layer_para.mode = DISP_LAYER_WORK_MODE_SCALER;
#else
    layer_para.mode = DISP_LAYER_WORK_MODE_NORMAL;
#endif
    layer_para.fb.size.width    = OUTPUT_MON_MB_UV_COMBINE_SIZE_W;
    layer_para.fb.size.height   = OUTPUT_MON_MB_UV_COMBINE_SIZE_H;
    layer_para.fb.mode          = DISP_MOD_NON_MB_UV_COMBINED;
    layer_para.fb.format        = DISP_FORMAT_YUV420;
    layer_para.fb.br_swap       = 0;
    layer_para.fb.seq           = DISP_SEQ_UVUV;
    layer_para.ck_enable        = 0;
    layer_para.alpha_en         = 0; 
    layer_para.alpha_val        = 0xff;
    layer_para.pipe             = 0; 
    layer_para.src_win.x        = 0;
    layer_para.src_win.y        = 0;
    layer_para.src_win.width    = OUTPUT_MON_MB_UV_COMBINE_SIZE_W;
    layer_para.src_win.height   = OUTPUT_MON_MB_UV_COMBINE_SIZE_H;
#if LAYER_USE_SCLAER_MODE
    layer_para.scn_win.x        = 0;
    layer_para.scn_win.y        = 0;
    layer_para.scn_win.width    = width;
    layer_para.scn_win.height   = height;
#else
    layer_para.scn_win.x        = (width-OUTPUT_MON_MB_UV_COMBINE_SIZE_W)>>1;
    layer_para.scn_win.y        = (height-OUTPUT_MON_MB_UV_COMBINE_SIZE_H)>>1;
    layer_para.scn_win.width    = OUTPUT_MON_MB_UV_COMBINE_SIZE_W;
    layer_para.scn_win.height   = OUTPUT_MON_MB_UV_COMBINE_SIZE_H;
#endif


#endif
	eLIBs_fseek(input_file_y[0],0,SEEK_END);
	ifile_size_y =  eLIBs_ftell(input_file_y[0]);
	eLIBs_fseek(input_file_y[0],0,SEEK_SET);
	
	eLIBs_fseek(input_file_c[0],0,SEEK_END);
	ifile_size_uv =  eLIBs_ftell(input_file_c[0]);
	eLIBs_fseek(input_file_c[0],0,SEEK_SET);

	// step1 : config input source
	DI_Source_Config.src_width = OUTPUT_MON_MB_UV_COMBINE_SIZE_W;
	DI_Source_Config.src_height = OUTPUT_MON_MB_UV_COMBINE_SIZE_H;
	DI_Source_Config.type.mod = DI_NON_MB_UVCOMBINE;
	DI_Source_Config.type.ps = 0 ;

	eLIBs_fioctrl(fp_di,DI_CMD_CONFIG_SRC,0,(void*)(&DI_Source_Config));

	// step2 : malloc buffer for frame , should be physical discontinuity
	DI_Fb_Addr.pre_frame_buf.ch0_addr = (__u32)esMEMS_Palloc((ifile_size_y+1023)>>10,0);
	DI_Fb_Addr.pre_frame_buf.ch1_addr = (__u32)esMEMS_Palloc((ifile_size_uv+1023)>>10,0);
	DI_Fb_Addr.cur_frame_buf.ch0_addr = (__u32)esMEMS_Palloc((ifile_size_y+1023)>>10,0);
	DI_Fb_Addr.cur_frame_buf.ch1_addr = (__u32)esMEMS_Palloc((ifile_size_uv+1023)>>10,0);
	DI_Fb_Addr.out_frame_buf.ch0_addr = (__u32)esMEMS_Palloc(((OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H)+1023)>>10,0);
	DI_Fb_Addr.out_frame_buf.ch1_addr = (__u32)esMEMS_Palloc(((OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H)/2+1023)>>10,0);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,ifile_size_y);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,ifile_size_uv);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.cur_frame_buf.ch0_addr,ifile_size_y);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.cur_frame_buf.ch1_addr,ifile_size_uv);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.out_frame_buf.ch0_addr,(OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H));
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.out_frame_buf.ch1_addr,(OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H)/2);

	eLIBs_memset((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,0x00,((ifile_size_y+1023)>>10)<<10);
	eLIBs_memset((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,0x80,((ifile_size_uv+1023)>>10)<<10);

	if(input_file_y[0])
	{
		eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch0_addr, 1, ifile_size_y, input_file_y[0]);
	}
	if(input_file_c[0])
	{
		eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch1_addr, 1, ifile_size_uv, input_file_c[0]);
	}
	
	
	eLIBs_fioctrl(fp_di,DI_CMD_SET_FB_ADDR,0,(void*)(&DI_Fb_Addr));

	__msg("start to do de-interlace.....................\n");
	// step3 : start to do deinterlace
	eLIBs_fioctrl(fp_di,DI_CMD_START,0,0);
	__msg("do de-interlace  finish......................\n");

#if TEST_IN_SDCARD_ENABLE
	if(output_file_y[0])
	{
		eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch0_addr, 1, (OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H), output_file_y[0]);
		eLIBs_fclose(output_file_y[0]);
		output_file_y[0] = NULL ;
	}
	if(output_file_c[0])
	{
		eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch1_addr, 1, (OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H)/2, output_file_c[0]);
		eLIBs_fclose(output_file_c[0]);
		output_file_c[0] = NULL ;
	}
#else
    layer_para.fb.addr[0]       = (__u32)DI_Fb_Addr.out_frame_buf.ch0_addr; 
    layer_para.fb.addr[1]       = (__u32)DI_Fb_Addr.out_frame_buf.ch1_addr; 

    arg[0] = layer;
    arg[1] = (__u32)&layer_para;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);

    arg[0] = layer;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_OPEN,0,(void*)arg);
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_TOP,0,(void*)arg);
#endif

	for(i=1; i<SOURCE_FILE_NUM; i++)
	{
		eLIBs_memcpy((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,(void*)DI_Fb_Addr.cur_frame_buf.ch0_addr,ifile_size_y);
		eLIBs_memcpy((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,(void*)DI_Fb_Addr.cur_frame_buf.ch1_addr,ifile_size_uv);
		eLIBs_CleanFlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,ifile_size_y);
		eLIBs_CleanFlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,ifile_size_uv);
		if(input_file_y[i])
		{
			eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch0_addr, 1, ifile_size_y, input_file_y[i]);
		}
		if(input_file_c[i])
		{
			eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch1_addr, 1, ifile_size_uv, input_file_c[i]);
		}
		eLIBs_fioctrl(fp_di,DI_CMD_SET_FB_ADDR,0,(void*)(&DI_Fb_Addr));
		eLIBs_fioctrl(fp_di,DI_CMD_START,0,0);
#if TEST_IN_SDCARD_ENABLE
		if(output_file_y[i])
		{
			eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch0_addr, 1, (OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H), output_file_y[i]);
			eLIBs_fclose(output_file_y[i]);
			output_file_y[i] = NULL ;
		}
		if(output_file_c[i])
		{
			eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch1_addr, 1, (OUTPUT_MON_MB_UV_COMBINE_SIZE_W*OUTPUT_MON_MB_UV_COMBINE_SIZE_H)/2, output_file_c[i]);
			eLIBs_fclose(output_file_c[i]);
			output_file_c[i] = NULL ;
		}
#else
		__msg("show di result %d.bmp ............\n",i);
	    layer_para.fb.addr[0]       = (__u32)DI_Fb_Addr.out_frame_buf.ch0_addr; 
	    layer_para.fb.addr[1]       = (__u32)DI_Fb_Addr.out_frame_buf.ch1_addr; 

	    arg[0] = layer;
	    arg[1] = (__u32)&layer_para;
	    arg[2] = 0;
	    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);
		__inf("press any key to continue ............\n");
		__getc();
#endif
	}
#if TEST_IN_SDCARD_ENABLE

#else
	arg[0] = layer;
	arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp, DISP_CMD_LAYER_RELEASE, 0, (void*)arg);
	eLIBs_fclose(p_disp);
#endif

	__msg("test de-interlace mon mb mode exit ............\n");
}

int de_interlace_test_mb(void)
{
	DI_Fb_Addr_t DI_Fb_Addr ;
	DI_Source_Config_t DI_Source_Config;
	ES_FILE *input_file_y[SOURCE_FILE_NUM],*input_file_c[SOURCE_FILE_NUM];
	__u32 i ;
	__s32 ifile_size_y,ifile_size_uv ;
#if TEST_IN_SDCARD_ENABLE
	ES_FILE *output_file_y[SOURCE_FILE_NUM],*output_file_c[SOURCE_FILE_NUM];
#else
	ES_FILE      * p_disp;
	__s32		width, height;
	__disp_layer_info_t layer_para;
    __u32 arg[3];
	__hdle layer = NULL;
#endif

	input_file_y[0] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_y0.bin", "rb");
	input_file_c[0] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_uv0.bin", "rb");

	input_file_y[1] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_y1.bin", "rb");
	input_file_c[1] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_uv1.bin", "rb");

	input_file_y[2] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_y2.bin", "rb");
	input_file_c[2] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_uv2.bin", "rb");

	input_file_y[3] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_y3.bin", "rb");
	input_file_c[3] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_uv3.bin", "rb");

	input_file_y[4] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_y4.bin", "rb");
	input_file_c[4] = eLIBs_fopen(MB_SOURCE_PATH"di_test_src_720_480_uv4.bin", "rb");

#if TEST_IN_SDCARD_ENABLE
	output_file_y[0] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_y0.bin", "wb");
	output_file_c[0] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_uv0.bin", "wb");
	output_file_y[1] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_y1.bin", "wb");
	output_file_c[1] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_uv1.bin", "wb");
	output_file_y[2] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_y2.bin", "wb");
	output_file_c[2] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_uv2.bin", "wb");
	output_file_y[3] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_y3.bin", "wb");
	output_file_c[3] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_uv3.bin", "wb");
	output_file_y[4] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_y4.bin", "wb");
	output_file_c[4] = eLIBs_fopen(MB_SOURCE_PATH"di_test_dst_720_480_uv4.bin", "wb");
#else
	/*open display driver handle*/
	p_disp = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
	if(!p_disp)
	{
		__wrn("open display device fail!\n");
		return EPDK_FAIL;
	}	

	width  = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_WIDTH, 0, 0);
	height = eLIBs_fioctrl(p_disp, DISP_CMD_SCN_GET_HEIGHT, 0, 0);

	__msg("width = %d, height = %d\n", width, height);
#if LAYER_USE_SCLAER_MODE
    arg[0] = DISP_LAYER_WORK_MODE_SCALER;
#else
    arg[0] = DISP_LAYER_WORK_MODE_NORMAL;
#endif
	arg[1] = 0;
    arg[2] = 0;
    layer = eLIBs_fioctrl(p_disp, DISP_CMD_LAYER_REQUEST, 0, (void*)arg);

#if LAYER_USE_SCLAER_MODE
    layer_para.mode = DISP_LAYER_WORK_MODE_SCALER;
#else
    layer_para.mode = DISP_LAYER_WORK_MODE_NORMAL;
#endif
    layer_para.fb.size.width    =  OUTPUT_MB_UV_COMBINE_SIZE_W;
    layer_para.fb.size.height   =  OUTPUT_MB_UV_COMBINE_SIZE_H;
    layer_para.fb.mode          = DISP_MOD_NON_MB_UV_COMBINED;
    layer_para.fb.format        = DISP_FORMAT_YUV420;
    layer_para.fb.br_swap       = 0;
    layer_para.fb.seq           = DISP_SEQ_UVUV;
    layer_para.ck_enable        = 0;
    layer_para.alpha_en         = 0; 
    layer_para.alpha_val        = 0xff;
    layer_para.pipe             = 0; 
    layer_para.src_win.x        = 0;
    layer_para.src_win.y        = 0;
    layer_para.src_win.x        = 0;
    layer_para.src_win.y        = 0;
    layer_para.src_win.width    = OUTPUT_MB_UV_COMBINE_SIZE_W;
    layer_para.src_win.height   = OUTPUT_MB_UV_COMBINE_SIZE_H;
#if LAYER_USE_SCLAER_MODE
    layer_para.scn_win.x        = 0;
    layer_para.scn_win.y        = 0;
    layer_para.scn_win.width    = width;
    layer_para.scn_win.height   = height;
#else
    layer_para.scn_win.x        = (width-OUTPUT_MON_MB_UV_COMBINE_SIZE_W)>>1;
    layer_para.scn_win.y        = (height-OUTPUT_MON_MB_UV_COMBINE_SIZE_H)>>1;
    layer_para.scn_win.width    = OUTPUT_MON_MB_UV_COMBINE_SIZE_W;
    layer_para.scn_win.height   = OUTPUT_MON_MB_UV_COMBINE_SIZE_H;
#endif
#endif
	eLIBs_fseek(input_file_y[0],0,SEEK_END);
	ifile_size_y =  eLIBs_ftell(input_file_y[0]);
	eLIBs_fseek(input_file_y[0],0,SEEK_SET);
	
	eLIBs_fseek(input_file_c[0],0,SEEK_END);
	ifile_size_uv =  eLIBs_ftell(input_file_c[0]);
	eLIBs_fseek(input_file_c[0],0,SEEK_SET);

	// step1 : config input source
	DI_Source_Config.src_width = OUTPUT_MB_UV_COMBINE_SIZE_W;
	DI_Source_Config.src_height = OUTPUT_MB_UV_COMBINE_SIZE_H;
	DI_Source_Config.type.mod = DI_MB_UVCOMBINE;
	DI_Source_Config.type.ps = 0 ;

	eLIBs_fioctrl(fp_di,DI_CMD_CONFIG_SRC,0,(void*)(&DI_Source_Config));

	// step2 : malloc buffer for frame , should be physical discontinuity
	DI_Fb_Addr.pre_frame_buf.ch0_addr = (__u32)esMEMS_Palloc((ifile_size_y+1023)>>10,0);
	DI_Fb_Addr.pre_frame_buf.ch1_addr = (__u32)esMEMS_Palloc((ifile_size_uv+1023)>>10,0);
	DI_Fb_Addr.cur_frame_buf.ch0_addr = (__u32)esMEMS_Palloc((ifile_size_y+1023)>>10,0);
	DI_Fb_Addr.cur_frame_buf.ch1_addr = (__u32)esMEMS_Palloc((ifile_size_uv+1023)>>10,0);
	DI_Fb_Addr.out_frame_buf.ch0_addr = (__u32)esMEMS_Palloc(((OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H)+1023)>>10,0);
	DI_Fb_Addr.out_frame_buf.ch1_addr = (__u32)esMEMS_Palloc(((OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H)/2+1023)>>10,0);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,ifile_size_y);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,ifile_size_uv);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.cur_frame_buf.ch0_addr,ifile_size_y);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.cur_frame_buf.ch1_addr,ifile_size_uv);
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.out_frame_buf.ch0_addr,(OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H));
	eLIBs_FlushDCacheRegion((void*)DI_Fb_Addr.out_frame_buf.ch1_addr,(OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H)/2);

	eLIBs_memset((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,0x00,((ifile_size_y+1023)>>10)<<10);
	eLIBs_memset((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,0x80,((ifile_size_uv+1023)>>10)<<10);

	if(input_file_y[0])
	{
		eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch0_addr, 1, ifile_size_y, input_file_y[0]);
	}
	if(input_file_c[0])
	{
		eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch1_addr, 1, ifile_size_uv, input_file_c[0]);
	}
	
	
	eLIBs_fioctrl(fp_di,DI_CMD_SET_FB_ADDR,0,(void*)(&DI_Fb_Addr));

	__msg("start to do de-interlace.....................\n");
	// step3 : start to do deinterlace
	eLIBs_fioctrl(fp_di,DI_CMD_START,0,0);
	__msg("do de-interlace  finish......................\n");

#if TEST_IN_SDCARD_ENABLE
	if(output_file_y[0])
	{
		eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch0_addr, 1, (OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H), output_file_y[0]);
		eLIBs_fclose(output_file_y[0]);
		output_file_y[0] = NULL ;
	}
	if(output_file_c[0])
	{
		eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch1_addr, 1, (OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H)/2, output_file_c[0]);
		eLIBs_fclose(output_file_c[0]);
		output_file_c[0] = NULL ;
	}
#else
    layer_para.fb.addr[0]       = (__u32)DI_Fb_Addr.out_frame_buf.ch0_addr; 
    layer_para.fb.addr[1]       = (__u32)DI_Fb_Addr.out_frame_buf.ch1_addr; 

    arg[0] = layer;
    arg[1] = (__u32)&layer_para;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);

    arg[0] = layer;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_OPEN,0,(void*)arg);
    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_TOP,0,(void*)arg);
#endif

	for(i=1; i<SOURCE_FILE_NUM; i++)
	{
		eLIBs_memcpy((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,(void*)DI_Fb_Addr.cur_frame_buf.ch0_addr,ifile_size_y);
		eLIBs_memcpy((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,(void*)DI_Fb_Addr.cur_frame_buf.ch1_addr,ifile_size_uv);
		eLIBs_CleanFlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch0_addr,ifile_size_y);
		eLIBs_CleanFlushDCacheRegion((void*)DI_Fb_Addr.pre_frame_buf.ch1_addr,ifile_size_uv);
		if(input_file_y[i])
		{
			eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch0_addr, 1, ifile_size_y, input_file_y[i]);
		}
		if(input_file_c[i])
		{
			eLIBs_fread((void*)DI_Fb_Addr.cur_frame_buf.ch1_addr, 1, ifile_size_uv, input_file_c[i]);
		}
		eLIBs_fioctrl(fp_di,DI_CMD_SET_FB_ADDR,0,(void*)(&DI_Fb_Addr));
		eLIBs_fioctrl(fp_di,DI_CMD_START,0,0);
#if TEST_IN_SDCARD_ENABLE
		if(output_file_y[i])
		{
			eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch0_addr, 1, (OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H), output_file_y[i]);
			eLIBs_fclose(output_file_y[i]);
			output_file_y[i] = NULL ;
		}
		if(output_file_c[i])
		{
			eLIBs_fwrite((void*)DI_Fb_Addr.out_frame_buf.ch1_addr, 1, (OUTPUT_MB_UV_COMBINE_SIZE_W*OUTPUT_MB_UV_COMBINE_SIZE_H)/2, output_file_c[i]);
			eLIBs_fclose(output_file_c[i]);
			output_file_c[i] = NULL ;
		}
#else
		__msg("show di result %d.bmp ............\n",i);
	    layer_para.fb.addr[0]       = (__u32)DI_Fb_Addr.out_frame_buf.ch0_addr; 
	    layer_para.fb.addr[1]       = (__u32)DI_Fb_Addr.out_frame_buf.ch1_addr; 

	    arg[0] = layer;
	    arg[1] = (__u32)&layer_para;
	    arg[2] = 0;
	    eLIBs_fioctrl(p_disp,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);
		__inf("press any key to continue ............\n");
		__getc();
#endif
	}
#if TEST_IN_SDCARD_ENABLE

#else
	arg[0] = layer;
	arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(p_disp, DISP_CMD_LAYER_RELEASE, 0, (void*)arg);
	eLIBs_fclose(p_disp);
#endif

	__msg("test de-interlace mon mb mode exit ............\n");
}
int de_interlace_test(void)
{
	mid = esMODS_MInstall(TEST_SOURCE_PATH"di.drv", 0);
	__inf("install di.drv mid=%d\n",mid);
	hDrv = esMODS_MOpen(mid , 0);
	esMODS_MIoctrl(hDrv, DRV_CMD_PLUGIN, 0, 0);

	esKRNL_TimeDly(1);
  	fp_di = eLIBs_fopen("b:\\USER\\DI", "r+");
  	__inf("open b:\\USER\\DI fp=%x\n",fp_di);

#if 0
	{
		__s32 debug = 0x55 ;
		while(debug==0x55)
			;
	}
#endif

	de_interlace_test_mb();
	de_interlace_test_nonmb();

	eLIBs_fclose(fp_di);
	esMODS_MClose(hDrv);
	esMODS_MUninstall(mid);
}


