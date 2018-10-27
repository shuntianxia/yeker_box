
#include "gadget_i.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef GG_DEBUG
//#define THIS_DEBUG
#endif

/**
 * You should configue this for your porting
 */
#define MW_TIMER_COEFFICIENT        2
#define MW_SAMPLE_POINTS            3//song adds

#define MW_DCLICK_INTERVAL          15
#define MW_LPRESS_PERIOD            200

// The following macro definitions are for free sliding across only one block
#define MW_CRITICAL_VELOCITY        6
#define MW_CURVES_CURR_POSI(curr)   ((curr) >> 6)   /* Path length = 64 */

// The following macro definitions are for free sliding across multi blocks
#define MW_MIN_VELOCITY             3
#define MW_RUNLENGTH                2
#define MW_FRICTION                 3

#if (MW_SAMPLE_POINTS < 2)
#error "MW_SAMPLE_POINTS must be larger than 2"
#endif

/**
 * Fast --> Slow
 * Path length = 64
 */
static const int momentum_curves_friction[MW_CRITICAL_VELOCITY + 1] = {
/* Pad,  1,  2,  3,  4,  5,  6, */
    64, 64, 60, 55, 45, 35, 20
};

/*** Configuration is done ***/

#if (MW_SAMPLE_POINTS <= 0)
#error "MW_SAMPLE_POINTS must be larger than 0"
#endif

#define block_no(p, b)              ((p) / (b))
#define is_block_edge(p, b)         (0 == ((p) % (b)))
#define half_block(b)               ((b) >> 1)
#define pre_block_head(n, b)        (((n) - 1) * (b))
#define block_edge(n, b)            ((n) * (b))
#define next_block_head(n, b)       (((n) + 1) * (b))

#define iner_htod(hdl)              ((hdl) ? ((icb_t *)(hdl)) : ((icb_t *)0))

typedef enum {
    PSTAT_READY = 0,
    PSTAT_CLICK,
    PSTAT_MOVE,
    PSTAT_INERTIA_MOTION
} progress_state_t;

typedef enum {
    TMID_DOUBLE_CLICK = 0,
    TMID_INERTIA,
    TMID_INERTIA_SBLOCK,
    TMID_INERTIA_BLOCKS
} timer_id_t;

typedef struct {
    // State control
    progress_state_t progress;
    // To get the initial velocity
    __u32 click_tm;
    int sample_x[MW_SAMPLE_POINTS];
    int sample_y[MW_SAMPLE_POINTS];
    int cursor;
    int points;
    GG_GESTUREDIRECT x_y; // Direction of X or Y
    int attr;
    H_WIN hmsg[GG_GESTURE_MSG_DEST_MAX];
    int steps;          // Steps for each block, such as window length
    int latest;         // Latest point
    int timer_id;       // 0 -- inertial sliding, 1 -- double click

    // Location information
    int init_block;
    int displace;       // Relative displacement with initial point

    // Momentum calculation
    int vi;        	    // Initial Velocity;
    int s;              // Distance;
    int direction;      // Indicates the display window running direction; 1 -- positive; -1 -- negative
    int critical_posi[MW_CRITICAL_VELOCITY + 1];
    int v_coefficient;
    int run;
} iner_momentum_t;

typedef struct {
    GG_GESTUREATTR cls;
    iner_momentum_t mo;
} icb_t;

static inline int iner_sblock_launch(icb_t * icb, __gui_msg_t * msg);
static inline int iner_sblock_ontimer(icb_t * icb, __gui_msg_t * msg);
static inline int iner_blocks_launch(icb_t * icb, __gui_msg_t * msg);
static inline int iner_blocks_ontimer(icb_t * icb, __gui_msg_t * msg);

