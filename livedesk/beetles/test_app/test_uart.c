
#include "epdk.h"

__u32 uart_driver_test(void)
{
	__u32 port_num ;
	ES_FILE* pfile_uart ;
	char path[32];

	__inf(" please input uart port num to test:");
	port_num = __getc();
	__inf(" %d\n",(port_num&0x7f-0x30));

	esDEV_Plugin("\\drv\\uart.drv", (port_num&0x7f-0x30), 0, 1);
	esKRNL_TimeDly(10);
	eLIBs_sprintf(path,"b:\\BUS\\UART%d",(port_num&0x7f-0x30));
	__msg("path=%s\n",path);
	pfile_uart =  eLIBs_fopen(path,"rb+");
	__msg("pfile_uart=%x\n",pfile_uart);

	eLIBs_fwrite("hello uart driver", 16, 1, pfile_uart);
	while(1)
	{
		eLIBs_fread((void*)path, 16, 1, pfile_uart);
		eLIBs_fwrite((void*)path, 16, 1, pfile_uart);

		esKRNL_TimeDly(200);
	}
}


