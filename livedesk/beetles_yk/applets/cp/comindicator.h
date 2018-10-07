
#ifndef H_COMINDICATOR_BENNIS_20121103
#define H_COMINDICATOR_BENNIS_20121103

typedef struct {
    H_LYR lyrTop;
    RECT wndRect;
    int winNr;
    //int winIndex;
} MwIndicatorWndArgs;

H_WIN MwIndicatorWndCreate(char * wndName, H_WIN parent, MwIndicatorWndArgs * args);
int MwIndicatorWndUpdate(H_WIN hWnd, int index);

#endif