static inline int iner_sendmessage(icb_t * icb, GG_GESTUREMESSAGE type, GG_GESTUREMSGDEST dest,
                                        int x, int y, GG_GESTUREDIRECT dir, int to)
{
    __gui_msg_t     msgex;
    int ret;
    msgex.id        = type;                         // Gesture type
    msgex.h_srcwin  = NULL;
    msgex.h_deswin  = icb->mo.hmsg[dest];
    msgex.dwAddData1= MAKELONG(x & 0xFFFF, y);      // x, y coordinate
    msgex.dwAddData2= MAKELONG(to & 0xFFFF, dir);   // The displacement, direct of motion
    msgex.dwReserved= MAKELONG(0, 0);
//	LogMI("GUI_MSG_TOUCH_DOWN");
    ret = GUI_PostMessage(&msgex);
#ifdef THIS_DEBUG
{
    char name[GUI_NAME_MAX];
    LogMI("%x, to=%d, ret = %d, dest=%d", type, to, ret, dest);
    if (ORANGE_OK == GUI_WinGetName(msgex.h_deswin, name)) {
        LogMI("Destination window name=%s", name);
    }
}
#endif
    return ret;
}

static inline int iner_sendmessage_todesktop(icb_t * icb, GG_GESTUREMESSAGE type,
                                        int x, int y, GG_GESTUREDIRECT dir, int to)
{
    __gui_msg_t     msgex;
    int ret;
    msgex.id        = type;                         // Gesture type
    msgex.h_srcwin  = NULL;
    msgex.h_deswin  = icb->cls.htimer;
    msgex.dwAddData1= MAKELONG(x & 0xFFFF, y);      // x, y coordinate
    msgex.dwAddData2= MAKELONG(to & 0xFFFF, dir);   // The displacement, direct of motion
    msgex.dwReserved= MAKELONG(0, 0);

    ret = GUI_PostMessage(&msgex);
#ifdef THIS_DEBUG
{
    char name[GUI_NAME_MAX];
    LogMI("%x, to=%d, ret = %d", type, to, ret);
    if (ORANGE_OK == GUI_WinGetName(msgex.h_deswin, name)) {
        LogMI("Destination window name=%s", name);
    }
}
#endif
    return ret;
}

void * GG_GestureOpen(void)
{
    icb_t * icb;

    icb = (icb_t *)esMEMS_Malloc(0, sizeof(icb_t));
    if (NULL == icb) {
        LogE("Malloc failed");
        return NULL;
    }
    eLIBs_memset(icb, 0, sizeof(icb_t));
    return icb;
}

int GG_GestureClose(void * hdl)
{
    icb_t * icb;
    if (NULL == (icb = iner_htod(hdl))) {
        return -1;
    }
    GUI_KillTimer(icb->cls.htimer, icb->cls.timerid);
    esMEMS_Mfree(0, icb);
    return 0;
}

// Calculate the position according to the corresponding elapsed time
void GG_GestureOnTimer(void * hdl, __gui_msg_t * msg)
{
    icb_t * icb;
    if (NULL == (icb = iner_htod(hdl))) {
        return;
    }
    if (TMID_INERTIA_SBLOCK == icb->mo.timer_id) {
        iner_sblock_ontimer(icb, msg);
    } else if (TMID_INERTIA_BLOCKS == icb->mo.timer_id) {
        iner_blocks_ontimer(icb, msg);
    } else {
        LogE("Error timer id");
    }
}

int GG_GestureOnDown(void * hdl, __gui_msg_t * msg, int x, int y, GG_GESTUREATTR * cls)
{
    icb_t * icb;

    if (NULL == (icb = iner_htod(hdl))) {
        return -1;
    }

    // Initiated the data structure while the first touch down
    if (PSTAT_READY == icb->mo.progress) {
        if (0 == cls->attr_x && 0 == cls->attr_y) {
            LogW("No gesture attribute");
            return -1;
        }

        icb->mo.progress = PSTAT_CLICK;

        eLIBs_memcpy(&icb->cls, cls, sizeof(GG_GESTUREATTR));
        icb->mo.sample_x[0] = x;
        icb->mo.sample_y[0] = y;
        icb->mo.cursor = 0;
        icb->mo.points = 1;
        eLIBs_memcpy(icb->mo.hmsg, cls->hmsg_x, sizeof(H_WIN) * GG_GESTURE_MSG_DEST_MAX);
        GUI_KillTimer(icb->cls.htimer, icb->cls.timerid);
        if (icb->cls.attr_x & GG_GESTURE_ATTR_MO) {
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MO, GG_GESTURE_MSG_DEST_MO, x, y, GG_GESTURE_DIR_LEFT_RIGHT, 0);
        }
        if (icb->cls.attr_x & GG_GESTURE_ATTR_LPRESS) {
            icb->mo.click_tm = GUI_GetTickCount();
        }
    } else {
        LogE("Gesture state machine initiated state error, %d", icb->mo.progress);
        return -1;
    }
    return 0;
}

