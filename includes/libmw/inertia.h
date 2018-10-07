

#ifndef H_INERTIA_BENNIS_20121026
#define H_INERTIA_BENNIS_20121026

// Gesture Attribute
#define MW_GESTURE_ATTR_POSITIVE    0x0001  // Default is negative motion
#define MW_GESTURE_ATTR_CLICK       0x0080  // Enable click feature
#define MW_GESTURE_ATTR_DCLICK      0x0010  // Enable double click
#define MW_GESTURE_ATTR_LPRESS      0x0020  // Enable long-press feature
#define MW_GESTURE_ATTR_SLIDE       0x0040  // Sliding without inertia
#define MW_GESTURE_ATTR_INER        0x0100  // Absolutely free sliding
#define MW_GESTURE_ATTR_INER_SBLOCK 0x0200  // Enable free sliding across only one block
#define MW_GESTURE_ATTR_INER_BLOCKS 0x0400  // Enable free sliding across multi blocks
#define MW_GESTURE_ATTR_COMB_MOVE   (MW_GESTURE_ATTR_SLIDE | MW_GESTURE_ATTR_INER | MW_GESTURE_ATTR_INER_SBLOCK | MW_GESTURE_ATTR_INER_BLOCKS)
    // MW_GESTURE_ATTR_DCLICK, MW_GESTURE_ATTR_LPRESS only for attr_x member using

typedef struct {
    int attr_x;
    int attr_y;

    // Block size
    int steps_x;
    int steps_y;

    // Start point position
    int start_x;
    int start_y;

    // They are only for MW_GESTURE_ATTR_INER_SBLOCK and MW_GESTURE_ATTR_INER_BLOCKS
    int headboundary;   // 0 -- false; 1 -- true
    int tailboundary;

    // For message send
    H_WIN msg_win;
    // For timer
    H_WIN h_win;
    __u16 timerid;
} iner_start_t;

void * iner_open(void);
int iner_close(void * hdl);
int iner_ondown(void * hdl, __gui_msg_t * msg, int x, int y, iner_start_t * cls);
int iner_onmove(void * hdl, __gui_msg_t * msg, int x, int y);
int iner_onup(void * hdl, __gui_msg_t * msg, int x, int y);
void iner_ontimer(void * hdl, __gui_msg_t * msg);

#endif


