
#ifndef H_EXT_WINDOW_BENNIS_20130131
#define H_EXT_WINDOW_BENNIS_20130131

#define EWIN_INFO_MAGIC             0x55AA8761  /* Magic */
/* Window Inertia Type */
#define SCR_WNDINFOMASK_GESTURE     0x00020000
#define SCR_WNDINFOMASK_SBUTTON     0x00000002  /* Scrolling Button control */
#define SCR_WNDINFOMASK_SBACKGROUND 0x00000004  /* Scrolling Background control */
#define SCR_WNDINFOMASK_SMANAGE     0x00000008  /* Scrolling management window */
#define SCR_WNDINFOMASK_SHOST       0x00000010  /* Scrolling main window */

typedef struct {
    long magic;         // Magic, improve robustness
    long mask[1];       // Data type mask
    H_WIN desktop;      // desktop window handle
    SIZE desktopSize;   // Desktop window size
    GG_GDI gdi;         // System GDI
    GG_GESTUREATTR gestureArgs; // Gesture attribute of this window
    H_WIN hWnd;         // My own window handle
    RECT wndRect;       // The location which is relative to screen
    H_LYR lyrLow;       // Background Layer
    SIZE lyrLowSize;    // Layer size
    H_LYR lyrHigher;    // Foreground Layer
	H_LYR inilyrHigher;	// Foreground Layer
    SIZE lyrHigherSize; // Layer size
    long lparam;        // User parameter
    void * wparam;      // For user parameter
	int   suspend;		//for standy mode 
	SIZE inilyrHigherSize; // Layer size
	int   langid;		//0 ÖÐÎÄ 1 Ó¢ÎÄ
} EWin_InheritInfo_ForHeader;

int EWin_CheckInherit(EWin_InheritInfo_ForHeader * inherit);

extern H_LYR EWin_LayerCreate(char * lyrName, SIZE * layer, RECT * screen, RECT * source, __u8 higherLayer);
extern H_WIN EWin_MainWindowCreate(char * name, EWin_InheritInfo_ForHeader * wndDpt, __pGUI_WIN_CB wndProc);
extern H_WIN EWin_FrameWindowCreate(char * name, EWin_InheritInfo_ForHeader * wndDpt, H_WIN hLayer, RECT * wndRect, __pGUI_WIN_CB wndProc);
extern H_WIN EWin_GestureAttrCollect(H_WIN start, __s32 x, __s32 y);

#endif

