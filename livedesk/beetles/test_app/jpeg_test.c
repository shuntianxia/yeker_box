
#include "epdk.h"

#define MAX_OVERLAY_BLOCK_NUM 40
#define OVERLAY_PALETTE_SIZE  64				/*the total number of color is 16, sizeof(argb)=4 ,16*4=64*/
#define OVERLAY_DATA_BUF_SIZE  0x40000          /*256k*/
#define OVERLAY_MAX_SRC_GROUP_NUM 20            /* overlay最多包含多少组资源 */

typedef struct overlay_block_header
{
    __u32    Next_BlkHdr_Addr;
    __u32    hor_plane;
    __u32    ver_plane;
    __u32    Mb_Pal_Idx;
}overlay_block_header_t;

typedef struct overlay_info
{
    __u8                    isEnabled;                              /* 当前有多少block */
    __u32                   nblock;                                 /* 当前有多少block */
    __u32                   nsrc;                                   /* 当前有多少组资源 */
    __lotus_overlay_src_init_t      srcPicGroup[OVERLAY_MAX_SRC_GROUP_NUM]; /* 资源信息 */
    __lotus_overlay_dis_par_t       dispInfo[MAX_OVERLAY_BLOCK_NUM];        /* 显示信息 */
    __u8                    dispGroupIndex[MAX_OVERLAY_BLOCK_NUM];  /* 显示图片在资源中的index ，该block 在显示的图片的在资源的哪个group */

    overlay_block_header_t* pblkHdrTbl;                             /*block header表*/
    __u32                   blkHdrTblSize;
    __u8*                   ppalette;                               /*设置给寄存器的调色板信息地址*/
    __u32                   paletteBufSize;
    __u8*                   pdata;                                  /*设置给寄存器的数据地址*/
    __u32                   dataBufSize;
}overlay_info_t;

typedef struct DISPLAY_FRAME_INFO
{
    __s32                   isKeyFrame;         /* 0--current frame may be encoded as non-key frame, 
                                                 * 1--current frame must be encoded as key frame
                                                 */
    SIZE                    size;               /*  encoder frame width,是指frame buffer的宽高. 
                                                 *  一般是宏块宽度的整数倍，在编码时,一般frame_buffer
                                                 *  的宽高就是真正图像的宽高. 
                                                 */
    __u8                    *addr[3];           /* data buffer address */
    
    __pixel_yuvfmt_t        fmt;                /* data format，具体的参考drv_csi.h */
    __yuv_mod_t             store_mod;          /* 图像的格式 */
    __yuv_seq_t             yuvseq;             /* yuv的排列顺序  */
    __cs_mode_t             cs_mode;            /*color space */
    
    __s64                   pts;                /* unit:ms */
    __s32                   pts_valid;
    overlay_info_t*         poverlayInfo;
    __u8                    pic_mode;          //0 for frame ; 1 for field mode
}__dis_frame_info_t;
typedef struct VIDEO_ENCODE_FORMAT
{
    __u32       codec_type;         // video codec type, __herb_codec_t
    __s32       width;              //encoder frame width,最好是宏块宽度的整数倍
    __s32       height;             //encoder frame height,最好是宏块高度的整数倍
    __s32       srcwidth;              //encoder frame width,最好是宏块宽度的整数倍
    __s32       srcheight;             //encoder frame height,最好是宏块高度的整数倍

    __u32       frame_rate;         // frame rate, 数值放大1000倍。
    __s32       color_format;       //__pixel_rgbfmt_t或__pixel_yuvfmt_t
    __s32       color_space;
    __s16       qp_max;
    __s16       qp_min;
    __s32       avg_bit_rate;       // average bit rate
    __u8        pic_mode;        /* 	FRAME = 0,	TOP_FIELD = 1,	BOTTOM_FIELD =  2	*/
    
    __s32       maxKeyInterval;         //keyframe Interval，指2个关键帧之间的P帧的最大数量
    __s16		jpgenc_quality;			// jpeg encoder quality setting 	 good:92 mid:85 low 75
} __video_encode_format_t;
typedef struct CODED_FRAME_INFO
{
    __s32       node_id;  //表示编码库内部的coded frame数组的元素下标号.
    __u8        *pData0;
    __s32       uSize0;
    __u8        *pData1;
    __s32       uSize1;
    __s64       pts;
    __s8        pts_valid;
    __s32       uLen0;       //buffer中的有效数据的长度,audio使用，video使用uSize,因为一定会全部用完。
    __s32       uLen1;       //buffer中的有效数据的长度
    __u8        *privateData;
    __s32       privateDataLen;
    __u8        *pextData;
    __s32       extDataLen;
} __coded_frame_info_t;

