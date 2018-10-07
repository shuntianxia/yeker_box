
#ifndef H_XMLPARSE_BENNIS_20121029
#define H_XMLPARSE_BENNIS_20121029

/**
 *
 */

#define PROFDEV_XMLPARSE_BUFF_LEN   1024

#ifdef __cplugplug
extern "C" {
#endif

uic_int32_t uic_xmlparse(char * xmlfile, uic_xmlctl_t * ctl);

#ifdef __cplugplug
}
#endif

#endif


