
#include "gadget_i.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif
#ifdef GG_DEBUG
//#define THIS_DEBUG
#endif

gg_handler GG_DOMCreate(gg_wchar * document)
{
    DOMNode * root;
    DOMParseDescriptor desc;
    root = (DOMNode *)GG_malloc(sizeof(DOMNode));
    if (!root) {
        LogE("Malloc failed");
        return GG_NULL;
    }
    desc.node = root;
    if (0 == GG_XMLParse(document, &desc)) {
        return root;
    }
    GG_DOMDestroy((gg_handler)root);
    LogE("XML parse error");
    return GG_NULL;
}

gg_int32 GG_DOMDestroy(gg_handler handler)
{
    DOMNode * node;
    DOMNode * release;
    gg_int32 i;

    if (!handler) {
        LogE("Null node argument");
        return -1;
    }
    node = (DOMNode *)handler;
    while (node) {
        while (node->lastChild) {
            node = node->lastChild;
        }
        release = node;
        if (node->preBrother) {
            node = node->preBrother;
        } else {
            if (node->ancestor) {
                node = node->ancestor;
                node->firstChild = GG_NULL;
                node->lastChild = GG_NULL;
            } else {
                node = GG_NULL;
            }
        }
#ifdef THIS_DEBUG
        if (release->tag) {
            LogMI("Release tag: %s", release->tag);
        }
#endif
        GG_free(release->value);
        release->value = GG_NULL;
        if (release->attributes) {
            for (i = 0; release->attributes[i]; i++) {
                GG_free(release->attributes[i]);
                release->attributes[i] = GG_NULL;
            }
        }
        GG_free(release->attributes);
        release->attributes = GG_NULL;
        GG_free(release->tag);
        release->tag = GG_NULL;
        GG_free(release);
        release = GG_NULL;
    }
    return 0;
}

gg_handler GG_DOMGetRoot(gg_handler handler)
{
    DOMNode * snode;
    if (!handler) {
        LogE("Null node argument");
        return GG_NULL;
    }
    snode = (DOMNode *)handler;
    if (NSTAT_DONE == snode->status) {
        return snode;
    } else {
        return GG_NULL;
    }
}

gg_int32 GG_DOMGetElementsByTagName(gg_handler node, gg_wchar * tag)
{
    DOMNode * start;
    DOMNode * snode;
    gg_int32 cnt;

    if (!node) {
        LogE("Null node argument");
        return -1;
    }

    cnt = 0;
    start = ((DOMNode *)node)->firstChild;
    while (start) {
#ifdef THIS_DEBUG
        LogMI("Search tag: (%s,%d)", start->tag, start->status);
#endif
        if (0 == GG_strcmp(start->tag, tag)) {
            if (NSTAT_DONE == start->status) {
                cnt++;
            } else {
                LogE("Wrong node status (%d)", start->status);
            }
        }

        snode = start->firstChild;
        if (!snode) {
            snode = start->nextBrother;
            if (!snode) {
                while (start->ancestor) {
                    if (start->ancestor == node) {
                        return cnt;
                    }
                    if (start->ancestor->nextBrother) {
                        snode = start->ancestor->nextBrother;
                        break;
                    } else {
                        start = start->ancestor;
                    }
                }
            }
        }
        start = snode;
    }

    return -1;
}

gg_handler GG_DOMGetNextElementByTagName(gg_handler node, gg_wchar * tag)
{
    DOMNode * start;
    DOMNode * snode;

    if (!node) {
        LogE("Null node argument");
        return GG_NULL;
    }

    start = ((DOMNode *)node)->firstChild;
    while (start) {
#ifdef THIS_DEBUG
        LogMI("Search tag: (%s,%d)", start->tag, start->status);
#endif
        if (0 == GG_strcmp(start->tag, tag)) {
            if (NSTAT_DONE == start->status) {
                return start;
            } else {
                LogE("Wrong node status (%d)", start->status);
            }
        }
        snode = start->firstChild;
        if (!snode) {
            snode = start->nextBrother;
            if (!snode) {
                while (start->ancestor) {
                    if (start->ancestor == node) {
                        return GG_NULL;
                    }
                    if (start->ancestor->nextBrother) {
                        snode = start->ancestor->nextBrother;
                        break;
                    } else {
                        start = start->ancestor;
                    }
                }
            }
        }
        start = snode;
    }
    return GG_NULL;
}

