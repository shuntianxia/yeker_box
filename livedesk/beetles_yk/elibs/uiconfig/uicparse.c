
#include "uicfg_in.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef UIC_DEBUG
// #define THIS_DEBUG
#endif

/*
const static uic_char_t * if_types[] = {
    "RS232",
    "RS485",
    "RF2.4G",
    "Zigbee",
    "Ethernet",
    UIC_NULL
};

const static uic_char_t * window_types[] = {
    "single",
    "horizontal2",
    "horizontal3",
    "combined12",
    "combined21",
    "combined22",
    UIC_NULL
};

const static uic_char_t * button_types[] = {
    "switch",
    "analog",
    "batch",
    UIC_NULL
};
*/

static uic_int32_t fill_uicfg_begin(uic_xmlctl_t * ctl, uic_cfg_t * cfg)
{
    uic_memset(ctl, 0, sizeof(uic_xmlctl_t));
    uic_memset(cfg, 0, sizeof(uic_cfg_t));
    ctl->cfg = cfg;
    return 0;
}

static uic_int32_t fill_uicfg_end(uic_xmlctl_t * ctl, uic_cfg_t * cfg)
{
    return 0;
}

uic_int32_t fill_uicfg(uic_xmlctl_t * ctl, uic_xmlpara_t * para)
{
    uic_char_t * str;

#ifdef THIS_DEBUG
    log_mi("Tag<%s>; Depth:%d; status: %d", para->tag, ctl->depth, ctl->tagstatus);
#endif
    if (0 == ctl->version) {
        if (1 == ctl->depth && UIC_TSTAT_ENTER == ctl->tagstatus) {
            if (0 == uic_strcmp(para->tag, "config") && 0 == uic_strcmp(para->attr, "version")) {
                str = uic_strchr(para->content, '.');
                str[0] = '\0';
                str++;
                ctl->version = uic_atoi(para->content);
                ctl->version = UIC_VERSION(ctl->version, uic_atoi(str));
#ifdef THIS_DEBUG
                log_mi("XML UI configuration file version: %d", ctl->version);
#endif
            } else {
                log_e("Wrong root node tag and attrbute");
                return -1;
            }
        } else {
            log_e("Wrong depth handle (%d) or tag status (%d)", ctl->depth, ctl->tagstatus);
            return -1;
        }
    }
    if (ctl->depth < 1) {
        log_e("Depth is wrong");
        return -1;
    }
    ctl->parser.depth = ctl->depth;
    ctl->parser.status = ctl->tagstatus;
    switch (ctl->version) {
    case 101:
        if (-1 == fill_uicfg_v01(&ctl->parser, para, &ctl->cfg->v01)) {
            log_e("Paser error");
            return -1;
        }
        break;
    default:
        log_e("Wrong version minor number: %d", ctl->version);
        return -1;
    }
    return 0;
}

static uic_cfg_t uiccfg;
static uic_xmlctl_t uicctl;

uic_int32_t register_uicfg(uic_char_t * uicfg_path)
{
    uic_int32_t ret;
    fill_uicfg_begin(&uicctl, &uiccfg);
    ret = uic_xmlparse(uicfg_path, &uicctl);
    if (-1 == ret) {
        log_e("uic_xmlparse error");
        return ret;
    }
    ret = fill_uicfg_end(&uicctl, &uiccfg);
    if (-1 == ret) {
        log_e("fill_uicfg_end error");
        return ret;
    }
    return 0;
}



