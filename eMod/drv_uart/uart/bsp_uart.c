/*
****************************************************************************************************
*                                               MELIS
*                               the Easy Portable/Player Develop Kits
*                                           UART Module
*
*                           (c) Copyright 2011-2014, All winners Co,Ld.
*                                       All Rights Reserved
*
* File    : bsp_uart.c
* By      : james.deng
* Version : 1.0.0
* Date    : 2011-12-19
* Descript: the implement of uart module.
* Update  : <date>          <author>            <version>           <notes>
*           2011-12-19      james.deng          1.0.0               build the file.
*           2012-02-11      james.deng          1.0.0               clear FIFO before and after set
*                                                                   the baudrate of uart.
*                                                                   check uart busy.
****************************************************************************************************
*/

#include "bsp_uart_i.h"


__u32 UART0_REG_BASE_VA;

static __uart_port_t UartPorts[UART_PORT_COUNT] =
{
    {0, UART_PORT_FREE, CSP_CCM_MOD_CLK_APB_UART0, INTC_IRQNO_UART0},
    {1, UART_PORT_FREE, CSP_CCM_MOD_CLK_APB_UART1, INTC_IRQNO_UART1},
    {2, UART_PORT_FREE, CSP_CCM_MOD_CLK_APB_UART2, INTC_IRQNO_UART2}
};



__inline static void uart_fifo_reset(__uart_port_t *pUart)
{
    __u32 value;
    __u32 i;
    
    value = READ_REG32(UART_REG_FCR(pUart->nPort));
    SET_REG_BIT(value, UART_REG_FCR_XFIFOR_BIT);
    SET_REG_BIT(value, UART_REG_FCR_RFIFOR_BIT);
    WRITE_REG32(value, UART_REG_FCR(pUart->nPort));
    
    for (i = 0; i < 0xff; i++);
}

__inline static void uart_enable_tbei(__uart_port_t *pUart)
{
    __u32 value;
    
    value = READ_REG32(UART_REG_IER(pUart->nPort));
    SET_REG_BIT(value, UART_REG_IER_ETBEI_BIT);
    WRITE_REG32(value, UART_REG_IER(pUart->nPort));
}

__inline static void uart_disable_tbei(__uart_port_t *pUart)
{
    __u32 value;
    
    value = READ_REG32(UART_REG_IER(pUart->nPort));
    CLEAR_REG_BIT(value, UART_REG_IER_ETBEI_BIT);
    WRITE_REG32(value, UART_REG_IER(pUart->nPort));
}

__inline static void uart_enable_rbfi(__uart_port_t *pUart)
{
    __u32 value;
    
    value = READ_REG32(UART_REG_IER(pUart->nPort));
    SET_REG_BIT(value, UART_REG_IER_ERBFI_BIT);
    WRITE_REG32(value, UART_REG_IER(pUart->nPort));
}

__inline static void uart_disable_rbfi(__uart_port_t* pUart)
{
    __u32 value;
    
    value = READ_REG32(UART_REG_IER(pUart->nPort));
    CLEAR_REG_BIT(value, UART_REG_IER_ERBFI_BIT);
    WRITE_REG32(value, UART_REG_IER(pUart->nPort));
}

__inline static void uart_thr_empty(__uart_port_t *pUart)
{
    // post write semaphore
    __msg("post uart%d write semaphore\n", pUart->nPort);
    esKRNL_SemPost(pUart->wSem);
}

static __s32 uart_recv_data(__uart_port_t *pUart)
{
    __u32 count;
    __s32 ret = EPDK_FAIL;
    
    // disable received data available interrupt
    uart_disable_rbfi(pUart);
    
    count = READ_REG32(UART_REG_RFL(pUart->nPort)) & 0x7f;
    if (count)
    {
        ret = BSP_UART_BufferWrite(&pUart->rBuffer, (const __u8 *)UART_REG_RBR(pUart->nPort), count);
    }
    
    // enable received data available interrupt
    uart_enable_rbfi(pUart);
    
    return ret;
}

__inline static void uart_recv_line_status(__uart_port_t *pUart)
{
    __u32 value = 0;
    
    value |= READ_REG32(UART_REG_USR(pUart->nPort)) << 8;
    value |= READ_REG32(UART_REG_LSR(pUart->nPort)) << 16;
    value |= READ_REG32(UART_REG_MSR(pUart->nPort)) << 24;
    
    pUart->nStatus = value;
}

