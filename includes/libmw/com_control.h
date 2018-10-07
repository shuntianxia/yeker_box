
#ifndef H_COM_CONTROL_BENNIS_20130218
#define H_COM_CONTROL_BENNIS_20130218

#ifdef __cplugplug
extern "C" {
#endif

/**
 * Maiwei Smart Home Communication Control Protocol
 *
 * Because the hardware does not have CSMA/CD (carrier sense multiple access / collision detect),
 * therefore, we do not design multiple host system.
 * In this system, there is only one host, multiple devices.
 * The following is the details.
 * 1. There is total 31 devices, the address is from 1 to 31, 0 is reserved for broadcast address.
 * 2. the hierarchy consists of physical layer, network layer, application layer.
 * 3. Commands for network layer: device discovery, data verification, link establish between devices.
 * 4. Commands for physical layer: system startup handle, data transmission.
 * 5. Commands for application layer: Smart home control instructions.
 * Caution:
 * Big endian for all packets
 */

//PACK_STRUCT_BEGIN
typedef struct {
    gg_uint8 index;
    gg_uint8 on;
} MW_COM_CONTROL_PAYLOAD_LAMP;
//PACK_STRUCT_END

//PACK_STRUCT_BEGIN
typedef struct {
    gg_uint8 index;
    gg_uint8 luminance;
} MW_COM_CONTROL_PAYLOAD_ANALOG_LAMP;
//PACK_STRUCT_END

/* MWCCP means Maiwei communication control protocol */
#define MWCCP_HEARDER_SIZE                  6
#define MWCCP_TAIL_SIZE                     2

#define MWCCP_ESCAPE                        0x55
#define MWCCP_PREAMBLE                      0xAA
#define MWCCP_TAIL                          0xCC

#define MWCCP_PROTOCOL_LAMP                 1
#define MWCCP_PROTOCOL_ANANLOG_LAMP         2

#define MWCCP_LAMP_ON                       1
#define MWCCP_LAMP_OFF                      0

extern int CCP_Init(void);
extern void * CCP_VerifyPacket(void * packet, int len);
extern int CCP_PackData(void * packet, void * payload, int len, gg_uint16 protocol);
extern gg_uint16 CPP_GetLen(void * packet);
extern gg_uint16 CPP_GetProtocol(void * packet);
extern int CPP_PacketIsEnd(gg_uint8 packetData);

#ifdef __cplugplug
 }
#endif

#endif