typedef enum __VDRV_ENC_LIB_COMMAND
{
    VDRV_ENC_LIB_CMD_OPEN = 0x00,               /* 初始化, 分配内存，配置中断处理函数，初始化MACC等操作*/
        
    VDRV_ENC_LIB_CMD_CLOSE,                     /* 释放内存，卸载中断处理函数。*/
    
    VDRV_ENC_LIB_CMD_CHKSPT=0x10,               /* 检查编码格式是否支持，aux = VIDEO_XVID, ...,
                                                 * ret = EPDK_TRUE/EPDK_FALSE 
                                                 */
                                                 
    VDRV_ENC_LIB_CMD_SET_VBSINF,                /* set video bitstream information for encode，
                                                 * 主要是图片大小等规格, pbuffer = __video_encode_format_t*, 
                                                 * 调用者提供内存 
                                                 */
                                                 
    VDRV_ENC_LIB_CMD_SET_MODE,                  /* aux = __vdrv_enc_mode_t */
    
    VDRV_ENC_LIB_CMD_INIT,                      /* 初始化编码库的若干参数.*/
    VDRV_ENC_LIB_SET_OUT_BUFFER_LEN,            /* 设置解码buffer len*/
    VDRV_ENC_LIB_GET_OUT_BUFFER_LEN,            /* 获取解码buffer len*/
    
    VDRV_ENC_LIB_CMD_SET_PIC=0x20,              /* 设置待编码的图像给下级解码模块, 
                                                 * pbuffer = __dis_frame_info_t 
                                                 */
                                                 
    VDRV_ENC_LIB_CMD_ENCODE,                    /* 将设置的帧编码, 
                                                 * ret = VDRV_ENC_ERR_NONE,
                                                 *       VDRV_ENC_ERR_VE_BUSY,
                                                 *       VDRV_ENC_ERR_VBS_OVERFLOW 3种返回值
                                                 */
                                                 
    VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME,         /* 查询编码后的帧, 
                                                 * pbuffer = __coded_frame_info_t*
                                                 * ret = EPDK_OK表示成功, EPDK_FAIL表示申请失败, 
                                                 */
                                                 
    VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME,       /* release coded frame, 
                                                 * aux = node_id of __coded_frame_info_t, 
                                                 * ret = VDRV_ENC_ERR_NONE, VDRV_ENC_ERR_FAIL
                                                 */
                                                 
	VDRV_ENC_LIB_CMD_ENCODE_DIS_ME,
    VDRV_ENC_LIB_CMD_GET_PREVIEW_SUPPORT = 0x30,/* 预览输出支持的格式 pbuffer = (__venc_preview_info_t *) */
    VDRV_ENC_LIB_CMD_SET_PREVIEW         = 0x31,/* 设置预览输出格式 */
    VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD,            //

    VDRV_ENC_LIB_CMD_SET_MD_PARA,
    VDRV_ENC_LIB_CMD_GET_MD_DETECT,
    VDRV_ENC_LIB_CMD_NEW,                        /*新创建一个设备*/
    VDRV_ENC_LIB_CMD_DEL,                        /*删除一个设备*/
    VDRV_ENC_LIB_CMD_REG_SYNC_CB,
    VDRV_ENC_LIB_CMD_REPORT_STAT,

    
} __vdrv_enc_lib_command_t;

enum Pattern_in_mode{
	YUV420   = 0,
    YUV422,
    YUV420_MACROBLOCK,

