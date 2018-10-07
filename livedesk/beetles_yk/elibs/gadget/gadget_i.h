
#ifndef H_GADGET_I_BENNIS_20130124
#define H_GADGET_I_BENNIS_20130124

/**
 * You should change the following including files if you are doing porting jobs.
 */
#include <apps.h>
#include <expat.h>
#include <gadget.h>

/**
 * Portable comment functions
 */
#define inline                  __inline
#define GG_memcpy(d, s, n)      eLIBs_memcpy((d), (s), (n))
#define GG_memset(m, d, n)      eLIBs_memset((m), (d), (n))
#define GG_strcmp(s1, s2)       eLIBs_strcmp((s1), (s2))
#define GG_strchr(s, c)         eLIBs_strchr((s), (c))
#define GG_strncpy(d, s, n)     eLIBs_strncpy((d), (s), (n))
#define GG_strcpy(d, s)         eLIBs_strcpy((d), (s))
#define GG_strlen(s)            eLIBs_strlen(s)
#define GG_atoi(s)              SLIB_atoi(s)

#ifdef __cplugplug
extern "C" {
#endif

/**
 * Utilities
 */
extern void * GG_malloc(gg_size size);
extern gg_int32 GG_free(void * heap);

#ifdef __cplugplug
}
#endif

/**
 * Debugging configuration
 */
#ifdef MW_PROJ_DEBUG
#define GG_DEBUG
#endif

#ifdef GG_DEBUG

#define LogD(...)       (eLIBs_printf("<Debug||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogI(...)       (eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogMI(...)      (eLIBs_printf("<%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogW(...)       (eLIBs_printf("<Warning||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogE(...)       (eLIBs_printf("<Error||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define LogFatal(...)   (eLIBs_printf("<Fatal||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"), esKSRV_Reset())

#else

#define LogD(...)
#define LogI(...)
#define LogMI(...)
#define LogW(...)
#define LogE(...)
#define LogFatal(...)

#endif

/**
 * Module version
 */
#define VERSION_MAJOR           0
#define VERSION_MINOR           1
#define VERSION_BUILD           426
#define VERSION_MERGE(a, i, b)  ((a) * 1000000 + (i) * 10000 + (b))

#include "dom_core.h"
#include "xml_parse.h"

#endif

