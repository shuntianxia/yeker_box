
#include "uicfg_in.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef UIC_DEBUG
#define THIS_DEBUG
#endif

static uic_int32_t dom_button_switch_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom);
static uic_int32_t dom_page_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom);
static uic_int32_t dom_interface_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom);
static uic_int32_t dom_window_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom);
static uic_int32_t dom_config_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom);
static uic_int32_t dom_root_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom);

#define DOM_INDEX_ENUM_START()          enum {
#define DOM_INDEX(set, tag)             DOM_ENTRY_##set##_##tag
#define DOM_INDEX_ENUM_END()            };
#define DOM_ENTRIES_START(set)          static xml_domentry_t dom_entries_##set[] = {
#define DOM_ENTRY(tag)                  {#tag, UIC_NULL, UIC_NULL, 0}
#define DOM_ENTRY_NEXT(tag)             {#tag, dom_entries_##tag, UIC_NULL, 0}
#define DOM_ENTRY_H(tag, hdl)           {#tag, UIC_NULL, hdl, 0}
#define DOM_ENTRY_NEXT_H(tag, hdl)      {#tag, dom_entries_##tag, hdl, 0}
#define DOM_ENTRY_END()                 {UIC_NULL, UIC_NULL, UIC_NULL, 0}
#define DOM_ENTRIES_END()               };
#define DOM_GET_ENTRIES(set)            dom_entries_##set
#define DOM_GET_ENTRIY(set, tag)        dom_entries_##set[DOM_INDEX(set, tag)]
inline xml_domentry_t * dom_search_entry_by_tag(xml_domentry_t * head, uic_char_t * tag)
{
    uic_int32_t i;
    if (UIC_NULL == head || UIC_NULL == tag) {
        log_e("Parameter error");
        return UIC_NULL;
    }
    for (i = 0; head[i].tag; i++) {
        if (0 == uic_strcmp(tag, head[i].tag)) {
            return &head[i];
        }
    }
#ifdef THIS_DEBUG
    log_mi("Tag (%s) didn't exit", tag);
#endif
    return UIC_NULL;
}

inline xmlhandle dom_search_entris_handle(xml_domentry_t * head)
{
    uic_int32_t i;
    for (i = 0; head[i].tag; i++) {
        if (head[i].handle) {
            return head[i].handle;
        }
    }
    return UIC_NULL;
}

inline uic_int32_t dom_iterate_entries_set_flag(xml_domentry_t * head, uic_int32_t flag)
{
    uic_int32_t i;
    for (i = 0; head[i].tag; i++) {
        head[i].flag = flag;
    }
    return i;
}

inline uic_int32_t dom_iterate_entries_compare_flag(xml_domentry_t * head, uic_int32_t flag)
{
    uic_int32_t i;
    for (i = 0; head[i].tag; i++) {
        if (head[i].flag != flag) {
            return -1;
        }
    }
    return 0;
}

