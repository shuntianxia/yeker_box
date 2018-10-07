
#ifndef H_UIC_IN_BENNIS_20121128
#define H_UIC_IN_BENNIS_20121128

/**
 * You should change the following including files if you are doing porting jobs.
 */
#include <apps.h>
#include <expat.h>

#include <uicfg.h>

#define UIC_NAME_SIZE           16
#define UIC_OPCODE_LEN          32
#define UIC_MAX_DATA_LEN        32
#define UIC_INTERFACE_NR        4
#define UIC_WINDOW_NR           16
#define UIC_BUTTON_NR           8
#define UIC_TAG_LEVELS          8

#define inline                  __inline
#define uic_memcpy(d, s, n)     eLIBs_memcpy((d), (s), (n))
#define uic_memset(m, d, n)     eLIBs_memset((m), (d), (n))
#define uic_strcmp(s1, s2)      eLIBs_strcmp((s1), (s2))
#define uic_strchr(s, c)        eLIBs_strchr((s), (c))
#define uic_strncpy(d, s, n)    eLIBs_strncpy((d), (s), (n))
#define uic_atoi(s)             SLIB_atoi(s)

#ifdef MW_PROJ_DEBUG
#define UIC_DEBUG
#endif

#ifdef UIC_DEBUG

#define log_d(...)      (eLIBs_printf("<Debug||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_i(...)      (eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_mi(...)     (eLIBs_printf("<%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_w(...)      (eLIBs_printf("<Warning||%s||L%d> ", __FILE__, __LINE__), \
                                eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_e(...)      (eLIBs_printf("<Error||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"))
#define log_fatal(...)  (eLIBs_printf("<Fatal||%s||L%d> ", __FILE__, __LINE__), \
                            eLIBs_printf(__VA_ARGS__), eLIBs_printf("\n"), esKSRV_Reset())

#else

#define log_d(...)
#define log_i(...)
#define log_mi(...)
#define log_w(...)
#define log_e(...)
#define log_fatal(...)

#endif

#define UIC_VERSION_MAJOR       00
#define UIC_VERSION_MINOR       01
#define UIC_VERSION(a, i)       ((a) * 100 + (i))

#include <uicfg_v01.h>
#include "uicparse.h"
#include "xmlparse.h"

#endif

