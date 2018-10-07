
#include "gadget_i.h"

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef GG_DEBUG
//#define THIS_DEBUG
#endif

static gg_int32 dumpNode(DOMNode * node)
{
    gg_int32 i;
    if (node) {
        LogD("Depth (%d)", node->depth);
        LogD("Status (%d)", node->status);
        if (node->tag) {
            LogD("Tag (%s)", node->tag);
        }
        if (node->attributes) {
            for (i = 0; node->attributes[i]; i += 2) {
                LogD("Attributes (%s)=(%s)", node->attributes[i], node->attributes[i + 1]);
            }
        }
        if (node->value) {
            LogD("Value (%s)", node->value);
        }
        LogD("Value size (%d)", node->valueSize);
        if (node->ancestor) {
            if (node->ancestor->tag) {
                LogD("Ancestor (%s)", node->ancestor->tag);
            }
        }
        if (node->preBrother) {
            if (node->preBrother->tag) {
                LogD("Previous brother (%s)", node->preBrother->tag);
            }
        }
        if (node->nextBrother) {
            if (node->nextBrother->tag) {
                LogD("Next brother (%s)", node->nextBrother->tag);
            }
        }
        if (node->firstChild) {
            if (node->firstChild->tag) {
                LogD("First child (%s)", node->firstChild->tag);
            }
        }
        if (node->lastChild) {
            if (node->lastChild->tag) {
                LogD("Last child (%s)", node->lastChild->tag);
            }
        }
    } else {
        LogE("Null DOM node");
    }
    return 0;
}

