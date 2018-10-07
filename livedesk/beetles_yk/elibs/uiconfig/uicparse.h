
#ifndef H_UICPARSE_BENNIS_20121128
#define H_UICPARSE_BENNIS_20121128

typedef union {
    uic_cfg_v01_t v01;
} uic_cfg_t;

typedef enum {
    UIC_TSTAT_NULL = 0,
    UIC_TSTAT_ENTER,
    UIC_TSTAT_DATA,
    UIC_TSTAT_EXIT
} uic_tagstatus_t;

typedef struct {
    uic_char_t * tag;
    uic_char_t * attr;
    uic_char_t * content;
} uic_xmlpara_t;

typedef uic_int32_t (*xmlhandle)(void * ctl, uic_xmlpara_t * para, void * dom);

typedef struct xml_domentry_tag {
    const uic_char_t * tag;
    const struct xml_domentry_tag * next;
    xmlhandle handle;
    uic_int32_t flag;
} xml_domentry_t;

typedef struct {
    uic_int32_t depth;
    uic_tagstatus_t status;

    xml_domentry_t * domtable[UIC_TAG_LEVELS];
    xmlhandle domhandle[UIC_TAG_LEVELS];
} uic_domparse_t;

typedef struct uic_xmlctl_tag {
    uic_cfg_t * cfg;
    uic_int32_t xmlparser;
    uic_int32_t depth;/* 1 -- root node */
    uic_tagstatus_t tagstatus;/* 1 -- enter, 2 -- normal, 3 -- exit */
    uic_char_t tag[UIC_TAG_LEVELS][UIC_NAME_SIZE];

    uic_int32_t version;

    uic_domparse_t parser;
} uic_xmlctl_t;

#define pre_domtable(ctl)           ((ctl)->domtable[(ctl)->depth - 2])
#define current_domtable(ctl)       ((ctl)->domtable[(ctl)->depth - 1])
#define next_domtable(ctl)          ((ctl)->domtable[(ctl)->depth])
#define pre_domhandle(ctl)          ((ctl)->domhandle[(ctl)->depth - 2])
#define current_domhandle(ctl)      ((ctl)->domhandle[(ctl)->depth - 1])
#define next_domhandle(ctl)         ((ctl)->domhandle[(ctl)->depth])

#ifdef __cplugplug
extern "C" {
#endif

uic_int32_t fill_uicfg(uic_xmlctl_t * ctl, uic_xmlpara_t * para);
uic_int32_t fill_uicfg_v01(uic_domparse_t * ctl, uic_xmlpara_t * para, uic_cfg_v01_t * cfg);

#ifdef __cplugplug
}
#endif

#endif


