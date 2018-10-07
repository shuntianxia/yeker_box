/*
*********************************************************************************************************
*                                                    MELIS
*                                    the Easy Portable/Player Develop Kits
*                                               Desktop Module
*
*                                    (c) Copyright 2006-2010, kevin.z China
*                                             All Rights Reserved
*
* File    : desktop_api.h
* By      : kevin.z
* Version : v1.0
* Date    : 2010-9-14 19:13
* Descript:
* Update  : date                auther      ver     notes
*********************************************************************************************************
*/
#ifndef __MOD_DESKTOP_API_H__
#define __MOD_DESKTOP_API_H__

typedef struct DESKTOP_API_ENTRY
{		
	//================================================================================//
	//========											reg											============//
	//================================================================================//

	//==========================			system			================================//
	//system\reg_audio_output.h
	__lion_audio_output_type_e  (*ptr_lion_reg_audio_output_get)			( void );
	__s32 						(*ptr_lion_reg_audio_output_set)			( __lion_audio_output_type_e audio_output );
	__lion_audio_output_type_e  (*ptr_lion_reg_audio_output_get_default)	( void );
	__s32 						(*ptr_lion_reg_audio_output_restore)		( void );
	void 						(*ptr_lion_reg_audio_output_reserved[5])	( void );
	//system\reg_auto_off.h
	__u32  						(*ptr_lion_reg_auto_off_get)				( void );
	__s32 						(*ptr_lion_reg_auto_off_set)				( __u32 time );
	__u32  						(*ptr_lion_reg_auto_off_get_default)		( void );
	__s32 						(*ptr_lion_reg_auto_off_restore)			( void );
	void 						(*ptr_lion_reg_auto_off_reserved[5])		( void );
	//system\reg_channel.h
	__audio_dev_chan_mode_t  	(*ptr_lion_reg_channel_get)					( void );
	__s32 						(*ptr_lion_reg_channel_set)					( __audio_dev_chan_mode_t channel );
	__audio_dev_chan_mode_t  	(*ptr_lion_reg_channel_get_default)			( void );
	__s32 						(*ptr_lion_reg_channel_restore)				( void );
	void 						(*ptr_lion_reg_channel_reserved[5])			( void );
	//system\reg_charset.h
	__epdk_charset_enm_e  		(*ptr_lion_reg_charset_get)					( void );
	__s32 						(*ptr_lion_reg_charset_set)					( __epdk_charset_enm_e charset );
	__epdk_charset_enm_e  		(*ptr_lion_reg_charset_get_default)			( void );
	__s32 						(*ptr_lion_reg_charset_restore)				( void );
	void 						(*ptr_lion_reg_charset_reserved[5])			( void );
	//system\reg_close_scn.h
	__u32  						(*ptr_lion_reg_close_scn_get)				( void );
	__s32 						(*ptr_lion_reg_close_scn_set)				( __u32 time );
	__u32  						(*ptr_lion_reg_close_scn_get_default)		( void );
	__s32 						(*ptr_lion_reg_close_scn_restore)			( void );
	void 						(*ptr_lion_reg_close_scn_reserved[5])		( void );
	//system\reg_detect_gate.h
	__bool  					(*ptr_lion_reg_detect_gate_get)				( void );
	__s32 						(*ptr_lion_reg_detect_gate_set)				( __bool detect_gate );
	__bool  					(*ptr_lion_reg_detect_gate_get_default)		( void );
	__s32 						(*ptr_lion_reg_detect_gate_restore)			( void );
	void 						(*ptr_lion_reg_detect_gate_reserved[5])		( void );
	//system\reg_display.h
	__lion_disp_format_e  		(*ptr_lion_reg_display_get)					( void );
	__s32 						(*ptr_lion_reg_display_set)					( __lion_disp_format_e display );
	__lion_disp_format_e  		(*ptr_lion_reg_display_get_default)			( void );
	__s32 						(*ptr_lion_reg_display_restore)				( void );
	void 						(*ptr_lion_reg_display_reserved[5])			( void );
	//system\reg_gamma.h
	__u32  						(*ptr_lion_reg_gamma_get)					( void );
	__s32 						(*ptr_lion_reg_gamma_set)					( __u32 value );
	__u32  						(*ptr_lion_reg_gamma_get_default)			( void );
	__s32 						(*ptr_lion_reg_gamma_restore)				( void );
	void 						(*ptr_lion_reg_gamma_reserved[5])			( void );
	//system\reg_keytone_gate.h
	__bool  					(*ptr_lion_reg_keytone_gate_get)			( void );
	__s32 						(*ptr_lion_reg_keytone_gate_set)			( __bool keytone_gate );
	__bool  					(*ptr_lion_reg_keytone_gate_get_default)	( void );
	__s32 						(*ptr_lion_reg_keytone_gate_restore)		( void );
	void 						(*ptr_lion_reg_keytone_gate_reserved[5])	( void );
	//system\reg_language.h
	__epdk_language_enm_e  		(*ptr_lion_reg_language_get)				( void );
	__s32 						(*ptr_lion_reg_language_set)				( __epdk_language_enm_e language );
	__epdk_language_enm_e  		(*ptr_lion_reg_language_get_default)		( void );
	__s32 						(*ptr_lion_reg_language_restore)			( void );
	void 						(*ptr_lion_reg_language_reserved[5])		( void );	
	//system\reg_volume.h
	__u32  						(*ptr_lion_reg_volume_get)					( void );
	__s32 						(*ptr_lion_reg_volume_set)					( __u32 volume );
	__u32  						(*ptr_lion_reg_volume_get_default)			( void );
	__s32 						(*ptr_lion_reg_volume_restore)				( void );
	void 						(*ptr_lion_reg_volume_reserved[5])			( void );
	//system\reg_app_restore.h
	__bool						(*ptr_lion_get_app_restore_flag)			( lion_app_e app );
	__s32						(*ptr_lion_set_app_restore_flag)			( lion_app_e app, __bool flag );
	__s32						(*ptr_lion_set_all_app_restore)				( void );
	//system\reg_lcd_bright.h
	__lion_bright_t 			(*ptr_lion_reg_lcd_bright_get)				(void);
	__s32 						(*ptr_lion_reg_lcd_bright_set)				(__lion_bright_t bright);
	__lion_bright_t 			(*ptr_lion_reg_lcd_bright_get_default)		(void);
	__s32 						(*ptr_lion_reg_lcd_bright_restore)			( void );
	//system\reg_lcd_contrast.h
	__u32			 			(*ptr_lion_reg_lcd_contrast_get)			(void);
	__s32 						(*ptr_lion_reg_lcd_contrast_set)			(__u32 value);
	__u32			 			(*ptr_lion_reg_lcd_contrast_get_default)	(void);
	__s32 						(*ptr_lion_reg_lcd_contrast_restore)		( void );
	//system\reg_lcd_saturation.h
	__u32			 			(*ptr_lion_reg_lcd_saturation_get)			(void);
	__s32 						(*ptr_lion_reg_lcd_saturation_set)			(__u32 value);
	__u32			 			(*ptr_lion_reg_lcd_saturation_get_default)	(void);
	__s32 						(*ptr_lion_reg_lcd_saturation_restore)		( void );
	//system\reg_tp_adjust.h
	__u32 						(*ptr_lion_tp_adjust_flag_get)				(void);
	__s32 						(*ptr_lion_tp_adjust_flag_set)				(__u32 flag);
	//==========================			movie			================================//
	//movie\reg_movie_repeat.h
	orc_play_mode_e  			(*ptr_lion_reg_movie_repeat_get)			( void );
	__s32 						(*ptr_lion_reg_movie_repeat_set)			( orc_play_mode_e repeat );
	orc_play_mode_e  			(*ptr_lion_reg_movie_repeat_get_default)	( void );
	__s32 						(*ptr_lion_reg_movie_repeat_restore)		( void );
	//movie\reg_zoom.h
	robin_zoom_e  				(*ptr_lion_reg_zoom_get)					( void );
	__s32 						(*ptr_lion_reg_zoom_set)					( robin_zoom_e zoom );
	robin_zoom_e  				(*ptr_lion_reg_zoom_get_default)			( void );
	__s32 						(*ptr_lion_reg_zoom_restore)				( void );
	//movie\reg_sub_size.h
	__u32  						(*ptr_lion_reg_sub_size_get)				( void );
	__s32 						(*ptr_lion_reg_sub_size_set)				( __u32 value );
	__u32  						(*ptr_lion_reg_sub_size_get_default)		( void );
	__s32 						(*ptr_lion_reg_sub_size_restore)			( void );
	//movie\reg_sub_gate.h
	__bool  					(*ptr_lion_reg_sub_gate_get)				( void );
	__s32   					(*ptr_lion_reg_sub_gate_set)				( __bool sub_gate );
	__bool    					(*ptr_lion_reg_sub_gate_get_default)		( void );
	__s32   					(*ptr_lion_reg_sub_gate_restore)			( void );
	//movie\reg_sub_frame_color.h
	__u32   					(*ptr_lion_reg_sub_frame_color_get)			( void );
	__s32    					(*ptr_lion_reg_sub_frame_color_set)			( __u32 sub_frame_color );
	__u32     					(*ptr_lion_reg_sub_frame_color_get_default)	( void );
	__s32    					(*ptr_lion_reg_sub_frame_color_restore)		( void );
	//movie\reg_sub_delay.h
	__s32      					(*ptr_lion_reg_sub_delay_get)				( void );
	__s32     					(*ptr_lion_reg_sub_delay_set)				( __s32 time );
	__s32      					(*ptr_lion_reg_sub_delay_get_default)		( void );
	__s32     					(*ptr_lion_reg_sub_delay_restore)			( void );
	//movie\reg_sub_color.h
	__u32       				(*ptr_lion_reg_sub_color_get)				( void );
	__s32      					(*ptr_lion_reg_sub_color_set)				( __u32 sub_color );
	__u32       				(*ptr_lion_reg_sub_color_get_default)		( void );
	__s32      					(*ptr_lion_reg_sub_color_restore)			( void );
	//movie\reg_sub_charset.h
	__epdk_charset_enm_e  		(*ptr_lion_reg_sub_charset_get)				( void );
	__s32 						(*ptr_lion_reg_sub_charset_set)				( __epdk_charset_enm_e sub_charset );
	__epdk_charset_enm_e  		(*ptr_lion_reg_sub_charset_get_default)		( void );
	__s32 						(*ptr_lion_reg_sub_charset_restore)			( void );
	__u32   					(*ptr_lion_reg_sub_position_get)			( void );
	__s32 						(*ptr_lion_reg_sub_position_set)			( __u32 value );
	__u32 						(*ptr_lion_reg_sub_position_get_default)	( void );
	__s32 						(*ptr_lion_reg_sub_position_restore)		( void );
	//movie\reg_bright.h
	__u32 						(*ptr_lion_reg_bright_get)					(void);
	__s32 						(*ptr_lion_reg_bright_set)					(__u32 bright);
	__u32 						(*ptr_lion_reg_bright_get_default)			(void);
	__s32 						(*ptr_lion_reg_bright_restore)				( void );
	//movie\reg_color_temperature.h
	__u32 						(*ptr_lion_reg_color_temperature_get)		( void );
	__s32 						(*ptr_lion_reg_color_temperature_set)		( __u32 value );
	__u32 						(*ptr_lion_reg_color_temperature_get_default)( void );
	__s32 						(*ptr_lion_reg_color_temperature_restore)	( void );
	//movie\reg_contrast.h
	__u32 						(*ptr_lion_reg_contrast_get)				( void );
	__s32 						(*ptr_lion_reg_contrast_set)				( __u32 value );
	__u32 						(*ptr_lion_reg_contrast_get_default)		( void );
	__s32 						(*ptr_lion_reg_contrast_restore)			( void );
	//movie\reg_saturation.h
	__u32 						(*ptr_lion_reg_saturation_get)				( void );
	__s32 						(*ptr_lion_reg_saturation_set)				( __u32 value );
	__u32 						(*ptr_lion_reg_saturation_get_default)		( void );
	__s32 						(*ptr_lion_reg_saturation_restore)			( void );
	//==========================			music			================================//
	//music\reg_music_repeat.h
	orc_play_mode_e  			(*ptr_lion_reg_music_repeat_get)			( void );
	__s32   					(*ptr_lion_reg_music_repeat_set)			( orc_play_mode_e repeat );
	orc_play_mode_e  			(*ptr_lion_reg_music_repeat_get_default)	( void );
	__s32   					(*ptr_lion_reg_music_repeat_restore)		( void );	
	//music\reg_eq.h
	__cedar_audio_eq_t			(*ptr_lion_reg_eq_get)( void );
	__s32   					(*ptr_lion_reg_eq_set)( __cedar_audio_eq_t eq );
	__cedar_audio_eq_t			(*ptr_lion_reg_eq_get_default)( void );
	__s32   					(*ptr_lion_reg_eq_restore)( void );
	//music\reg_lyric_mode.h
	__txtbox_mode_t  			(*ptr_lion_reg_lyric_mode_get)				( void );
	__s32 						(*ptr_lion_reg_lyric_mode_set)				( __txtbox_mode_t lyric_mode );
	__txtbox_mode_t  			(*ptr_lion_reg_lyric_mode_get_default)		( void );
	__s32 						(*ptr_lion_reg_lyric_mode_restore)			( void );
	//music\reg_lyric_focus_color.h
	__u32   					(*ptr_lion_reg_lyric_focus_color_get)		( void );
	__s32  						(*ptr_lion_reg_lyric_focus_color_set)		( __u32 lyric_focus_color );
	__u32   					(*ptr_lion_reg_lyric_focus_color_get_default)( void );
	__s32  						(*ptr_lion_reg_lyric_focus_color_restore)	( void );
	//music\reg_lyric_unfocus_color.h
	__u32   					(*ptr_lion_reg_lyric_unfocus_color_get)		( void );
	__s32  						(*ptr_lion_reg_lyric_unfocus_color_set)		( __u32 lyric_unfocus_color );
	__u32   					(*ptr_lion_reg_lyric_unfocus_color_get_default)( void );
	__s32  						(*ptr_lion_reg_lyric_unfocus_color_restore)	( void );	
	__u32  						(*ptr_lion_reg_backgrd_get)					( void );
	__s32  						(*ptr_lion_reg_backgrd_set)					( __u32 value );
	__u32  						(*ptr_lion_reg_backgrd_get_default)			( void );
	__s32  						(*ptr_lion_reg_backgrd_restore)				( void );
	//music\reg_album.h
	__u32  						(*ptr_lion_reg_album_get)					( void );
	__s32  						(*ptr_lion_reg_album_set)					( __u32 value );
	__u32  						(*ptr_lion_reg_album_get_default)			( void );
	__s32  						(*ptr_lion_reg_album_restore)				( void );

	//==========================			photo			================================//
	//photo\reg_switch_mode.h
	__willow_switch_mode_t  	(*ptr_lion_reg_switch_mode_get)				( void );
	__s32 						(*ptr_lion_reg_switch_mode_set)				( __willow_switch_mode_t switch_mode );
	__willow_switch_mode_t  	(*ptr_lion_reg_switch_mode_get_default)		( void );
	__s32 						(*ptr_lion_reg_switch_mode_restore)			( void );
	//photo\reg_switch_time.h
	__s32   					(*ptr_lion_reg_switch_time_get)				( void );
	__s32  						(*ptr_lion_reg_switch_time_set)				( __s32 time );
	__s32   					(*ptr_lion_reg_switch_time_get_default)		( void );
	__s32  						(*ptr_lion_reg_switch_time_restore)			( void );
	//photo\reg_photo_repeat.h
	__anole_playorder_t  		(*ptr_lion_reg_photo_repeat_get)			( void );
	__s32 						(*ptr_lion_reg_photo_repeat_set)			( __anole_playorder_t repeat );
	__anole_playorder_t  		(*ptr_lion_reg_photo_repeat_get_default)	( void );
	__s32 						(*ptr_lion_reg_photo_repeat_restore)		( void );
	//explorer\reg_exp_sort.h
	cat_sort_type_t 			(*ptr_lion_reg_sort_mode_get)				( void );
	__s32 						(*ptr_lion_reg_sort_mode_set)				( cat_sort_type_t mode );
	cat_sort_type_t 			(*ptr_lion_reg_sort_mode_get_default)		( void );
	__s32 						(*ptr_lion_reg_sort_mode_restore)			( void );
	//ebook\reg_ebook_backgrd.h
	__u32  						(*ptr_lion_reg_ebook_backgrd_get)			( void );
	__s32 						(*ptr_lion_reg_ebook_backgrd_set)			( __u32 value );
	__u32 						(*ptr_lion_reg_ebook_backgrd_get_default)	( void );
	__s32 						(*ptr_lion_reg_ebook_backgrd_restore)		( void );
	//ebook\reg_ebook_color.h
	__u32  						(*ptr_lion_reg_ebook_color_get)				( void );
	__s32 						(*ptr_lion_reg_ebook_color_set)				( __u32 ebook_color );
	__u32  						(*ptr_lion_reg_ebook_color_get_default)		( void );
	__s32 						(*ptr_lion_reg_ebook_color_restore)			( void );
	//ebook\reg_ebook_switch_time.h
	__s32  						(*ptr_lion_reg_ebook_switch_time_get)		( void );
	__s32 						(*ptr_lion_reg_ebook_switch_time_set)		( __s32 time );
	__s32  						(*ptr_lion_reg_ebook_switch_time_get_default)	( void );
	__s32 						(*ptr_lion_reg_ebook_switch_time_restore)	( void );
	//ebook\reg_ebook_bright.h
	__lion_bright_t 				(*ptr_lion_reg_ebook_bright_get)				(void);
	__s32 						(*ptr_lion_reg_ebook_bright_set)				(__lion_bright_t bright);
	__lion_bright_t 				(*ptr_lion_reg_ebook_bright_get_default)		(void);
	__s32 						(*ptr_lion_reg_ebook_bright_restore)			( void );	
	//fm\reg_fm_area.h
	__lion_fm_area_t  			(*ptr_lion_reg_fm_area_get)					( void );
	__s32 						(*ptr_lion_reg_fm_area_set)					( __lion_fm_area_t area );
	__lion_fm_area_t  			(*ptr_lion_reg_fm_area_get_default)			( void );
	__s32 						(*ptr_lion_reg_fm_area_restore)				( void );
	//fm\reg_fm_freq.h
	__u32  						(*ptr_lion_reg_fm_freq_get)					( void );
	__s32 						(*ptr_lion_reg_fm_freq_set)					( __u32 value );
	__u32  						(*ptr_lion_reg_fm_freq_get_default)			( void );
	__s32 						(*ptr_lion_reg_fm_freq_restore)				( void );
	//fm\reg_fm_send.h
	__bool  					(*ptr_lion_reg_fm_send_get)					( void );
	__s32 						(*ptr_lion_reg_fm_send_set)					( __bool fm_send );
	__bool  					(*ptr_lion_reg_fm_send_get_default)			( void );
	__s32 						(*ptr_lion_reg_fm_send_restore)				( void );
	//================================================================================//
	//========											functions											============//
	//================================================================================//
	//dsk_display_gamma.h
	__s32 						(*ptr_dsk_set_gamma)						( __u32 gamma );
	__s32 						(*ptr_dsk_gamma_on)							(void);
	__s32 						(*ptr_dsk_gamma_off)						(void);
	void 						(*ptr_dsk_gamma_reserved[5])				(void);
	//dsk_display_lcd.h
	__s32 						(*ptr_dsk_display_lcd_on)					(void);
	__s32 						(*ptr_dsk_display_lcd_off)					(void);
	__s32 						(*ptr_dsk_display_set_lcd_bright)			(__lion_bright_t level);
	__lion_bright_t 			(*ptr_dsk_display_get_lcd_bright)			( void );
	void		 				(*ptr_dsk_display_lcd_reserved[5])			( void );
	//dsk_display_misc.h
	__s32 						(*ptr_dsk_display_hdmi_audio_enable)		( __bool flag );
	__s32 						(*ptr_dsk_display_get_size)					(__s32 *p_width, __s32 *p_height);
	__s32 						(*ptr_dsk_display_set_backcolor)			(__u32 de_color);
	__u32 						(*ptr_dsk_display_get_backcolor)			(void);
	void 						(*ptr_dsk_display_misc_reserved[5])			(void);
	//dsk_display_output.h
	__lion_disp_format_e  		(*ptr_dsk_display_get_format)				( void );
	__s32 						(*ptr_dsk_display_set_format)				( __lion_disp_format_e type ); 
	__disp_output_type_t  		(*ptr_dsk_display_get_output_type)			( void );
	__s32  						(*ptr_dsk_display_dskfmt2epdkfmt)			( __lion_disp_format_e format, __disp_output_type_t *output_p );
	__lion_disp_format_e  		(*ptr_dsk_display_epdkfmt2dskfmt)			( __disp_output_type_t output, __s32 format );
	__s32  						(*ptr_dsk_display_on)						( __disp_output_type_t output );
	__s32  						(*ptr_dsk_display_off)						( void );
	void  						(*ptr_dsk_display_output_reserved[5])		( void );
	//dsk_display_enhance.h	
	__s32  						(*ptr_dsk_display_enhance_on)				(void);
	__s32  						(*ptr_dsk_display_enhance_off)				(void);
	__s32  						(*ptr_dsk_display_set_bright)				(__u32 bright);
	__s32  						(*ptr_dsk_display_get_bright)				(void);
	__s32  						(*ptr_dsk_display_set_contrast)				(__u32 contrast);
	__s32  						(*ptr_dsk_display_get_contrast)				(void);
	__s32  						(*ptr_dsk_display_set_saturation)			(__u32 saturation);
	__s32  						(*ptr_dsk_display_get_saturation)			(void);	
	
	//dsk_fw_update.h
	__s32 						(*ptr_dsk_fw_update)						( const char *file,H_LYR lyr);
	void 						(*ptr_dsk_fw_update_reserved[5])			( void );

	//dsk_keytone.h    
	__s32 						(*ptr_dsk_keytone_init)						(const char *keytone_file);
	__s32 						(*ptr_dsk_keytone_exit)						(void);
	__s32 						(*ptr_dsk_keytone_on)						(void);
	__s32 						(*ptr_dsk_keytone_set_state)				(__set_keytone_t state);
	__s32 						(*ptr_dsk_keytone_get_state)				(__set_keytone_t *p_state);
	void 						(*ptr_dsk_keytone_reserved[5])				(void);
	
	//dsk_usbh.h
	__s32						(*ptr_dsk_send_insmod_usbh_cmd) 			(void);
	__s32						(*ptr_dsk_send_rmmod_usbh_cmd)				(void);
	__bool						(*ptr_dsk_usbh_is_working)					(void);
	void						(*ptr_dsk_usbh_reserved[5])					(void);

	//dsk_audio_if.h
	__s32 						(*ptr_dsk_set_audio_if)						( __audio_dev_interface_t dev_if_type );
	__audio_dev_interface_t  	(*ptr_dsk_get_audio_if)						( void );
	__s32 						(*ptr_dsk_set_hdmi_audio_gate)				( __bool flag );
	void 						(*ptr_dsk_audio_reserved[5])				( void );

	//dsk_volume.h
	__s32						(*ptr_dsk_volume_set) 						(int volume);
	__s32						(*ptr_dsk_volume_get) 						(void);
	__s32						(*ptr_dsk_volume_inc) 						(void);
	__s32						(*ptr_dsk_volume_dec) 						(void); // 减小音量
	__s32						(*ptr_dsk_volume_off)						(void);
	__s32						(*ptr_dsk_volume_on) 						(void);
	__bool						(*ptr_dsk_volume_is_on)						(void);
	void						(*ptr_dsk_volume_reserved[10])				(void);

	//dsk_charset.h
	__epdk_charset_enm_e  		(*ptr_dsk_get_langres_charset)				( void );
	__s32 						(*ptr_dsk_set_fs_charset)					( __epdk_charset_enm_e charset );	
	__epdk_charset_enm_e  		(*ptr_dsk_get_fs_charset)					( void );	
	__s32 						(*ptr_dsk_set_isn_charset)					( __epdk_charset_enm_e charset );	
	__epdk_charset_enm_e  		(*ptr_dsk_get_isn_charset)					( void );

	//dsk_langres.h
	__s32 						(*ptr_dsk_langres_init)						( const char *file );
	void  						(*ptr_dsk_langres_exit)						( void );
	__s32 						(*ptr_dsk_langres_set_type)					( __u32 language_id );
	__s32 						(*ptr_dsk_langres_get_type)					( void );
	__s32 						(*ptr_dsk_langres_get_menu_len)				( __u32 text_id );
	__s32 						(*ptr_dsk_langres_get_menu_text)			( __u32 text_id, void *buf, __u32 size );

	//dsk_theme.h
	__s32 						(*ptr_dsk_theme_init)						( const char *file );
	__s32 						(*ptr_dsk_Systheme_init)					( const char *file );
	void  						(*ptr_dsk_theme_exit)						( void );
	__s32 						(*ptr_dsk_theme_set_style)					( __u32 style_id );
	__s32 						(*ptr_dsk_theme_get_style)					( void );
	HTHEME 						(*ptr_dsk_theme_open)						( __u32 theme_id );
	HTHEME 						(*ptr_dsk_Systheme_open)					( __u32 theme_id );
	void  						(*ptr_dsk_theme_close)						( HTHEME handle );
	__s32 						(*ptr_dsk_theme_hdl2size)					( HTHEME handle );
	void*						(*ptr_dsk_theme_hdl2buf)					( HTHEME handle );
	void*						(*ptr_dsk_Systheme_hdl2buf)					( HTHEME handle );
	//dsk_orchid.h
	__s32                   	(*ptr_dsk_orchid_check)             		(void);
	__s32                   	(*ptr_dsk_orchid_createDisk)        		(void);
	__s32                   	(*ptr_dsk_orchid_save_npl)          		(void);
	__s32                   	(*ptr_dsk_orchid_load_last_npl)     		(void);	
	__s32 						(*ptr_dsk_orchid_update)					(void);
	void  						(*ptr_dsk_orchid_reserved[10])				(void);

	//dsk_power.h    
	__s32                   	(*ptr_dsk_power_get_battery_level)  		(power_level_e *level);     // 剩余电量level
	__s32                   	(*ptr_dsk_power_get_voltage_level)  		(power_level_e *level);     // 工作电压level
	__s32                   	(*ptr_dsk_power_get_battary_state)  		(power_state_e *state);     // 查询 battary 状态    
	__s32 						(*ptr_dsk_power_set_off_time)				(__u32 time);	
	__bool 						(*ptr_dsk_power_is_low)						(void);	
	void 						(*ptr_dsk_speaker_turnoff)					(void);
	void 						(*ptr_dsk_speaker_turnon)					(void);	
	void 						(*ptr_dsk_speaker_resume)					(void);
	void 						(*ptr_dsk_speak_init)					(void);
	void 						(*ptr_dsk_speak_exit)					(void);
	void 						(*ptr_dsk_speaker_reserved[10])				(void);

	//dsk_usbd.h
	void                    	(*ptr_dsk_usbd_remove)         				(void);
	void                    	(*ptr_dsk_usbd_reserved[10])    			(void);	

	//dsk_auto_off.h
	__s32                     	(*ptr_dsk_set_auto_off_time)				( __u32 time );
	__s32 						(*ptr_dsk_power_off)						(void);

	
	//msg_srv/msg_emit.h
	__s32 						(*ptr_msg_emit_init)						(void);
	__s32						(*ptr_msg_emit_deinit)						(void);
	__s32 						(*ptr_msg_emit_register_hook)				(__memit_hook_t *hook);
	void 						(*ptr_msg_emit_reserved[10])				(void);

	//================================================================================//
	//========											engine											============//
	//================================================================================//

	//============================			    walkman			================================//
	//dsk_walkman.h
	__s32 						(*ptr_dsk_wkm_init)							( dsk_wkm_mode_e work_mode );
	void 						(*ptr_dsk_wkm_exit)							( void );
	__s32  						(*ptr_dsk_wkm_open)							( void );
	void  						(*ptr_dsk_wkm_close)						( void );	
	__bool 						(*ptr_dsk_wkm_is_open)						( void );
	__pCBK_t 					(*ptr_dsk_wkm_set_cb)						( dsk_wkm_event_e event, __pCBK_t cb, void * ctx);
	__pCBK_t 					(*ptr_dsk_wkm_get_cb)						( dsk_wkm_event_e event );
	__s32 						(*ptr_dsk_wkm_set_attr)						(void * attr);
	void *						(*ptr_dsk_wkm_get_attr)						(void);
	//dsk_wkm_ab.h
	__s32 						(*ptr_dsk_wkm_set_ab_a)						( void );
	__s32 						(*ptr_dsk_wkm_set_ab_b)						( void );
	__s32 						(*ptr_dsk_wkm_set_ab_loop_count)			( __u32 count );
	__s32 						(*ptr_dsk_wkm_cancle_ab)					( void );
	//dsk_wkm_star.h
	__s32 						(*ptr_dsk_wkm_set_star)						( const char *file, __u32 star );
	__s32 						(*ptr_dsk_wkm_get_star)						( const char *file );
	//dsk_wkm_play_speed.h
	__s32 						(*ptr_dsk_wkm_set_play_speed)				( __s32 play_speed );
	__s32 						(*ptr_dsk_wkm_get_play_speed)				( void );
	//dsk_wkm_lyric.h
	__bool 						(*ptr_dsk_wkm_lyric_exist)					( void );
	__cedar_lyric_fmt_t 		(*ptr_dsk_wkm_get_lyric_format)				( void );
	__s32 						(*ptr_dsk_wkm_get_lyric_item_count)			( void );
	__s32 						(*ptr_dsk_wkm_get_lyric_item_list)			( __cedar_subtitle_item_t  *lyric_item_list, __u32 item_count );
	//dsk_wkm_npl.h
	__s32 						(*ptr_dsk_wkm_npl_add_item)					( const char *file );
	__s32 						(*ptr_dsk_wkm_npl_delete_item)				( const char *file );
	__s32 						(*ptr_dsk_wkm_npl_clear)					( void );
	__s32 						(*ptr_dsk_wkm_npl_set_cur)					( __u32 index );
	__s32 						(*ptr_dsk_wkm_npl_get_cur)					( void );
	__s32  						(*ptr_dsk_wkm_npl_get_next)					( void );
	__s32 						(*ptr_dsk_wkm_npl_file2index)				( const char *file );
	__s32 						(*ptr_dsk_wkm_npl_index2file)				( __u32 index, char *file );
	__s32 						(*ptr_dsk_wkm_npl_get_total_count)			( void );
	__s32 						(*ptr_dsk_wkm_npl_save_list)				( void );	
	__s32 						(*ptr_dsk_wkm_npl_load_last_list)			( void );
	//dsk_wkm_play_mode.h
	__s32 						(*ptr_dsk_wkm_set_play_mode)				( orc_play_mode_e  play_mode );
	orc_play_mode_e 			(*ptr_dsk_wkm_get_play_mode)				( void );
	//dsk_wkm_eq.h
	__s32 						(*ptr_dsk_wkm_set_eq_mode)					( __cedar_audio_eq_t  eq_mode );
	__cedar_audio_eq_t 			(*ptr_dsk_wkm_get_eq_mode)					( void );
	__s32 						(*ptr_dsk_wkm_set_eq_value)					( const __s8 *eq_value_list );
	__s32 						(*ptr_dsk_wkm_get_eq_value)					( __s8 *eq_value_list );
	__s32 						(*ptr_dsk_wkm_eq_mode2value)				( __cedar_audio_eq_t eq_mode, __s8 *eq_value_list );
	//dsk_wkm_media_info.h
	__s32  						(*ptr_dsk_wkm_get_now_playing_file)			(char * filename);
	__s32 						(*ptr_dsk_wkm_get_tag)						( __cedar_tag_inf_t  *tag_info_p );
	__s32 						(*ptr_dsk_wkm_get_cur_time)					( void );
	__s32 						(*ptr_dsk_wkm_get_total_time)				( void );
	__cedar_audio_fmt_t 		(*ptr_dsk_wkm_get_audio_encoding)			( void );
	__u32 						(*ptr_dsk_wkm_get_audio_bps)				( void );
	__u32 						(*ptr_dsk_wkm_get_audio_sample_rate)		( void );
	__s32 						(*ptr_dsk_wkm_get_audio_info)				( const char *file, dsk_wkm_audio_info_t *info_p );
	__s32 						(*ptr_dsk_wkm_get_album_cover)				( const char *file, void *buf, const SIZE *size_p, dsk_wkm_pic_zoom_e zoom );
	//dsk_wkm_ff_rr_speed.h
	__s32 						(*ptr_dsk_wkm_set_ff_rr_speed)				( __u32 ff_rr_speed );
	__s32 						(*ptr_dsk_wkm_get_ff_rr_speed)				( void );
	//dsk_wkm_spectrum.h
	__s32 						(*ptr_dsk_wkm_enable_spectrum)				( void );
	__s32 						(*ptr_dsk_wkm_disable_spectrum)				( void );
	__s32 						(*ptr_dsk_wkm_get_spectrum_info)			( __u16 *value_list );
	//dsk_wkm_fsm_ctrl.h
	__s32 						(*ptr_dsk_wkm_play_file)					( const char *file );
	__s32 						(*ptr_dsk_wkm_play_next)					( void );
	__s32 						(*ptr_dsk_wkm_play_prev)					( void );
	__s32 						(*ptr_dsk_wkm_stop)							( void );
	__s32 						(*ptr_dsk_wkm_pause)						( void );
	__s32 						(*ptr_dsk_wkm_resume_play)					( void );
	__s32 						(*ptr_dsk_wkm_ff)							( void );
	__s32 						(*ptr_dsk_wkm_rr)							( void );
	__s32 						(*ptr_dsk_wkm_jump)							( __u32 time );
	__cedar_status_t 			(*ptr_dsk_wkm_get_fsm_state)				( void );
	__bool 						(*ptr_dsk_wkm_is_playing)					( void );
	__bool 						(*ptr_dsk_wkm_get_pause_state)				( void );
	void 						(*ptr_dsk_wkm_clear_feedback_msgQ)			( void );
	feedback_msg_t 				(*ptr_dsk_wkm_get_feedback_msg)				( void );
    __s32 				        (*ptr_dsk_wkm_monitor_enable)				( void );
    __s32 				        (*ptr_dsk_wkm_monitor_disable)				( void );
	//============================				radio 		================================//
	//dsk_radio.h
	__s32						(*ptr_dsk_radio_open)						(void);
	__bool 						(*ptr_dsk_radio_is_open)					(void);
	__pCBK_t					(*ptr_dsk_radio_set_cb)						( dsk_radio_event_e event, __pCBK_t cb, void * ctx );
	__pCBK_t					(*ptr_dsk_radio_get_cb)						( dsk_radio_event_e event );	
	__s32                       (*ptr_dsk_radio_mute)						(void);	                            
	__s32                       (*ptr_dsk_radio_unmute)						(void);	                            
	__s32                       (*ptr_dsk_radio_set_band)					(dsk_radio_band_mode_t band_mode);	                            
	__s32                       (*ptr_dsk_radio_set_threshold)				(dsk_radio_threshold_mode_t threshold);	                            
	__s32                       (*ptr_dsk_radio_set_audio_mode)				(dsk_radio_audio_mode_t audio_mode);	                            
	__s32                       (*ptr_dsk_radio_get_band)					(__s32 *cur_band);	                            
	__s32                       (*ptr_dsk_radio_get_audio_mode)				(__s32 *audio_mode);	                            
	__s32                       (*ptr_dsk_radio_get_min_freq)				(__s32 *min_freq);	                            
	__s32                       (*ptr_dsk_radio_get_max_freq)				(__s32 *max_freq);	
	__s32 						(*ptr_dsk_radio_close)						(void);
	//dsk_radio_receive.h
	__s32 						(*ptr_dsk_radio_rcv_open)					(void);
	__s32 						(*ptr_dsk_radio_rcv_close)					(void);
	__bool 						(*ptr_dsk_radio_rcv_is_open)				(void);
	__s32 						(*ptr_dsk_radio_rcv_set_freq_play)			(__s32 freq);
	__s32 						(*ptr_dsk_radio_rcv_auto_search)			(void);
	__s32 						(*ptr_dsk_radio_rcv_get_cur_freq)			(__u32 *freq);
	__s32 						(*ptr_dsk_radio_rcv_set_search_mode)		(__u32 search_mode);
	__s32 						(*ptr_dsk_radio_rcv_search_all)				(void);
	__s32 						(*ptr_dsk_radio_rcv_next_freq_play)			(void);
	__s32 						(*ptr_dsk_radio_rcv_pre_freq_play)			(void);
	__s32 						(*ptr_dsk_radio_rcv_set_play_list_type)		( __play_list_type_e play_list_type );
	__s32 						(*ptr_dsk_radio_rcv_get_play_list_type)		( __s32 *play_list_type);
	__s32 						(*ptr_dsk_radio_rcv_get_search_result_vn)	(__s32 *valid_num);
	__s32 						(*ptr_dsk_radio_rcv_get_favorite_vn)		(__s32 *valid_num);
	__s32 						(*ptr_dsk_radio_rcv_get_search_result_chaninfo)(__s32 id, dsk_radio_chan_t *chan_info);
	__s32 						(*ptr_dsk_radio_rcv_get_favorite_chaninfo)	(__s32 id, dsk_radio_chan_t *chan_info);
	__s32 						(*ptr_dsk_radio_rcv_get_favorite)			(char *file_name);
	__s32 						(*ptr_dsk_radio_rcv_save_favorite)			(char *file_name);
	__s32 						(*ptr_dsk_radio_rcv_get_search_result)		(char *file_name);
	__s32 						(*ptr_dsk_radio_rcv_save_search_result)		(char *file_name);
	__s32 						(*ptr_dsk_radio_rcv_add_chan_to_favorite)	(__u32 id,dsk_radio_chan_t *chan_info);
	__s32 						(*ptr_dsk_radio_rcv_del_chan_from_favorite)	(__u32 id);
	__s32                       (*ptr_dsk_radio_send_open)                  (void);
	__s32                       (*ptr_dsk_radio_send_close)                 (void);
	__bool 						(*ptr_dsk_radio_send_is_open)				(void);
	__s32                       (*ptr_dsk_radio_send_set_freq_play)         (__s32 freq);
	__s32                       (*ptr_dsk_radio_send_get_cur_freq)          (__u32 *freq);
	__s32                       (*ptr_dsk_radio_send_set_gain)              (__u32 gain);
	__s32                       (*ptr_dsk_radio_send_get_gain)              (__u32 *gain);
} desktop_api_entry_t;