__inline static void uart_busy_detect(__uart_port_t *pUart)
{
    pUart->nStatus = 0x01;
}

static __s32 uart_timeout(__uart_port_t *pUart)
{
    __u32 count;
    __s32 ret = EPDK_FAIL;
    
    // disable received data available interrupt
    uart_disable_rbfi(pUart);
    
    count = READ_REG32(UART_REG_RFL(pUart->nPort)) & 0x7f;
    if (count)
    {
        ret = BSP_UART_BufferWrite(&pUart->rBuffer, (const __u8 *)UART_REG_RBR(pUart->nPort), count);
    }
    else
    {
        __u32 value;
        value = READ_REG32(UART_REG_RBR(pUart->nPort));
    }
    
    // enable received data available interrupt
    uart_enable_rbfi(pUart);
    
    return ret;
}

static __bool uart_irq_handler(void *p_arg)
{
    __uart_port_t *pUart = (__uart_port_t *)p_arg;
    __u32         iid;
    __bool        ret = EPDK_FALSE;
    
    __msg("enter uart%d isr...\n", pUart->nPort);
    if (NULL == pUart)
        return ret;
    
    iid = READ_REG32(UART_REG_IIR(pUart->nPort)) & 0x0f;
    switch (iid)
    {
        case UART_INT_MS:
        {
            __msg("modem status interrupt\n");
            break;
        }
        
        case UART_INT_NIP:
        {
            __msg("no interrupt pending interrupt\n");
            break;
        }
        
        case UART_INT_THRE:
        {
            __msg("thr empty interrupt\n");
            if (pUart->nType == 4 || pUart->nType == 8)
            {
                uart_thr_empty(pUart);
                ret = EPDK_TRUE;
            }
            
            break;
        }
        
        case UART_INT_RDA:
        {
            __msg("received data available interrupt\n");
            uart_recv_data(pUart);
            ret = EPDK_TRUE;
            break;
        }
        
        case UART_INT_RLS:
        {
            __msg("receiver line status interrupt\n");
            uart_recv_line_status(pUart);
            ret = EPDK_TRUE;
            break;
        }
        
        case UART_INT_BD:
        {
            // uart is busy right row
            __msg("busy detect interrupt\n");
            uart_busy_detect(pUart);
            ret = EPDK_TRUE;
            break;
        }
        
        case UART_INI_CT:
        {
            __msg("character timeout interrupt\n");
            uart_timeout(pUart);
            ret = EPDK_TRUE;
            break;
        }
        
        default:
            break;
    }
    
    return ret;
}

#define UART_IrqHandlerGen(n)                    \
static __bool UART##n##_IsrCallBack(void *p_arg) \
{                                                \
    return uart_irq_handler(p_arg);              \
}

UART_IrqHandlerGen(0)
UART_IrqHandlerGen(1)
UART_IrqHandlerGen(2)

static __bool (*UART_ISRCallback[])(void *p_arg) =
{
    UART0_IsrCallBack, UART1_IsrCallBack, UART2_IsrCallBack
};

static __s32 uart_request_clock(__u32 nPort)
{
    __s32 ret;
    
    UartPorts[nPort].hClk = esCLK_OpenMclk(UartPorts[nPort].nClkNo);
    if (NULL == UartPorts[nPort].hClk)
    {
        __wrn("open uart%d clock failed\n", nPort);
        return EPDK_FAIL;
    }
    esCLK_MclkReset(UartPorts[nPort].hClk,0);
    ret = esCLK_MclkOnOff(UartPorts[nPort].hClk, CLK_ON);
    if (ret)
    {
        __wrn("set uart%d clock on failed\n", nPort);
        return EPDK_FAIL;
    }
    
    return EPDK_OK;
}

