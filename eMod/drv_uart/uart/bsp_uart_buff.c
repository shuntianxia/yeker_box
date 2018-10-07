/*
****************************************************************************************************
*                                               MELIS
*                               the Easy Portable/Player Develop Kits
*                                           UART Module
*
*                           (c) Copyright 2011-2014, All winners Co,Ld.
*                                       All Rights Reserved
*
* File    : bsp_uart_buff.c
* By      : james.deng
* Version : 1.0.0
* Date    : 2011-12-19
* Descript: the implement of uart buffer.
* Update  : <date>          <author>            <version>           <notes>
*           2011-12-19      james.deng          1.0.0               build the file.
*           2012-02-11      james.deng          1.1.0               change the implement of buffer.
****************************************************************************************************
*/

//#include <string.h>
#include "bsp_uart_i.h"

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
__s32 BSP_UART_BufferCreate(__uart_buffer_t *pBuffer, __u32 nBufferSize)
{
    if (NULL == pBuffer || 0 == nBufferSize)
    {
        __wrn("invalid parameters\n");
        return EPDK_FAIL;
    }
    
    memset(pBuffer, 0, sizeof(__uart_buffer_t));
    
    pBuffer->pStartAddr = esMEMS_Balloc(nBufferSize);
    if (NULL == pBuffer->pStartAddr)
    {
        __wrn("allocate memory failed\n");
        return EPDK_FAIL;
    }
    
    pBuffer->pEndAddr    = pBuffer->pStartAddr + nBufferSize;
    pBuffer->nBufferSize = nBufferSize;
    pBuffer->pHead       = pBuffer->pStartAddr;
    pBuffer->pTail       = pBuffer->pStartAddr;
    
    return EPDK_OK;
}

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
__s32 BSP_UART_BufferDestory(__uart_buffer_t *pBuffer)
{
    if (NULL == pBuffer)
    {
        __wrn("invalid parameter\n");
        return EPDK_FAIL;
    }
    
    if (pBuffer->pStartAddr)
    {
        esMEMS_Bfree(pBuffer->pStartAddr, pBuffer->nBufferSize);
    }
    
    memset(pBuffer, 0, sizeof(__uart_buffer_t));
    
    return EPDK_OK;
}

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
__s32 BSP_UART_BufferClear(__uart_buffer_t *pBuffer)
{
    __s32 cpu_sr;
    
    if (NULL == pBuffer)
    {
        __wrn("invalid parameter\n");
        return EPDK_FAIL;
    }
    
    // read and flush is mutex
    // only need to prevent interrupt
    ENTER_CRITICAL(cpu_sr);
    pBuffer->pHead = pBuffer->pTail = pBuffer->pStartAddr;
    EXIT_CRITICAL(cpu_sr);
    
    return EPDK_OK;
}

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
__u32 BSP_UART_BufferRead(__uart_buffer_t *pBuffer, __u8 *pData, __u32 nSize)
{
    __s32 nRead;
    __u8  *pRead;
    
    if (NULL == pBuffer || NULL == pData || 0 == nSize)
    {
        __wrn("invalid parameter\n");
        return 0;
    }
    
    // access pTail once, it's OK?
    pRead = pBuffer->pHead;
    nRead = pBuffer->pTail - pRead;
    if (nRead < 0)
    {
        // only read right part
        nRead = pBuffer->pEndAddr - pRead;
    }
    else if (nRead == 0)
    {
        // buffer is empty
        return 0;
    }
    
    if (nSize < nRead)
    {
        // only read nSize
        nRead = nSize;
    }
    
    // copy data to destination
    memcpy(pData, pRead, nRead);
    
    // update pHead
    pBuffer->pHead += nRead;
    if (pBuffer->pHead >= pBuffer->pEndAddr)
    {
        // redirect pHead
        pBuffer->pHead = pBuffer->pStartAddr;
    }
    
    return nRead;
}

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
__u32 BSP_UART_BufferWrite(__uart_buffer_t *pBuffer, const __u8 *pData, __u32 nSize)
{
    __u32 nWrite;
    __u8  *pWrite;
    
    if (NULL == pBuffer || NULL == pData || 0 == nSize)
    {
        __wrn("invalid parameter\n");
        return 0;
    }
    
    
    for (nWrite = 0, pWrite = pBuffer->pTail; nWrite < nSize; nWrite++, pWrite++)
    {
        if (pWrite >= pBuffer->pEndAddr)
        {
            // redirect pWrite
            pWrite = pBuffer->pStartAddr;
        }
        
        // read fifo
        *pWrite = *pData;
    }
    
    // update pTail
    pBuffer->pTail = pWrite;
    
    return nWrite;
}