#ifndef USED_BY_DESKTOP     //not used by desktop module itsef

#define DESKTOP_ENTRY_TBL   (0xe1f00000)   //this address is depended on the link address of mod_desktop
#define DESKTOP_FUNC_ENTRY  ((desktop_api_entry_t *)DESKTOP_ENTRY_TBL)

//================================================================================//
//========											reg											============//
//================================================================================//

//==========================			system			================================//
//system\reg_audio_output.h
#define	lion_reg_audio_output_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_audio_output_get)
#define	lion_reg_audio_output_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_audio_output_set) 		
#define	lion_reg_audio_output_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_audio_output_get_default) 
#define	lion_reg_audio_output_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_audio_output_restore) 	
//system\reg_auto_off.h
#define	lion_reg_auto_off_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_auto_off_get) 			
#define	lion_reg_auto_off_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_auto_off_set) 			
#define	lion_reg_auto_off_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_auto_off_get_default) 	
#define	lion_reg_auto_off_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_auto_off_restore) 		
//system\reg_channel.h
#define lion_reg_channel_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_channel_get)					
#define	lion_reg_channel_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_channel_set)					
#define lion_reg_channel_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_channel_get_default)			
#define	lion_reg_channel_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_channel_restore)				
//reg_charset.h
#define	lion_reg_charset_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_charset_get)					
#define	lion_reg_charset_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_charset_set)					
#define	lion_reg_charset_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_charset_get_default)			
#define	lion_reg_charset_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_charset_restore)				
//system\reg_close_scn.h
#define	lion_reg_close_scn_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_close_scn_get)				
#define	lion_reg_close_scn_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_close_scn_set)				
#define	lion_reg_close_scn_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_close_scn_get_default)		
#define	lion_reg_close_scn_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_close_scn_restore)			
//system\reg_detect_gate.h
#define	lion_reg_detect_gate_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_detect_gate_get)				
#define	lion_reg_detect_gate_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_detect_gate_set)				
#define	lion_reg_detect_gate_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_detect_gate_get_default)		
#define	lion_reg_detect_gate_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_detect_gate_restore)			
//system\reg_display.h
#define	lion_reg_display_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_display_get)					
#define	lion_reg_display_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_display_set)					
#define	lion_reg_display_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_display_get_default)			
#define	lion_reg_display_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_display_restore)				
//system\reg_gamma.h
#define	lion_reg_gamma_get					(DESKTOP_FUNC_ENTRY->ptr_lion_reg_gamma_get)					
#define	lion_reg_gamma_set					(DESKTOP_FUNC_ENTRY->ptr_lion_reg_gamma_set)					
#define	lion_reg_gamma_get_default			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_gamma_get_default)			
#define	lion_reg_gamma_restore				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_gamma_restore)				
//system\reg_keytone_gate.h
#define	lion_reg_keytone_gate_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_keytone_gate_get) 		
#define	lion_reg_keytone_gate_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_keytone_gate_set) 		
#define	lion_reg_keytone_gate_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_keytone_gate_get_default) 
#define	lion_reg_keytone_gate_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_keytone_gate_restore) 	
//system\reg_language.h
#define	lion_reg_language_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_language_get) 			
#define	lion_reg_language_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_language_set) 			
#define	lion_reg_language_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_language_get_default) 	
#define	lion_reg_language_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_language_restore) 		
//system\reg_volume.h
#define	lion_reg_volume_get					(DESKTOP_FUNC_ENTRY->ptr_lion_reg_volume_get)					
#define	lion_reg_volume_set					(DESKTOP_FUNC_ENTRY->ptr_lion_reg_volume_set)					
#define	lion_reg_volume_get_default			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_volume_get_default)			
#define	lion_reg_volume_restore				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_volume_restore)	
//system\reg_app_restore.h
#define lion_get_app_restore_flag			(DESKTOP_FUNC_ENTRY->ptr_lion_get_app_restore_flag)	
#define lion_set_app_restore_flag			(DESKTOP_FUNC_ENTRY->ptr_lion_set_app_restore_flag)	
#define lion_set_all_app_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_set_all_app_restore)	
//system\reg_lcd_bright.h
#define lion_reg_lcd_bright_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_bright_get)
#define lion_reg_lcd_bright_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_bright_set)
#define lion_reg_lcd_bright_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_bright_get_default)
#define lion_reg_lcd_bright_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_bright_restore)
//system\reg_lcd_contrast.h
#define lion_reg_lcd_contrast_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_contrast_get)
#define lion_reg_lcd_contrast_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_contrast_set)
#define lion_reg_lcd_contrast_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_contrast_get_default)
#define lion_reg_lcd_contrast_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_contrast_restore)
//system\reg_lcd_saturation.h
#define lion_reg_lcd_saturation_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_saturation_get)
#define lion_reg_lcd_saturation_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_saturation_set)
#define lion_reg_lcd_saturation_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_saturation_get_default)
#define lion_reg_lcd_saturation_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lcd_saturation_restore)
//system\reg_tp_adjust.h
#define lion_tp_adjust_flag_get					(DESKTOP_FUNC_ENTRY->ptr_lion_tp_adjust_flag_get)
#define lion_tp_adjust_flag_set					(DESKTOP_FUNC_ENTRY->ptr_lion_tp_adjust_flag_set)

