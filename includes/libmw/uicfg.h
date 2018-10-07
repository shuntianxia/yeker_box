
#ifndef H_UICFG_BENNIS_20121128
#define H_UICFG_BENNIS_20121128

#ifdef __cplugplug
extern "C" {
#endif

typedef char                    uic_int8_t;
typedef unsigned char           uic_uint8_t;
typedef short                   uic_int16_t;
typedef unsigned short          uic_uint16_t;
typedef int                     uic_int32_t;
typedef unsigned int            uic_uint32_t;
typedef char                    uic_bool_t;
typedef char                    uic_char_t;
typedef short                   uic_wchar_t;
#define UIC_TRUE                1
#define UIC_FALSE               0
#define UIC_NULL                ((void *)0)
#define UIC_LF                  '\r'
#define UIC_TAB                 '\t'
#define UIC_NL                  '\n'

uic_int32_t register_uicfg(uic_char_t * uicfg_path);

#ifdef __cplugplug
}
#endif

#endif

