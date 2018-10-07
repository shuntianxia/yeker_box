
#ifndef H_UICFG_V01_BENNIS_20121207
#define H_UICFG_V01_BENNIS_20121207

typedef struct {
    uic_char_t name[UIC_NAME_SIZE];
    union {
        struct {
            uic_int32_t baudrate;
            uic_int32_t bits;
            uic_int32_t paritybit;
            uic_int32_t stop;
        } rs232;
        struct {
            uic_int32_t baudrate;
            uic_int32_t bits;
            uic_int32_t paritybit;
            uic_int32_t stop;
        } rs485;
        struct {
            uic_int32_t rate;
        } rf24g;
        struct {
            uic_int32_t rate;
        } zigbee;
        struct {
            uic_int32_t rate;
        } eth;
    } cfg;
} uic_if_v01_t;

typedef struct {
    uic_uint8_t on_code[UIC_OPCODE_LEN];
    uic_uint8_t off_code[UIC_OPCODE_LEN];
} uic_button_switch_v01_t;

typedef struct {
    uic_int32_t slider_blks;
    uic_int32_t slider_bg;
    uic_int32_t slider_fore;
    uic_int32_t slider_on_x;
    uic_int32_t slider_on_y;
    uic_int32_t slider_off_x;
    uic_int32_t slider_off_y;
    uic_int32_t min;
    uic_int32_t max;
    uic_uint8_t code_pattern[UIC_OPCODE_LEN];
    uic_uint8_t on_code[UIC_OPCODE_LEN];
    uic_uint8_t off_code[UIC_OPCODE_LEN];
} uic_button_analog_v01_t;

typedef struct {
    uic_uint8_t * on_seq_code;
    uic_uint8_t * off_seq_code;
} uic_button_batchv01_t;

typedef struct {
    uic_char_t type[UIC_NAME_SIZE];
    uic_char_t name[UIC_NAME_SIZE];
    uic_int32_t background;
    uic_int32_t btn_nr;

    struct {
        uic_char_t type[UIC_NAME_SIZE];
        uic_char_t name[UIC_NAME_SIZE];
        uic_int32_t icon_on;
        uic_int32_t icon_off;
        uic_int32_t x;
        uic_int32_t y;
        uic_int32_t width;
        uic_int32_t height;
        union {
            uic_button_switch_v01_t sw;
            uic_button_analog_v01_t analog;
            uic_button_batchv01_t batch;
        } para;
    } button[UIC_BUTTON_NR];
} uic_window_v01_t;

typedef struct {
    // Basic information
    uic_char_t name[UIC_NAME_SIZE];
    uic_char_t manufacturer[UIC_NAME_SIZE];
    uic_char_t model[UIC_NAME_SIZE];
    uic_int32_t resolution_x;
    uic_int32_t resolution_y;

    // Next level
    uic_int32_t iface_nr;
    uic_if_v01_t iface[UIC_INTERFACE_NR];
    uic_int32_t wnd_nr;
    uic_char_t category[UIC_WINDOW_NR][UIC_NAME_SIZE];
    uic_window_v01_t wnd[UIC_WINDOW_NR];
} uic_cfg_v01_t;

#ifdef __cplugplug
extern "C" {
#endif

#ifdef __cplugplug
}
#endif

#endif

