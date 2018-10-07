
#include "epdk.h"

__u32 pwm_driver_test(void)
{
	ES_FILE* pfile_pwm ;

	__u32 channel ;
	__u32	active_cycle ;
	eLIBs_printf("please input the pwm channel :\n");
	
	channel = __getc();
	channel = (channel & 0x3f) - 0x30;
	
	esDEV_Plugin("\\drv\\pwm.drv", 0, 0, 1);
	esKRNL_TimeDly(10);
	pfile_pwm =  eLIBs_fopen("b:\\USER\\PWM","rb+");
	eLIBs_printf("pfile_pwm=%x\n",pfile_pwm);

	while(1)
	{
		eLIBs_printf("please input the pwm channel :\n");
		
		active_cycle = __getc();
		active_cycle = (active_cycle & 0x3f) - 0x30;
		
		eLIBs_fioctrl(pfile_pwm,PWM_CMD_SET_ACTIVY_CYCLE,active_cycle,0);
		esKRNL_TimeDly(200);
	}
}

