/************************************************************************************
 *          Get APETAG information
 * FileName:Tag_APE.c     Author:yangfuyang  Date:2011.08.10
 *Discription: 该文件提供的接口函数为：int GetInfo_APETAG(__audio_file_info_t *AIF)
               能够获得音频文件中APETAGV1及APETAGV2的标签内容，通过AIF所指向的结构体
               返回所获得的内容。
**************************************************************************************/
#include "osPlatform.h"
#include	"bitstream.h"	
#include    "tagInfo.h"
static int	ReadItemValue(__s8** ItemName, __audio_file_info_t *AIF,int ItemSize);
static int ape_get_str(__audio_file_info_t *AIF, int maxlen, char *buf, int buflen);
/*APEV2标签头或标签尾结构体*/
typedef struct APETagHF
{
	char headname[8];                           //头名称APETAGEX
	char version[4];                            //版本
	int tagsize;                              //标签大小，不包括标签头
	int itemcount;                            //标签元素个数
	char tagflags[4];                           //标签标志
	char reserved[8];                           //标签保留，值全为零
}APEHF;

/*APEV2标签元素结构体*/
typedef struct APETagItem 
{
	int valuesize;                             //标签元素内容的大小
	char itemflags[4];                           //标签元素标志
	char itemkey[256];                           //标签元素关键字                            
}APEItem;

extern char *eLIBs_strchrlast(char *pstr, char srch_char);
int GetInfo_APETAG(__audio_file_info_t *AIF)
{
	char c;
    char filename[1024] = {0};
    char *file_ext = NULL;
	int i;
	int j;
	int flag;                         //标志，0：找不到尾标签，1：找到尾标签
	APEHF header;
	APEItem Item;
    __id3_image_info_t image;
	FILE *fp;
	flag = 0;
	fp = ((BsInfo *)AIF->ulBSINFO)->ReadFile;
	memset(&header,0,sizeof(APEHF));

	if (fp == NULL)
	{
		//printf("The file pointer is NULL!\n");
		return 0;
	}

	/*从文件尾开始搜索APE文件尾标签*/
	fseek(fp,-32L,SEEK_END);
	fread(&header,sizeof(APEHF),1,fp); 
    //PRINT("header>>headname:%s, tagsize:%x, itemcount:%x\n", header.headname,header.tagsize,header.itemcount);
	if (strncmp(header.headname,"APETAGEX",8) == 0)
	{
		flag = 1;
		//printf("this media has an apetag footer!\n");
		fseek(fp,-header.tagsize,SEEK_END);

	}
	else
	{
		fseek(fp,-(32L + 128L),SEEK_END);                     //可能存在ID3V1，再向前128字节查找
		fread(&header,sizeof(APEHF),1,fp); 

		if (strncmp(header.headname,"APETAGEX",8) == 0)
		{
			flag = 1;
			//printf("this media has an apetag footer!\n");
			fseek(fp,-(header.tagsize + 128L),SEEK_END);
		}
	}
	if (!flag)
	{
		/*从文件头部开始查找APETAG*/
		fseek(fp,0,SEEK_SET);
		fread(&header,sizeof(APEHF),1,fp);
		if (!strncmp(header.headname,"APETAGEX",8))
		{
			//printf("This medium has APEv2Tag header!\n");

			fseek(fp,32L,SEEK_SET);

		}
		else
		{
			//printf("this media file doesn't have an apetag!\n");
			return 0;
		}

	}
	/*搜寻标签元素中的关键字和其对应的值 */
	for (i = 0;i<header.itemcount;i++)
	{
		memset(&Item,0,sizeof(APEItem));
		j=0;
		fread(&Item.valuesize,4,1,fp);
		fread(Item.itemflags,4,1,fp);

        //PRINT("valuesize:%x\n",Item.valuesize);
		if (Item.valuesize<0)
		{
			break;
		}
		fread(&c,1,1,fp);
		while (c!=0x00)
		{
			Item.itemkey[j++] = c;
			fread(&c,1,1,fp);
		}
		

		/*把与关键字对应得元素内容存入相应的结构体*/
		if (!strcmp(Item.itemkey,"Title"))
		{
			AIF->ultitle_sz = ReadItemValue(&AIF->ultitle,AIF,Item.valuesize);
			AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_8;
		}
		else if (!strcmp(Item.itemkey,"Artist"))
		{
			AIF->ulauthor_sz = ReadItemValue(&AIF->ulauthor,AIF,Item.valuesize);
			AIF->ulauthorCharEncode = A_AUDIO_FONTTYPE_UTF_8;
		}
		else if (!strcmp(Item.itemkey,"Album"))
		{
			AIF->ulAlbum_sz = ReadItemValue(&AIF->ulAlbum,AIF,Item.valuesize);
			AIF->ulAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_8;
		}
		else if (!strcmp(Item.itemkey,"Year"))
		{
			AIF->ulYear_sz = ReadItemValue(&AIF->ulYear,AIF,Item.valuesize);
			AIF->ulYearCharEncode  = A_AUDIO_FONTTYPE_UTF_8; 
		}
		else if (!strcmp(Item.itemkey,"Genre"))
		{
			AIF->ulGenre_sz = ReadItemValue(&AIF->ulGenre,AIF,Item.valuesize);
			AIF->ulGenreCharEncode = A_AUDIO_FONTTYPE_UTF_8;
		}
		else
		{
            if (Item.itemflags[0] & (1 << 1)) //add by yefl 20150113
            {
                int tmp;
                memset(filename, 0, sizeof(filename));
                Item.valuesize -= ape_get_str(AIF, Item.valuesize, filename, sizeof(filename));
                if (Item.valuesize <= 0) 
                {
                    PRINT("Skipping binary tag '%s'.\n", Item.itemkey);
                    return 0;
                }
                //PRINT("filename:%s, picture size:%d\n",filename,Item.valuesize);
                memset(&image,0,sizeof(__id3_image_info_t));
                image.length = Item.valuesize;
               // image.pic_type = i_type;
                file_ext = eLIBs_strchrlast(filename, '.');
                if (!file_ext)
                {
                    PRINT("strrchr %s error\n", filename);
                    return 0;
                }
                file_ext++;  
                //PRINT("file extension name: %s\n",file_ext);
                if(!strcmp(file_ext, "bmp"))
                {
                   image.img_format = IMG_FORMAT_BMP;
                }
                else if(!strcmp(file_ext, "jpeg"))
                {
                   image.img_format = IMG_FORMAT_JPG;
                }
                else if(!strcmp(file_ext, "png"))
                {
                   image.img_format = IMG_FORMAT_PNG;
                }
                else
                {
                   image.img_format = IMG_FORMAT_UNSUPPORT;
                }
               // PRINT("image format:%d\n", image.img_format);
                AIF->ulAPic_sz = sizeof(__id3_image_info_t);
                image.FileLocation = ftell(fp);
                if(image.FileLocation < 0)image.FileLocation = 0;
                AIF->InforBufLeftLength -= 4;
                AIF->InforBuf += 4;
                AIF->ulAPic = (__id3_image_info_t*)(AIF->InforBuf);
                if(AIF->InforBufLeftLength>=AIF->ulAPic_sz)
                {				
                    AIF->InforBufLeftLength -=AIF->ulAPic_sz;
                    memcpy(AIF->ulAPic,&image,AIF->ulAPic_sz);
                    AIF->InforBuf += AIF->ulAPic_sz;
                }
                //PRINT("length:%d filelocation:%#x\n",image.length,image.FileLocation);
            }
            
			fseek(fp,Item.valuesize,SEEK_CUR);
		}
        #if 0
        PRINT("Item:%d, itemkey:%s size:%d, itemflags:",i+1, Item.itemkey,Item.valuesize);
        for(j=0; j<4; j++) eLIBs_printf("%02x",Item.itemflags[j]);
        eLIBs_printf("\n");
        #endif
	}
    fseek(fp,0,SEEK_SET);
	return 1;

}

