#include	"bitstream.h"

int	GetInfo_GETTAG_VBRI(char *ptr)
{
	int	VBRIFrame=0;
	int	flag;
	ptr +=32;
	flag=GetInfo_ExtractI4(ptr);
	if(flag == 0x56425249)/*VBRI*/
	{
		VBRIFrame=GetInfo_ExtractI4(ptr+14);

	}
	return VBRIFrame;
}
