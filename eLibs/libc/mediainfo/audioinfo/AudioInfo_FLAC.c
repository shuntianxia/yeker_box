/***************************************
	MPEG LAYI II III audio infor parser
	support Xing /ID3V2 /ID3V1/VBRI tag 
	* 20081208  修正bitrate计算
**************************************/
#include "AuInParser.h"

#include	"tagInfo.h"
#include	"bitstream.h"	

static unsigned clz (unsigned x);


#define IMAGE_MIME_TYPE_BMP     "image/bmp"
#define IMAGE_MIME_TYPE_JPEG	"image/jpeg"
#define IMAGE_MIME_TYPE_PNG		"image/png"

#define likely(p)   (!!(p))
#define unlikely(p) (!!(p))
#define clz8( x ) (clz(x) - ((sizeof(unsigned) - sizeof (__u8)) * 8))

static const char flaccodecInfo[1][4]=
{	
	"flac"
};

static unsigned clz (unsigned x)
{
    unsigned i = sizeof (x) * 8;

    while (x)
    {
        x = x >> 1;
        i--;
    }
    return i;
}


/**
 * Converts the first character from a UTF-8 sequence into a code point.
 *
 * @param str an UTF-8 bytes sequence
 * @return 0 if str points to an empty string, i.e. the first character is NUL;
 * number of bytes that the first character occupies (from 1 to 4) otherwise;
 * -1 if the byte sequence was not a valid UTF-8 sequence.
 */
size_t vlc_towc (const char *str, __u32 *pwc)
{
    __u8 *ptr = (__u8 *)str, c;
    __u32 cp;
    int charlen = 0;

   // assert (str != NULL);

    c = *ptr;
    if (unlikely(c > 0xF4))
        return -1;

    charlen = clz8 (c ^ 0xFF);
    switch (charlen)
    {
        case 0: // 7-bit ASCII character -> short cut
            *pwc = c;
            return c != '\0';

        case 1: // continuation byte -> error
            return -1;

        case 2:
            if (unlikely(c < 0xC2)) // ASCII overlong
                return -1;
            cp = (c & 0x1F) << 6;
            break;

        case 3:
            cp = (c & 0x0F) << 12;
            break;

        case 4:
            cp = (c & 0x07) << 16;
            break;

        default:
          //  assert (0);
    }

    /* Unrolled continuation bytes decoding */
    switch (charlen)
    {
        case 4:
            c = *++ptr;
            if (unlikely((c >> 6) != 2)) // not a continuation byte
                return -1;
            cp |= (c & 0x3f) << 12;

            if (unlikely(cp >= 0x110000)) // beyond Unicode range
                return -1;
            /* fall through */
        case 3:
            c = *++ptr;
            if (unlikely((c >> 6) != 2)) // not a continuation byte
                return -1;
            cp |= (c & 0x3f) << 6;

            if (unlikely(cp >= 0xD800 && cp < 0xE000)) // UTF-16 surrogate
                return -1;
            if (unlikely(cp < (1u << (5 * charlen - 4)))) // non-ASCII overlong
                return -1;
            /* fall through */
        case 2:
            c = *++ptr;
            if (unlikely((c >> 6) != 2)) // not a continuation byte
                return -1;
            cp |= (c & 0x3f);
            break;
    }

    *pwc = cp;
    return charlen;
}

/**
 * Replaces invalid/overlong UTF-8 sequences with question marks.
 * Note that it is not possible to convert from Latin-1 to UTF-8 on the fly,
 * so we don't try that, even though it would be less disruptive.
 *
 * @return str if it was valid UTF-8, NULL if not.
 */
