
#include "gadget_i.h"
#include <com_control.h>

#ifdef THIS_DEBUG
#undef THIS_DEBUG
#endif

#ifdef GG_DEBUG
#define THIS_DEBUG
#endif

typedef struct {
    gg_uint32 packetID;
    int init;
    gg_uint8 tail;
} CPP_DPT;

//PACK_STRUCT_BEGIN
typedef struct {
    gg_uint8 preamble[2];
    gg_uint16 id;
    gg_uint16 length;
    gg_uint16 parity;
    gg_uint16 protocol;
} MW_COM_CONTROL_HEADER;
//PACK_STRUCT_END

static gg_uint16 mychksum(void * dataptr, int len)
{
  gg_uint32 acc;
    
  for(acc = 0; len > 1; len -= 2) {
      /*    acc = acc + *((u16_t *)dataptr)++;*/
    acc += *(gg_uint16 *)dataptr;
    dataptr = (void *)((gg_uint16 *)dataptr + 1);
  }

  /* add up any odd byte */
  if(len == 1) {
    acc += htons((gg_uint16)((*(gg_uint8 *)dataptr) & 0xff) << 8);
  }
  acc = (acc >> 16) + (acc & 0xffffUL);

  if((acc & 0xffff0000) != 0) {
    acc = (acc >> 16) + (acc & 0xffffUL);
  }

  return (gg_uint16)acc;
}

static int EscapePacket(unsigned char * dst, unsigned char * src, int len)
{
    int i;
    int d;
    dst[0] = MWCCP_ESCAPE;
    dst[1] = MWCCP_PREAMBLE;
    for (i = 2, d = 2; i < len; i++, d++) {
        if (MWCCP_ESCAPE == (unsigned char)src[i]) {
            dst[d++] = MWCCP_ESCAPE;
        }
        dst[d] = src[i];
    }
    dst[d++] = MWCCP_ESCAPE;
    dst[d++] = MWCCP_TAIL;
    return d;
}

static int UnescapePacket(unsigned char * packet, int len)
{
#define READY       0
#define CONTENT     1
#define END         2
    unsigned char * dst;
    int d;
    int i;
    int status;
    dst = packet;
    for (i = 0, d = 0, status = READY; i < len; i++) {
        // Search the packet header
        if (READY == status) {
            if (MWCCP_ESCAPE == (unsigned char)packet[i]) {
                if (MWCCP_PREAMBLE == (unsigned char)packet[i + 1]) {
                    status = CONTENT;
                    dst[0] = MWCCP_ESCAPE;
                    dst[1] = MWCCP_PREAMBLE;
                    d = 2;
                }
                i++;
            }
        } else if (CONTENT == status) {
            if (MWCCP_ESCAPE == (unsigned char)packet[i]) {
                if (MWCCP_TAIL == (unsigned char)packet[i + 1]) {
                    return d;
                } else if (MWCCP_PREAMBLE == (unsigned char)packet[i + 1]) {
                    LogE("Wrong restart");
                    return -1;
                }
            } else {
                dst[d++] = packet[i];
            }
        }
    }
    return -1;
}

static CPP_DPT ccpDpt = {0, 0, 0};

int CCP_Init(void)
{
    ccpDpt.packetID = 0;
    ccpDpt.tail = 0;
    if (MWCCP_HEARDER_SIZE != sizeof(MW_COM_CONTROL_HEADER)) {
        ccpDpt.init = 0;
        LogE("Packet hearder structure size error");
        return -1;
    }
    ccpDpt.init = 1;
    return 0;
}

void * CCP_VerifyPacket(void * packet, int len)
{
    MW_COM_CONTROL_HEADER * header;
    gg_uint16 chksum;
    gg_uint16 packetLen;

    if (0 == ccpDpt.init) {
        LogE("CCP module didn't initialized");
        return NULL;
    }
    len = UnescapePacket((unsigned char *)packet, len);
    if (-1 == len) {
        LogE("Packet format error");
        return NULL;
    }
    header = (MW_COM_CONTROL_HEADER *)packet;
    if ((MWCCP_ESCAPE != header->preamble[0]) ||
        (MWCCP_PREAMBLE != header->preamble[1])) {
        LogE("Packet preamble error");
        return NULL;
    }
    packetLen = ntohs(header->length);
    if (len < MWCCP_HEARDER_SIZE || packetLen != len) {
        LogE("Packet length error, packet (%d), real len (%d)", packetLen, len);
        return NULL;
    }
    chksum = ntohs(header->parity);
    header->length = 0;
    header->parity = 0;
    if (chksum != mychksum(packet, len)) {
        LogE("Packet checksum error");
        return NULL;
    }
    LogD("Checksum (%x), packet parity (%x)", mychksum(packet, len), chksum);
    return (void *)((unsigned char *)packet + MWCCP_HEARDER_SIZE);
}

int CCP_PackData(void * packet, void * payload, int len, gg_uint16 protocol)
{
    MW_COM_CONTROL_HEADER * header;
    if (!packet || !payload) {
        LogE("Arguments error");
        return -1;
    }
    if (0 == ccpDpt.init) {
        LogE("CCP module didn't initialized");
        return -1;
    }
    header = (MW_COM_CONTROL_HEADER *)payload;
    header->preamble[0] = MWCCP_ESCAPE;
    header->preamble[1] = MWCCP_PREAMBLE;
    header->id = htons(ccpDpt.packetID++);
    header->length = 0;
    header->parity = 0;
    header->protocol = htons(protocol);
    header->parity = htons(mychksum(payload, len));
    header->length = htons(len);
    return EscapePacket((unsigned char *)packet, (unsigned char *)payload, len);
}

gg_uint16 CPP_GetLen(void * packet)
{
    MW_COM_CONTROL_HEADER * header;
    if (0 == ccpDpt.init) {
        LogE("CCP module didn't initialized");
        return 0;
    }
    header = (MW_COM_CONTROL_HEADER *)packet;
    return ntohs(header->length) - MWCCP_HEARDER_SIZE;
}

gg_uint16 CPP_GetProtocol(void * packet)
{
    MW_COM_CONTROL_HEADER * header;
    if (0 == ccpDpt.init) {
        LogE("CCP module didn't initialized");
        return 0;
    }
    header = (MW_COM_CONTROL_HEADER *)packet;
    return ntohs(header->protocol);
}

int CPP_PacketIsEnd(gg_uint8 packetData)
{
    if (0 == ccpDpt.init) {
        LogE("CCP module didn't initialized");
        return 0;
    }
    if (MWCCP_ESCAPE == packetData) {
        ccpDpt.tail = MWCCP_ESCAPE;
        return -1;
    }
    if (MWCCP_ESCAPE == ccpDpt.tail) {
        if (MWCCP_TAIL == packetData) {
            return 0;
        } else {
            ccpDpt.tail = 0;
            return -1;
        }
    }
    return -1;
}



