
#ifndef H_XML_PARSE_BENNIS_20130124
#define H_XML_PARSE_BENNIS_20130124

/**
 *
 */

#define GG_XMLPARSE_BUFF_LEN        1024

#ifdef __cplugplug
extern "C" {
#endif

extern gg_int32 GG_XMLParse(char * xmlfile, DOMParseDescriptor * ctl);

#ifdef __cplugplug
}
#endif

#endif


