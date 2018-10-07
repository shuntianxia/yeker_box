/*
****************************************************************************************************
*                                               MELIS
*                               the Easy Portable/Player Develop Kits
*                                           UART Module
*
*                           (c) Copyright 2011-2014, All winners Co,Ld.
*                                       All Rights Reserved
*
* File    : bsp_uart_i.h
* By      : james.deng
* Version : 1.0.0
* Date    : 2011-12-19
* Descript: the private header of uart module.
* Update  : <date>          <author>            <version>           <notes>
*           2011-12-19      james.deng          1.0.0               build the file.
****************************************************************************************************
*/

#ifndef __BSP_UART_I_H__
#define __BSP_UART_I_H__

#include "epdk.h"
#include "bsp_uart_buff.h"

//------------------------------------------------------------------------------
// Register read and write operations
//------------------------------------------------------------------------------
#define READ_REG8 (reg)			(*(volatile unsigned char  *)(reg))
#define READ_REG16(reg)			(*(volatile unsigned short *)(reg))
#define READ_REG32(reg)			(*(volatile unsigned int   *)(reg))

#define WRITE_REG8 (Value, reg)	(*(volatile unsigned char  *)(reg) = (Value))
#define WRITE_REG16(Value, reg)	(*(volatile unsigned short *)(reg) = (Value))
#define WRITE_REG32(Value, reg)	(*(volatile unsigned int   *)(reg) = (Value))

//------------------------------------------------------------------------------
// Basic bit operations
//------------------------------------------------------------------------------
#define SET_REG_BIT(val, pos)               ((val) |= 1U<<(pos))
#define CLEAR_REG_BIT(val, pos)             ((val) &= ~(1U<<(pos)))
#define TEST_REG_BIT(val, pos)              ((val) & (1U<<(pos)))
#define BITS_REG_MOD(len)                   ((1U<<(len)) - 1)
#define BITS_REG_MASK(len, pos)             (~(BITS_REG_MOD(len)<<(pos)))
#define CLEAR_REG_BITS(val, pos, len)       (val) &= BITS_REG_MASK(len, pos) 
#define SET_REG_BITS(val, pos, len, _val)   (val) = ((val) & BITS_REG_MASK(len, pos)) | (((_val) & BITS_REG_MOD(len))<<(pos))
#define TEST_REG_BITS(val, pos, len, _val)  ((val) & (~BITS_REG_MASK(len, pos))) == ((_val)<<(pos))
#define GET_REG_BITS_VAL(val, pos, len)     (((val)>>(pos)) & BITS_REG_MOD(len))

#define UART0_REG_BASE                  0x01c25000

extern __u32 UART0_REG_BASE_VA;

// uart register definition
#define UART_REG_SIZE                   0x400
#define UART_REG_ADDR(port)             (UART0_REG_BASE_VA + port * UART_REG_SIZE)

#define UART_REG_RBR(port)              (UART_REG_ADDR(port) + 0x00)
#define UART_REG_THR(port)              (UART_REG_ADDR(port) + 0x00)
#define UART_REG_DLL(port)              (UART_REG_ADDR(port) + 0x00)
#define UART_REG_DLH(port)              (UART_REG_ADDR(port) + 0x04)
#define UART_REG_IER(port)              (UART_REG_ADDR(port) + 0x04)
#define UART_REG_IIR(port)              (UART_REG_ADDR(port) + 0x08)
#define UART_REG_FCR(port)              (UART_REG_ADDR(port) + 0x08)
#define UART_REG_LCR(port)              (UART_REG_ADDR(port) + 0x0c)
#define UART_REG_MCR(port)              (UART_REG_ADDR(port) + 0x10)
#define UART_REG_LSR(port)              (UART_REG_ADDR(port) + 0x14)
#define UART_REG_MSR(port)              (UART_REG_ADDR(port) + 0x18)
#define UART_REG_SCH(port)              (UART_REG_ADDR(port) + 0x1c)
#define UART_REG_USR(port)              (UART_REG_ADDR(port) + 0x7c)
#define UART_REG_TFL(port)              (UART_REG_ADDR(port) + 0x80)
#define UART_REG_RFL(port)              (UART_REG_ADDR(port) + 0x84)
#define UART_REG_HALT(port)             (UART_REG_ADDR(port) + 0xa4)

// Divisor Latch Low Register
#define UART_REG_DLL_DLL_BIT            0

// Divisor Latch Hiht Register
#define UART_REG_DLH_DLH_BIT            0

// Interrupt Enable Register
#define UART_REG_IER_PTIME              4
#define UART_REG_IER_EMSI_BIT           3
#define UART_REG_IER_ELSI_BIT           2
#define UART_REG_IER_ETBEI_BIT          1
#define UART_REG_IER_ERBFI_BIT          0

// Interrupt Identity Register
// interrupt definition
#define UART_INT_MS                     0x00                // modem status
#define UART_INT_NIP                    0x01                // no interrupt pending
#define UART_INT_THRE                   0x02                // THR empty
#define UART_INT_RDA                    0x04                // received data available
#define UART_INT_RLS                    0x06                // receiver line status
#define UART_INT_BD                     0x07                // busy detect
#define UART_INI_CT                     0x0c                // character timeout

// FIFO Control Register
#define UART_REG_FCR_RT_BIT             6
#define UART_REG_FCR_TFT_BIT            4
#define UART_REG_FCR_DMAM_BIT           3
#define UART_REG_FCR_XFIFOR_BIT         2
#define UART_REG_FCR_RFIFOR_BIT         1
#define UART_REG_FCR_FIFOE_BIT          0

// Line Control Register
#define UART_REG_LCR_DLAB_BIT           7
#define UART_REG_LCR_BC_BIT             6
#define UART_REG_LCR_EPS_BIT            4
#define UART_REG_LCR_PEN_BIT            3
#define UART_REG_LCR_STOP_BIT           2
#define UART_REG_LCR_DLS_BIT            0

// Modem Control Register
#define UART_REG_MCR_SIRE_BIT           6
#define UART_REG_MCR_AFCE_BIT           5
#define UART_REG_MCR_LOOP_BIT           4
#define UART_REG_MCR_RTS_BIT            1
#define UART_REG_MCR_DTR_BIT            0

// Line Status Register
#define UART_REG_LSR_FIFOERR_BIT        7
#define UART_REG_LSR_TEMT_BIT           6
#define UART_REG_LSR_THRE_BIT           5
#define UART_REG_LSR_BI_BIT             4
#define UART_REG_LSR_FE_BIT             3
#define UART_REG_LSR_PE_BIT             2
#define UART_REG_LSR_OE_BIT             1
#define UART_REG_LSR_DR_BIT             0
                                              
// Status Register
#define UART_REG_USR_RFF_BIT            4
#define UART_REG_USR_RFNE_BIT           3
#define UART_REG_USR_TFE_BIT            2
#define UART_REG_USR_TFNF_BIT           1
#define UART_REG_USR_BUSY_BIT           0

#define UART_PORT_FREE                  0
#define UART_PORT_USED                  1

// uart soft buffer size for reading
#define UART_BUFFER_SIZE                4096

// uart port
typedef struct __UART_PORT
{
    __u32           nPort;
    __u32           nUsed;
    __u32           nClkNo;
    __u32           nIntNo;
    
    __hdle          hPin;
    __hdle          hClk;
    __u32           nType;
    __u32           nStatus;
    __uart_buffer_t rBuffer;
    __krnl_event_t  *wSem;
} __uart_port_t;

#endif // __BSP_UART_I_H__
