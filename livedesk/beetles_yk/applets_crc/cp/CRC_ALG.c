#include <apps.h>
#include "scrolling_i.h"
#include "CRC_ALG.h"

unsigned char calcrc_1byte(unsigned char abyte)
{
   char i,crc_1byte = 0;
   for(i = 0; i < 8; i++){
	   if(((crc_1byte^abyte) & 0x01)){
		   crc_1byte ^= 0x18;
		   crc_1byte >>= 1;
		   crc_1byte |= 0x80;
	   }else{
	   	   crc_1byte>>=1;
	   }
	   abyte>>=1;
   }
   return (crc_1byte);
}
unsigned char crc8_verify(unsigned char *p, unsigned char len)
{
	unsigned char crc = 0,*ptr = p;
	while(len--){
		//LogMI("*ptr = %X",*ptr);
		crc = calcrc_1byte(crc^*ptr++);
	}
	//LogMI("crc = %X",crc);
	return(crc);
}
