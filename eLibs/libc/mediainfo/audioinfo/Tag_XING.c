
#include	"bitstream.h"


int	GetInfo_GETTAG_XING(int Mpeg12flag/*0==mpeg1  other==mpeg2/2.5*/,int channel,char *ptr)
{
	int		XingFrame=0;
	int		flag;
	if(Mpeg12flag==0)  
	{//mpeg1
		if(channel == 1)  
			ptr+=(17+4);
		else
			ptr+=(32+4);
	}
	else
	{
	//mpeg2
		if(channel == 1)  
			ptr+=(9+4);
		else
			ptr+=(17+4);
	
	}

	flag=GetInfo_ExtractI4(ptr);
	if(flag == 0x58696e67)/*Xing*/
	{
		flag=GetInfo_ExtractI4(ptr+4);
		if((flag &0x1) == 1)
		{
			XingFrame = GetInfo_ExtractI4(ptr+8);
		}
	}

	return XingFrame;
} 