static __s32 uart_release_clock(__u32 nPort)
{
    __s32 ret;
    
    if (NULL == UartPorts[nPort].hClk)
    {
        __wrn("clock uart%d clock didn't open\n", nPort);
        return EPDK_FAIL;
    }
    ret = esCLK_MclkOnOff(UartPorts[nPort].hClk, CLK_OFF);
    esCLK_MclkReset(UartPorts[nPort].hClk,1);
	esCLK_CloseMclk(UartPorts[nPort].hClk);
    UartPorts[nPort].hClk = NULL ;
    return EPDK_OK;
}
static __s32 uart_get_type(__u32 nPort)
{
    char  uart_para[16] = "uart_para0";
    __s32 uart_type;
    __s32 ret;
    
    uart_para[9] = '0' + nPort;
    ret = esCFG_GetKeyValue(uart_para, "uart_type", &uart_type, 1);
    if (ret)
    {
        __wrn("get uart%d type failed\n", nPort);
        return ret;
    }
    
    return uart_type;
}

static __s32 uart_config(__u32 nPort)
{
    __u32 value;
    __s32 ret = EPDK_OK;
    
    // get uart port type
    UartPorts[nPort].nType = uart_get_type(nPort);
    __msg("uart%d port type: %d\n", nPort, UartPorts[nPort].nType);
    
    // config uart according to it's type
    switch (UartPorts[nPort].nType)
    {
        // Auto flow control disable
        case 2:
        {
            // default work mode is UART_WORK_MODE and disable auto flow control
            WRITE_REG32(0, UART_REG_MCR(nPort));
            
            // enable FIFO
            value = (0x01 << UART_REG_FCR_RT_BIT) | (0x01 << UART_REG_FCR_XFIFOR_BIT) | (0x01 << UART_REG_FCR_RFIFOR_BIT) | (0x01 << UART_REG_FCR_FIFOE_BIT);
            WRITE_REG32(value, UART_REG_FCR(nPort));
            
            // enable receiver line status interrupts
            // enable received data available interrupts, 0x05
            value = (0x01 << UART_REG_IER_ELSI_BIT) | (0x01 << UART_REG_IER_ERBFI_BIT);
            WRITE_REG32(value, UART_REG_IER(nPort));
            
            break;
        }
        
        // Half auto flow control enable
        case 3:
        {
            // default work mode is UART_WORK_MODE and enable auto flow control, 0x22
            value = (0x01 << UART_REG_MCR_AFCE_BIT) | (0x01 << UART_REG_MCR_RTS_BIT);
            WRITE_REG32(value, UART_REG_MCR(nPort));
            
            // enable FIFO, 0xc7
            value = (0x03 << UART_REG_FCR_RT_BIT) | (0x01 << UART_REG_FCR_XFIFOR_BIT) | (0x01 << UART_REG_FCR_RFIFOR_BIT) | (0x01 << UART_REG_FCR_FIFOE_BIT);
            WRITE_REG32(value, UART_REG_FCR(nPort));
            
            // enable receiver line status interrupts
            // enable received data available interrupts, 0x05
            value = (0x01 << UART_REG_IER_ELSI_BIT) | (0x01 << UART_REG_IER_ERBFI_BIT);
            WRITE_REG32(value, UART_REG_IER(nPort));
            
            break;
        }
        
        // Auto flow control enable
        case 4:
        {
            // default work mode is UART_WORK_MODE and enable auto flow control, 0x22
            value = (0x01 << UART_REG_MCR_AFCE_BIT) | (0x01 << UART_REG_MCR_RTS_BIT);
            WRITE_REG32(value, UART_REG_MCR(nPort));
            
            // enable FIFO, 0xf7
            value = (0x03 << UART_REG_FCR_RT_BIT) | (0x03 << UART_REG_FCR_TFT_BIT) | (0x01 << UART_REG_FCR_XFIFOR_BIT) | (0x01 << UART_REG_FCR_RFIFOR_BIT) | (0x01 << UART_REG_FCR_FIFOE_BIT);
            WRITE_REG32(value, UART_REG_FCR(nPort));
            
            // enable receiver line status interrupts
            // enable transmit holding register empty interrupt
            // enable received data available interrupts, 0x07
            value = (0x01 << UART_REG_IER_ELSI_BIT) | (0x01 << UART_REG_IER_ETBEI_BIT) | (0x01 << UART_REG_IER_ERBFI_BIT);
            WRITE_REG32(value, UART_REG_IER(nPort));
            
            break;
        }
        
        case 8:
        {
            // default work mode is UART_WORK_MODE and enable auto flow control, 0x22
            value = (0x01 << UART_REG_MCR_AFCE_BIT) | (0x01 << UART_REG_MCR_RTS_BIT);
            WRITE_REG32(0, UART_REG_MCR(nPort));
            
            // enable FIFO, 0xf7
            value = (0x03 << UART_REG_FCR_RT_BIT) | (0x03 << UART_REG_FCR_TFT_BIT) | (0x01 << UART_REG_FCR_XFIFOR_BIT) | (0x01 << UART_REG_FCR_RFIFOR_BIT) | (0x01 << UART_REG_FCR_FIFOE_BIT);
            WRITE_REG32(0x07, UART_REG_FCR(nPort));
            
            // enable receiver modem status interrupts
            // enable receiver line status interrupts
            // enable transmit holding register empty interrupt
            // enable received data available interrupts, 0x0f
            value = (0x01 << UART_REG_IER_EMSI_BIT) | (0x01 << UART_REG_IER_ELSI_BIT) | (0x01 << UART_REG_IER_ETBEI_BIT) | (0x01 << UART_REG_IER_ERBFI_BIT);
            WRITE_REG32(0, UART_REG_IER(nPort));
            
            break;
        }
        
        default:
        {
            __wrn("uart%d type unsupported\n", nPort);
            ret = EPDK_FAIL;
            break;
        }
    }
    
    return ret;
}

