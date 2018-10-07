
#include "uicfg_in.h"

#ifdef UIC_DEBUG
#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
// #define THIS_DEBUG
#endif

#ifdef THIS_DEBUG
uic_int32_t depth = 0;
#endif
static void XMLCALL pdStartElementHandler(void * userData, const XML_Char * name, const XML_Char ** attr)
{
#ifdef THIS_DEBUG
    uic_int32_t i;

    depth++;
    log_mi("Tag: <%s>%d", name, depth);
    for (i = 0; attr[i]; i += 2) {
        log_mi("Attr: %s='%s'", attr[i], attr[i + 1]);
    }
#else
    uic_xmlctl_t * ctl;
    uic_xmlpara_t para;
    uic_int32_t i;

    if (userData) {
        ctl = (uic_xmlctl_t *)userData;
        if (-1 == ctl->xmlparser) {
            log_e("XML parse error");
            return;
        }

        if (ctl->depth >= UIC_TAG_LEVELS) {
            log_e("Tag is too deep, %d", ctl->depth);
            return;
        }
        ctl->depth++;
        ctl->tagstatus = UIC_TSTAT_ENTER;
        uic_strncpy(ctl->tag[ctl->depth - 1], name, UIC_NAME_SIZE);
        ctl->tag[ctl->depth - 1][UIC_NAME_SIZE - 1] = '\0';
        para.tag = ctl->tag[ctl->depth - 1];
        if (attr[0]) {
            for (i = 0; attr[i]; i += 2) {
                para.attr = (uic_char_t *)attr[i];
                para.content = (uic_char_t *)attr[i + 1];
                // log_e("Tag:%s, Depth:%d, Attr:%s, Content:%s", para.tag, ctl->depth, para.attr, para.content);
                ctl->xmlparser = fill_uicfg(ctl,  &para);
            }
        } else {
            para.attr = UIC_NULL;
            para.content = UIC_NULL;
            ctl->xmlparser = fill_uicfg(ctl,  &para);
        }
    }
#endif
}

static void XMLCALL pdEndElementHandler(void * userData, const XML_Char * name)
{
#ifdef THIS_DEBUG
    log_mi("Tag end: <%s>", name);
    depth--;
#else
    uic_xmlctl_t * ctl;
    uic_xmlpara_t para;

    if (userData) {
        ctl = (uic_xmlctl_t *)userData;
        if (-1 == ctl->xmlparser) {
            log_e("XML parse error");
            return;
        }

        ctl->tagstatus = UIC_TSTAT_EXIT;
        para.tag = ctl->tag[ctl->depth - 1];
        para.attr = UIC_NULL;
        para.content = UIC_NULL;
        // log_e("Tag:%s, Depth:%d", para.tag, ctl->depth);
        ctl->xmlparser = fill_uicfg(ctl,  &para);
        ctl->depth--;
    }
#endif
}

static void XMLCALL pdDataHandler(void * userData, const XML_Char * s, int len)
{
#ifdef THIS_DEBUG
    XML_Char da[UIC_NAME_SIZE];
    uic_int32_t i;
    uic_memcpy(da, s, len);
    da[len] = '\0';
    for (i = 0; i < len; i++) {
        //log_mi("%x", s[i]);
        ;
    }
    for (i = 0; (i < len) && (da[i] == '\t' || da[i] == '\n' || da[i] == '\r'); i++);
    if (i < len) {
        log_mi("Data: %s", &da[i]);
    }
#else
    uic_xmlctl_t * ctl;
    uic_xmlpara_t para;
    uic_char_t da[UIC_NAME_SIZE];
    uic_int32_t i;

    if (userData) {
        ctl = (uic_xmlctl_t *)userData;
        if (-1 == ctl->xmlparser) {
            log_e("XML parse error");
            return;
        }

        uic_memcpy(da, s, len);
        da[len] = '\0';
        for (i = 0; (i < len) && (da[i] == UIC_LF || da[i] == UIC_NL|| da[i] == UIC_TAB); i++);
        ctl->tagstatus = UIC_TSTAT_DATA;
        para.tag = ctl->tag[ctl->depth - 1];
        para.attr = UIC_NULL;
        para.content = &da[i];
        // log_e("Tag:%s, Depth:%d, Content:%s", para.tag, ctl->depth, para.content);
        ctl->xmlparser = fill_uicfg(ctl,  &para);
    }
#endif
}

uic_int32_t uic_xmlparse(char * xmlfile, uic_xmlctl_t * ctl)
{
    __hdle hdfile;
    const XML_Char * encodingName = NULL;
    XML_Parser parser;
    enum XML_ParamEntityParsing peParsing = XML_PARAM_ENTITY_PARSING_NEVER;
    XML_Char * buffer;
    uic_int32_t readsize;
    uic_int32_t done;

    if (NULL == xmlfile || NULL == ctl) {
        log_e("Arguments error");
        return -1;
    }

    buffer = esMEMS_Malloc(0, PROFDEV_XMLPARSE_BUFF_LEN);
    if (NULL == buffer) {
        log_e("Malloc buffer failed");
        return -1;
    }

    hdfile = esFSYS_fopen(xmlfile, "rb");
    if (NULL == hdfile) {
        esMEMS_Mfree(0, buffer);
        log_e("Open device profile(%s) failed", xmlfile);
        return -1;
    }
    parser = XML_ParserCreate(encodingName);
    if (NULL == parser) {
        esFSYS_fclose(hdfile);
        esMEMS_Mfree(0, buffer);
        log_e("Create XML parser failed");
        return -1;
    }
    ctl->depth = 0;
    ctl->xmlparser = 0;
    ctl->tagstatus = UIC_TSTAT_NULL;
    eLIBs_memset(buffer, 0, PROFDEV_XMLPARSE_BUFF_LEN);
    XML_SetParamEntityParsing(parser, peParsing);
    XML_SetElementHandler(parser, pdStartElementHandler, pdEndElementHandler);
    XML_SetCharacterDataHandler(parser, pdDataHandler);
    XML_SetUserData(parser, ctl);
    while (1) {
        done = 0;
        readsize = esFSYS_fread(buffer, 1, PROFDEV_XMLPARSE_BUFF_LEN, hdfile);
        if (readsize <= 0) {
            log_e("Read file failed");
            break;
        }
        // End file reading
        if (readsize < PROFDEV_XMLPARSE_BUFF_LEN) {
            done = XML_TRUE;
        }
        if (XML_Parse(parser, (const char *)buffer, readsize, done) == XML_STATUS_ERROR) {
            log_e("Parse error at line %d, %s\n", XML_GetCurrentLineNumber(parser),
              XML_ErrorString(XML_GetErrorCode(parser)));
            break;
        }
    }
    XML_ParserFree(parser);
    esFSYS_fclose(hdfile);
    esMEMS_Mfree(0, buffer);
    return 0;
}


