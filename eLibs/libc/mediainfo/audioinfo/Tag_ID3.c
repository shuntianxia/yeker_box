/*
*   2008-12-02:修改跳过TEXT ENCODERing 00 flag
*   2008-12-03 : 修正ID3V2的genre
*/

//#include	"auinparser.h"
#include	"bitstream.h"	
#include    "tagInfo.h"
/*
*	ID3 V1
*
*/
//#define ReadUTF8(x,y,z) ReadNunicode(x,y,z)
#define IMAGE_MIME_TYPE_BMP     "image/bmp"
#define IMAGE_MIME_TYPE_JPEG	"image/jpeg"
#define IMAGE_MIME_TYPE_PNG		"image/png"
static const char ID3V1GENRE[148+2][22]=
{
	"Blues",
	"ClassicRock",
	"Country",
	"Dance",
	"Disco",
	"Funk",
	"Grunge",
	"Hip-Hop",
	"Jazz",
	"Metal",
	"NewAge",
	"Oldies",
	"Other",
	"Pop",
	"R&B",
	"Rap",
	"Reggae",
	"Rock",
	"Techno",
	"Industrial",
	"Alternative",
	"Ska",
	"DeathMetal",
	"Pranks",
	"Soundtrack",
	"Euro-Techno",
	"Ambient",
	"Trip-Hop",
	"Vocal",
	"Jazz+Funk",
	"Fusion",
	"Trance",
	"Classical",
	"Instrumental",
	"Acid",
	"House",
	"Game",
	"SoundClip",
	"Gospel",
	"Noise",
	"AlternRock",
	"Bass",
	"Soul",
	"Punk",
	"Space",
	"Meditative",
	"InstrumentalPop",
	"InstrumentalRock",
	"Ethnic",
	"Gothic",
	"Darkwave",
	"Techno-Industrial",
	"Electronic",
	"Pop-Folk",
	"Eurodance",
	"Dream",
	"SouthernRock",
	"Comedy",
	"Cult",
	"Gangsta",
	"Top40",
	"ChristianRap",
	"Pop/Funk",
	"Jungle",
	"NativeAmerican",
	"Cabaret",
	"NewWave",
	"Psychadelic",
	"Rave",
	"Showtunes",
	"Trailer",
	"Lo-Fi",
	"Tribal",
	"AcidPunk",
	"AcidJazz",
	"Polka",
	"Retro",
	"Musical",
	"Rock&Roll",
	"HardRock",
 /* Extended genres */
 	"Folk",
 	"Folk-Rock",
 	"NationalFolk",
 	"Swing",
 	"FastFusion",
 	"Bebob",
 	"Latin",
 	"Revival",
 	"Celtic",
 	"Bluegrass",
 	"Avantgarde",
 	"GothicRock",
 	"ProgessiveRock",
 	"PsychedelicRock",
 	"SymphonicRock",
 	"SlowRock",
 	"BigBand",
 	"Chorus",
 	"EasyListening",
 	"Acoustic",
 	"Humour",
 	"Speech",
 	"Chanson",
 	"Opera",
 	"ChamberMusic",
 	"Sonata",
 	"Symphony",
 	"BootyBass",
 	"Primus",
 	"PornGroove",
 	"Satire",
 	"SlowJam",
 	"Club",
 	"Tango",
 	"Samba",
 	"Folklore",
 	"Ballad",
 	"PowerBallad",
 	"RhythmicSoul",
 	"Freestyle",
 	"Duet",
 	"PunkRock",
 	"DrumSolo",
 	"Acapella",
 	"Euro-House",
 	"DanceHall",
 	"Goa",
 	"Drum&Bass",
 	"Club-House",
 	"Hardcore",
 	"Terror",
 	"Indie",
 	"BritPop",
 	"Negerpunk",
 	"PolskPunk",
 	"Beat",
 	"ChristianGangstaRap",
 	"HeavyMetal",
 	"BlackMetal",
 	"Crossover",
 	"ContemporaryChristian",
 	"ChristianRock",
 	"Merengue",
 	"Salsa",
 	"TrashMetal",
 	"Anime",
 	"JPop",
 	"Synthpop",
	"Remix",
	"Cover"
};
#define ID3TAGNUM 8
static const unsigned	int ID3UniqueTag[2][ID3TAGNUM]=
{
	{ //ID3V2.3 2.4
		0x54495432,//TIT2
		0x54504531,//TPE1
		0x54414C42,//TALB
		0x54594552,//TYER
		0x54434f4e,//TCON
		0x41504943,//APIC
		0x434f4d4d,//COMM
		0x544c414e//TLAN
	},
	{ //ID3V2.1 
		0x545432,//TT2
		0x545031,//TP1
		0x54414C,//TAL
		0x545945,//TYE
		0x54434f,//TCO
		0x415049,//API
		0x434f4d,//COM
		0x544c41//TLA
	}
};
//static char GenreTemp[512];
static void	ReadNunicode(__s32 *FrameLen,__s8** Frame, __audio_file_info_t *AIF,int frameTLen);
static void	Readunicode(__s32 *FrameLen,__s8** Frame, __audio_file_info_t *AIF,int frameTLen);
int GetInfo_GETID3_Len(unsigned char *ptr,int len)
{
    int Id3v2len = 0;
	if(len<10)
	{
		return 0;
	}
	if((ptr[0]==0x49)&&(ptr[1]==0x44)&&(ptr[2]==0x33))
	{
		Id3v2len = ptr[6]<<7 | ptr[7];
		Id3v2len = Id3v2len<<7 | ptr[8];
		Id3v2len = Id3v2len<<7 | ptr[9];
		Id3v2len += 10;
		if (ptr[5] & 0x10)
           Id3v2len += 10;
	}
    return Id3v2len;
	
}
unsigned	int	GetInfo_GETID3_V2(__audio_file_info_t *AIF)  //IF HAVE RETURN ID3 tag length
{
	int Id3v2len;
	int Id3Version=0;
	unsigned	int	Id3v2lenRet=0;
	unsigned	int BsVal;
	unsigned	int temp[10];

	char	data;
//	char    data1;
	int		i;
	int		frameinfoseg=0;
	int     TxtEncodeflag;
	int ulLanguage_encoding = 0;
//	int     UnicodeBOM;
	
	BsInfo      *BSINFO = (BsInfo *)AIF->ulBSINFO;
	struct	
	{
		unsigned	int UID;
		int			length;
		short		flag;

	}ID3FRAME;
	PRINT("cky debug3\n");
	BsVal = GetInfo_ExtractI4(BSINFO->bsbufptr);
	BsVal >>=8;
	if(BsVal == 0x494433/*ID3*/)  //ID3v2/file identifier   "ID3" 
	{	
		PRINT("cky debug4\n");
		BsVal = GetInfo_GetBsInByte(3,AIF);

		Id3Version=GetInfo_GetBsInByte(2,AIF);//ID3v2 version         
        Id3Version &= 0xff00;//ver and revision,now use ver
		if(Id3Version==0x200)
			Id3Version =1;
		else if((Id3Version ==0x300) ||(Id3Version ==0x400))
			Id3Version = 0;
		else//for error and future;20100825lszhang
      Id3Version = 0;


		temp[5] = GetInfo_GetBsInByte(1,AIF);//ID3v2 flags             
		temp[3] = GetInfo_GetBsInByte(1,AIF);
		temp[2] = GetInfo_GetBsInByte(1,AIF);
		temp[1] = GetInfo_GetBsInByte(1,AIF);
		temp[0] = GetInfo_GetBsInByte(1,AIF);
		Id3v2len = temp[3]<<7 | temp[2];
		Id3v2len = Id3v2len<<7 | temp[1];
		Id3v2len = Id3v2len<<7 | temp[0];
		//Id3v2len +=0xa;//head length	
		Id3v2lenRet = Id3v2len;
		if (temp[5] & 0x10)
           Id3v2lenRet += 10;
		while((Id3v2len>=10)&&(frameinfoseg<ID3TAGNUM))
		{
			if(Id3Version == 1) //ID3V2.2
			{
				ID3FRAME.UID = GetInfo_GetBsInByte(3,AIF);
				ID3FRAME.length = GetInfo_GetBsInByte(3,AIF);
				//ID3FRAME.flag = GetInfo_GetBsInByte(1,AIF);
				Id3v2len -=6;				
			}
			else
			{
				ID3FRAME.UID = GetInfo_GetBsInByte(4,AIF);
				ID3FRAME.length = GetInfo_GetBsInByte(4,AIF);
				ID3FRAME.flag = GetInfo_GetBsInByte(2,AIF);
				Id3v2len -=10;
			}
			if(GetInfo_return<2)break;//no data;
			if(ID3FRAME.length<0)break;
			if(ID3FRAME.length==0)continue;
		PRINT("cky UID[%x]\n",ID3FRAME.UID);
			if(ID3FRAME.UID == ID3UniqueTag[Id3Version][0]/* 0x54495432*/) /*TIT2*/
			{	//read tile
				frameinfoseg++;
				TxtEncodeflag=GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				
				switch(TxtEncodeflag)
				{
				case 0:
					ReadNunicode(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);
					if(AIF->InforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                AIF->InforBufLeftLength -= (ID3FRAME.length-1);
					AIF->InforBuf          += (ID3FRAME.length-1);
	
/*
					AIF->ultitle_sz = ID3FRAME.length-1;
					AIF->ultitle = AIF->InforBuf;
					if(AIF->InforBufLeftLength<AIF->ultitle_sz)
						break;
					AIF->InforBufLeftLength -=AIF->ultitle_sz;
					
					for(i=0;i<AIF->ultitle_sz;i++)
					{
						data= (char )GetInfo_GetBsInByte(1,AIF);
						*AIF->InforBuf++= data;
					}
*/					
					break;
				case 1:
					AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Readunicode(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);
/*
					AIF->ultitle_sz = ID3FRAME.length-1;
					AIF->ultitle = AIF->InforBuf;
					AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					if(AIF->InforBufLeftLength<AIF->ultitle_sz)
						break;
					AIF->InforBufLeftLength -=AIF->ultitle_sz;
					UnicodeBOM = GetInfo_GetBsInByte(2,AIF);

					for(i=0;i<AIF->ultitle_sz-2;i++)
					{
						data= (char )GetInfo_GetBsInByte(1,AIF);
						*AIF->InforBuf++= data;
					}
					
*/					break;
				case 2:
					AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Readunicode(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);
					break;
				case 3:
					AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					ReadNunicode(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);// ReadUTF8(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);
					break;
				default:
					GetInfo_SkipBsInByte(ID3FRAME.length-1,AIF);
					
				}

				Id3v2len -=ID3FRAME.length;
			}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][1]/*0x54504531*/) /*TPE1*/
			{
				frameinfoseg++;
				TxtEncodeflag=GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				switch(TxtEncodeflag)
				{
				case 0:
					ReadNunicode(&AIF->ulauthor_sz,&AIF->ulauthor,AIF,ID3FRAME.length);
					if(AIF->InforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                AIF->InforBufLeftLength -= (ID3FRAME.length-1);
					AIF->InforBuf          += (ID3FRAME.length-1);
					break;
				case 1:
					AIF->ulauthorCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Readunicode(&AIF->ulauthor_sz,&AIF->ulauthor,AIF,ID3FRAME.length);
					break;
				case 2:
					AIF->ulauthorCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Readunicode(&AIF->ulauthor_sz,&AIF->ulauthor,AIF,ID3FRAME.length);
					break;
				case 3:
					AIF->ulauthorCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					ReadNunicode(&AIF->ulauthor_sz,&AIF->ulauthor,AIF,ID3FRAME.length);// ReadUTF8(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);
					break;
				default:
					GetInfo_SkipBsInByte(ID3FRAME.length-1,AIF);					
				}
				Id3v2len -=ID3FRAME.length;
/*
				GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				AIF->ulauthor_sz = ID3FRAME.length-1;
				AIF->ulauthor = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ulauthor_sz)
					break;
				AIF->InforBufLeftLength -=AIF->ulauthor_sz;
				for(i=0;i<AIF->ulauthor_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					*AIF->InforBuf++= data;
				}
				Id3v2len -=ID3FRAME.length;
*/
			}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][2]/*0x54414C42*/) /*TALB*/
			{
				frameinfoseg++;
				TxtEncodeflag=GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				switch(TxtEncodeflag)
				{
				case 0:
					ReadNunicode(&AIF->ulAlbum_sz,&AIF->ulAlbum,AIF,ID3FRAME.length);
					if(AIF->InforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                AIF->InforBufLeftLength -= (ID3FRAME.length-1);
					AIF->InforBuf          += (ID3FRAME.length-1);
					break;
				case 1:
					AIF->ulAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Readunicode(&AIF->ulAlbum_sz,&AIF->ulAlbum,AIF,ID3FRAME.length);
					break;
				case 2:
					AIF->ulAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Readunicode(&AIF->ulAlbum_sz,&AIF->ulAlbum,AIF,ID3FRAME.length);
					break;
				case 3:
					AIF->ulAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					ReadNunicode(&AIF->ulAlbum_sz,&AIF->ulAlbum,AIF,ID3FRAME.length);// ReadUTF8(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);
					break;
				default:
					GetInfo_SkipBsInByte(ID3FRAME.length-1,AIF);
					
				}
				Id3v2len -=ID3FRAME.length;
/*
				GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				AIF->ulAlbum_sz = ID3FRAME.length-1;
				AIF->ulAlbum = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ulAlbum_sz)
					break;
				AIF->InforBufLeftLength -=AIF->ulAlbum_sz;
				for(i=0;i<AIF->ulAlbum_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					*AIF->InforBuf++= data;
				}
				Id3v2len -=ID3FRAME.length;
*/			}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][3]/*0x54594552*/) /*TYER*/
			{
				frameinfoseg++;
				TxtEncodeflag=GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				switch(TxtEncodeflag)
				{
				case 0:
					ReadNunicode(&AIF->ulYear_sz,&AIF->ulYear,AIF,ID3FRAME.length);
					if(AIF->InforBufLeftLength< (ID3FRAME.length-1))
		              break;
	                AIF->InforBufLeftLength -= (ID3FRAME.length-1);
					AIF->InforBuf          += (ID3FRAME.length-1);
					break;
				case 1:
					AIF->ulYearCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					Readunicode(&AIF->ulYear_sz,&AIF->ulYear,AIF,ID3FRAME.length);
					break;
				case 2:
					AIF->ulYearCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					Readunicode(&AIF->ulYear_sz,&AIF->ulYear,AIF,ID3FRAME.length);
					break;
				case 3:
					AIF->ulYearCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					ReadNunicode(&AIF->ulYear_sz,&AIF->ulYear,AIF,ID3FRAME.length);// ReadUTF8(&AIF->ultitle_sz,&AIF->ultitle,AIF,ID3FRAME.length);
					break;
				default:
					GetInfo_SkipBsInByte(ID3FRAME.length-1,AIF);
					
				}
				Id3v2len -=ID3FRAME.length;
/*
				GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				AIF->ulYear_sz = ID3FRAME.length-1;
				AIF->ulYear = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ulYear_sz)
					break;
				AIF->InforBufLeftLength -=AIF->ulYear_sz;
				for(i=0;i<AIF->ulYear_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					*AIF->InforBuf++= data;
				}
				Id3v2len -=ID3FRAME.length;
*/
			}

			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][4]/*0x54434f4e*/) /*TCON*/
			{
				//char GenreTemp[512];
				__s8 *GenreTemp = AIF->InforBuf;//for use AIF->InforBuf buffer
				int GenreLen,ReadLen=10;
				unsigned int GenreIndex;

				frameinfoseg++;
				TxtEncodeflag=GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				GenreLen = ID3FRAME.length-1;
				Id3v2len -=ID3FRAME.length;
				//if(ReadLen>GenreLen)
					ReadLen = GenreLen;
				if(ReadLen>AIF->InforBufLeftLength)
				{
					ReadLen = AIF->InforBufLeftLength;
				}

				if(ReadLen == 0)
				{	
					continue;
				}
				for(i=0;i<ReadLen;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					GenreTemp[i]= data;
				}				
				GenreLen -= ReadLen;
				GetInfo_SkipBsInByte(GenreLen,AIF);

				if(GenreTemp[0]=='(')
				{
					switch(TxtEncodeflag)
					{
					case 0:
						break;
					case 1:
					case 3:
						for(i=0;i<((ReadLen-2)/2);i++)
						{
							GenreTemp[i]= GenreTemp[2*i+2];//skip BOM
						}
						ReadLen = (ReadLen-2)/2;
						break;
						
					case 2:
						for(i=0;i<((ReadLen-2)/2);i++)
						{
							GenreTemp[i]= GenreTemp[2*i+1+2];//skip BOM
						}
						ReadLen = (ReadLen-2)/2;
						break;
						
					}
					
					if((GenreTemp[1]=='R')&&(GenreTemp[2]=='X'))
						GenreIndex = 148;
					else if((GenreTemp[1]=='C')&&(GenreTemp[2]=='R'))
						GenreIndex = 149;
					else
					{	
						i = 1;
						GenreIndex = 0;
						//while(i<ReadLen)
						while((i<ReadLen)&&(GenreIndex<148+2))
						{	
							if(GenreTemp[i]==')')
								break;
							GenreIndex *=10;
							GenreIndex += (int)((GenreTemp[i]-0x30)&0xff);
							i++;
						}
						if(GenreIndex<148+2)
						{
							AIF->ulGenreCharEncode = A_AUDIO_FONTTYPE_ISOIEC8859_1;
							AIF->ulGenre_sz = 22;
							AIF->ulGenre = AIF->InforBuf;
							if(AIF->InforBufLeftLength<22)
								return 0;
							AIF->InforBufLeftLength -=22;			
							
							memcpy(AIF->ulGenre,ID3V1GENRE[GenreIndex],22);
							AIF->ulGenre_sz=GetInfo_StringLen((char*)AIF->ulGenre);
							AIF->InforBuf += 22;
						}
						
					}
				}else{
					AIF->ulGenreCharEncode = ( __a_audio_fonttype_e)TxtEncodeflag;
					AIF->ulGenre_sz =  ReadLen;	
					AIF->ulGenre = AIF->InforBuf;
					i = 0;
					if((TxtEncodeflag==1)||(TxtEncodeflag==2))
					{
						i=2;
						AIF->ulGenre_sz -=2;
					}
					AIF->InforBufLeftLength -= ReadLen - i;
					for(;i<ReadLen;i++)
					{
						*AIF->InforBuf++ = GenreTemp[i];
					}
				}
/*
				AIF->ulGenre_sz = ID3FRAME.length-1;
				AIF->ulGenre = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ulGenre_sz)
					break;
				AIF->InforBufLeftLength -=AIF->ulGenre_sz;
				for(i=0;i<AIF->ulGenre_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1);
					*AIF->InforBuf++= data;
				}
				Id3v2len -=ID3FRAME.length;
*/
			}
			
			else if(ID3FRAME.UID == ID3UniqueTag[Id3Version][5]/*0x41504943*/) /*APIC*/
			{
				char *p = 0;
				__s8 *GenreTemp = AIF->InforBuf;//for use AIF->InforBuf buffer
				__id3_image_info_t image;
				frameinfoseg++;
				PRINT("cky1\n");
				TxtEncodeflag=GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				Id3v2len -=ID3FRAME.length;
				ID3FRAME.length -= 1;
				memset(&image,0,sizeof(__id3_image_info_t));
				if(ID3FRAME.length == 0)continue;
				    
				i = 0;
				do
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					GenreTemp[i]= data;
					i++;
				}while(data != 0);
				p = (char*)GenreTemp;
				if(!strcmp(p, IMAGE_MIME_TYPE_BMP))
				{
				   image.img_format = IMG_FORMAT_BMP;
				}
				else if(!strcmp(p, IMAGE_MIME_TYPE_JPEG))
				{
				   image.img_format = IMG_FORMAT_JPG;
				}
				else if(!strcmp(p, IMAGE_MIME_TYPE_PNG))
				{
				   image.img_format = IMG_FORMAT_PNG;
				}
				else
				{
				   image.img_format = IMG_FORMAT_UNSUPPORT;
				}
					PRINT("cky2[%d]\n",image.img_format);
				ID3FRAME.length -=  i;
				if(ID3FRAME.length == 0)continue;
				    image.pic_type = GetInfo_GetBsInByte(1,AIF);
				ID3FRAME.length -= 1;				
				if(ID3FRAME.length == 0)continue;
				    
				i = 0;
				do
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					if((TxtEncodeflag ==1)||(TxtEncodeflag ==2))
					{
						data |= (char )GetInfo_GetBsInByte(1,AIF);
						i++;
					}
					i++;
				}while(data != 0);

				switch(TxtEncodeflag)
				{
				case 0:
					
					break;
				case 1:
					AIF->ulAPicCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					//GetInfo_GetBsInByte(1,AIF);
					//i++;
					break;
				case 2:
					AIF->ulAPicCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;//2;
					//GetInfo_GetBsInByte(1,AIF);
					//i++;
					break;
				case 3:
					AIF->ulAPicCharEncode = A_AUDIO_FONTTYPE_UTF_8;
					
					break;
				default:
					;
					
				}
				PRINT("cky3[%d]\n",TxtEncodeflag);
				ID3FRAME.length -=i;
				if(ID3FRAME.length == 0)continue;
				AIF->ulAPic_sz = sizeof(__id3_image_info_t);
				image.length = ID3FRAME.length;
				image.FileLocation = GetInfo_GetFileLocation(AIF);
				if(image.FileLocation < 0)image.FileLocation = 0;
				AIF->InforBufLeftLength -= 4-((int)(AIF->InforBuf)&0x3);
				AIF->InforBuf += 4-((int)(AIF->InforBuf)&0x3);
				AIF->ulAPic = (__id3_image_info_t*)(AIF->InforBuf);
				if(AIF->InforBufLeftLength<AIF->ulAPic_sz)
					break;				
				AIF->InforBufLeftLength -=AIF->ulAPic_sz;
				memcpy(AIF->ulAPic,&image,AIF->ulAPic_sz);
				AIF->InforBuf += AIF->ulAPic_sz;
				GetInfo_SkipBsInByte(ID3FRAME.length,AIF);
			PRINT("cky3[%d][0x%x]\n",image.length,image.FileLocation);

				