static int 	ReadItemValue(__s8** ItemName, __audio_file_info_t *AIF,int ItemSize)
{
	int ret = 0;
	FILE *fp;
	fp = ((BsInfo *)AIF->ulBSINFO)->ReadFile;
	*ItemName = AIF->InforBuf;//

	if(AIF->InforBufLeftLength <= ItemSize)
	{
		if(AIF->InforBufLeftLength>0x40)
		{
			ret =fread(AIF->InforBuf,1,0x40,fp);
			if(ret != 0x40)
			{
				return 0;
			}
			AIF->InforBufLeftLength -= ret;
			AIF->InforBuf += ret;
			ItemSize -= ret;
		}
		fseek(fp,ItemSize,SEEK_CUR);
		return ret;
	}

	if(fp)
	{
		ret =fread(AIF->InforBuf,1,ItemSize,fp);
		if(ret != ItemSize)
		{
			return 0;
		}
		AIF->InforBufLeftLength -= ret;
		AIF->InforBuf += ret;
	}
	
	return ret;
}

static int ape_get_str(__audio_file_info_t *AIF, int maxlen, char *buf, int buflen)
{
    int i = 0;
    unsigned char j = 0;
    unsigned int offset = 0;
    char byte[1024] = {0};
    FILE *fp = ((BsInfo *)AIF->ulBSINFO)->ReadFile;
    
    if ((buflen <= 0) || (buflen > sizeof(byte)))
        return -1;
    offset = ftell(fp);
    fread(byte,sizeof(byte),1,fp); 
    // reserve 1 byte for terminating 0
    buflen = FFMIN(buflen - 1, maxlen);
    for (i = 0; i < buflen; i++)
    {
        if (!(buf[i] = byte[j++]))
        {
            fseek(fp,offset+i+1,SEEK_SET);
            return i + 1;
        }
    }
    buf[i] = 0;
    for (; i < maxlen; i++)
    {
        if (!byte[j++])
        {
            fseek(fp,offset+i+1,SEEK_SET);
            return i + 1;
        }
    }
    fseek(fp,offset+maxlen,SEEK_SET);
    return maxlen;
}