//==========================			movie			================================//
//movie\reg_movie_repeat.h
#define lion_reg_movie_repeat_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_movie_repeat_get)			
#define lion_reg_movie_repeat_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_movie_repeat_set)			
#define lion_reg_movie_repeat_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_movie_repeat_get_default)	
#define lion_reg_movie_repeat_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_movie_repeat_restore)		
//movie\reg_zoom.h
#define lion_reg_zoom_get					(DESKTOP_FUNC_ENTRY->ptr_lion_reg_zoom_get)					
#define lion_reg_zoom_set					(DESKTOP_FUNC_ENTRY->ptr_lion_reg_zoom_set)					
#define lion_reg_zoom_get_default			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_zoom_get_default)			
#define lion_reg_zoom_restore				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_zoom_restore)				
//movie\reg_sub_size.h
#define lion_reg_sub_size_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_size_get)				
#define lion_reg_sub_size_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_size_set)				
#define lion_reg_sub_size_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_size_get_default)		
#define lion_reg_sub_size_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_size_restore)			
//movie\reg_sub_gate.h
#define lion_reg_sub_gate_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_gate_get)				
#define lion_reg_sub_gate_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_gate_set)				
#define lion_reg_sub_gate_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_gate_get_default)		
#define lion_reg_sub_gate_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_gate_restore)			
//movie\reg_sub_frame_color.h
#define lion_reg_sub_frame_color_get		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_frame_color_get)			
#define lion_reg_sub_frame_color_set		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_frame_color_set)			
#define lion_reg_sub_frame_color_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_frame_color_get_default)	
#define lion_reg_sub_frame_color_restore	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_frame_color_restore)		
//movie\reg_sub_delay.h
#define lion_reg_sub_delay_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_delay_get)				
#define lion_reg_sub_delay_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_delay_set)				
#define lion_reg_sub_delay_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_delay_get_default)		
#define lion_reg_sub_delay_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_delay_restore)			
//movie\reg_sub_color.h
#define lion_reg_sub_color_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_color_get)				
#define lion_reg_sub_color_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_color_set)				
#define lion_reg_sub_color_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_color_get_default)		
#define lion_reg_sub_color_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_color_restore)			
//movie\reg_sub_charset.h
#define lion_reg_sub_charset_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_charset_get)				
#define lion_reg_sub_charset_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_charset_set)				
#define lion_reg_sub_charset_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_charset_get_default)		
#define lion_reg_sub_charset_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_charset_restore)			
#define		lion_reg_sub_position_get		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_position_get)
#define 	lion_reg_sub_position_set		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_position_set)
#define 	lion_reg_sub_position_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_position_get_default)	
#define 	lion_reg_sub_position_restore	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sub_position_restore)		
//movie\reg_bright.h
#define		lion_reg_bright_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_bright_get)			
#define 	lion_reg_bright_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_bright_set)			
#define		lion_reg_bright_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_bright_get_default)	
#define 	lion_reg_bright_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_bright_restore)		
//movie\reg_color_temperature.h
#define 	lion_reg_color_temperature_get	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_color_temperature_get)		
#define 	lion_reg_color_temperature_set	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_color_temperature_set)		
#define 	lion_reg_color_temperature_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_color_temperature_get_default)
#define 	lion_reg_color_temperature_restore	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_color_temperature_restore)	
//movie\reg_contrast.h
#define 	lion_reg_contrast_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_contrast_get)				
#define 	lion_reg_contrast_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_contrast_set)				
#define 	lion_reg_contrast_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_contrast_get_default)		
#define 	lion_reg_contrast_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_contrast_restore)			
//movie\reg_saturation.h
#define 	lion_reg_saturation_get		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_saturation_get)				
#define 	lion_reg_saturation_set		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_saturation_set)				
#define 	lion_reg_saturation_get_default (DESKTOP_FUNC_ENTRY->ptr_lion_reg_saturation_get_default)		
#define 	lion_reg_saturation_restore	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_saturation_restore)			