int GG_GestureOnMove(void * hdl, __gui_msg_t * msg, int x, int y)
{
    icb_t * icb;
    int to;
    __u32 tm;

    if (NULL == (icb = iner_htod(hdl))) {
        return -1;
    }

    // Change to move state from click state
    if (PSTAT_CLICK == icb->mo.progress) {
        icb->mo.progress = PSTAT_MOVE;
		if (icb->cls.attr_x & GG_GESTURE_ATTR_MO) {
			iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MO, GG_GESTURE_MSG_DEST_MO, x, y, GG_GESTURE_DIR_LEFT_RIGHT, 0);
		}
        if (0 == (icb->cls.attr_x & GG_GESTURE_ATTR_COMB_MOVE)) {
            icb->mo.x_y         = GG_GESTURE_DIR_TOP_DOWN;
            icb->mo.attr        = icb->cls.attr_y;
            eLIBs_memcpy(icb->mo.hmsg, icb->cls.hmsg_y, sizeof(H_WIN) * GG_GESTURE_MSG_DEST_MAX);
            icb->mo.steps       = icb->cls.steps_y;
            icb->mo.latest      = icb->mo.sample_y[0];
            if (icb->cls.steps_y) {
                icb->mo.init_block  = block_no(icb->cls.start_y, icb->cls.steps_y);
            } else {
                icb->mo.init_block  = 0;
            }
            icb->mo.displace    = icb->cls.start_y;
        } else if (0 == (icb->cls.attr_y & GG_GESTURE_ATTR_COMB_MOVE)) {
            icb->mo.x_y         = GG_GESTURE_DIR_LEFT_RIGHT;
            icb->mo.attr        = icb->cls.attr_x;
            icb->mo.steps       = icb->cls.steps_x;
            icb->mo.latest      = icb->mo.sample_x[0];
            if (icb->cls.steps_x) {
                icb->mo.init_block  = block_no(icb->cls.start_x, icb->cls.steps_x);
            } else {
                icb->mo.init_block  = 0;
            }
            icb->mo.displace    = icb->cls.start_x;
        } else if (GG_MATH_ABS(x - icb->mo.sample_x[0]) >= (GG_MATH_ABS(y - icb->mo.sample_y[0]) * 3)) {
            // Decide the direction
            icb->mo.x_y         = GG_GESTURE_DIR_LEFT_RIGHT;
            icb->mo.attr        = icb->cls.attr_x;
            icb->mo.steps       = icb->cls.steps_x;
            icb->mo.latest      = icb->mo.sample_x[0];
            if (icb->cls.steps_x) {
                icb->mo.init_block  = block_no(icb->cls.start_x, icb->cls.steps_x);
            } else {
                icb->mo.init_block  = 0;
            }
            icb->mo.displace    = icb->cls.start_x;
        } else {
            icb->mo.x_y         = GG_GESTURE_DIR_TOP_DOWN;
            icb->mo.attr        = icb->cls.attr_y;
            eLIBs_memcpy(icb->mo.hmsg, icb->cls.hmsg_y, sizeof(H_WIN) * GG_GESTURE_MSG_DEST_MAX);
            icb->mo.steps       = icb->cls.steps_y;
            icb->mo.latest      = icb->mo.sample_y[0];
            if (icb->cls.steps_y) {
                icb->mo.init_block  = block_no(icb->cls.start_y, icb->cls.steps_y);
            } else {
                icb->mo.init_block  = 0;
            }
            icb->mo.displace    = icb->cls.start_y;
        }
        if (0 == (icb->mo.attr & GG_GESTURE_ATTR_COMB_MOVE)) {
            icb->mo.progress = PSTAT_READY;
            return 0;
        }
        //iner_sendmessage_todesktop(icb, GUI_MSG_USER_GESTURE_NEIGHBOR, x, y, icb->mo.x_y, icb->mo.displace);
#ifdef THIS_DEBUG
        LogMI("the direction is %d", icb->mo.x_y);
#endif
    }

    if (PSTAT_MOVE == icb->mo.progress) {
        // the window postion will be moved to.
        if (GG_GESTURE_DIR_LEFT_RIGHT == icb->mo.x_y) {
            to = x;
        } else {
            to = y;
        }
        if (icb->mo.attr & GG_GESTURE_ATTR_POSITIVE) {
            icb->mo.displace += (to - icb->mo.latest);
        } else {
            icb->mo.displace -= (to - icb->mo.latest);
        }
        icb->mo.latest = to;
        if (icb->mo.attr & GG_GESTURE_ATTR_SLIDE) {
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MOVE, GG_GESTURE_MSG_DEST_SLIDE, x, y, icb->mo.x_y, icb->mo.displace);
        } else if (icb->mo.attr & GG_GESTURE_ATTR_INER) {
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MOVE, GG_GESTURE_MSG_DEST_INER, x, y, icb->mo.x_y, icb->mo.displace);
        } else if (icb->mo.attr & GG_GESTURE_ATTR_INER_SBLOCK) {
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MOVE, GG_GESTURE_MSG_DEST_INER_SBLOCK, x, y, icb->mo.x_y, icb->mo.displace);
        } else if (icb->mo.attr & GG_GESTURE_ATTR_INER_BLOCKS) {
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MOVE, GG_GESTURE_MSG_DEST_INER_BLOCKS, x, y, icb->mo.x_y, icb->mo.displace);
        }
        GG_MATH_CYCLE_INC_0(icb->mo.cursor, MW_SAMPLE_POINTS);
        icb->mo.sample_x[icb->mo.cursor] = x;
        icb->mo.sample_y[icb->mo.cursor] = y;
        icb->mo.points++;