static __s32 uart_request_pins(__u32 nPort)
{
    char            uart_para[16] = "uart_para0";
    __u32           gpio_cnt = 2;
    user_gpio_set_t *gpio_set = NULL;
    __s32           ret = EPDK_FAIL;
    
    uart_para[9] = '0' + nPort;
    gpio_cnt = UartPorts[nPort].nType;
    gpio_set = (user_gpio_set_t *)esMEMS_Balloc(sizeof(user_gpio_set_t) * gpio_cnt);
    if (NULL == gpio_set)
    {
        __wrn("alloc memory for uart%d failed\n", nPort);
        goto out;
    }
    
    ret = esCFG_GetGPIOSecData(uart_para, (void *)gpio_set, gpio_cnt);
    if (ret)
    {
        __wrn("get gpio for uart%d failed\n", nPort);
        goto out;
    }
    
    UartPorts[nPort].hPin = esPINS_PinGrpReq(gpio_set, gpio_cnt);
    if (NULL == UartPorts[nPort].hPin)
    {
        __wrn("request pins for uart%d failed\n", nPort);
        goto out;
    }
    
out:
    if (gpio_set)
    {
        esMEMS_Bfree(gpio_set, sizeof(user_gpio_set_t) * gpio_cnt);
        gpio_set = NULL;
    }
    
    return ret;
}

__inline static __s32 uart_check_read_ready(__uart_port_t *pUart)
{
    return TEST_REG_BIT(READ_REG32(UART_REG_USR(pUart->nPort)), UART_REG_USR_RFNE_BIT);
}

__inline static __s32 uart_check_write_ready(__uart_port_t *pUart)
{
    return TEST_REG_BIT(READ_REG32(UART_REG_USR(pUart->nPort)), UART_REG_USR_TFNF_BIT);
}