DOM_INDEX_ENUM_START()
    DOM_INDEX(button_switch, name) = 0,
    DOM_INDEX(button_switch, icon_on),
    DOM_INDEX(button_switch, icon_off),
    DOM_INDEX(button_switch, x),
    DOM_INDEX(button_switch, y),
    DOM_INDEX(button_switch, width),
    DOM_INDEX(button_switch, height),
    DOM_INDEX(button_switch, on_code),
    DOM_INDEX(button_switch, off_code),
    DOM_INDEX(button_switch, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(button_switch)
    DOM_ENTRY_H(name, dom_button_switch_handle),
    DOM_ENTRY(icon_on),
    DOM_ENTRY(icon_off),
    DOM_ENTRY(x),
    DOM_ENTRY(y),
    DOM_ENTRY(width),
    DOM_ENTRY(height),
    DOM_ENTRY(on_code),
    DOM_ENTRY(off_code),
    DOM_ENTRY_END()
DOM_ENTRIES_END()

DOM_INDEX_ENUM_START()
    DOM_INDEX(button_analog, name) = 0,
    DOM_INDEX(button_analog, icon_on),
    DOM_INDEX(button_analog, icon_off),
    DOM_INDEX(button_analog, x),
    DOM_INDEX(button_analog, y),
    DOM_INDEX(button_analog, width),
    DOM_INDEX(button_analog, height),
    DOM_INDEX(button_analog, on_code),
    DOM_INDEX(button_analog, off_code),
    DOM_INDEX(button_analog, slider_blks),
    DOM_INDEX(button_analog, slider_bg),
    DOM_INDEX(button_analog, slider_fore),
    DOM_INDEX(button_analog, slider_on_x),
    DOM_INDEX(button_analog, slider_on_y),
    DOM_INDEX(button_analog, slider_off_x),
    DOM_INDEX(button_analog, slider_off_y),
    DOM_INDEX(button_analog, min),
    DOM_INDEX(button_analog, max),
    DOM_INDEX(button_analog, code_pattern),
    DOM_INDEX(button_analog, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(button_analog)
    DOM_ENTRY_H(name, dom_button_switch_handle),
    DOM_ENTRY(icon_on),
    DOM_ENTRY(icon_off),
    DOM_ENTRY(x),
    DOM_ENTRY(y),
    DOM_ENTRY(width),
    DOM_ENTRY(height),
    DOM_ENTRY(on_code),
    DOM_ENTRY(off_code),
    DOM_ENTRY(slider_blks),
    DOM_ENTRY(slider_bg),
    DOM_ENTRY(slider_fore),
    DOM_ENTRY(slider_on_x),
    DOM_ENTRY(slider_on_y),
    DOM_ENTRY(slider_off_x),
    DOM_ENTRY(slider_off_y),
    DOM_ENTRY(min),
    DOM_ENTRY(max),
    DOM_ENTRY(code_pattern),
    DOM_ENTRY_END()
DOM_ENTRIES_END()

DOM_INDEX_ENUM_START()
    DOM_INDEX(button_batch, name) = 0,
    DOM_INDEX(button_batch, icon_on),
    DOM_INDEX(button_batch, icon_off),
    DOM_INDEX(button_batch, x),
    DOM_INDEX(button_batch, y),
    DOM_INDEX(button_batch, width),
    DOM_INDEX(button_batch, height),
    DOM_INDEX(button_batch, on_seq_code),
    DOM_INDEX(button_batch, off_seq_code),
    DOM_INDEX(button_batch, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(button_batch)
    DOM_ENTRY_H(name, dom_button_switch_handle),
    DOM_ENTRY(icon_on),
    DOM_ENTRY(icon_off),
    DOM_ENTRY(x),
    DOM_ENTRY(y),
    DOM_ENTRY(width),
    DOM_ENTRY(height),
    DOM_ENTRY(on_seq_code),
    DOM_ENTRY(off_seq_code),
    DOM_ENTRY_END()
DOM_ENTRIES_END()

DOM_INDEX_ENUM_START()
    DOM_INDEX(page, type) = 0,
    DOM_INDEX(page, name),
    DOM_INDEX(page, background),
    DOM_INDEX(page, button_switch),
    DOM_INDEX(page, button_analog),
    DOM_INDEX(page, button_batch),
    DOM_INDEX(page, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(page)
    DOM_ENTRY_H(type, dom_page_handle),
    DOM_ENTRY(name),
    DOM_ENTRY(background),
    DOM_ENTRY_NEXT(button_switch),
    DOM_ENTRY_NEXT(button_analog),
    DOM_ENTRY_NEXT(button_batch),
    DOM_ENTRY_END()
DOM_ENTRIES_END()


DOM_INDEX_ENUM_START()
    DOM_INDEX(interface, name) = 0,
    DOM_INDEX(interface, baudrate),
    DOM_INDEX(interface, bits),
    DOM_INDEX(interface, paritybit),
    DOM_INDEX(interface, stop),
    DOM_INDEX(interface, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(interface)
    DOM_ENTRY_H(name, dom_interface_handle),
    DOM_ENTRY(baudrate),
    DOM_ENTRY(bits),
    DOM_ENTRY(paritybit),
    DOM_ENTRY(stop),
    DOM_ENTRY_END()
DOM_ENTRIES_END()

DOM_INDEX_ENUM_START()
    DOM_INDEX(window, category),
    DOM_INDEX(window, page),
    DOM_INDEX(window, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(window)
    DOM_ENTRY_H(category, dom_window_handle),
    DOM_ENTRY_NEXT(page),
    DOM_ENTRY_END()
DOM_ENTRIES_END()

DOM_INDEX_ENUM_START()
    DOM_INDEX(config, name),
    DOM_INDEX(config, manufacturer),
    DOM_INDEX(config, model),
    DOM_INDEX(config, resolution_x),
    DOM_INDEX(config, resolution_y),
    DOM_INDEX(config, interface),
    DOM_INDEX(config, window),
    DOM_INDEX(config, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(config)
    DOM_ENTRY_H(name, dom_config_handle),
    DOM_ENTRY(manufacturer),
    DOM_ENTRY(model),
    DOM_ENTRY(resolution_x),
    DOM_ENTRY(resolution_y),
    DOM_ENTRY_NEXT(interface),
    DOM_ENTRY_NEXT(window),
    DOM_ENTRY_END()
DOM_ENTRIES_END()

DOM_INDEX_ENUM_START()
    DOM_INDEX(root, config) = 0,
    DOM_INDEX(root, max_index)
DOM_INDEX_ENUM_END()

DOM_ENTRIES_START(root)
    DOM_ENTRY_NEXT_H(config, dom_root_handle),
    DOM_ENTRY_END()
DOM_ENTRIES_END()

static uic_int32_t dom_button_switch_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom)
{
    uic_domparse_t * ctl;
    uic_cfg_v01_t * cfg;
    uic_window_v01_t * button;

    ctl = (uic_domparse_t *)phony_ctl;
    cfg = (uic_cfg_v01_t *)dom;
    button = &cfg->wnd[0];
    if (DOM_GET_ENTRIES(button_switch) != current_domtable(ctl)) {
        log_e("Entries is different");
        return -1;
    }
    if (ctl->status == UIC_TSTAT_DATA) {
        if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, name).tag, para->tag)) {
            uic_strncpy(button->button[0].name, para->content, UIC_NAME_SIZE);
            button->button[0].name[UIC_NAME_SIZE - 1] = '\0';
            DOM_GET_ENTRIY(button_switch, name).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, icon_on).tag, para->tag)) {
            button->button[0].icon_on = uic_atoi(para->content);
            DOM_GET_ENTRIY(button_switch, icon_on).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, icon_off).tag, para->tag)) {
            button->button[0].icon_off = uic_atoi(para->content);
            DOM_GET_ENTRIY(button_switch, icon_off).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, x).tag, para->tag)) {
            button->button[0].x = uic_atoi(para->content);
            DOM_GET_ENTRIY(button_switch, x).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, y).tag, para->tag)) {
            button->button[0].y = uic_atoi(para->content);
            DOM_GET_ENTRIY(button_switch, y).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, width).tag, para->tag)) {
            button->button[0].width = uic_atoi(para->content);
            DOM_GET_ENTRIY(button_switch, width).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, height).tag, para->tag)) {
            button->button[0].height = uic_atoi(para->content);
            DOM_GET_ENTRIY(button_switch, height).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, on_code).tag, para->tag)) {
            DOM_GET_ENTRIY(button_switch, on_code).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(button_switch, off_code).tag, para->tag)) {
            DOM_GET_ENTRIY(button_switch, off_code).flag = 1;
        }
    }
    return 0;
}