#ifdef THIS_DEBUG
        LogMI("Move displace: %d", icb->mo.displace);
#endif
    } else {
#ifdef THIS_DEBUG
        LogE("Wrong move state machine, %d", icb->mo.progress);
#endif
        return -1;
    }
    return 0;
}

int GG_GestureOnLpress(void * hdl, __gui_msg_t * msg, int x, int y)
{
    icb_t * icb;
    __u32 tm;

    if (NULL == (icb = iner_htod(hdl))) {
        return -1;
    }
	if(PSTAT_CLICK == icb->mo.progress){
      if (icb->cls.attr_x & GG_GESTURE_ATTR_LPRESS) {
         tm = GUI_GetTickCount();
         if ((tm - icb->mo.click_tm) > MW_LPRESS_PERIOD) {
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_LPRESS, GG_GESTURE_MSG_DEST_LPRESS, x, y, GG_GESTURE_DIR_LEFT_RIGHT, 0);
            icb->mo.progress = PSTAT_READY;
            return 0;
         }
      }
	}
	return 0;
}
// Figure out the initial velocity and the distance will run according to the finger moving
int GG_GestureOnUp(void * hdl, __gui_msg_t * msg, int x, int y)
{
    icb_t * icb;
    int pre;

    if (NULL == (icb = iner_htod(hdl))) {
        return -1;
    }

    if (PSTAT_CLICK == icb->mo.progress) {
        // Just touch down, no move
        if (icb->cls.attr_x & GG_GESTURE_ATTR_CLICK) {
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_CLICK, GG_GESTURE_MSG_DEST_CLICK, x, y, GG_GESTURE_DIR_LEFT_RIGHT, 0);
        }
        icb->mo.progress = PSTAT_READY;
    } else if (PSTAT_MOVE == icb->mo.progress) {
        if (icb->mo.attr & GG_GESTURE_ATTR_SLIDE) {
            // No inertial motion attribute
            iner_sendmessage(icb, GUI_MSG_USER_GESTURE_DONE, GG_GESTURE_MSG_DEST_SLIDE, x, y, icb->mo.x_y, icb->mo.displace);
            icb->mo.progress = PSTAT_READY;
            return 0;
        }

        icb->mo.progress = PSTAT_INERTIA_MOTION;

        if (icb->mo.points < MW_SAMPLE_POINTS) {
            pre = 0;
        } else {
            icb->mo.points = MW_SAMPLE_POINTS;
            pre = icb->mo.cursor;
            GG_MATH_CYCLE_INC_0(pre, MW_SAMPLE_POINTS);
        }

        icb->mo.points;
        if (GG_GESTURE_DIR_LEFT_RIGHT == icb->mo.x_y) {
            icb->mo.vi = (icb->mo.sample_x[icb->mo.cursor] - icb->mo.sample_x[pre]) / icb->mo.points;
        } else {
            icb->mo.vi = (icb->mo.sample_y[icb->mo.cursor] - icb->mo.sample_y[pre]) / icb->mo.points;
        }

#ifdef THIS_DEBUG
    {
        int i;
        LogMI("Cur pointer %d of %d, pre %d", icb->mo.cursor, icb->mo.points, pre);
        for (i = 0; i < MW_SAMPLE_POINTS; i++) {
            LogMI("%d", icb->mo.sample_x[i]);
        }
        LogMI("Delta: %d, vi %d", icb->mo.displace, icb->mo.vi);
    }
#endif
        icb->mo.sample_x[0] = x;
        icb->mo.sample_y[0] = y;

        if (icb->mo.attr & GG_GESTURE_ATTR_INER) {
    //        iner_motion_free(icb, msg);
        } else if (icb->mo.attr & GG_GESTURE_ATTR_INER_SBLOCK) {
            iner_sblock_launch(icb, msg);
        } else if (icb->mo.attr & GG_GESTURE_ATTR_INER_BLOCKS) {
            iner_blocks_launch(icb, msg);
        } else {
            LogE("Wrong parameter");
        }
    } else {
        LogE("Wrong move state machine, %d", icb->mo.progress);
        return -1;
    }
    return 0;
}

