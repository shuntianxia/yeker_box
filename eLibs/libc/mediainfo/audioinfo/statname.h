
/**************************************************************************************
 *
 * statname.h - name mangling macros for static linking
 **************************************************************************************/

#ifndef _STATNAME_GETINFO_H
#define _STATNAME_GETINFO_H

/* define STAT_PREFIX to a unique name for static linking 
 * all the C functions and global variables will be mangled by the preprocessor
 *   e.g. void FFT(int *fftbuf) becomes void cook_FFT(int *fftbuf)
 */
#define STAT_PREFIXA		GetInfo

#define STATCC1A(x,y)	STATCC2A(x,y)
#define STATCC2A(x,y)	x##y  

#ifdef STAT_PREFIXA
#define STATNAMEA(func)	STATCC1A(STAT_PREFIXA, func)
#else
#define STATNAMEA(func)	func
#endif

/* global functions */
#define	GetInfo_AAC									STATNAMEA(GetInfo_AAC				)
#define	GetInfo_AC3									STATNAMEA(GetInfo_AC3				)
#define GetInfo_APE                                 STATNAMEA(GetInfo_APE               )
#define GetInfo_DTS                                 STATNAMEA(GetInfo_DTS               )
#define GetInfo_FLAC                                STATNAMEA(GetInfo_FLAC              )
#define GetInfo_MP3                                 STATNAMEA(GetInfo_MP3               )
#define GetInfo_OGG                                 STATNAMEA(GetInfo_OGG               )
#define GetInfo_RA                                  STATNAMEA(GetInfo_RA                )
#define GetInfo_WAV                                 STATNAMEA(GetInfo_WAV               )
#define GetInfo_WMA                                 STATNAMEA(GetInfo_WMA               )
#define GetInfo_AMR                                 STATNAMEA(GetInfo_AMR               )
#define GetInfo_ATRAC                               STATNAMEA(GetInfo_ATRAC             )
#define GetInfo_MID                                 STATNAMEA(GetInfo_MID               )






 

/* global (const) data */
//#define pkkey				STATNAME(pkkey)

/* assembly functions - either inline or in separate asm file */
//#define MULSHIFT32			STATNAME(MULSHIFT32)

#endif	/* _STATNAME_GETINFO_H */