//=======================		music	=========================//
//music\reg_music_repeat.h
#define lion_reg_music_repeat_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_music_repeat_get)			
#define lion_reg_music_repeat_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_music_repeat_set)			
#define lion_reg_music_repeat_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_music_repeat_get_default)	
#define lion_reg_music_repeat_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_music_repeat_restore)			
//music\reg_eq.h
#define lion_reg_eq_get						(DESKTOP_FUNC_ENTRY->ptr_lion_reg_eq_get)
#define lion_reg_eq_set						(DESKTOP_FUNC_ENTRY->ptr_lion_reg_eq_set)
#define lion_reg_eq_get_default				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_eq_get_default)
#define lion_reg_eq_restore					(DESKTOP_FUNC_ENTRY->ptr_lion_reg_eq_restore)
//music\reg_lyric_mode.h
#define lion_reg_lyric_mode_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_mode_get)				
#define lion_reg_lyric_mode_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_mode_set)				
#define lion_reg_lyric_mode_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_mode_get_default)		
#define lion_reg_lyric_mode_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_mode_restore)			
//music\reg_lyric_focus_color.h
#define lion_reg_lyric_focus_color_get		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_focus_color_get)		
#define lion_reg_lyric_focus_color_set		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_focus_color_set)		
#define lion_reg_lyric_focus_color_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_focus_color_get_default)
#define lion_reg_lyric_focus_color_restore	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_focus_color_restore)	
//music\reg_lyric_unfocus_color.h
#define lion_reg_lyric_unfocus_color_get	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_unfocus_color_get)		
#define lion_reg_lyric_unfocus_color_set	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_unfocus_color_set)		
#define lion_reg_lyric_unfocus_color_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_unfocus_color_get_default)
#define lion_reg_lyric_unfocus_color_restore	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_lyric_unfocus_color_restore)	
#define 	lion_reg_backgrd_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_backgrd_get)		
#define 	lion_reg_backgrd_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_backgrd_set)				
#define 	lion_reg_backgrd_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_backgrd_get_default)		
#define 	lion_reg_backgrd_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_backgrd_restore)				
//music\reg_album.h
#define 	lion_reg_album_get 				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_album_get)					
#define 	lion_reg_album_set 				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_album_set)					
#define 	lion_reg_album_get_default 		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_album_get_default)			
#define 	lion_reg_album_restore 			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_album_restore)				