/*
				GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				AIF->ulYear_sz = ID3FRAME.length-1;
				AIF->ulYear = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ulYear_sz)
					break;
				AIF->InforBufLeftLength -=AIF->ulYear_sz;
				for(i=0;i<AIF->ulYear_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					*AIF->InforBuf++= data;
				}
				Id3v2len -=ID3FRAME.length;
*/
			}
			else if((ID3FRAME.UID == ID3UniqueTag[Id3Version][6]/*0x434f4d4d*/) /*COMM*/
				    ||(ID3FRAME.UID == ID3UniqueTag[Id3Version][6]/*0x544c414e*/) /*TLAN*/)
			{
				char ulcomm[4] ={0,0,0,0} ;
				

				frameinfoseg++;
				TxtEncodeflag=GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				ulcomm[0] = GetInfo_GetBsInByte(1,AIF);
				ulcomm[1] = GetInfo_GetBsInByte(1,AIF);
				ulcomm[2] = GetInfo_GetBsInByte(1,AIF);
				ulLanguage_encoding = GetInfo_Language_encoding(ulcomm);
				GetInfo_SkipBsInByte(ID3FRAME.length-1-3,AIF);
				Id3v2len -=ID3FRAME.length;
/*
				GetInfo_GetBsInByte(1,AIF);//TEXT ENCODERing 00
				AIF->ulauthor_sz = ID3FRAME.length-1;
				AIF->ulauthor = AIF->InforBuf;
				if(AIF->InforBufLeftLength<AIF->ulauthor_sz)
					break;
				AIF->InforBufLeftLength -=AIF->ulauthor_sz;
				for(i=0;i<AIF->ulauthor_sz;i++)
				{
					data= (char )GetInfo_GetBsInByte(1,AIF);
					*AIF->InforBuf++= data;
				}
				Id3v2len -=ID3FRAME.length;
*/
			}
			else
			{
				if(Id3v2len<ID3FRAME.length)
					ID3FRAME.length = Id3v2len;
				GetInfo_SkipBsInByte(ID3FRAME.length,AIF);
				Id3v2len -=ID3FRAME.length;
			}
      
		} 
		  
		GetInfo_SkipBsInByte(Id3v2len,AIF);
		Id3v2lenRet +=10;
	} 
	if(ulLanguage_encoding==A_AUDIO_FONTTYPE_WINDOWS_1251)
	{
			GetInfo_encoding_change(AIF);
	}
	//for other ID3
	BsVal = GetInfo_ExtractI4(BSINFO->bsbufptr);
	BsVal >>=8;
	while(BsVal == 0x494433/*ID3*/) 
	{
		unsigned char *p = (unsigned char*)temp;
		for(i=0;i<10;i++)
		{
			*p++ = GetInfo_ShowBs(i,1,AIF);
		}
		i =  GetInfo_GETID3_Len((unsigned char*)temp,10);
		if(i<0)
		{
			break;
		}
		GetInfo_SkipBsInByte(i,AIF);
		Id3v2lenRet +=i;
		BsVal = GetInfo_ExtractI4(BSINFO->bsbufptr);
		BsVal >>=8;
	}
  PRINT("Id3v2lenRet = %d\n",Id3v2lenRet);    
	return	Id3v2lenRet;
}     
      
