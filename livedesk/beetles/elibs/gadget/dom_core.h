
#ifndef H_DOM_CORE_BENNIS_20130124
#define H_DOM_CORE_BENNIS_20130124

#ifdef __cplugplug
extern "C" {
#endif

typedef enum {
    NSTAT_START = 0,
    NSTAT_DATA,
    NSTAT_DONE
} DOMNodeStatus;

typedef struct __DOMNode {
    gg_int32 depth;
    DOMNodeStatus status;/* 1 -- enter new tag, 2 -- normal, 3 -- exit tag */
    struct __DOMNode * ancestor;
    struct __DOMNode * nextBrother;
    struct __DOMNode * preBrother;
    struct __DOMNode * firstChild;
    struct __DOMNode * lastChild;
    gg_wchar * tag;
    gg_wchar ** attributes;
    gg_wchar * value;
    gg_size valueSize;
} DOMNode;

typedef struct {
    gg_int32 parser;/* -1 -- parse interrupted, 0 -- normal */
    gg_int32 depth;/* 0 -- root node */
    DOMNode * preNode;
    DOMNode * node;
} DOMParseDescriptor;

#ifdef __cplugplug
}
#endif

#endif