char *EnsureUTF8( char *str )
{
    char *ret = str;
    size_t n;
    __u32 cp;

    while ((n = vlc_towc (str, &cp)) != 0)
        if (likely(n != (size_t)-1))
            str += n;
        else
        {
            *str++ = '?';
            ret = NULL;
        }
    return ret;
}
#if 0
char *strndup (const char *str, size_t max)
{
    size_t len = strnlen (str, max);
    char *res = malloc (len + 1);

    PRINT("strndup len:%d\n",len);
    if (res)
    {
        memcpy (res, str, len);
        res[len] = '\0';
    }
    return res;
}
#endif
/*
宽度    描述
32      图片类型（同ID3v2 APIC）
        0 - Other
        1 - 32x32 pixels 'file icon' (PNG only)
        2 - Other file icon
        3 - Cover (front)
        4 - Cover (back)
        5 - Leaflet page
        6 - Media (e.g. label side of CD)
        7 - Lead artist/lead performer/soloist
        8 - Artist/performer
        9 - Conductor
        10 - Band/Orchestra
        11 - Composer
        12 - Lyricist/text writer
        13 - Recording Location
        14 - During recording
        15 - During performance
        16 - Movie/video screen capture
        17 - A bright coloured fish
        18 - Illustration
        19 - Band/artist logotype
        20 - Publisher/Studio logotype

32      MIME 类型说明字符长度（byte）
N*8     MIME类型说明字符
32      描述符长度（byte）
N*8     描述符UTF-8
32      图片宽度
32      图片高度
32      图片颜色深度
32      索引图使用的颜色数目，0非索引图
32      图片数据长度
N*8     图片二进制文件
*/
int ParsePicture(int size,__audio_file_info_t *AIF)
{
	unsigned int     i_type, i_len, width, height, i;
	char    psz_name[128];
	char    *psz_mime = NULL;
    char    *psz_description = NULL;
    __id3_image_info_t image;
	BsInfo *BSINFO = (BsInfo *)AIF->ulBSINFO;

    /* read the picture type */
    i_type = GetInfo_GetBsInByte(4,AIF); // 图片类型  
    i_len = GetInfo_GetBsInByte(4,AIF);//MIME 类型说明字符长度（byte）
    PRINT("Picture info size:%d, type: %d, type str len:%d\n",size, i_type, i_len);
    
    /* picture mimetype */
    if ((size < i_len + 4 ) || (!(psz_mime = malloc(i_len + 1)))) 
    {
        goto error;
    }
    i = 0;
    memset(psz_mime, 0, (i_len+1));
    while(i_len > 0)//MIME类型说明字符
    {
        psz_mime[i++] = GetInfo_GetBsInByte(1,AIF);
        if (i == i_len)
        {
            psz_mime[i_len] = '\0';
            break;
        }
    }
    PRINT("type str:%s\n", psz_mime);

    i_len = GetInfo_GetBsInByte(4,AIF);//描述符长度（byte）
    PRINT("Descriptor len:%d \n", i_len);
    if ((size < (i_len + 4*4+4)) || (!(psz_description = malloc(i_len + 1)))) 
    {
        goto error;
    }
    i = 0;
    memset(psz_description, 0, (i_len+1));
    while(i_len > 0)//描述符UTF-8
    {
        psz_description[i++] = GetInfo_GetBsInByte(1,AIF);
        if (i == i_len)
        {
            psz_description[i_len] = '\0';
            break;
        }
    }
    EnsureUTF8( psz_description );
    PRINT("\nDescriptor str:%s\n", psz_description);
    
    /*
    跳过4个字段:图片宽度 图片高度 图片颜色深度 索引图使用的颜色数目，0非索引图
    */
    GetInfo_SkipBsInByte(4*4,AIF);
    i_len = GetInfo_GetBsInByte(4,AIF);//图片数据长度
    PRINT("picture data len: %d\n", i_len);
    if((i_len + 4*4+4+4) > size )
    {
        goto error;
    }
    memset(&image,0,sizeof(__id3_image_info_t));
    image.length = i_len;
    image.pic_type = i_type;

    if(!strcmp(psz_mime, IMAGE_MIME_TYPE_BMP))
    {
       image.img_format = IMG_FORMAT_BMP;
    }
    else if(!strcmp(psz_mime, IMAGE_MIME_TYPE_JPEG))
    {
       image.img_format = IMG_FORMAT_JPG;
    }
    else if(!strcmp(psz_mime, IMAGE_MIME_TYPE_PNG))
    {
       image.img_format = IMG_FORMAT_PNG;
    }
    else
    {
       image.img_format = IMG_FORMAT_UNSUPPORT;
    }
    PRINT("image format:%d\n", image.img_format);
    AIF->ulAPic_sz = sizeof(__id3_image_info_t);
    image.FileLocation = GetInfo_GetFileLocation(AIF);
    if(image.FileLocation < 0)image.FileLocation = 0;
    AIF->InforBufLeftLength -= 4;
    AIF->InforBuf += 4;
    AIF->ulAPic = (__id3_image_info_t*)(AIF->InforBuf);
    if(AIF->InforBufLeftLength<AIF->ulAPic_sz)
    	goto error;				
    AIF->InforBufLeftLength -=AIF->ulAPic_sz;
    memcpy(AIF->ulAPic,&image,AIF->ulAPic_sz);
    AIF->InforBuf += AIF->ulAPic_sz;
    GetInfo_SkipBsInByte(image.length,AIF);
    PRINT("length:%d filelocation:%#x\n",image.length,image.FileLocation);
        
error:
    if (psz_mime)
        free( psz_mime );
    if (psz_description)
        free( psz_description );
	
}