    BAYER_BG_GR  =4,
    BAYER_RG_GB,
    BAYER_GB_RG,
    BAYER_GR_BG
};
typedef enum
{
    M_ENCODE = 0,             //编码器只输出编码后的帧数据
    M_ISP_THUMB,              //编码器只输出预览图像
    M_ENCODE_ISP_THUMB,       //编码器同时输出预览图像和编码数据
    M_UNSUPPORT,
} __venc_output_mod_t;


typedef struct __VIDEO_FRAME_INF
{
    __u16       width;          //the stored picture width for luma because of mapping
    __u16       height;         //the stored picture height for luma because of mapping
    __u16       frame_rate;     //the source picture frame rate
    __u16       eAspectRatio;   //the source picture aspect ratio
    __u16       color_format;   //the source picture color format

} __video_frame_inf_t;

//Rectangle
typedef struct Rect
{
    short       uStartX;    // Horizontal start point.
    short       uStartY;    // Vertical start point.
    short       uWidth;     // Horizontal size.
    short       uHeight;    // Vertical size.

} __vdrv_rect_t;
typedef struct Video_Info
{
    __u16       width;          //the stored picture width for luma because of mapping，宏块mapping时的图像宽度
    __u16       height;         //the stored picture height for luma because of mapping，宏块mapping时的图像高度
    __u16       frame_rate;     //the source picture frame rate, VAL*LOTUS_RATE_BASE
    __u16       eAspectRatio;   //the source picture aspect ratio，像素宽高比,数值乘以1000
    __u16       color_format;   //the source picture color format, __pixel_yuvfmt_t或__pixel_rgbfmt_t

} __video_info_t;

typedef struct PanScanInfo
{
    __u32 uNumberOfOffset;
    __s16 HorizontalOffsets[3];

} __pan_scan_info_t;

enum RepeatField
{
    REPEAT_FIELD_NONE,          //means no field should be repeated

    REPEAT_FIELD_TOP,           //means the top field should be repeated
    REPEAT_FIELD_BOTTOM,        //means the bottom field should be repeated

    REPEAT_FIELD_
};

typedef struct Display_Info
{
    __bool              bProgressiveSrc;    // Indicating the source is progressive or not
    __bool              bTopFieldFirst;     // VPO should check this flag when bProgressiveSrc is FALSE
    enum RepeatField    eRepeatField;       // only check it when frame rate is 24FPS and interlace output
    __video_info_t      pVideoInfo;         // a pointer to structure stored video information
    __pan_scan_info_t   pPanScanInfo;
    __vdrv_rect_t       src_rect;           // source valid size,宏块对齐, 也就是frame buf的宽高
    __vdrv_rect_t       dst_rect;           // source display size,真正的图像的宽高.
    __u8                top_index;          // frame buffer index containing the top field，帧编号
    __u32               top_y;              // the address of frame buffer, which contains top field luminance
    __u32               top_c;              // the address of frame buffer, which contains top field chrominance
    __u32               top_c2;             // 如果是yuv分开存放，top_c存放u，top_c2存放v.

    //the following is just for future
    __u8                bottom_index;       // frame buffer index containing the bottom field
    __u32               bottom_y;           // the address of frame buffer, which contains bottom field luminance
    __u32               bottom_c;           // the address of frame buffer, which contains bottom field chrominance
    __u32               bottom_c2;          // 如果是yuv分开存放，bottom_c存放u，bottom_c2存放v.
    
    //time stamp of the frame
    __s64               uPts;               // time stamp of the frame,   单位:us
    __s32               bPtsValid;

    //DE_DRV config information
    __fb_format_t       fb_format;         //根据DE_drv定义的数据格式填写该变量
} __display_info_t;

typedef struct
{
    __s32 frm_id;
    __s32 hor_factor;
    __s32 ver_factor;
    __display_info_t *ppreview_frm_info;
}__venc_preview_frm_t;