unsigned	int	GetInfo_GETID3_V1(__audio_file_info_t *AIF)  //IF HAVE RETURN ID3 tag length
{     
	int BsVal;
	int RetVal=0;
	int i;
	__u32 GEN=0;
	char data;
		BsVal = GetInfo_GetBsInByte(3,AIF);
      
		if(BsVal == 0x544147)
		{ 
			RetVal = 1;
			if(AIF->ultitle_sz)
			{
			    GetInfo_SkipBsInByte(30,AIF);  
			}
			else
			{
////tile
    			AIF->ultitle_sz = 30;
    			AIF->ultitle = AIF->InforBuf;
    			if(AIF->InforBufLeftLength<30)
    					return 0;
    			AIF->InforBufLeftLength -=30;
    			for(i=0;i<30;i++)
    			{
    				data= (char )GetInfo_GetBsInByte(1,AIF);
    				*AIF->InforBuf++= data;
    			}
		    }
		    
//////author
            if(AIF->ulauthor_sz)
			{
			    GetInfo_SkipBsInByte(30,AIF);  
			}
			else
			{
    			AIF->ulauthor_sz = 30;
    			AIF->ulauthor = AIF->InforBuf;
    			if(AIF->InforBufLeftLength<30)
    				return 0;
    			AIF->InforBufLeftLength -=30;
    			for(i=0;i<AIF->ulauthor_sz;i++)
    			{
    				data= (char )GetInfo_GetBsInByte(1,AIF);
    				*AIF->InforBuf++= data;
    			}
    		}
//////Album
            if(AIF->ulAlbum_sz)
			{
			    GetInfo_SkipBsInByte(30,AIF);  
			}
			else
			{
    			AIF->ulAlbum_sz	= 30;
    			AIF->ulAlbum	= AIF->InforBuf;
    			if(AIF->InforBufLeftLength<30)
    				return 0;
    			AIF->InforBufLeftLength -=30;
    			for(i=0;i<AIF->ulAlbum_sz;i++)
    			{
    				data= (char )GetInfo_GetBsInByte(1,AIF);
    				*AIF->InforBuf++= data;
    			}
    		}
//////Year
            if(AIF->ulYear_sz)
			{
			    GetInfo_SkipBsInByte(4,AIF);  
			}
			else
			{
    			AIF->ulYear_sz	= 4;
    			AIF->ulYear		= AIF->InforBuf;
    			if(AIF->InforBufLeftLength<4)
    				return 0;
    			AIF->InforBufLeftLength -=4;
    			for(i=0;i<AIF->ulYear_sz;i++)
    			{
    				data= (char )GetInfo_GetBsInByte(1,AIF);
    				*AIF->InforBuf++= data;
    			}
    		}
			

/////////////////gener
            GetInfo_SkipBsInByte(30,AIF);
            if(AIF->ulGenre_sz)
			{
			    ;//nothing  
			}
			else
			{
			//GetInfo_SkipBsInByte(64);
    			
    			GEN= GetInfo_GetBsInByte(1,AIF);
    			if(GEN<148)
    			{
    				AIF->ulGenre_sz = 22;
    				AIF->ulGenre = AIF->InforBuf;
    				if(AIF->InforBufLeftLength<22)
    					return 0;
    				AIF->InforBufLeftLength -=22;			
    	
    				memcpy(AIF->ulGenre,ID3V1GENRE[GEN],22);
    				AIF->ulGenre_sz=GetInfo_StringLen((char*)AIF->ulGenre);
    			}
    		}
		}
		return	RetVal;

}