gg_handler GG_DOMGetBrotherElementByTagName(gg_handler node, gg_wchar * tag)
{
    DOMNode * snode;

    snode = (DOMNode *)node;
    while (snode) {
#ifdef THIS_DEBUG
        LogMI("Search tag: (%s,%d)", snode->tag, snode->status);
#endif
        if (0 == GG_strcmp(snode->tag, tag)) {
            if (NSTAT_DONE == snode->status) {
                return snode;
            } else {
                LogE("Wrong node status (%d)", snode->status);
            }
        }
        snode = snode->nextBrother;
    }
    return GG_NULL;
}

gg_handler GG_DOMGetpreBrotherElementByTagName(gg_handler node, gg_wchar * tag)
{
	DOMNode * snode;

	snode = (DOMNode *)node;
	while (snode) {
#ifdef THIS_DEBUG
		LogMI("Search tag: (%s,%d)", snode->tag, snode->status);
#endif
		if (0 == GG_strcmp(snode->tag, tag)) {
			if (NSTAT_DONE == snode->status) {
				return snode;
			} else {
				LogE("Wrong node status (%d)", snode->status);
			}
		}
		snode = snode->preBrother;
	}
	return GG_NULL;
}

gg_int32 GG_DOMHasChildNodes(gg_handler node)
{
    DOMNode * snode;
    if (!node) {
        LogE("Null node argument");
        return -1;
    }
    snode = (DOMNode *)node;
    if (snode->firstChild) {
        if (NSTAT_DONE == snode->status) {
            return 0;
        } else {
            return -1;
        }
    } else {
        return -1;
    }
}

gg_handler GG_DOMGetAncestor(gg_handler node)
{
    DOMNode * snode;
    if (!node) {
        LogE("Null node argument");
        return GG_NULL;
    }
    snode = (DOMNode *)node;
    if (NSTAT_DONE == snode->status) {
        return snode->ancestor;
    } else {
        return GG_NULL;
    }
}

gg_handler GG_DOMGetFirstChild(gg_handler node)
{
    DOMNode * snode;
    if (!node) {
        LogE("Null node argument");
        return GG_NULL;
    }
    snode = (DOMNode *)node;
    if (NSTAT_DONE == snode->status) {
        return snode->firstChild;
    } else {
        return GG_NULL;
    }
}

gg_handler GG_DOMGetNextBrother(gg_handler node)
{
    DOMNode * snode;
    if (!node) {
        LogE("Null node argument");
        return GG_NULL;
    }
    snode = (DOMNode *)node;
    if (NSTAT_DONE == snode->status) {
        return snode->nextBrother;
    } else {
        return GG_NULL;
    }
}

gg_wchar * GG_DOMGetNodeName(gg_handler node)
{
    DOMNode * snode;
    if (!node) {
        LogE("Null node argument");
        return GG_NULL;
    }
    snode = (DOMNode *)node;
    if (NSTAT_DONE == snode->status) {
        return snode->tag;
    } else {
        return GG_NULL;
    }
}

gg_wchar ** GG_DOMGetNodeAttributes(gg_handler node)
{
    DOMNode * snode;
    if (!node) {
        LogE("Null node argument");
        return GG_NULL;
    }
    snode = (DOMNode *)node;
    if (NSTAT_DONE == snode->status) {
        return snode->attributes;
    } else {
        return GG_NULL;
    }
}

gg_wchar * GG_DOMGetNodeValue(gg_handler node)
{
    DOMNode * snode;
    if (!node) {
        LogE("Null node argument");
        return GG_NULL;
    }
    snode = (DOMNode *)node;
    if (NSTAT_DONE == snode->status) {
        return snode->value;
    } else {
        return GG_NULL;
    }
}