// Find out the initial velocity, acceleration, friction and direction before the free moving
static inline void iner_sblock_init(icb_t * icb)
{
    int i;

    // LogMI("Iner initial velocity and distance: %d, %d", icb->mo.vi, icb->mo.s);
    if (icb->mo.vi > MW_CRITICAL_VELOCITY) {
        icb->mo.v_coefficient = icb->mo.vi / MW_CRITICAL_VELOCITY + 4;
    } else {
        icb->mo.v_coefficient = 5;
    }
    if (icb->mo.v_coefficient < 1) {
        icb->mo.v_coefficient = 1;
    }
    icb->mo.vi = MW_CRITICAL_VELOCITY;
    for (i = icb->mo.vi; i > 0; i--) {
        icb->mo.critical_posi[i] = MW_CURVES_CURR_POSI(momentum_curves_friction[i] * icb->mo.s);
        // log_i("%d:%d", i, icb->mo.critical_posi[i]);
    }
    // Judge the direction
    if (icb->mo.attr & GG_GESTURE_ATTR_POSITIVE) {
        icb->mo.direction = -icb->mo.direction;
    }

    icb->mo.run = 0;
}

static inline int iner_sblock_launch(icb_t * icb, __gui_msg_t * msg)
{
    int tmp;

    tmp = block_no(icb->mo.displace, icb->mo.steps);
#ifdef THIS_DEBUG
    LogMI("Start single block scrolling: block No.=%d, init_block=%d, vi=%d", tmp, icb->mo.init_block, icb->mo.vi);
#endif
    if (icb->mo.init_block == tmp) {
        // Display 2 and 3 windows direction; calculate the distance (mo->s); confirm the velocity;
        if (icb->cls.tailboundary) {
            icb->mo.direction = -1;
            if (icb->mo.vi < 0) {
                icb->mo.vi = 0;
            }
            icb->mo.s = icb->mo.displace - block_edge(icb->mo.init_block, icb->mo.steps);
        } else {
            if (icb->mo.vi > MW_CRITICAL_VELOCITY) {
                icb->mo.direction = -1;
                icb->mo.s = icb->mo.displace - block_edge(icb->mo.init_block, icb->mo.steps);
            } else {
                icb->mo.vi = GG_MATH_ABS(icb->mo.vi);
                if (icb->mo.vi > MW_CRITICAL_VELOCITY) {
                    icb->mo.direction = 1;
                    icb->mo.s = next_block_head(icb->mo.init_block, icb->mo.steps) - icb->mo.displace;
                } else {
                    if (icb->mo.displace > (block_edge(icb->mo.init_block, icb->mo.steps) + half_block(icb->mo.steps))) {
                        icb->mo.direction = 1;
                        icb->mo.s = next_block_head(icb->mo.init_block, icb->mo.steps) - icb->mo.displace;
                    } else {
                        icb->mo.direction = -1;
                        icb->mo.s = icb->mo.displace - block_edge(icb->mo.init_block, icb->mo.steps);
                    }
                }
            }
        }
        iner_sblock_init(icb);
        icb->mo.timer_id = TMID_INERTIA_SBLOCK;
        GUI_SetTimer(icb->cls.htimer, icb->cls.timerid, MW_TIMER_COEFFICIENT, NULL);
#ifdef THIS_DEBUG
        LogMI("Free scroll: dire = %d, location = %d, vc = %d, s = %d",
                    icb->mo.direction, icb->mo.displace, icb->mo.v_coefficient, icb->mo.s);
#endif
    // Display 1 and 2 windows direction
    } else if ((icb->mo.init_block - 1) == tmp) {
        if (icb->cls.headboundary) {
            icb->mo.direction = 1;
            if (icb->mo.vi > 0) {
                icb->mo.vi = 0;
            }
            icb->mo.vi = GG_MATH_ABS(icb->mo.vi);
            icb->mo.s = block_edge(icb->mo.init_block, icb->mo.steps) - icb->mo.displace;
        } else {
            if (icb->mo.vi > MW_CRITICAL_VELOCITY) {
                icb->mo.direction = -1;
                icb->mo.s = icb->mo.displace - pre_block_head(icb->mo.init_block, icb->mo.steps);
            } else {
                icb->mo.vi = GG_MATH_ABS(icb->mo.vi);
                if (icb->mo.vi > MW_CRITICAL_VELOCITY) {
                    icb->mo.direction = 1;
                    icb->mo.s = block_edge(icb->mo.init_block, icb->mo.steps)- icb->mo.displace;
                } else {
                    if (icb->mo.displace > (pre_block_head(icb->mo.init_block, icb->mo.steps) + half_block(icb->mo.steps))) {
                        icb->mo.direction = 1;
                        icb->mo.s = block_edge(icb->mo.init_block, icb->mo.steps) - icb->mo.displace;
                    } else {
                        icb->mo.direction = -1;
                        icb->mo.s = icb->mo.displace - pre_block_head(icb->mo.init_block, icb->mo.steps);
                    }
                }
            }
        }
        iner_sblock_init(icb);
        icb->mo.timer_id = TMID_INERTIA_SBLOCK;
        GUI_SetTimer(icb->cls.htimer, icb->cls.timerid, MW_TIMER_COEFFICIENT, NULL);
#ifdef THIS_DEBUG
        LogMI("Free scroll: dire = %d, location = %d, vc = %d, s = %d",
                    icb->mo.direction, icb->mo.displace, icb->mo.v_coefficient, icb->mo.s);
#endif
    } else {
        LogE("Wrong displacement %d", icb->mo.displace);
        icb->mo.displace = block_edge(icb->mo.init_block, icb->mo.steps);
        iner_sendmessage(icb, GUI_MSG_USER_GESTURE_ERROR, GG_GESTURE_MSG_DEST_INER_SBLOCK,
                    icb->mo.sample_x[0], icb->mo.sample_y[0], icb->mo.x_y, icb->mo.displace);
        icb->mo.progress = PSTAT_READY;
    }
    return 0;
}