static int Flac_GetInfo_GETID3_Len(__audio_file_info_t *AIF,int len)
{
    int Id3v2len = 0;
    unsigned int ID3_head = 0; 
    char id3_byte[10] = {0};
		
    if(len<10)
    {
        return 0;
    }
    ID3_head = GetInfo_ShowBs(0, 4, AIF);
    ID3_head >>= 8;
    if (ID3_head == 0x494433)
    {
        id3_byte[5] = GetInfo_ShowBs(5, 1, AIF);
        id3_byte[6] = GetInfo_ShowBs(6, 1, AIF);
        id3_byte[7] = GetInfo_ShowBs(7, 1, AIF);
        id3_byte[8] = GetInfo_ShowBs(8, 1, AIF);
        id3_byte[9] = GetInfo_ShowBs(9, 1, AIF);
        Id3v2len = id3_byte[6]<<7 | id3_byte[7];
        Id3v2len = Id3v2len<<7 | id3_byte[8];
        Id3v2len = Id3v2len<<7 | id3_byte[9];
        Id3v2len += 10;
        if (id3_byte[5] & 0x10)
               Id3v2len += 10;
    }
	
    return Id3v2len;
}

int GetInfo_FLAC(FILE *bs,__audio_file_info_t *AIF)  //return 1 :succed 0 :fail
{
	int ret=0;
    unsigned int ID3V2_ret = 0;
	__int64 filelen;
	int ID3Len;//cky add 
	///flac check
	filelen=GetInfo_BsInit(bs,AIF);
	if(filelen<0) return ERROR;

    ID3Len = Flac_GetInfo_GETID3_Len(AIF,10);	//处理flac文件中出现id3情况
    PRINT("ID3Len:%d \n",ID3Len);
    if (0 != ID3Len)
    {
        ID3V2_ret = GetInfo_GETID3_V2(AIF);
        PRINT("ID3V2_ret :%d\n",ID3V2_ret);
    }
    
	if(GetInfo_GetBsInByte(4,AIF) == 0x664c6143) //flaC
	{
		int blocktype,blocklen;
		int lastblock=0;
		unsigned data;
		__int64 totalsamples = 0;
		__int64 TempData;
		int	headlen = 4;
		ret = 1;
		while(!lastblock)
		{
			blocktype = GetInfo_GetBsInByte(1,AIF);
			blocklen = GetInfo_GetBsInByte(3,AIF);
			lastblock = (blocktype&0x80)?1:0; //最后一个metadata为’1’,其他为’0’
			blocktype &=0x7f;
			headlen += (blocklen+4);
			switch(blocktype)
			{
			case 0:  //METADATA_BLOCK_STREAMINFO 
			    PRINT("STREAMINFO\n");
				GetInfo_SkipBsInByte(10,AIF);
				data = GetInfo_GetBsInByte(4,AIF);//
				if(GetInfo_return<4)
				{
					ret = 0;
					goto FLAC_error;
				}
				AIF->ulSampleRate = (data>>12)&0xfffff;
				AIF->ulChannels = ((data>>9)&0x7)+1;
				AIF->ulBitsSample = ((data>>4)&0x1f)+1;
				totalsamples = data&0xf;
				totalsamples <<=32;
				totalsamples |= GetInfo_GetBsInByte(4,AIF);
				TempData = totalsamples *1000;				
				AIF->ulDuration = (unsigned int)(TempData/AIF->ulSampleRate);
				//AIF->ulBitRate = (filelen/(AIF->ulDuration/1000))*8;				
				GetInfo_SkipBsInByte(blocklen - 18,AIF);			
				break;
			case 4: //VORBIS_COMMENT 
			    PRINT("COMMENT\n");
				GetInfo_oggStandardTag(AIF);
				break;
			case 6: //picture 
				{
                    PRINT("picture\n");
					ParsePicture(blocklen,AIF);
					break;
				}
			default:
                PRINT("other blocktype:%d, len:%d\n",blocktype,blocklen);
				GetInfo_SkipBsInByte(blocklen,AIF);
				break;
			}
		}
		AIF->ulBitRate = (unsigned int)((double)filelen*8/((double)totalsamples/(double)AIF->ulSampleRate));
		if(AIF->InforBufLeftLength>=4)
		{
			AIF->ulAudio_name_sz = 4;
			AIF->ulAudio_name = AIF->InforBuf;
			AIF->InforBuf +=AIF->ulAudio_name_sz;
			AIF->InforBufLeftLength -=AIF->ulAudio_name_sz;			
			memcpy(AIF->ulAudio_name,flaccodecInfo[0],AIF->ulAudio_name_sz);
			
		}
	}
FLAC_error:
  GetInfo_BsEndRelease(AIF);
	return ret;
}