/*
****************************************************************************************************
*                                       BSP_UART_Init
*              
* Description: Init uart module.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_Init(void)
{
    UART0_REG_BASE_VA = esMEMS_GetIoVAByPA(UART0_REG_BASE, 0);
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_Exit
*              
* Description: Exit uart module.
*              
* Arguments  : 
*              
* Return     : EPDK_OK
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_Exit(void)
{
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_Request
*              
* Description: Request a uart port.
*              
* Arguments  : nPort        : the port.
*              nBaudrate    : the baudrate of port.
*              pUartPara    : the parameter of uart.
*              nBufferSize  : the size of uart sotf buffer.
*              
* Return     : __hdle       : the handle of uart, NULL indicate fail to request a uart port.
*              
* Notes      : 
*              
****************************************************************************************************
*/
__hdle BSP_UART_Request(__u32 nPort, __u32 nBaudrate, const __uart_para_t *pUartPara, __u32 nBufferSize)
{
    __hdle hUart = NULL;
    __s32  cpu_sr;
    __s32  ret;
    
    if (nPort > UART_PORT_COUNT - 1)
    {
        __wrn("invalid port: %u\n", nPort);
        goto out;
    }
    
    ENTER_CRITICAL(cpu_sr);
    if (UART_PORT_USED == UartPorts[nPort].nUsed)
    {
        __wrn("uart%d used by someone else\n", nPort);
        EXIT_CRITICAL(cpu_sr);
        goto out;
    }
    UartPorts[nPort].nUsed = UART_PORT_USED;
    EXIT_CRITICAL(cpu_sr);
    
    hUart = (__hdle)&UartPorts[nPort];
    
    // request clock resource
    if (uart_request_clock(nPort))
    {
        __wrn("request clock resource for uart%d failed\n", nPort);
        goto out;
    }
    
    // config baudrate before config controller
    ret = BSP_UART_SetBaudrate(hUart, nBaudrate);
    if (ret)
    {
        __wrn("config baudrate for uart%d failed\n", nPort);
        goto out;
    }
    
    // set parameter
    ret = BSP_UART_SetPara(hUart, pUartPara);
    if (ret)
    {
        __wrn("set the parameter of uart%d failed\n", nPort);
        goto out;
    }
    
    // config uart controller
    ret = uart_config(nPort);
    if (ret)
    {
        __wrn("config uart%d controller failed\n", nPort);
        goto out;
    }
    
    // request pins resource
    if (uart_request_pins(nPort))
    {
        __wrn("request pins resource for uart%d failed\n", nPort);
        goto out;
    }
    
    // create uart software buffer
    if (nBufferSize <= 64)
        nBufferSize = UART_BUFFER_SIZE;
    ret = BSP_UART_BufferCreate(&UartPorts[nPort].rBuffer, nBufferSize);
    if (ret)
    {
        __wrn("create uart software buffer failed\n");
        goto out;
    }
    
    // create write semaphore
    UartPorts[nPort].wSem = esKRNL_SemCreate(0);
    if (NULL == UartPorts[nPort].wSem)
    {
        __wrn("create write semaphore for uart%d failed\n", nPort);
        goto out;
    }
    
    // register isr
    ret = esINT_InsISR(UartPorts[nPort].nIntNo, UART_ISRCallback[nPort], (void *)hUart);
    if (ret)
    {
        __wrn("install isr for uart%d failed\n", nPort);
        goto out;
    }
    
    // enable interrupt
    ret = esINT_EnableINT(UartPorts[nPort].nIntNo);
    if (ret)
    {
        __wrn("enable uart%d interrupt failed\n", nPort);
        goto out;
    }
    
    return hUart;
    
out:
    UartPorts[nPort].nUsed = UART_PORT_FREE;
    
    // unregister isr
    // ...
    
    if (UartPorts[nPort].wSem)
    {
        __u8 err;
        esKRNL_SemDel(UartPorts[nPort].wSem, OS_DEL_ALWAYS, &err);
		UartPorts[nPort].wSem = NULL ;
    }
    
    if (UartPorts[nPort].hPin)
    {
        esPINS_PinGrpRel(UartPorts[nPort].hPin, 0);
        UartPorts[nPort].hPin = NULL;
    }
    
    if (UartPorts[nPort].hClk)
    {
        esCLK_MclkOnOff(UartPorts[nPort].hClk, CLK_OFF);
        esCLK_CloseMclk(UartPorts[nPort].hClk);
        UartPorts[nPort].hClk = NULL;
    }
    
    return NULL;
}

