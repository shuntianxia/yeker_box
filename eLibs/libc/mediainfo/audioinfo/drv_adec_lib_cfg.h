/*
*********************************************************************************************************
*                                                    eMOD
*                                   the Easy Portable/Player Operation System
*                                               mod_mmp sub-system
*                                              cedar library module
*
*                                       (c) Copyright 2008-2009, kevin China
*                                               All Rights Reserved
*
* File   : drv_adec_lib_cfg.h
* Version: V1.0
* By     : kevin
* Date   : 2008-11-20 16:30
*********************************************************************************************************
*/

#ifndef _DRV_ADEC_LIB_CFG_H_
#define _DRV_ADEC_LIB_CFG_H_

/*
================================================================================
                                TYPE REDEFINE MODULE
================================================================================
*/
#ifndef __u32
typedef unsigned int        __u32;
#endif

#ifndef __u16
typedef unsigned short      __u16;
#endif

#ifndef __u8
typedef unsigned char       __u8;
#endif

#ifndef __s32
typedef signed int          __s32;
#endif

#ifndef __s16
typedef signed short        __s16;
#endif

#ifndef __s8
typedef signed char         __s8;
#endif

#ifndef __bool
typedef signed char         __bool;
#endif

#ifndef __stk
typedef unsigned int        __stk;
#endif

#ifndef __cpu_sr
typedef unsigned int        __cpu_sr;
#endif

#ifndef __fp32
typedef float               __fp32;
#endif

#ifndef __fp64
typedef double              __fp64;
#endif

#ifndef __hdle
typedef unsigned int        __hdle;
#endif

#ifndef __size
typedef unsigned int        __size;
#endif

#ifndef __size_t
typedef unsigned int        __size_t;
#endif

#ifndef __s64
typedef __int64 __s64;
#endif



/*
================================================================================
                                MEMORY MANAGEMENT MODULE
================================================================================
*/
extern void *eLIBs_malloc(__size_t size);
extern void eLIBs_free(void *ptr);

#define malloc(size)        eLIBs_malloc(size)
#define free(pbuf)          eLIBs_free((void *)pbuf)


/*
================================================================================
                                CRITICAL OPERATION MODULE
================================================================================
*/
extern __size_t eLIBs_strlen    (const char * pstr);
extern char   * eLIBs_strcpy    (char * pdest, const char * psrc);
extern char   * eLIBs_strncpy   (char * pdest, const char * psrc, __size_t len_max);
extern __s16    eLIBs_strcmp    (const char * p1_str, const char * p2_str);
extern __s16    eLIBs_strncmp   (const char * p1_str, const char * p2_str,   __size_t len_max);
extern char   * eLIBs_strchr    (char * pstr,   char     srch_char);
extern char   * eLIBs_strstr    (char * pstr,   char   * srch_str);
extern void     eLIBs_memset    (void * pmem,   __u8     data_val, __size_t size);
extern void   * eLIBs_memcpy    (void * pdest,  const void   * psrc,     __size_t size);

#define MEM_SET(buf, val, size)         eLIBs_memset(buf, val, size)
#define memset(buf, val, size)          eLIBs_memset(buf, val, size)
#define MEM_CPY(dst, src, size)         eLIBs_memcpy(dst, src, size)
#define memcpy(dst, src, size)          eLIBs_memcpy(dst, src, size)

#define STR_NCMP(dst, src, size)        eLIBs_strncmp(dst, src, size)
#define strncmp(dst, src, size)         eLIBs_strncmp(dst, src, size)
#define STR_CMP(s1, s2)                 eLIBs_strcmp(s1, s2)
#define strcmp(s1, s2)                  eLIBs_strcmp(s1, s2)
#define STR_LEN(s)                      eLIBs_strlen(s)
#define strlen(s)                       eLIBs_strlen(s)
#define STR_NCPY(s1, s2, n)             eLIBs_strncpy(s1, s2, n)
#define strncpy(s1, s2, n)              eLIBs_strncpy(s1, s2, n)
#define STR_CPY(s1, s2)                 eLIBs_strcpy(s1, s2)
#define strcpy(s1, s2)                  eLIBs_strcpy(s1, s2)
#define STR_CHR(s, ch)                  eLIBs_strchr(s,ch)
#define strchr(s, ch)                   eLIBs_strchr(s,ch)
#define STR_STR(s1, s2)                 eLIBs_strstr(s1, s2)
#define strstr(s1, s2)                  eLIBs_strstr(s1, s2)


/*
================================================================================
                                PRINT MODULE
================================================================================
*/
#if 0
#define PRINT(...)                       {   \
                                            eLIBs_printf("LOG:L%d(%s):", __LINE__, __FILE__);  \
    						                eLIBs_printf(__VA_ARGS__);  \
    						             }
#else
#define PRINT(...)
#endif
#define fprintf(...)
#define stderr