#define	 TEST_IN_SDCARD_ENABLE	1
#if TEST_IN_SDCARD_ENABLE
#define  TEST_SOURCE_PATH	"f:\\AW1663_test\\jpeg_test\\"
#else
#define  TEST_SOURCE_PATH	"D:\\AW1663_test\\jpeg_test\\"
#endif


int jpeg_enc_test(void)
{
	__s32 id;
	__s32 node_id;
	__s32 mid_lotus_id ;
	__mp* pDev;
	__u32 i; 
	__dis_frame_info_t   pic_info;  //frame info
	__video_encode_format_t video_info; // init info
	__coded_frame_info_t output_buffer; // output buffer
	__venc_preview_frm_t preview_frame = {0};
	__display_info_t preview_frm_info = {0};
	//__u32 dstwidth = 1920;
	//__u32 dstheight = 1080;
	__u32 dstwidth = 2560;
	__u32 dstheight = 1920;
	//__u32 dstwidth = 640;
	//__u32 dstheight = 480;
	__u32 srcwidth = 640;
	__u32 srcheight = 480;
	//__u32 tmp_dstwidth = 640*3;
	//__u32 tmp_dstheight = 480*3;

	void* ptmpbuffer;
	ES_FILE *input_file_y,*input_file_c;
	ES_FILE* disphd ;
	__u32 arg[3] ;
	__u32 layer_hdl ;
	ES_FILE *output_file_y,*output_file_c, *out_file;

	__disp_layer_info_t layer_para = {0};
	__u32 address_y , address_c  ;
 	__u8    end_of_file[2] = {0xff, 0xd9};

#if 0
	{
		__s32 debug = 0x55 ;
		while(debug==0x55)
			;
	}
#endif
	preview_frame.hor_factor = 1 ;
	preview_frame.ppreview_frm_info = &preview_frm_info ;
	preview_frame.ppreview_frm_info->top_y = (__u32)esMEMS_Palloc((dstwidth*dstheight+1023)>>10,0);;
	preview_frame.ppreview_frm_info->top_c = (__u32)esMEMS_Palloc((dstwidth*dstheight+1023)>>10,0);;


	address_y = (__u32)esMEMS_Palloc((dstwidth*dstheight+1023)>>10,0);
	address_c = (__u32)esMEMS_Palloc((dstwidth*dstheight+1023)>>10,0);
	
	eLIBs_memset(&video_info, 0, sizeof(__video_encode_format_t));
	eLIBs_memset(&pic_info, 0, sizeof(__dis_frame_info_t));
	eLIBs_memset(&output_buffer, 0, sizeof(__coded_frame_info_t));

	disphd = eLIBs_fopen("b:\\DISP\\DISPLAY", "r+");
    if(!disphd)
    {
        __wrn("open display device fail!\n");
        return EPDK_FAIL;
    }
	srcwidth = 1920;
	srcheight = 1080;
	input_file_y = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080uvcombine420-y.bin", "rb");
	input_file_c = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080uvcombine420-uv.bin", "rb");

	if(input_file_y)
	{
		eLIBs_fread((void*)address_y, 1, srcwidth*srcheight, input_file_y);
		eLIBs_fclose(input_file_y);
		input_file_y = NULL ;
	}

	if(input_file_c)
	{
		eLIBs_fread((void*)address_c, 1, srcwidth*srcheight/2, input_file_c);
		eLIBs_fclose(input_file_c);
		input_file_c = NULL ;
	}

  	pic_info.isKeyFrame = 1;
	pic_info.size.width = srcwidth;
  	pic_info.size.height = srcheight;
 	pic_info.addr[0] = (__u8 *)address_y;
 	pic_info.addr[1] = (__u8 *)address_c;


    pic_info.fmt = PIXEL_YUV420;
    pic_info.store_mod = YUV_MOD_UV_NON_MB_COMBINED;
    pic_info.yuvseq = YUV_SEQ_UVUV;
    pic_info.cs_mode = BT601;
    pic_info.pts_valid = 0;
    pic_info.pic_mode = 0;

	video_info.codec_type = SW_CODEC_TYPE_MJPEG ;
	video_info.width = dstwidth;
	video_info.height = dstheight;

	video_info.srcwidth = srcwidth;
	video_info.srcheight = srcheight;
	video_info.frame_rate = 30 *1000;
	video_info.color_format = YUV420;
	video_info.color_space = BT601;
	video_info.qp_max = 40;
	video_info.qp_min = 20;
	video_info.avg_bit_rate = 3*1024*1024;
	video_info.pic_mode = 0;
	video_info.maxKeyInterval = 25;
	video_info.jpgenc_quality = 90;

	mid_lotus_id = esMODS_MInstall(TEST_SOURCE_PATH"\\venc_mjp.drv", 0);
	if(mid_lotus_id)
	{
		__msg("succeed install venc_mjp.drv  mid_lotus_id=%d..........\n",mid_lotus_id);
		pDev = esMODS_MOpen(mid_lotus_id,0);
		__msg("succeed install venc_mjp.drv  pDev=%x..........\n",pDev);
	}

	id = esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_NEW, 0, 0);
	__msg("id = %d\n",id);

	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_SET_OUT_BUFFER_LEN, id, (void *)(8*1024*1024));
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_INIT, id, 0);
	__here__;
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_VBSINF, (__s32)id, &video_info);

	__here__;
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_OPEN, id, 0);
	__here__;

	// test M_ISP_THUMB mode 
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ISP_THUMB);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
#if TEST_IN_SDCARD_ENABLE
	output_file_y = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080preview_y1.bin", "wb");
	output_file_c = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080preview-uv1.bin", "wb");

	__msg("preview_frame.ppreview_frm_info->dst_rect.uWidth=%d %d\n",preview_frame.ppreview_frm_info->dst_rect.uWidth,preview_frame.ppreview_frm_info->dst_rect.uHeight);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_y, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight, output_file_y);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_c, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight/2, output_file_c);
	eLIBs_fclose(output_file_y);
	eLIBs_fclose(output_file_c);