/*
****************************************************************************************************
*                                       BSP_UART_Release
*              
* Description: Release a uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_Release(__hdle hUart)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __s32         cpu_sr;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    ENTER_CRITICAL(cpu_sr);
    if (UART_PORT_FREE == pUart->nUsed)
    {
        EXIT_CRITICAL(cpu_sr);
        return EPDK_FAIL;
    }
    pUart->nUsed = UART_PORT_FREE;
    EXIT_CRITICAL(cpu_sr);
    
    esINT_DisableINT(pUart->nIntNo);
    esINT_UniISR(pUart->nIntNo, UART_ISRCallback[pUart->nPort]);
    
    if (UartPorts[pUart->nPort].wSem)
    {
        __u8 err;
        esKRNL_SemDel(UartPorts[pUart->nPort].wSem, OS_DEL_ALWAYS, &err);
		UartPorts[pUart->nPort].wSem = NULL ;
    }
    
    if (pUart->hClk)
    {
        esCLK_MclkOnOff(pUart->hClk, CLK_OFF);
        esCLK_CloseMclk(pUart->hClk);
        pUart->hClk = NULL;
    }
    
    if (pUart->hPin)
    {
        esPINS_PinGrpRel(pUart->hPin, 0);
        pUart->hPin = NULL;
    }
    uart_release_clock(pUart->nPort);
	if(&UartPorts[pUart->nPort].rBuffer)
	{
		BSP_UART_BufferDestory(&UartPorts[pUart->nPort].rBuffer);
	}
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_SetPara
*              
* Description: Set the parameter of uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              pUartPara    : the parameter of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_SetPara(__hdle hUart, const __uart_para_t *pUartPara)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __u32         value;
    
    if (NULL == pUart || NULL == pUartPara)
        return EPDK_FAIL;
    
    value = READ_REG32(UART_REG_LCR(pUart->nPort));
    SET_REG_BITS(value, UART_REG_LCR_EPS_BIT, 1, pUartPara->nEvenParity);
    SET_REG_BITS(value, UART_REG_LCR_PEN_BIT, 1, pUartPara->nParityEnable);
    SET_REG_BITS(value, UART_REG_LCR_STOP_BIT, 1, pUartPara->nStopBit);
    SET_REG_BITS(value, UART_REG_LCR_DLS_BIT, 2, pUartPara->nDataLen);
    WRITE_REG32(value, UART_REG_LCR(pUart->nPort));
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_SetBaudrate
*              
* Description: Set the baudrate of uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              nBaudRate    : the baudrate of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_SetBaudrate(__hdle hUart, __u32 nBaudrate)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __s32         nApbClk;
    __u32         divisor;
    __u32         value;
    __u32         time;
    
    if (NULL == pUart || 0 == nBaudrate)
    {
        return EPDK_FAIL;
    }
    
    nApbClk = esCLK_GetSrcFreq(CSP_CCM_SYS_CLK_APB);
    if (nApbClk < 0)
    {
        return EPDK_FAIL;
    }
    divisor = (nApbClk + (nBaudrate << 3)) / (nBaudrate << 4);
    
    // clear fifo
    uart_fifo_reset(pUart);
    
    // check uart busy
    time = 0xffffff;
    do
    {
        value = READ_REG32(UART_REG_USR(pUart->nPort));
    } while (time-- && TEST_REG_BIT(value, UART_REG_USR_BUSY_BIT));
    if (0 == time)
    {
        eLIBs_printf("ERROR: uart%d set baudrate check busy timeout\n", pUart->nPort);
        return EPDK_FAIL;
    }
    
    value = READ_REG32(UART_REG_LCR(pUart->nPort));
    SET_REG_BIT(value, UART_REG_LCR_DLAB_BIT);
    WRITE_REG32(value, UART_REG_LCR(pUart->nPort));
    
    value = READ_REG32(UART_REG_DLL(pUart->nPort));
    SET_REG_BITS(value, UART_REG_DLL_DLL_BIT, 8, divisor & 0xff);
    WRITE_REG32(value, UART_REG_DLL(pUart->nPort));
    
    value = READ_REG32(UART_REG_DLH(pUart->nPort));
    SET_REG_BITS(value, UART_REG_DLH_DLH_BIT, 8, (divisor >> 8) & 0xff);
    WRITE_REG32(value, UART_REG_DLH(pUart->nPort));
    
    value = READ_REG32(UART_REG_LCR(pUart->nPort));
    CLEAR_REG_BIT(value, UART_REG_LCR_DLAB_BIT);
    WRITE_REG32(value, UART_REG_LCR(pUart->nPort));
    
    // clear fifo
    uart_fifo_reset(pUart);
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_Read
*              
* Description: Read data from a uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              nSize        : item size in bytes.
*              nCount       : Maximum number of items to be read.
*              pBuffer      : Storage location for data.
*              
* Return     : __u32        : the number of full items actually read, which may be less than count
*                             if buffer hasn't so much data.
*              
* Notes      : 
*              
****************************************************************************************************
*/
__u32 BSP_UART_Read(__hdle hUart, __u32 nSize, __u32 nCount, __u8 *pBuffer)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    
    if (NULL == pUart || 0 == nSize || 0 == nCount || NULL == pBuffer)
        return 0;
    
    return BSP_UART_BufferRead(&pUart->rBuffer, pBuffer, nSize * nCount);
}