/*
================================================================================
                                FILE OPERATION MODULE
================================================================================
*/
#define SEEK_SET 0          /* start of stream (see fseek) */
#define SEEK_CUR 1          /* current position in stream (see fseek) */
#define SEEK_END 2          /* end of stream (see fseek) */

#define ELIBS_SEEK_SET 0    /* start of stream (see fseek)                                  */
#define ELIBS_SEEK_CUR 1    /* current position in stream (see fseek)                       */
#define ELIBS_SEEK_END 2    /* end of stream (see fseek)                                    */

typedef struct __ESFILE
{
    __u32   tmp;
}ES_FILE;

typedef struct __ESFILE     FILE;

//==============================================================================
//                      FILE SYSTEM SYSTEM-CALL
//==============================================================================
//..#define FS_CACHE_MODE 
#if 0//def FS_CACHE_MODE
#undef FOPEN
#undef fopen
#undef FCLOSE
#undef fclose
#undef FREAD
#undef fread
#undef FSEEK
#undef fseek
#undef FTELL
#undef ftell
#undef FWRITE
#undef fwrite

extern FILE     *CEDAR_fopen(const char *path, const char *mode);
extern int      CEDAR_fclose(FILE *fp);
extern int      CEDAR_fread(void *buf, int size, int count, FILE *fp);
extern int      CEDAR_fwrite(void *buf, int size, int count, FILE *fp);
extern int      CEDAR_fseek(FILE *fp, __s64 offset, int origin);
extern __s64    CEDAR_ftell(FILE *fp);
extern int      CEDAR_FIOCTRL(FILE *fp, __s32 Cmd, __s32 Aux, void *pBuffer);
extern __u32    CEDAR_fsize(FILE *fp);      //¼ÆËãfilesizeµÄº¯Êý


#define FOPEN(name, mode)               CEDAR_fopen((name),(mode))
#define fopen(name, mode)               CEDAR_fopen((name),(mode))
#define FCLOSE(fptr)                    CEDAR_fclose((fptr))
#define fclose(fptr)                    CEDAR_fclose((fptr))
#define FREAD(buf,unit,size,fptr)       CEDAR_fread((buf),(unit),(size),(fptr))
#define fread(buf,unit,size,fptr)       CEDAR_fread((buf),(unit),(size),(fptr))
#define FWRITE(buf,unit,size,fptr)      CEDAR_fwrite((buf),(unit),(size),(fptr))
#define fwrite(buf,unit,size,fptr)      CEDAR_fwrite((buf),(unit),(size),(fptr))
#define FSEEK(fptr,offset,origin)       CEDAR_fseek((fptr),(offset),(origin))
#define fseek(fptr,offset,origin)       CEDAR_fseek((fptr),(offset),(origin))
#define FTELL(fptr)                     CEDAR_ftell((fptr))
#define ftell(fptr)                     CEDAR_ftell((fptr))

#else

extern ES_FILE  *eLIBs_fopen(const char * /*ES_FILEname*/, const char * /*mode*/);
extern int      eLIBs_fclose(ES_FILE * /*stream*/);
extern __size   eLIBs_fread(void * /*ptr*/,
                    __size /*size*/, __size /*nmemb*/, ES_FILE * /*stream*/);
extern __size   eLIBs_fwrite(const void * /*ptr*/,
                    __size /*size*/, __size /*nmemb*/, ES_FILE * /*stream*/);
extern int      eLIBs_fseek(ES_FILE * /*stream*/,   int /*offset*/, int /*whence*/);
extern __s64 eLIBs_flltell(ES_FILE * /*stream*/);
extern int eLIBs_ftell(ES_FILE * /*stream*/);


#define FOPNE(fpath, mode)          eLIBs_fopen(fpath, mode)
#define fopen(fpath, mode)          eLIBs_fopen(fpath, mode)

#define FCLOSE(fp)                  eLIBs_fclose(fp)
#define fclose(fp)                  eLIBs_fclose(fp)

#define FREAD(buf, size, cnt, fp)   eLIBs_fread(buf, size, cnt, fp)
#define fread(buf, size, cnt, fp)   eLIBs_fread(buf, size, cnt, fp)

#define FWRITE(buf, size, cnt, fp)  eLIBs_fwrite(buf, size, cnt, fp)
#define fwrite(buf, size, cnt, fp)  eLIBs_fwrite(buf, size, cnt, fp)

#define FSEEK(fp, offset, type)     eLIBs_fseek(fp, offset, type)
#define fseek(fp, offset, type)     eLIBs_fseek(fp, offset, type)

//#define FTELL(fp)                   eLIBs_ftell(fp)
//#define ftell(fp)                   eLIBs_ftell(fp)
#define FTELL(fp)                     eLIBs_flltell((fp))
#define ftell(fp)                     eLIBs_flltell((fp))
#endif
#endif  //_DRV_ADEC_CFG_H_