//=======================		photo	=========================//
//photo\reg_switch_mode.h
#define lion_reg_switch_mode_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_mode_get)				
#define lion_reg_switch_mode_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_mode_set)				
#define lion_reg_switch_mode_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_mode_get_default)		
#define lion_reg_switch_mode_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_mode_restore)			
//photo\reg_switch_time.h
#define lion_reg_switch_time_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_time_get)				
#define lion_reg_switch_time_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_time_set)				
#define lion_reg_switch_time_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_time_get_default)		
#define lion_reg_switch_time_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_switch_time_restore)			
//photo\reg_photo_repeat.h
#define lion_reg_photo_repeat_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_photo_repeat_get)
#define lion_reg_photo_repeat_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_photo_repeat_set)
#define lion_reg_photo_repeat_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_photo_repeat_get_default)
#define lion_reg_photo_repeat_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_photo_repeat_restore)
//explorer\reg_exp_sort.h
#define lion_reg_sort_mode_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sort_mode_get)	
#define lion_reg_sort_mode_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sort_mode_set)	
#define lion_reg_sort_mode_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sort_mode_get_default)
#define lion_reg_sort_mode_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_sort_mode_restore)	
//ebook\reg_ebook_backgrd.h
#define lion_reg_ebook_backgrd_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_backgrd_get)	
#define lion_reg_ebook_backgrd_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_backgrd_set)		
#define lion_reg_ebook_backgrd_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_backgrd_get_default)	
#define lion_reg_ebook_backgrd_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_backgrd_restore)	
//ebook\reg_ebook_color.h
#define lion_reg_ebook_color_get			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_color_get)	
#define lion_reg_ebook_color_set			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_color_set)	
#define lion_reg_ebook_color_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_color_get_default)
#define lion_reg_ebook_color_restore		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_color_restore)
//ebook\reg_ebook_switch_time.h
#define lion_reg_ebook_switch_time_get		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_switch_time_get)
#define lion_reg_ebook_switch_time_set		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_switch_time_set)
#define lion_reg_ebook_switch_time_get_default	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_switch_time_get_default)
#define lion_reg_ebook_switch_time_restore	(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_switch_time_restore)
//ebook\reg_ebook_bright.h
#define lion_reg_ebook_bright_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_bright_get)
#define lion_reg_ebook_bright_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_bright_set)
#define lion_reg_ebook_bright_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_bright_get_default)
#define lion_reg_ebook_bright_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_ebook_bright_restore)