/*
****************************************************************************************************
*                                       BSP_UART_Write
*              
* Description: Write data to a uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              nSize        : item size in bytes.
*              nCount       : Maximum number of items to be write.
*              pBuffer      : Storage location for data.
*              
* Return     : __u32        : the the number of full items actually written.
*              
* Notes      : 
*              
****************************************************************************************************
*/
__u32 BSP_UART_Write(__hdle hUart, __u32 nSize, __u32 nCount, const __u8 *pBuffer)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __u8          err;
    __u32         count;
    __u8          *pWrite;
    __u32         time;
    
    if (NULL == pUart || 0 == nSize || 0 == nCount || NULL == pBuffer)
        return 0;
    
    pWrite = (__u8 *)pBuffer;
    if (pUart->nType == 2 || pUart->nType == 3)
    {
        for (count = nSize * nCount; count; count--)
        {
            time = 0xffffff;
            while (time-- && uart_check_write_ready(pUart) == 0);
            if (0 == time)
                return nSize * nCount - count;
            
            WRITE_REG32(*pWrite++, UART_REG_THR(pUart->nPort));
        }
    }
    else
    {
        __u32 i;
        __u32 nWrite;
        
        // disable interrupt
        uart_enable_tbei(pUart);
        
        for (count = nSize * nCount; count; count -= nWrite)
        {
            // wait uart is ready
            __msg("wait uart%d is write ready\n", pUart->nPort);
            esKRNL_SemPend(pUart->wSem, 0, &err);
            
            // send data to uart
            __msg("send data to uart%d\n", pUart->nPort);
            nWrite = count < 32 ? count: 32;
            for (i = 0; i < nWrite; i++)
            {
                WRITE_REG32(*pWrite++, UART_REG_THR(pUart->nPort));
            }
        }
        
        // enable interrupt
        uart_disable_tbei(pUart);
    }
    
    return nSize * nCount;
}