static void	ReadNunicode(__s32 *FrameLen,__s8** Frame, __audio_file_info_t *AIF,int frameTLen)
{
	char data;
	int i;
	*FrameLen = frameTLen -1 ;
	*Frame = AIF->InforBuf;//
	
	if(AIF->InforBufLeftLength< *FrameLen)
		return;
	AIF->InforBufLeftLength -= *FrameLen;
	
	for(i=0;i<*FrameLen;i++)
	{
		data= (char )GetInfo_GetBsInByte(1,AIF);
		*AIF->InforBuf++= data;
	}
	return;
	/*
	AIF->ultitle_sz = ID3FRAME.length-1;
	AIF->ultitle = AIF->InforBuf;
	if(AIF->InforBufLeftLength<AIF->ultitle_sz)
	break;
	AIF->InforBufLeftLength -=AIF->ultitle_sz;
	
	  for(i=0;i<AIF->ultitle_sz;i++)
	  {
						data= (char )GetInfo_GetBsInByte(1,AIF);
						*AIF->InforBuf++= data;
						}
	*/
};

static void	Readunicode(__s32 *FrameLen,__s8** Frame, __audio_file_info_t *AIF,int frameTLen)
{
	int UnicodeBOM;
	char data,data1,data2;
	int i;
	if(frameTLen<4)
		{
			*FrameLen = 0;
			GetInfo_SkipBsInByte(frameTLen-1,AIF);
			return;
		}
	*FrameLen = frameTLen -1-2 ;//BOM LEN
	*Frame = AIF->InforBuf;//
	
	if(AIF->InforBufLeftLength< *FrameLen)
		return;
	AIF->InforBufLeftLength -= *FrameLen;
	UnicodeBOM = GetInfo_GetBsInByte(2,AIF);

	for(i=0;i<(*FrameLen)/2;i++)
	{
		data= (char )GetInfo_GetBsInByte(1,AIF);
		data1= (char )GetInfo_GetBsInByte(1,AIF);
		if(UnicodeBOM == 0xfeff)
		{
			data2=data;
			data=data1;
			data1=data2;
		}
		*AIF->InforBuf++= data;
		*AIF->InforBuf++= data1;
	}
	GetInfo_SkipBsInByte(*FrameLen&0x01,AIF);
	return;
	/*
					AIF->ultitle_sz = ID3FRAME.length-1;
					AIF->ultitle = AIF->InforBuf;
					AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
					if(AIF->InforBufLeftLength<AIF->ultitle_sz)
						break;
					AIF->InforBufLeftLength -=AIF->ultitle_sz;
					UnicodeBOM = GetInfo_GetBsInByte(2,AIF);

					for(i=0;i<AIF->ultitle_sz-2;i++)
					{
						data= (char )GetInfo_GetBsInByte(1,AIF);
						*AIF->InforBuf++= data;
					}
					
					break;
	*/
};