static inline int iner_sblock_ontimer(icb_t * icb, __gui_msg_t * msg)
{
    int i;
    if (PSTAT_INERTIA_MOTION != icb->mo.progress) {
        GUI_KillTimer(icb->cls.htimer, icb->cls.timerid);
        iner_sendmessage(icb, GUI_MSG_USER_GESTURE_ERROR, GG_GESTURE_MSG_DEST_INER_SBLOCK, 0, 0, GG_GESTURE_DIR_LEFT_RIGHT, 0);
        icb->mo.progress = PSTAT_READY;
        LogE("Wrong handle about ontimer");
        return -1;
    }
    for (i = icb->mo.vi; i > 0; i--) {
        if (icb->mo.run <= icb->mo.critical_posi[i]) {
            break;
        }
    }
    if (i == 0) {
        i = 1;
    }
    icb->mo.vi = i;
    icb->mo.run += (icb->mo.vi * icb->mo.v_coefficient);
    // LogMI("Timer : loc = %d, vi = %d, run = %d", icb->mo.displace, icb->mo.vi, icb->mo.run);
    if (icb->mo.run >= icb->mo.s) {
        GUI_KillTimer(icb->cls.htimer, icb->cls.timerid);
        iner_sendmessage(icb, GUI_MSG_USER_GESTURE_DONE, GG_GESTURE_MSG_DEST_INER_SBLOCK, 0, 0, icb->mo.x_y, icb->mo.displace + (icb->mo.s * icb->mo.direction));
        icb->mo.progress = PSTAT_READY;
    } else {
        iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MOVE, GG_GESTURE_MSG_DEST_INER_SBLOCK, 0, 0, icb->mo.x_y, icb->mo.displace + (icb->mo.run * icb->mo.direction));
    }
    return 0;
}