//fm\reg_fm_area.h
#define	lion_reg_fm_area_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_area_get)
#define	lion_reg_fm_area_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_area_set)
#define	lion_reg_fm_area_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_area_get_default)
#define	lion_reg_fm_area_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_area_restore)
//fm\reg_fm_freq.h
#define	lion_reg_fm_freq_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_freq_get)
#define	lion_reg_fm_freq_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_freq_set)
#define	lion_reg_fm_freq_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_freq_get_default)
#define	lion_reg_fm_freq_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_freq_restore)
//fm\reg_fm_send.h
#define	lion_reg_fm_send_get				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_send_get)
#define	lion_reg_fm_send_set				(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_send_set)
#define	lion_reg_fm_send_get_default		(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_send_get_default)
#define	lion_reg_fm_send_restore			(DESKTOP_FUNC_ENTRY->ptr_lion_reg_fm_send_restore)
//================================================================================//
//========											functions											============//
//================================================================================//
//dsk_display_gamma.h
#define	dsk_set_gamma						(DESKTOP_FUNC_ENTRY->ptr_dsk_set_gamma) 					
#define	dsk_gamma_on						(DESKTOP_FUNC_ENTRY->ptr_dsk_gamma_on)							
#define	dsk_gamma_off						(DESKTOP_FUNC_ENTRY->ptr_dsk_gamma_off) 					
//dsk_display_lcd.h
#define	dsk_display_lcd_on					(DESKTOP_FUNC_ENTRY->ptr_dsk_display_lcd_on)					
#define	dsk_display_lcd_off					(DESKTOP_FUNC_ENTRY->ptr_dsk_display_lcd_off)					
#define	dsk_display_set_lcd_bright			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_set_lcd_bright)			
#define	dsk_display_get_lcd_bright			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_lcd_bright)			
//dsk_display_misc.h
#define	dsk_display_hdmi_audio_enable		(DESKTOP_FUNC_ENTRY->ptr_dsk_display_hdmi_audio_enable) 	
#define	dsk_display_get_size				(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_size)					
#define	dsk_display_set_backcolor			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_set_backcolor) 		
#define	dsk_display_get_backcolor			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_backcolor) 		
//dsk_display_output.h
#define	dsk_display_get_format				(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_format)				
#define	dsk_display_set_format				(DESKTOP_FUNC_ENTRY->ptr_dsk_display_set_format)				
#define	dsk_display_get_output_type			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_output_type)			
#define	dsk_display_dskfmt2epdkfmt			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_dskfmt2epdkfmt)			
#define	dsk_display_epdkfmt2dskfmt			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_epdkfmt2dskfmt)			
#define	dsk_display_on						(DESKTOP_FUNC_ENTRY->ptr_dsk_display_on)						
#define	dsk_display_off						(DESKTOP_FUNC_ENTRY->ptr_dsk_display_off)						
//dsk_display_enhance.h	
#define dsk_display_enhance_on 				(DESKTOP_FUNC_ENTRY->ptr_dsk_display_enhance_on)
#define dsk_display_enhance_off 			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_enhance_off)
#define dsk_display_set_bright 				(DESKTOP_FUNC_ENTRY->ptr_dsk_display_set_bright)
#define dsk_display_get_bright 				(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_bright)
#define dsk_display_set_contrast 			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_set_contrast)
#define dsk_display_get_contrast 			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_contrast)
#define dsk_display_set_saturation 			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_set_saturation)
#define dsk_display_get_saturation 			(DESKTOP_FUNC_ENTRY->ptr_dsk_display_get_saturation)