/*
****************************************************************************************************
*                                       BSP_UART_ClearFIFO
*              
* Description: Clear the uart port fifo.
*              
* Arguments  : hUart        : the handle of uart.
*              type         : UART_FIFO_TYPE_RX or UART_FIFO_TYPE_TX.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_ClearFIFO(__hdle hUart, __uart_fifo_type_t type)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __u32         value;
    __s32         ret = EPDK_OK;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    value = READ_REG32(UART_REG_FCR(pUart->nPort));
    switch (type)
    {
        case UART_FIFO_TYPE_RX:
            SET_REG_BIT(value, UART_REG_FCR_RFIFOR_BIT);
            WRITE_REG32(value, UART_REG_FCR(pUart->nPort));
            break;
        
        case UART_FIFO_TYPE_TX:
            SET_REG_BIT(value, UART_REG_FCR_XFIFOR_BIT);
            WRITE_REG32(value, UART_REG_FCR(pUart->nPort));
            break;
        
        default:
            ret = EPDK_FAIL;
            break;
    }
    
    return ret;
}

/*
****************************************************************************************************
*                                       BSP_UART_EnableINT
*              
* Description: Enable a uart port interrupt.
*              
* Arguments  : hUart        : the handle of uart.
*              nIrqSrc      : the interrupt source of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_EnableINT(__hdle hUart, __u32 nIrqSrc)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __u32         value;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    value = READ_REG32(UART_REG_IER(pUart->nPort));
    value |= nIrqSrc & 0x8f;
    WRITE_REG32(value, UART_REG_IER(pUart->nPort));
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_DisableINT
*              
* Description: Disable a uart port interrupt.
*              
* Arguments  : hUart        : the handle of uart.
*              nIrqSrc      : the interrupt source of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_DisableINT(__hdle hUart, __u32 nIrqSrc)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __u32         value;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    value = READ_REG32(UART_REG_IER(pUart->nPort));
    value &= ~(nIrqSrc & 0x8f);
    WRITE_REG32(value, UART_REG_IER(pUart->nPort));
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_QueryINT
*              
* Description: Query a uart port interrupt state.
*              
* Arguments  : hUart        : the handle of uart.
*              pStatus      : the status of interrupt. bit[31:24] main interrupt number. bit[4:0] interrupt source.
*              
* Return     : 
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_QueryINT(__hdle hUart, __u32 *pStatus)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    __u32         status = 0;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    status |= pUart->nIntNo << 24;
    status |= READ_REG32(UART_REG_IIR(pUart->nPort)) & 0x0f;
    *pStatus = status;
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_CheckStatus
*              
* Description: Check uart port status.
*              
* Arguments  : hUart        : the handle of uart.
*              pStatus      : the status of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_CheckStatus(__hdle hUart, __u32 *pStatus)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    *pStatus = pUart->nStatus;
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_Lock
*              
* Description: Lock the uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_Lock(__hdle hUart)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    // fix me in isr
    // ...
    
    WRITE_REG32(0x01, UART_REG_HALT(pUart->nPort));
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_Unlock
*              
* Description: Unlock the uart port.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_Unlock(__hdle hUart)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    // fix me in isr
    // ...
    
    WRITE_REG32(0, UART_REG_HALT(pUart->nPort));
    
    return EPDK_OK;
}

/*
****************************************************************************************************
*                                       BSP_UART_DumpReg
*              
* Description: Dump the uart register.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_DumpReg(__hdle hUart, __u32 nLineNo, const char *strFileName)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    eLIBs_printf("// L%d(%s) dump uart%d register\n", nLineNo, strFileName, pUart->nPort);
    eLIBs_printf("DLL : 0x%x\n", READ_REG32(UART_REG_DLL(pUart->nPort)));
    eLIBs_printf("DLH : 0x%x\n", READ_REG32(UART_REG_DLH(pUart->nPort)));
    eLIBs_printf("IER : 0x%x\n", READ_REG32(UART_REG_IER(pUart->nPort)));
    eLIBs_printf("IIR : 0x%x\n", READ_REG32(UART_REG_IIR(pUart->nPort)));
    eLIBs_printf("FCR : 0x%x\n", READ_REG32(UART_REG_FCR(pUart->nPort)));
    eLIBs_printf("LCR : 0x%x\n", READ_REG32(UART_REG_LCR(pUart->nPort)));
    eLIBs_printf("MCR : 0x%x\n", READ_REG32(UART_REG_MCR(pUart->nPort)));
    eLIBs_printf("LSR : 0x%x\n", READ_REG32(UART_REG_LSR(pUart->nPort)));
    eLIBs_printf("MSR : 0x%x\n", READ_REG32(UART_REG_MSR(pUart->nPort)));
    eLIBs_printf("SCH : 0x%x\n", READ_REG32(UART_REG_SCH(pUart->nPort)));
    eLIBs_printf("USR : 0x%x\n", READ_REG32(UART_REG_USR(pUart->nPort)));
    eLIBs_printf("TFL : 0x%x\n", READ_REG32(UART_REG_TFL(pUart->nPort)));
    eLIBs_printf("RFL : 0x%x\n", READ_REG32(UART_REG_RFL(pUart->nPort)));
    eLIBs_printf("HALT: 0x%x\n", READ_REG32(UART_REG_HALT(pUart->nPort)));
    
    return EPDK_FAIL;
}

/*
****************************************************************************************************
*                                       BSP_UART_FlushRecvBuffer
*              
* Description: Only flush recv buffer.
*              
* Arguments  : hUart        : the handle of uart.
*              
* Return     : EPDK_OK
*              EPDK_FAIL
*              
* Notes      : 
*              
****************************************************************************************************
*/
__s32 BSP_UART_FlushRecvBuffer(__hdle hUart)
{
    __uart_port_t *pUart = (__uart_port_t *)hUart;
    
    if (NULL == pUart)
        return EPDK_FAIL;
    
    return BSP_UART_BufferClear(&pUart->rBuffer);
}