static uic_int32_t dom_page_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom)
{
    uic_domparse_t * ctl;
    uic_cfg_v01_t * cfg;

    ctl = (uic_domparse_t *)phony_ctl;
    cfg = (uic_cfg_v01_t *)dom;
    if (DOM_GET_ENTRIES(page) != current_domtable(ctl)) {
        log_e("Entries is different");
        return -1;
    }
    if (ctl->status == UIC_TSTAT_DATA) {
        if (0 == uic_strcmp(DOM_GET_ENTRIY(page, type).tag, para->tag)) {
            uic_strncpy(cfg->wnd[0].type, para->content, UIC_NAME_SIZE);
            cfg->name[UIC_NAME_SIZE - 1] = '\0';
            DOM_GET_ENTRIY(page, type).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(page, name).tag, para->tag)) {
            uic_strncpy(cfg->wnd[0].name, para->content, UIC_NAME_SIZE);
            cfg->name[UIC_NAME_SIZE - 1] = '\0';
            DOM_GET_ENTRIY(page, name).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(page, background).tag, para->tag)) {
            cfg->wnd[0].background = uic_atoi(para->content);
            DOM_GET_ENTRIY(page, background).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(page, button_switch).tag, para->tag)) {
            DOM_GET_ENTRIY(page, button_switch).flag = 1;
        }
    }
    return 0;
}

static uic_int32_t dom_interface_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom)
{
    uic_domparse_t * ctl;
    uic_cfg_v01_t * cfg;
    uic_if_v01_t * iface;

    ctl = (uic_domparse_t *)phony_ctl;
    cfg = (uic_cfg_v01_t *)dom;
    iface = &cfg->iface[0];
    log_e("interface, d=%d", ctl->depth);
    if (DOM_GET_ENTRIES(interface) != current_domtable(ctl)) {
        log_e("Entries is different");
        return -1;
    }
    if (ctl->status == UIC_TSTAT_DATA) {
        if (0 == uic_strcmp(DOM_GET_ENTRIY(interface, baudrate).tag, para->tag)) {
            iface->cfg.rs232.baudrate = uic_atoi(para->content);
            DOM_GET_ENTRIY(interface, baudrate).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(interface, bits).tag, para->tag)) {
            iface->cfg.rs232.bits = uic_atoi(para->content);
            DOM_GET_ENTRIY(interface, bits).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(interface, paritybit).tag, para->tag)) {
            iface->cfg.rs232.paritybit = uic_atoi(para->content);
            DOM_GET_ENTRIY(interface, paritybit).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(interface, stop).tag, para->tag)) {
            iface->cfg.rs232.stop = uic_atoi(para->content);
            DOM_GET_ENTRIY(interface, stop).flag = 1;
        }
    }
    return 0;
}

