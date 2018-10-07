/*
****************************************************************************************************
*                                               MELIS
*                               the Easy Portable/Player Develop Kits
*                                           SDMMC Module
*
*                           (c) Copyright 2011-2014, All winners Co,Ld.
*                                       All Rights Reserved
*
* File    : bsp_uart_buff.h
* By      : james.deng
* Version : 1.0.0
* Date    : 2011-12-19
* Descript: the header of uart buffer.
* Update  : <date>          <author>            <version>           <notes>
*           2011-12-19      james.deng          1.0.0               build the file.
****************************************************************************************************
*/

#ifndef __BSP_UART_BUFF_H__
#define __BSP_UART_BUFF_H__

typedef struct __UART_BUFFER
{
    __u8  *pStartAddr;
    __u8  *pEndAddr;
    __u32 nBufferSize;
    
    __u8  *pHead;
    __u8  *pTail;
} __uart_buffer_t;

/*
****************************************************************************************************
*                                       BSP_UART_BufferCreate
*              
* Description: Create a buffer for uart port.
*              
* Arguments  : pBuffer      : the buffer pointer, allocated by caller.
*              nBufferSize  : the size of buffer in bytes.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_BufferCreate(__uart_buffer_t *pBuffer, __u32 nBufferSize);

/*
****************************************************************************************************
*                                       BSP_UART_BufferDestory
*              
* Description: Destory the uart buffer.
*              
* Arguments  : pBuffer      : the uart buffer.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_BufferDestory(__uart_buffer_t *pBuffer);

/*
****************************************************************************************************
*                                       BSP_UART_BufferClear
*              
* Description: Clear uart buffer.
*              
* Arguments  : pBuffer      : the uart buffer.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_BufferClear(__uart_buffer_t *pBuffer);

/*
****************************************************************************************************
*                                       BSP_UART_BufferRead
*              
* Description: Read data from uart buffer.
*              
* Arguments  : pBuffer      : the uart buffer.
*              pData        : Storage location for data.
*              nSize        : Maximum number of items to be write.
*              
* Return     : __u32        : the number of full items actually read, which may be less than count
*                             if buffer hasn't so much data.
*              
* Notes      : 
*              
****************************************************************************************************
*/
__u32 BSP_UART_BufferRead(__uart_buffer_t *pBuffer, __u8 *pData, __u32 nSize);

/*
****************************************************************************************************
*                                       BSP_UART_BufferWrite
*              
* Description: Write data to uart buffer.
*              
* Arguments  : pBuffer      : the uart buffer.
*              pData        : Storage location for data. The receiver buffer register address.
*              nSize        : Maximum number of items to be write.
*              
* Return     : __u32        : the the number of full items actually written.
*              
* Notes      : Make sure nSize is less than buffer size, overflow occur otherwise.
*              
****************************************************************************************************
*/
__u32 BSP_UART_BufferWrite(__uart_buffer_t *pBuffer, const __u8 *pData, __u32 nSize);

#endif // __BSP_UART_BUFF_H__
