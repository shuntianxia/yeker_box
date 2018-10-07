//#include	"auinparser.h"
#include	"bitstream.h"	

unsigned int GetInfo_oggStandardTag(__audio_file_info_t *AIF)
{
	int retVal;
	
	int i,k,comment;
	unsigned char string;
	int maxflag =0;//20100818for ogg get tag buffer data not file;
	__s8 *pString;
	BsInfo *BSINFO = (BsInfo *)AIF->ulBSINFO;
	if(BSINFO->bsbufLen>(READFILELEN+LEFTFILELEN))
		maxflag = 1;
	retVal = GetInfo_GetBsInByteLI(4,AIF);//get vendorlen
	GetInfo_SkipBsInByte(retVal,AIF);
	comment = GetInfo_GetBsInByteLI(4,AIF);//get comments
	for(i=0;i<comment;i++)
	{
		retVal = GetInfo_GetBsInByteLI(4,AIF);
		if(maxflag)
		{
			if(BSINFO->bsbufLen<retVal)
				return 0;
		}
		if(AIF->InforBufLeftLength<retVal)
		{
			GetInfo_SkipBsInByte(retVal,AIF);
		}
		else
		{
			AIF->InforBufLeftLength -= retVal;
			pString = AIF->InforBuf;
			for(k=0;k<retVal;k++)
			{
				string = GetInfo_GetBsInByteLI(1,AIF);
				*AIF->InforBuf++= string;
				
			}
			if(((pString[0] =='g')|(pString[0] =='G'))&((pString[1] =='e')|(pString[1] =='E'))&((pString[2] =='n')|(pString[2] =='N'))&((pString[3] =='r')|(pString[3] =='R'))&((pString[4] =='e')|(pString[4] =='E')))
			{
				AIF->ulGenre_sz = retVal-6;
				AIF->ulGenre    = ( __s8*)(pString+6);
				AIF->ulGenreCharEncode = A_AUDIO_FONTTYPE_UTF_8;
			} 
			else if(((pString[0] =='t')|(pString[0] =='T'))&((pString[1] =='i')|(pString[1] =='I'))&((pString[2] =='t')|(pString[2] =='T'))&((pString[3] =='l')|(pString[3] =='L'))&((pString[4] =='e')|(pString[4] =='E')))
			{ 
				AIF->ultitle_sz = retVal-6;
				AIF->ultitle    = ( __s8*)(pString+6);
				AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_8; 
			}
			else if(((pString[0] =='a')|(pString[0] =='A'))&((pString[1] =='r')|(pString[1] =='R'))&((pString[2] =='t')|(pString[2] =='T'))&((pString[3] =='i')|(pString[3] =='I'))&((pString[4] =='s')|(pString[4] =='S'))&((pString[5] =='t')|(pString[5] =='T')))
			{
				AIF->ulauthor_sz = retVal-7;
				AIF->ulauthor    = ( __s8*)(pString+7);
				AIF->ulauthorCharEncode = A_AUDIO_FONTTYPE_UTF_8;
			}
			else if(((pString[0] =='a')|(pString[0] =='A'))&((pString[1] =='l')|(pString[1] =='L'))&((pString[2] =='b')|(pString[2] =='B'))&((pString[3] =='u')|(pString[3] =='U'))&((pString[4] =='m')|(pString[4] =='M')))
			{
				AIF->ulAlbum_sz = retVal-6;
				AIF->ulAlbum    = ( __s8*)(pString+6);
				AIF->ulAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_8;
			}
			else if(((pString[0] =='d')|(pString[0] =='D'))&((pString[1] =='a')|(pString[1] =='A'))&((pString[2] =='t')|(pString[2] =='T'))&((pString[3] =='e')|(pString[3] =='E')))
			{
				AIF->ulYear_sz = retVal-5;
				AIF->ulYear    = ( __s8*)(pString+5);
				AIF->ulYearCharEncode  = A_AUDIO_FONTTYPE_UTF_8; 
			}
			else
			{
				AIF->InforBufLeftLength += retVal;
				AIF->InforBuf = pString;
			}
			//*AIF->InforBuf++;//FOR last bytes=\0 	
		}

	}
    return 1;

}