//dsk_fw_update.h
#define	dsk_fw_update						(DESKTOP_FUNC_ENTRY->ptr_dsk_fw_update) 					

//dsk_keytone.h    
#define	dsk_keytone_init					(DESKTOP_FUNC_ENTRY->ptr_dsk_keytone_init)						
#define	dsk_keytone_exit					(DESKTOP_FUNC_ENTRY->ptr_dsk_keytone_exit)						
#define	dsk_keytone_on						(DESKTOP_FUNC_ENTRY->ptr_dsk_keytone_on)						
#define	dsk_keytone_set_state				(DESKTOP_FUNC_ENTRY->ptr_dsk_keytone_set_state) 			
#define	dsk_keytone_get_state				(DESKTOP_FUNC_ENTRY->ptr_dsk_keytone_get_state) 			

//dsk_usbh.h
#define	dsk_send_insmod_usbh_cmd			(DESKTOP_FUNC_ENTRY->ptr_dsk_send_insmod_usbh_cmd)			
#define	dsk_send_rmmod_usbh_cmd				(DESKTOP_FUNC_ENTRY->ptr_dsk_send_rmmod_usbh_cmd)				
#define	dsk_usbh_is_working					(DESKTOP_FUNC_ENTRY->ptr_dsk_usbh_is_working)					

//dsk_audio_if.h
#define dsk_set_audio_if					(DESKTOP_FUNC_ENTRY->ptr_dsk_set_audio_if)
#define	dsk_get_audio_if  					(DESKTOP_FUNC_ENTRY->ptr_dsk_get_audio_if)	
#define	dsk_set_hdmi_audio_gate 			(DESKTOP_FUNC_ENTRY->ptr_dsk_set_hdmi_audio_gate)

//dsk_volume.h
#define	dsk_volume_set						(DESKTOP_FUNC_ENTRY->ptr_dsk_volume_set)				
#define	dsk_volume_get						(DESKTOP_FUNC_ENTRY->ptr_dsk_volume_get)				
#define	dsk_volume_inc						(DESKTOP_FUNC_ENTRY->ptr_dsk_volume_inc)				
#define	dsk_volume_dec						(DESKTOP_FUNC_ENTRY->ptr_dsk_volume_dec)				
#define	dsk_volume_off						(DESKTOP_FUNC_ENTRY->ptr_dsk_volume_off) 					
#define	dsk_volume_on						(DESKTOP_FUNC_ENTRY->ptr_dsk_volume_on)						
#define	dsk_volume_is_on					(DESKTOP_FUNC_ENTRY->ptr_dsk_volume_is_on)		

//dsk_charset.h
#define dsk_get_langres_charset 			(DESKTOP_FUNC_ENTRY->ptr_dsk_get_langres_charset)
#define dsk_set_fs_charset					(DESKTOP_FUNC_ENTRY->ptr_dsk_set_fs_charset)
#define dsk_get_fs_charset 					(DESKTOP_FUNC_ENTRY->ptr_dsk_get_fs_charset)
#define dsk_set_isn_charset					(DESKTOP_FUNC_ENTRY->ptr_dsk_set_isn_charset)
#define dsk_get_isn_charset 				(DESKTOP_FUNC_ENTRY->ptr_dsk_get_isn_charset)

//dsk_langres.h
#define dsk_langres_init					(DESKTOP_FUNC_ENTRY->ptr_dsk_langres_init)			
#define dsk_langres_exit 					(DESKTOP_FUNC_ENTRY->ptr_dsk_langres_exit)					
#define dsk_langres_set_type				(DESKTOP_FUNC_ENTRY->ptr_dsk_langres_set_type)					
#define dsk_langres_get_type				(DESKTOP_FUNC_ENTRY->ptr_dsk_langres_get_type)				
#define dsk_langres_get_menu_len			(DESKTOP_FUNC_ENTRY->ptr_dsk_langres_get_menu_len)			
#define dsk_langres_get_menu_text			(DESKTOP_FUNC_ENTRY->ptr_dsk_langres_get_menu_text)			

//dsk_theme.h
#define dsk_theme_init						(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_init)
#define dsk_Systheme_init					(DESKTOP_FUNC_ENTRY->ptr_dsk_Systheme_init)

#define dsk_theme_exit 						(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_exit)						
#define dsk_theme_set_style					(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_set_style)					
#define dsk_theme_get_style					(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_get_style)					
#define dsk_theme_open						(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_open)
#define dsk_Systheme_open					(DESKTOP_FUNC_ENTRY->ptr_dsk_Systheme_open)

#define dsk_theme_close 					(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_close)						
#define dsk_theme_hdl2size					(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_hdl2size)					
#define	dsk_theme_hdl2buf					(DESKTOP_FUNC_ENTRY->ptr_dsk_theme_hdl2buf)					
#define dsk_Systheme_hdl2buf				(DESKTOP_FUNC_ENTRY->ptr_dsk_Systheme_hdl2buf)

// ./dsk_sys/dsk_orchid.h
#define	dsk_orchid_check					(DESKTOP_FUNC_ENTRY->ptr_dsk_orchid_check)					
#define	dsk_orchid_createDisk				(DESKTOP_FUNC_ENTRY->ptr_dsk_orchid_createDisk) 			
#define	dsk_orchid_save_npl					(DESKTOP_FUNC_ENTRY->ptr_dsk_orchid_save_npl)				
#define	dsk_orchid_load_last_npl			(DESKTOP_FUNC_ENTRY->ptr_dsk_orchid_load_last_npl)			
#define	dsk_orchid_update					(DESKTOP_FUNC_ENTRY->ptr_dsk_orchid_update) 				

//./dsk_sys/dsk_power.h    
#define	dsk_power_get_battery_level			(DESKTOP_FUNC_ENTRY->ptr_dsk_power_get_battery_level)		
#define	dsk_power_get_voltage_level			(DESKTOP_FUNC_ENTRY->ptr_dsk_power_get_voltage_level)		
#define	dsk_power_get_battary_state			(DESKTOP_FUNC_ENTRY->ptr_dsk_power_get_battary_state)		
#define	dsk_power_set_off_time				(DESKTOP_FUNC_ENTRY->ptr_dsk_power_set_off_time)				
#define	dsk_power_is_low					(DESKTOP_FUNC_ENTRY->ptr_dsk_power_is_low)						
#define	dsk_speaker_turnoff					(DESKTOP_FUNC_ENTRY->ptr_dsk_speaker_turnoff)					
#define	dsk_speaker_turnon					(DESKTOP_FUNC_ENTRY->ptr_dsk_speaker_turnon)					
#define	dsk_speaker_resume					(DESKTOP_FUNC_ENTRY->ptr_dsk_speaker_resume)
#define dsk_speak_init						(DESKTOP_FUNC_ENTRY->ptr_dsk_speak_init)	
#define dsk_speak_exit						(DESKTOP_FUNC_ENTRY->ptr_dsk_speak_exit)				

// ./dsk_sys/dsk_usbd.h
#define	dsk_usbd_remove						(DESKTOP_FUNC_ENTRY->ptr_dsk_usbd_remove)		

//dsk_auto_off.h
#define dsk_set_auto_off_time 				(DESKTOP_FUNC_ENTRY->ptr_dsk_set_auto_off_time)
#define dsk_power_off						(DESKTOP_FUNC_ENTRY->ptr_dsk_power_off)