#else
	//request layer
    arg[0] = DISP_LAYER_WORK_MODE_SCALER;
    arg[1] = 0;
    arg[2] = 0;
    layer_hdl = eLIBs_fioctrl(disphd, DISP_CMD_LAYER_REQUEST, 0, (void*)arg);
    if(layer_hdl == NULL)
    {
        __wrn("request layer0 fail\n");
    }

	arg[0] = layer_hdl;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(disphd,DISP_CMD_LAYER_OPEN,0,(void*)arg);
    eLIBs_fioctrl(disphd,DISP_CMD_LAYER_TOP,0,(void*)arg);

	layer_para.scn_win.width    = eLIBs_fioctrl(disphd, DISP_CMD_SCN_GET_WIDTH, 0, NULL);
    layer_para.scn_win.height   = eLIBs_fioctrl(disphd, DISP_CMD_SCN_GET_HEIGHT, 0, NULL);

	layer_para.fb.addr[0]       = preview_frame.ppreview_frm_info->top_y;
    layer_para.fb.addr[1]       = preview_frame.ppreview_frm_info->top_c;
    layer_para.fb.addr[2]       = NULL;
    layer_para.fb.size.width    = preview_frame.ppreview_frm_info->dst_rect.uWidth;
    layer_para.fb.size.height   = preview_frame.ppreview_frm_info->dst_rect.uHeight;
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
    layer_para.src_win.width    = preview_frame.ppreview_frm_info->src_rect.uWidth;
    layer_para.src_win.height   = preview_frame.ppreview_frm_info->src_rect.uHeight;
    layer_para.scn_win.x        = 0;
    layer_para.scn_win.y        = 0;

    layer_para.mode = DISP_LAYER_WORK_MODE_SCALER;
    layer_para.pipe = 0;
    arg[0] = layer_hdl;
    arg[1] = (__u32)&layer_para;
    arg[2] = 0;
    eLIBs_fioctrl(disphd,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME, id, &output_buffer);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME, id, (void*)output_buffer.node_id);

