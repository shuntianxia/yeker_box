

#include "epdk.h"


__s32 test_jpeg_driver(void)
{
	jpeg_enc_test();	
	return EPDK_OK;
}

__s32 test_print_selection(void)
{
	__u32 select ;
	__inf("	1 test csi ..................................\n");
	__inf("	2 test jpeg .................................\n");
	__inf("	3 test de-interlace..........................\n");
	__inf("	4 test motion jpeg ..........................\n");
	__inf("	5 test tv in ................................\n");
	__inf("	6 test uart  ................................\n");
	__inf("	7 tv in with deinterlace ....................\n");
	__inf("	8 test pwm ..................................\n");

	select = 5;//__getc();
	
	return (select&0x7f-0x30);
}
__s32 test_app(void)
{
	__log("start to test please input selection to test :\n");
	switch(test_print_selection())
	{
		case 1:
			__inf("start to test csi\n");
			test_video_in(0);
			break ;
		case 2:
			__inf("start to test jpeg\n");
			test_jpeg_driver();
			break ;
		case 3:
			__inf("start to test de-interlace-----------\n");
			de_interlace_test();
			break ;
		case 4:
			__inf("start to test mjpeg-----------\n");
			mjpeg_test_start();
			break ;
		case 5:
			__inf("start to test tv in-----------\n");
			test_video_in(1);
			break ;
		case 6:
			__inf("start to test uart driver-----------\n");
			uart_driver_test();
			break ;
		case 7:
			test_tvin_with_di();
			break ;
		case 8:
			pwm_driver_test();
			break ;

		default:
			__inf("error selection\n");
			break ;
	}
	return EPDK_OK;
}