static gg_int32 dumpAllNodes(DOMNode * root)
{
    DOMNode * start;
    DOMNode * snode;

    if (!root) {
        LogE("Null node argument");
        return -1;
    }

    dumpNode(root);
    start = ((DOMNode *)root)->firstChild;
    while (start) {
        dumpNode(start);
        snode = start->firstChild;
        if (!snode) {
            snode = start->nextBrother;
            if (!snode) {
                while (start->ancestor) {
                    if (start->ancestor == root) {
                        return 0;
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
    return 0;
}

static inline gg_int32 setTreeHierarchy(DOMParseDescriptor * dpd, DOMNode * current)
{
    if (dpd->node && dpd->preNode) {
        if (0 == current->depth) {
            // Root node, do nothing
        } else if (dpd->preNode->depth < current->depth) {
            // Enter the deeper tag hierarchy
            dpd->node->firstChild = current;
            dpd->node->lastChild = current;
            current->ancestor = dpd->node;
            dpd->preNode = dpd->node;
            dpd->node = current;
        } else if (dpd->preNode->depth == current->depth) {
            // Have the same tag hierarchy
            dpd->preNode->nextBrother = current;
            current->ancestor = dpd->preNode->ancestor;
            current->ancestor->lastChild = current;
            current->preBrother = dpd->preNode;
            dpd->node = current;
        } else {
            // if (dpd->preNode->depth > current->depth)
            // Exit the previous child node
            LogE("Unhandled node");
            return -1;
        }
    } else {
        LogE("Insert node failed");
        return -1;
    }
    return 0;
}

static void XMLCALL pdStartElementHandler(void * userData, const XML_Char * name, const XML_Char ** attr)
{
    DOMParseDescriptor * dpd;
    DOMNode * node;
    gg_int32 i;

    if (!userData) {
        LogE("Null user data");
        return;
    }
    dpd = (DOMParseDescriptor *)userData;
    if (-1 == dpd->parser) {
        LogE("XML parse error");
        return;
    }
    if (0 == dpd->depth) {
        dpd->preNode = dpd->node;
        node = dpd->node;
    } else {
        node = (DOMNode *)GG_malloc(sizeof(DOMNode));
        if (!node) {
            LogE("XML parse malloc failed");
            goto error_end;
        }
    }
    GG_memset(node, 0, sizeof(DOMNode));
    node->tag = (gg_wchar *)GG_DupStr((gg_char *)name);
    if (!(node->tag)) {
        LogE("XML parse dupstr failed");
        goto error_end;
    }
    if (attr[0]) {
        for (i = 0; attr[i]; i += 2) {
            ;
        }
        i = sizeof(gg_wchar *) * (i + 2);
        node->attributes = (gg_wchar **)GG_malloc(i);
#ifdef THIS_DEBUG
        LogMI("Malloc point array (%d,%d)", i, sizeof(gg_wchar *));
#endif
        if (!(node->attributes)) {
            LogE("XML parse malloc failed");
            goto error_end;
        }
        GG_memset(node->attributes, 0, i);
        for (i = 0; attr[i]; i += 2) {
            node->attributes[i] = (gg_wchar *)GG_DupStr((gg_char *)(attr[i]));
            if (!(node->attributes[i])) {
                LogE("XML parse dupstr failed");
                goto error_end;
            }
            node->attributes[i + 1] = (gg_wchar *)GG_DupStr((gg_char *)(attr[i + 1]));
            if (!(node->attributes[i + 1])) {
                LogE("XML parse dupstr failed");
                goto error_end;
            }
        }
    }
    /* Set the DOM node status */
    node->depth = dpd->depth;
    node->status = NSTAT_START;
    /* Start setting tree hierarchy */
    if (-1 == setTreeHierarchy(dpd, node)) {
        goto error_end;
    }
    /* End of tree hierarchy setting */
    dpd->depth++;

#ifdef THIS_DEBUG
    //LogD("Enter >>>>>>>>>>>>>>");
    //dumpNode(dpd->preNode);
    //dumpNode(dpd->node);
    //LogD("Enter <<<<<<<<<<<<<<");
#endif
    return;

error_end:
    dpd->parser = -1;
    if (node) {
        if (node->attributes) {
            for (i = 0; node->attributes[i]; i++) {
                GG_free(node->attributes[i]);
                node->attributes[i] = GG_NULL;
            }
        }
        GG_free(node->attributes);
        node->attributes = GG_NULL;
        GG_free(node->tag);
        node->tag = GG_NULL;
        if (0 != node->depth) {
            GG_free(node);
            node = GG_NULL;
        }
    }
    dpd->depth++;
}

static void XMLCALL pdEndElementHandler(void * userData, const XML_Char * name)
{
    DOMParseDescriptor * dpd;

    if (!userData) {
        LogE("Null user data");
        return;
    }
    dpd = (DOMParseDescriptor *)userData;
    if (-1 == dpd->parser) {
        LogE("XML parse error");
        return;
    }
    dpd->depth--;
    dpd->node->status = NSTAT_DONE;
#ifdef THIS_DEBUG
    //LogD("End >>>>>>>>>>>>>>>>>");
    //dumpNode(dpd->preNode);
    //dumpNode(dpd->node);
    //LogD("End <<<<<<<<<<<<<<<<<");
#endif
    dpd->preNode = dpd->node;
    dpd->node = dpd->preNode->ancestor;
}

static void XMLCALL pdDataHandler(void * userData, const XML_Char * s, int len)
{
    DOMParseDescriptor * dpd;
    gg_int32 i;
    gg_wchar * temp;

    if (!userData) {
        LogE("Null user data");
        return;
    }
    dpd = (DOMParseDescriptor *)userData;
    if (-1 == dpd->parser) {
        LogE("XML parse error");
        return;
    }
    // Limitation
    // Don't append the next data to tag value, just get the first segment tag value
    if ((dpd->node) && (NSTAT_START == dpd->node->status || NSTAT_DATA == dpd->node->status)) {
        for (i = 0; (i < len) && (s[i] == GG_LF || s[i] == GG_NL|| s[i] == GG_TAB); i++);
        if (i < len) {
            temp = (gg_wchar *)GG_malloc(dpd->node->valueSize + len + 2 - i);
            if (!(temp)) {
                dpd->parser = -1;
                LogE("XML parse malloc failed");
                return;
            }
            if (dpd->node->valueSize && dpd->node->value) {
                GG_memcpy(temp, dpd->node->value, dpd->node->valueSize);
                GG_free(dpd->node->value);
                dpd->node->value = GG_NULL;
#ifdef THIS_DEBUG
                LogD("Realloc");
#endif
            }
            GG_memcpy(&temp[dpd->node->valueSize], &s[i], len - i);
            dpd->node->valueSize = dpd->node->valueSize + len - i;
            dpd->node->value = temp;
            dpd->node->value[dpd->node->valueSize] = '\0';
            dpd->node->value[dpd->node->valueSize + 1] = '\0';
            dpd->node->status = NSTAT_DATA;
        }
#ifdef THIS_DEBUG
        else {
            LogD("Tag (%s): Null node value", dpd->node->tag);
        }
#endif
    } else {
        dpd->parser = -1;
        LogE("Get node value failed");
#ifdef THIS_DEBUG
        dumpNode(dpd->node);
#endif
    }
}

gg_int32 GG_XMLParse(char * xmlfile, DOMParseDescriptor * ctl)
{
    __hdle hdfile;
    const XML_Char * encodingName = NULL;
    XML_Parser parser;
    enum XML_ParamEntityParsing peParsing = XML_PARAM_ENTITY_PARSING_NEVER;
    XML_Char * buffer;
    int readsize;
    int readCnt;
    int done;
    DOMNode * root;

    if (NULL == xmlfile || NULL == ctl) {
        LogE("Arguments error");
        return -1;
    }

    buffer = (XML_Char * )esMEMS_Malloc(0, GG_XMLPARSE_BUFF_LEN);
    if (NULL == buffer) {
        LogE("Malloc buffer failed");
        return -1;
    }

    hdfile = esFSYS_fopen(xmlfile, "rb");
    if (NULL == hdfile) {
        esMEMS_Mfree(0, buffer);
        LogE("Open device profile(%s) failed", xmlfile);
        return -1;
    }
    parser = XML_ParserCreate(encodingName);
    if (NULL == parser) {
        esFSYS_fclose(hdfile);
        esMEMS_Mfree(0, buffer);
        LogE("Create XML parser failed");
        return -1;
    }
    ctl->parser = 0;
    ctl->depth = 0;
    ctl->preNode = GG_NULL;
    root = ctl->node;

    eLIBs_memset(buffer, 0, GG_XMLPARSE_BUFF_LEN);
    XML_SetParamEntityParsing(parser, peParsing);
    XML_SetElementHandler(parser, pdStartElementHandler, pdEndElementHandler);
    XML_SetCharacterDataHandler(parser, pdDataHandler);
    XML_SetUserData(parser, ctl);
    done = XML_FALSE;
    readCnt = 0;
    while (1) {
        readsize = esFSYS_fread(buffer, 1, GG_XMLPARSE_BUFF_LEN, hdfile);
        if (readsize <= 0) {
            if (readCnt > 0) {
                done = XML_TRUE;
            } else {
                LogE("Read file end or failed");
            }
            break;
        }
#ifdef THIS_DEBUG
        LogD("Read file (%d)", readsize);
#endif
        readCnt += readsize;
        // End file reading
        if (readsize < GG_XMLPARSE_BUFF_LEN) {
            done = XML_TRUE;
        }
        if (XML_Parse(parser, (const char *)buffer, readsize, done) == XML_STATUS_ERROR) {
            LogE("Parse error at line %d, %s\n", XML_GetCurrentLineNumber(parser),
              XML_ErrorString(XML_GetErrorCode(parser)));
            break;
        }
    }
    XML_ParserFree(parser);
    esFSYS_fclose(hdfile);
    esMEMS_Mfree(0, buffer);
    if (done && (0 == ctl->parser) && (0 == ctl->depth)) {
#ifdef THIS_DEBUG
        dumpAllNodes(root);
#endif
        return 0;
    } else {
        dumpAllNodes(root);
        return -1;
    }
}