//msg_srv/msg_emit.h
#define msg_emit_init						(DESKTOP_FUNC_ENTRY->ptr_msg_emit_init							)	
#define	msg_emit_deinit						(DESKTOP_FUNC_ENTRY->ptr_msg_emit_deinit						)	
#define msg_emit_register_hook				(DESKTOP_FUNC_ENTRY->ptr_msg_emit_register_hook					)			

//================================================================================//
//========											engine											============//
//================================================================================//
//============================			    walkman			================================//
//dsk_walkman.h
#define dsk_wkm_init 						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_init)							
#define dsk_wkm_exit						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_exit)						
#define dsk_wkm_open						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_open)	
#define dsk_wkm_is_open						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_is_open)
#define dsk_wkm_close						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_close)						
#define dsk_wkm_set_cb 						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_cb)						
#define dsk_wkm_get_cb						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_cb)	
#define dsk_wkm_set_attr 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_attr)	
#define dsk_wkm_get_attr					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_attr)	

//dsk_wkm_ab.h
#define dsk_wkm_set_ab_a 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_ab_a)						
#define dsk_wkm_set_ab_b 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_ab_b)						
#define dsk_wkm_set_ab_loop_count 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_ab_loop_count)			
#define dsk_wkm_cancle_ab 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_cancle_ab)					
//dsk_wkm_star.h
#define dsk_wkm_set_star 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_star)						
#define dsk_wkm_get_star 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_star)						
//dsk_wkm_play_speed.h
#define dsk_wkm_set_play_speed 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_play_speed)				
#define dsk_wkm_get_play_speed 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_play_speed)				
//dsk_wkm_lyric.h
#define  dsk_wkm_lyric_exist				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_lyric_exist)					
#define dsk_wkm_get_lyric_format			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_lyric_format)				
#define dsk_wkm_get_lyric_item_count 		(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_lyric_item_count)			
#define dsk_wkm_get_lyric_item_list 		(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_lyric_item_list)			
//dsk_wkm_npl.h
#define dsk_wkm_npl_add_item 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_add_item)					
#define dsk_wkm_npl_delete_item 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_delete_item)				
#define dsk_wkm_npl_clear 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_clear)					
#define dsk_wkm_npl_set_cur 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_set_cur)					
#define dsk_wkm_npl_get_cur 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_get_cur)	
#define dsk_wkm_npl_get_next				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_get_next)
#define dsk_wkm_npl_file2index 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_file2index)				
#define dsk_wkm_npl_index2file 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_index2file)				
#define dsk_wkm_npl_get_total_count 		(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_get_total_count)			
#define dsk_wkm_npl_save_list				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_save_list)	
#define dsk_wkm_npl_load_last_list			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_npl_load_last_list)	
//dsk_wkm_play_mode.h
#define dsk_wkm_set_play_mode 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_play_mode)				
#define dsk_wkm_get_play_mode				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_play_mode)				
//dsk_wkm_eq.h
#define dsk_wkm_set_eq_mode 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_eq_mode)					
#define dsk_wkm_get_eq_mode					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_eq_mode)					
#define dsk_wkm_set_eq_value 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_eq_value)					
#define dsk_wkm_get_eq_value 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_eq_value)					
#define dsk_wkm_eq_mode2value 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_eq_mode2value)				
//dsk_wkm_media_info.h
#define dsk_wkm_get_now_playing_file		(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_now_playing_file)
#define dsk_wkm_get_tag 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_tag)						
#define dsk_wkm_get_cur_time 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_cur_time)					
#define dsk_wkm_get_total_time 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_total_time)				
#define dsk_wkm_get_audio_encoding			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_audio_encoding)			
#define dsk_wkm_get_audio_bps 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_audio_bps)				
#define dsk_wkm_get_audio_sample_rate 		(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_audio_sample_rate)		
#define dsk_wkm_get_audio_info 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_audio_info)				
#define dsk_wkm_get_album_cover 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_album_cover)				
//dsk_wkm_ff_rr_speed.h
#define dsk_wkm_set_ff_rr_speed 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_set_ff_rr_speed)				
#define dsk_wkm_get_ff_rr_speed 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_ff_rr_speed)				
//dsk_wkm_spectrum.h
#define dsk_wkm_enable_spectrum 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_enable_spectrum)				
#define dsk_wkm_disable_spectrum 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_disable_spectrum)				
#define dsk_wkm_get_spectrum_info 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_spectrum_info)			
//dsk_wkm_fsm_ctrl.h
#define dsk_wkm_play_file 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_play_file)	
#define dsk_wkm_play_next 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_play_next)
#define dsk_wkm_play_prev 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_play_prev)
#define dsk_wkm_stop 						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_stop)							
#define dsk_wkm_pause 						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_pause)						
#define dsk_wkm_resume_play 				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_resume_play)					
#define dsk_wkm_ff 							(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_ff)							
#define dsk_wkm_rr 							(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_rr)							
#define dsk_wkm_jump 						(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_jump)							
#define dsk_wkm_get_fsm_state				(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_fsm_state)				
#define dsk_wkm_is_playing 					(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_is_playing)					
#define dsk_wkm_get_pause_state 			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_pause_state)					
#define dsk_wkm_clear_feedback_msgQ 		(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_clear_feedback_msgQ)			
#define dsk_wkm_get_feedback_msg			(DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_get_feedback_msg)				
#define dsk_wkm_monitor_enable			    (DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_monitor_enable)				
#define dsk_wkm_monitor_disable			    (DESKTOP_FUNC_ENTRY->ptr_dsk_wkm_monitor_disable)				
//============================				radio 		================================//
//dsk_radio.h
#define dsk_radio_open 						(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_open)
#define dsk_radio_is_open					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_is_open)
#define	dsk_radio_set_cb					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_set_cb)	
#define	dsk_radio_get_cb					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_get_cb)
#define dsk_radio_mute						(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_mute)
#define dsk_radio_unmute					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_unmute)
#define dsk_radio_set_band					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_set_band)
#define dsk_radio_set_threshold				(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_set_threshold)
#define dsk_radio_set_audio_mode			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_set_audio_mode)
#define dsk_radio_get_band					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_get_band)
#define dsk_radio_get_audio_mode			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_get_audio_mode)
#define dsk_radio_get_min_freq				(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_get_min_freq)
#define dsk_radio_get_max_freq				(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_get_max_freq)			
#define dsk_radio_close 					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_close)				
//dsk_radio_receive.h
#define dsk_radio_rcv_open					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_open)
#define dsk_radio_rcv_close					(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_close)
#define dsk_radio_rcv_is_open 				(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_is_open)
#define dsk_radio_rcv_set_freq_play			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_set_freq_play)	
#define dsk_radio_rcv_auto_search			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_auto_search) 
#define dsk_radio_rcv_get_cur_freq 			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_cur_freq)
#define dsk_radio_rcv_set_search_mode		(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_set_search_mode) 
#define dsk_radio_rcv_search_all			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_search_all)	
#define dsk_radio_rcv_next_freq_play		(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_next_freq_play)	
#define dsk_radio_rcv_pre_freq_play			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_pre_freq_play)	
#define dsk_radio_rcv_set_play_list_type	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_set_play_list_type)	
#define dsk_radio_rcv_get_play_list_type	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_play_list_type)
#define dsk_radio_rcv_get_search_result_vn	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_search_result_vn)
#define dsk_radio_rcv_get_favorite_vn		(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_favorite_vn) 
#define dsk_radio_rcv_get_search_result_chaninfo	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_search_result_chaninfo)
#define dsk_radio_rcv_get_favorite_chaninfo	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_favorite_chaninfo)	
#define dsk_radio_rcv_get_favorite			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_favorite)	
#define dsk_radio_rcv_save_favorite			(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_save_favorite)	
#define dsk_radio_rcv_get_search_result		(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_get_search_result)	
#define dsk_radio_rcv_save_search_result	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_save_search_result)	
#define dsk_radio_rcv_add_chan_to_favorite 	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_add_chan_to_favorite)	
#define dsk_radio_rcv_del_chan_from_favorite	(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_rcv_del_chan_from_favorite)	

#define dsk_radio_send_open		                    (DESKTOP_FUNC_ENTRY->ptr_dsk_radio_send_open                                )
#define dsk_radio_send_close		                (DESKTOP_FUNC_ENTRY->ptr_dsk_radio_send_close                               )
#define dsk_radio_send_is_open						(DESKTOP_FUNC_ENTRY->ptr_dsk_radio_send_is_open								)
#define dsk_radio_send_set_freq_play	            (DESKTOP_FUNC_ENTRY->ptr_dsk_radio_send_set_freq_play                       )
#define dsk_radio_send_get_cur_freq	                (DESKTOP_FUNC_ENTRY->ptr_dsk_radio_send_get_cur_freq                        )
#define dsk_radio_send_set_gain		                (DESKTOP_FUNC_ENTRY->ptr_dsk_radio_send_set_gain                            )
#define dsk_radio_send_get_gain		                (DESKTOP_FUNC_ENTRY->ptr_dsk_radio_send_get_gain                            )
#endif  //USED_BY_DESKTOP

#endif  //__MOD_DESKTOP_API_H__


