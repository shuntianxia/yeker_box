#ifndef		OS_PLATFORM_H_
#define 	OS_PLATFORM_H_

#ifdef	__OS_LINUX
	#ifndef __OS_ANDROID
		#include <sys/eldk_types.h>
	#endif
	#include <stdio.h>
	#include "statname.h"
#endif 

#ifdef	__OS_LINUX

 	#ifndef 	NULL
		#define 	NULL 0
	#endif
	
	#ifndef 	size_t
		typedef unsigned int size_t; 
	#endif
	
	#ifdef __OS_ANDROID
		typedef int64_t __int64;

#include "include_stream/cedarx_stream.h"
#define fopen(pathname,flags) open(pathname, flags)
#define fread(buf, size, cnt, fp)  read(fp, buf, size*cnt)
#define	ftell(stream)	lseek64(stream,0,1)
#define fseek(fp, offset, type)  lseek64(fp, offset, type)
#define fclose(fp) close(fp)

	#elif	__OS_LINUX	
		typedef __s64  int64_t; 
		typedef __u64 uint64_t; 
		typedef __s64  __int64;
	#endif
#else
	#ifndef 	int64_t
	  typedef __int64  int64_t; 
	#endif
	#ifndef 	uint64_t
	  typedef unsigned __int64 uint64_t; 
	#endif
#endif
   // #define DOS
	#ifdef	DOS
		#include <stdio.h>
		#include <stdlib.h>
		#include <string.h>
	//#define eLIBs_printf printf
	#endif
	#define UCOS
#ifdef UCOS
	
		#ifndef 	NULL
		#define 	NULL 0
		#endif
		
		#ifndef 	size_t
		typedef unsigned int size_t; 
		#endif
	
		#include "drv_adec_lib_cfg.h"
		
	#endif
#ifndef INT64
     typedef __int64 INT64;
#endif

#endif //OS_PLATFORM_H_