static uic_int32_t dom_window_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom)
{
    uic_domparse_t * ctl;
    uic_cfg_v01_t * cfg;
#ifdef THIS_DEBUG
    xml_domentry_t * srh;
#endif

    ctl = (uic_domparse_t *)phony_ctl;
    cfg = (uic_cfg_v01_t *)dom;
#ifdef THIS_DEBUG
    log_mi("window, d=%d", ctl->depth);
#endif
    if (DOM_GET_ENTRIES(window) != current_domtable(ctl)) {
        log_e("Entries is different");
        return -1;
    }
    if (ctl->status == UIC_TSTAT_DATA) {
        if (0 == uic_strcmp(DOM_GET_ENTRIY(window, category).tag, para->tag)) {
            uic_strncpy(cfg->category[0], para->content, UIC_NAME_SIZE);
            cfg->name[UIC_NAME_SIZE - 1] = '\0';
            DOM_GET_ENTRIY(window, category).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(window, page).tag, para->tag)) {
            DOM_GET_ENTRIY(window, page).flag = 1;
        }
    }
#ifdef THIS_DEBUG
    else if (ctl->status == UIC_TSTAT_EXIT) {
        srh = dom_search_entry_by_tag(current_domtable(ctl), para->tag);
        if (UIC_NULL == srh) {
            return 0;
        }
        if (srh->next) {
            if (0 == uic_strcmp(DOM_GET_ENTRIY(window, page).tag, para->tag)) {
                log_mi("<%s> page type=%s, name=%s", para->tag, cfg->wnd[0].type, cfg->wnd[0].name);
                log_mi("<%s> page background=%d, nr=%d", para->tag, cfg->wnd[0].background, cfg->wnd[0].btn_nr);
            }
        }
    }
#endif
    return 0;
}

static uic_int32_t dom_config_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom)
{
    uic_domparse_t * ctl;
    uic_cfg_v01_t * cfg;
#ifdef THIS_DEBUG
    xml_domentry_t * srh;
#endif

    ctl = (uic_domparse_t *)phony_ctl;
    cfg = (uic_cfg_v01_t *)dom;
    log_e("config, d=%d, content=%s", ctl->depth, para->content);
    if (DOM_GET_ENTRIES(config) != current_domtable(ctl)) {
        log_e("Entries is different");
        return -1;
    }
    if (ctl->status == UIC_TSTAT_DATA) {
        if (0 == uic_strcmp(DOM_GET_ENTRIY(config, name).tag, para->tag)) {
            uic_strncpy(cfg->name, para->content, UIC_NAME_SIZE);
            cfg->name[UIC_NAME_SIZE - 1] = '\0';
            DOM_GET_ENTRIY(config, name).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(config, manufacturer).tag, para->tag)) {
            uic_strncpy(cfg->manufacturer, para->content, UIC_NAME_SIZE);
            cfg->manufacturer[UIC_NAME_SIZE - 1] = '\0';
            DOM_GET_ENTRIY(config, manufacturer).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(config, model).tag, para->tag)) {
            uic_strncpy(cfg->model, para->content, UIC_NAME_SIZE);
            cfg->model[UIC_NAME_SIZE - 1] = '\0';
            DOM_GET_ENTRIY(config, model).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(config, resolution_x).tag, para->tag)) {
            cfg->resolution_x = uic_atoi(para->content);
            DOM_GET_ENTRIY(config, resolution_x).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(config, resolution_y).tag, para->tag)) {
            cfg->resolution_y = uic_atoi(para->content);
            DOM_GET_ENTRIY(config, resolution_y).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(config, interface).tag, para->tag)) {
            DOM_GET_ENTRIY(config, interface).flag = 1;
        } else if (0 == uic_strcmp(DOM_GET_ENTRIY(config, window).tag, para->tag)) {
            DOM_GET_ENTRIY(config, window).flag = 1;
        }
    }