#endif
	#if 1
	// test M_ENCODE mode
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ENCODE);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME, id, &output_buffer);

	out_file = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080yuv420.jpg", "wb");

	__msg("output_buffer.uSize0=%d output_buffer.pData0=%x\n",output_buffer.uSize0,output_buffer.pData0);
	eLIBs_fwrite(output_buffer.pData0, 1, output_buffer.uSize0, out_file);

	if(output_buffer.uSize1) {
		eLIBs_fwrite(output_buffer.pData1, 1, output_buffer.uSize1, out_file);
	}
	eLIBs_fclose(out_file);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME, id, (void*)output_buffer.node_id);
	#endif
	
	#if 1
	// test M_ENCODE_ISP_THUMB  mode
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ENCODE_ISP_THUMB);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME, id, &output_buffer);

	out_file = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080yuv2.jpg", "wb");
	output_file_y = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080preview_y2.bin", "wb");
	output_file_c = eLIBs_fopen(TEST_SOURCE_PATH"\\1920_1080preview-uv2.bin", "wb");

	__log("preview_frame.ppreview_frm_info->dst_rect.uWidth=%d %d\n",preview_frame.ppreview_frm_info->dst_rect.uWidth,preview_frame.ppreview_frm_info->dst_rect.uHeight);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_y, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight, output_file_y);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_c, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight/2, output_file_c);
	__log("output_buffer.uSize0=%d output_buffer.uSize1=%d\n",output_buffer.uSize0,output_buffer.uSize1);
	eLIBs_fwrite(output_buffer.pData0, 1, output_buffer.uSize0, out_file);

	if(output_buffer.uSize1) {
		eLIBs_fwrite(output_buffer.pData1, 1, output_buffer.uSize1, out_file);
	}
	eLIBs_fclose(out_file);
	eLIBs_fclose(output_file_y);
	eLIBs_fclose(output_file_c);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME, id, (void*)output_buffer.node_id);
	#endif

	srcwidth = 1280;
	srcheight = 720;
	input_file_y = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_720uvcombine420-y.bin", "rb");
	input_file_c = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_720uvcombine420-uv.bin", "rb");

	if(input_file_y)
	{
		eLIBs_fread((void*)address_y, 1, srcwidth*srcheight, input_file_y);
		eLIBs_fclose(input_file_y);
		input_file_y = NULL ;
	}

	if(input_file_c)
	{
		eLIBs_fread((void*)address_c, 1, srcwidth*srcheight/2, input_file_c);
		eLIBs_fclose(input_file_c);
		input_file_c = NULL ;
	}

  	pic_info.isKeyFrame = 1;
	pic_info.size.width = srcwidth;
  	pic_info.size.height = srcheight;
 	pic_info.addr[0] = (__u8 *)address_y;
 	pic_info.addr[1] = (__u8 *)address_c;


    pic_info.fmt = PIXEL_YUV420;
    pic_info.store_mod = YUV_MOD_UV_NON_MB_COMBINED;
    pic_info.yuvseq = YUV_SEQ_UVUV;
    pic_info.cs_mode = BT601;
    pic_info.pts_valid = 0;
    pic_info.pic_mode = 0;

	video_info.codec_type = SW_CODEC_TYPE_MJPEG ;
	video_info.width = dstwidth;
	video_info.height = dstheight;

	video_info.srcwidth = srcwidth;
	video_info.srcheight = srcheight;
	video_info.frame_rate = 30 *1000;
	video_info.color_format = YUV420;
	video_info.color_space = BT601;
	video_info.qp_max = 40;
	video_info.qp_min = 20;
	video_info.avg_bit_rate = 3*1024*1024;
	video_info.pic_mode = 0;
	video_info.maxKeyInterval = 25;
	video_info.jpgenc_quality = 25;

	__here__;
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_VBSINF, (__s32)id, &video_info);

	#if 1
	// test M_ISP_THUMB mode 
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ISP_THUMB);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);

	output_file_y = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_720preview_y1.bin", "wb");
	output_file_c = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_7200preview-uv1.bin", "wb");

	__msg("preview_frame.ppreview_frm_info->dst_rect.uWidth=%d %d\n",preview_frame.ppreview_frm_info->dst_rect.uWidth,preview_frame.ppreview_frm_info->dst_rect.uHeight);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_y, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight, output_file_y);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_c, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight/2, output_file_c);
	eLIBs_fclose(output_file_y);
	eLIBs_fclose(output_file_c);

	#endif

	#if 1
	// test M_ENCODE mode
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ENCODE);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME, id, &output_buffer);

	out_file = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_720yuv420.jpg", "wb");

	__msg("output_buffer.uSize0=%d output_buffer.uSize1=%d\n",output_buffer.uSize0,output_buffer.uSize1);
	eLIBs_fwrite(output_buffer.pData0, 1, output_buffer.uSize0, out_file);

	if(output_buffer.uSize1) {
		eLIBs_fwrite(output_buffer.pData1, 1, output_buffer.uSize1, out_file);
	}
	eLIBs_fclose(out_file);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME, id, (void*)output_buffer.node_id);
	#endif
	
	#if 1
	// test M_ENCODE_ISP_THUMB  mode
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ENCODE_ISP_THUMB);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME, id, &output_buffer);

	out_file = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_720yuv2.jpg", "wb");
	output_file_y = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_720preview_y2.bin", "wb");
	output_file_c = eLIBs_fopen(TEST_SOURCE_PATH"\\1280_720preview-uv2.bin", "wb");

	__log("preview_frame.ppreview_frm_info->dst_rect.uWidth=%d %d\n",preview_frame.ppreview_frm_info->dst_rect.uWidth,preview_frame.ppreview_frm_info->dst_rect.uHeight);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_y, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight, output_file_y);
	eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_c, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight/2, output_file_c);
	__log("output_buffer.uSize0=%d output_buffer.uSize1=%d\n",output_buffer.uSize0,output_buffer.uSize1);
	eLIBs_fwrite(output_buffer.pData0, 1, output_buffer.uSize0, out_file);

	if(output_buffer.uSize1) {
		eLIBs_fwrite(output_buffer.pData1, 1, output_buffer.uSize1, out_file);
	}
	eLIBs_fclose(out_file);
	eLIBs_fclose(output_file_y);
	eLIBs_fclose(output_file_c);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME, id, (void*)output_buffer.node_id);
	#endif

	#if 0
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ENCODE);
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
	for(i=0;i<10;i++)
	{
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME, id, &output_buffer);
		// debug preview frame data
		if(1)
		{
			ES_FILE *out_file;
			static __u32 index = 0;
			char file_name[32] ;
			eLIBs_sprintf(file_name,"f:\\mjpeg_test\\yuv420encode%d.jpg",index);
			out_file = eLIBs_fopen(file_name, "wb");
			index++;
			__msg("file_nam=%s\n",file_name);
			__msg("uSize0=%d uSize1=%d\n",output_buffer.uSize0,output_buffer.uSize1);
			eLIBs_fwrite(output_buffer.pData0, 1, output_buffer.uSize0, out_file);

			if(output_buffer.uSize1) {
				eLIBs_fwrite(output_buffer.pData1, 1, output_buffer.uSize1, out_file);
			}
			eLIBs_fclose(out_file);
		}

		//esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME, id, (void*)output_buffer.node_id);

	}
	
	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_OUTPUT_MOD, (__s32)id, (void *)M_ENCODE_ISP_THUMB);

	__here__;

	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_SET_PIC, id, &pic_info);

	//request layer
    arg[0] = DISP_LAYER_WORK_MODE_SCALER;
    arg[1] = 0;
    arg[2] = 0;
    layer_hdl = eLIBs_fioctrl(disphd, DISP_CMD_LAYER_REQUEST, 0, (void*)arg);
    if(layer_hdl == NULL)
    {
        __wrn("request layer0 fail\n");
    }

	arg[0] = layer_hdl;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(disphd,DISP_CMD_LAYER_OPEN,0,(void*)arg);

	arg[0] = layer_hdl;
    arg[1] = 0;
    arg[2] = 0;
    eLIBs_fioctrl(disphd,DISP_CMD_LAYER_TOP,0,(void*)arg);

	layer_para.scn_win.width    = eLIBs_fioctrl(disphd, DISP_CMD_SCN_GET_WIDTH, 0, NULL);
    layer_para.scn_win.height   = eLIBs_fioctrl(disphd, DISP_CMD_SCN_GET_HEIGHT, 0, NULL);

	//while(1)
	{
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_ENCODE, id, &preview_frame);

		layer_para.fb.addr[0]       = preview_frame.ppreview_frm_info->top_y;
	    layer_para.fb.addr[1]       = preview_frame.ppreview_frm_info->top_c;
	    layer_para.fb.addr[2]       = NULL;
	    layer_para.fb.size.width    = preview_frame.ppreview_frm_info->dst_rect.uWidth;
	    layer_para.fb.size.height   = preview_frame.ppreview_frm_info->dst_rect.uHeight;
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
	    layer_para.src_win.width    = preview_frame.ppreview_frm_info->src_rect.uWidth;
	    layer_para.src_win.height   = preview_frame.ppreview_frm_info->src_rect.uHeight;
	    layer_para.scn_win.x        = 0;
	    layer_para.scn_win.y        = 0;

        layer_para.mode = DISP_LAYER_WORK_MODE_SCALER;
        layer_para.pipe = 0;
        arg[0] = layer_hdl;
        arg[1] = (__u32)&layer_para;
        arg[2] = 0;
        eLIBs_fioctrl(disphd,DISP_CMD_LAYER_SET_PARA,0,(void*)arg);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_QUERY_CODED_FRAME, id, &output_buffer);
		esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_RELEASE_CODED_FRAME, id, (void*)output_buffer.node_id);

		// debug preview frame data
		if(1)
		{
			ES_FILE *output_file_y,*output_file_c, *out_file;

			out_file = eLIBs_fopen("f:\\yuv420encode.jpg", "wb");
			output_file_y = eLIBs_fopen("f:\\yuv420uvcombine_preview_y.bin", "wb");
			output_file_c = eLIBs_fopen("f:\\yuv420uvcombine_preview-uv.bin", "wb");

			__msg("preview_frame.ppreview_frm_info->dst_rect.uWidth=%d %d\n",preview_frame.ppreview_frm_info->dst_rect.uWidth,preview_frame.ppreview_frm_info->dst_rect.uHeight);
			//eLIBs_FlushDCacheRegion((void*)preview_frame.ppreview_frm_info->top_y,preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight);
			//eLIBs_FlushDCacheRegion((void*)preview_frame.ppreview_frm_info->top_c,preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight/2);
			//eLIBs_FlushDCacheRegion(esMEMS_VA2PA((void*)preview_frame.ppreview_frm_info->top_y),preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight);
			//eLIBs_FlushDCacheRegion(esMEMS_VA2PA((void*)preview_frame.ppreview_frm_info->top_c),preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight/2);
			eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_y, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight, output_file_y);
			eLIBs_fwrite((void*)preview_frame.ppreview_frm_info->top_c, 1, preview_frame.ppreview_frm_info->dst_rect.uWidth*preview_frame.ppreview_frm_info->dst_rect.uHeight/2, output_file_c);
			__msg("output_buffer.uSize0=%d output_buffer.uSize1=%d\n",output_buffer.uSize0,output_buffer.uSize1);
			eLIBs_fwrite(output_buffer.pData0, 1, output_buffer.uSize0, out_file);

			if(output_buffer.uSize1) {
				eLIBs_fwrite(output_buffer.pData1, 1, output_buffer.uSize1, out_file);
			}
			eLIBs_fclose(out_file);
			eLIBs_fclose(output_file_y);
			eLIBs_fclose(output_file_c);
		}

	}
	#endif

	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_CLOSE, id, 0);

	esMODS_MIoctrl(pDev, VDRV_ENC_LIB_CMD_DEL, id, 0);

	esMODS_MClose(pDev);

	esMODS_MUninstall(mid_lotus_id);
	
}