static inline int iner_blocks_launch(icb_t * icb, __gui_msg_t * msg)
{
    if (icb->mo.vi < 0) {
        icb->mo.direction = -1;
    } else {
        icb->mo.direction = 1;
    }
    icb->mo.vi = GG_MATH_ABS(icb->mo.vi);
    if (icb->mo.vi < MW_MIN_VELOCITY) {
        icb->mo.vi = 1;
        if ((icb->mo.displace % icb->mo.steps) < half_block(icb->mo.steps)) {
            icb->mo.direction = -1;
        }
    }
    if (0 == (icb->mo.attr & GG_GESTURE_ATTR_POSITIVE)) {
        icb->mo.direction = -icb->mo.direction;
    }
    icb->mo.v_coefficient = MW_RUNLENGTH;
    icb->mo.timer_id = TMID_INERTIA_BLOCKS;
    GUI_SetTimer(icb->cls.htimer, icb->cls.timerid, MW_TIMER_COEFFICIENT, NULL);
#if 1
    LogMI("Free scroll: dire = %d, displace = %d, vi = %d",
                    icb->mo.direction, icb->mo.displace, icb->mo.vi);
#endif
    return 0;
}

static inline int iner_blocks_ontimer(icb_t * icb, __gui_msg_t * msg)
{
    if (PSTAT_INERTIA_MOTION != icb->mo.progress) {
        GUI_KillTimer(icb->cls.htimer, icb->cls.timerid);
        iner_sendmessage(icb, GUI_MSG_USER_GESTURE_ERROR, GG_GESTURE_MSG_DEST_INER_BLOCKS, 0, 0, GG_GESTURE_DIR_LEFT_RIGHT, 0);
        icb->mo.progress = PSTAT_READY;
        LogE("Wrong handle about ontimer");
        return -1;
    }

    icb->mo.displace += (icb->mo.vi * icb->mo.direction);
    icb->mo.v_coefficient--;
    if (icb->mo.v_coefficient <= 0) {
        icb->mo.v_coefficient = MW_RUNLENGTH;
        icb->mo.vi -= MW_FRICTION;
        if (icb->mo.vi < 1) {
            icb->mo.vi = 1;
        }
    }

    LogMI("Timer : loc = %d, vi = %d", icb->mo.displace, icb->mo.vi);
    if (icb->mo.vi < MW_MIN_VELOCITY && (0 == icb->mo.displace % icb->mo.steps)) {
        GUI_KillTimer(icb->cls.htimer, icb->cls.timerid);
        iner_sendmessage(icb, GUI_MSG_USER_GESTURE_DONE, GG_GESTURE_MSG_DEST_INER_BLOCKS, 0, 0, icb->mo.x_y, icb->mo.displace);
        icb->mo.progress = PSTAT_READY;
    } else {
        iner_sendmessage(icb, GUI_MSG_USER_GESTURE_MOVE, GG_GESTURE_MSG_DEST_INER_BLOCKS, 0, 0, icb->mo.x_y, icb->mo.displace);
    }
    return 0;
}