#ifdef THIS_DEBUG
    else if (ctl->status == UIC_TSTAT_EXIT) {
        srh = dom_search_entry_by_tag(current_domtable(ctl), para->tag);
        if (UIC_NULL == srh) {
            return 0;
        }
        if (srh->next) {
            if (0 == uic_strcmp(DOM_GET_ENTRIY(config, interface).tag, para->tag)) {
                log_mi("<%s> rs232 baudrate=%d, bits=%d", para->tag, cfg->iface[0].cfg.rs232.baudrate, cfg->iface[0].cfg.rs232.bits);
                log_mi("<%s> rs232 parity=%d, stop=%d", para->tag, cfg->iface[0].cfg.rs232.paritybit, cfg->iface[0].cfg.rs232.stop);
            } else if (0 == uic_strcmp(DOM_GET_ENTRIY(config, window).tag, para->tag)) {
                log_mi("<%s> window category=%s", para->tag, cfg->category[0]);
            }
        }
    }
#endif
    return 0;
}

static uic_int32_t dom_root_handle(void * phony_ctl, uic_xmlpara_t * para, void * dom)
{
    uic_domparse_t * ctl;
#ifdef THIS_DEBUG
    uic_cfg_v01_t * cfg;
#endif
    xml_domentry_t * srh;

    ctl = (uic_domparse_t *)phony_ctl;
#ifdef THIS_DEBUG
    cfg = (uic_cfg_v01_t *)dom;
    log_e("root, d=%d", ctl->depth);
#endif
    if (DOM_GET_ENTRIES(root) != current_domtable(ctl)) {
        log_e("Entries is different");
        return -1;
    }
    if (ctl->status == UIC_TSTAT_DATA) {
        srh = dom_search_entry_by_tag(current_domtable(ctl), para->tag);
        if (UIC_NULL == srh) {
            return 0;
        }
        srh->flag = 1;
    }
#ifdef THIS_DEBUG
    else if (ctl->status == UIC_TSTAT_EXIT) {
        log_mi("<config> name=%s, manufacturer=%s, model=%s", cfg->name, cfg->manufacturer, cfg->model);
        log_mi("<config> resolution=%d,%d", cfg->resolution_x, cfg->resolution_y);
    }
#endif
    return 0;
}

uic_int32_t fill_uicfg_v01(uic_domparse_t * ctl, uic_xmlpara_t * para, uic_cfg_v01_t * cfg)
{
    xml_domentry_t * srh;
    if (ctl->status == UIC_TSTAT_ENTER) {
        if ((1 == ctl->depth) && (UIC_NULL == current_domtable(ctl))) {
            current_domtable(ctl) = DOM_GET_ENTRIES(root);
            pre_domtable(ctl) = UIC_NULL;
            next_domtable(ctl) = UIC_NULL;
            current_domhandle(ctl) = dom_search_entris_handle(current_domtable(ctl));
            pre_domhandle(ctl) = UIC_NULL;
            next_domhandle(ctl) = UIC_NULL;
        }

        if (current_domtable(ctl)) {
            srh = dom_search_entry_by_tag(current_domtable(ctl), para->tag);
            if (UIC_NULL == srh) {
                return 0;
            }
            if (srh->next) {
#ifdef THIS_DEBUG
                log_mi("Next tag (%s)", srh->next->tag);
#endif
                dom_iterate_entries_set_flag((xml_domentry_t *)srh->next, 0);
                next_domtable(ctl) = (xml_domentry_t *)srh->next;
                next_domhandle(ctl) = dom_search_entris_handle(next_domtable(ctl));
            } else {
                next_domtable(ctl) = UIC_NULL;
                next_domhandle(ctl) = UIC_NULL;
            }
        } else {
            log_mi("Null current DOM table");
            return -1;
        }
        if (current_domhandle(ctl)) {
            (*current_domhandle(ctl))(ctl, para, cfg);
        }
    } else if (ctl->status == UIC_TSTAT_DATA) {
        if (current_domhandle(ctl)) {
            (*current_domhandle(ctl))(ctl, para, cfg);
        }
    } else if (ctl->status == UIC_TSTAT_EXIT) {
        if (current_domtable(ctl)) {
            if (current_domhandle(ctl)) {
                (*current_domhandle(ctl))(ctl, para, cfg);
            }
            srh = dom_search_entry_by_tag(current_domtable(ctl), para->tag);
            if (srh && srh->next) {
                if (dom_iterate_entries_compare_flag((xml_domentry_t *)srh->next, 1)) {
                    log_e("There are some tags may didn't assigned");
                    //return -1;
                }
            }
        } else {
            log_mi("Null current DOM table");
            return -1;
        }
    } else {
        log_e("Wrong tag status %d", ctl->status);
        return -1;
    }
    return 0;
}

