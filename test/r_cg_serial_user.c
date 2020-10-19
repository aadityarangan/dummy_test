/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011, 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_serial_user.c
* Version      : CodeGenerator for RL78/G13 V2.05.04.02 [20 Nov 2019]
* Device(s)    : R5F100FJ
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 19-10-2020
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt INTST0 r_uart0_interrupt_send
#pragma interrupt INTSR0 r_uart0_interrupt_receive
#pragma interrupt INTCSI11 r_csi11_interrupt
#pragma interrupt INTIICA0 r_iica0_interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_serial.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_extern_glb_var.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
extern volatile uint8_t * gp_csi11_rx_address;         /* csi11 receive buffer address */
extern volatile uint16_t  g_csi11_rx_length;           /* csi11 receive data length */
extern volatile uint16_t  g_csi11_rx_count;            /* csi11 receive data count */
extern volatile uint8_t * gp_csi11_tx_address;         /* csi11 send buffer address */
extern volatile uint16_t  g_csi11_send_length;         /* csi11 send data length */
extern volatile uint16_t  g_csi11_tx_count;            /* csi11 send data count */
extern volatile uint8_t   g_iica0_master_status_flag;  /* iica0 master flag */ 
extern volatile uint8_t   g_iica0_slave_status_flag;   /* iica0 slave flag */
extern volatile uint8_t * gp_iica0_rx_address;         /* iica0 receive buffer address */
extern volatile uint16_t  g_iica0_rx_cnt;              /* iica0 receive data length */
extern volatile uint16_t  g_iica0_rx_len;              /* iica0 receive data count */
extern volatile uint8_t * gp_iica0_tx_address;         /* iica0 send buffer address */
extern volatile uint16_t  g_iica0_tx_cnt;              /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */
void r_iica0_data_direction_handler(void);

/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = RXD0;

    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi11_interrupt
* Description  : This function is INTCSI11 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_csi11_interrupt(void)
{
    volatile uint8_t err_type;
    volatile uint8_t sio_dummy;

    err_type = (uint8_t)(SSR03 & _0001_SAU_OVERRUN_ERROR);
    SIR03 = (uint16_t)err_type;

    if (1U == err_type)
    {
        r_csi11_callback_error(err_type);    /* overrun error occurs */
    }
    else
    {
        if (g_csi11_tx_count > 0U)
        {
            if (0U != gp_csi11_rx_address)
            {
                *gp_csi11_rx_address = SIO11;
                gp_csi11_rx_address++;
            }
            else
            {
                sio_dummy = SIO11;
            }

            if (0U != gp_csi11_tx_address)
            {
                SIO11 = *gp_csi11_tx_address;
                gp_csi11_tx_address++;
            }
            else
            {
                SIO11 = 0xFFU;
            }

            g_csi11_tx_count--;
        }
        else 
        {
            if (0U == g_csi11_tx_count)
            {
                if (0U != gp_csi11_rx_address)
                {
                    *gp_csi11_rx_address = SIO11;
                }
                else
                {
                    sio_dummy = SIO11;
                }
            }

            r_csi11_callback_sendend();    /* complete send */
            r_csi11_callback_receiveend();    /* complete receive */
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_csi11_callback_receiveend
* Description  : This function is a callback function when CSI11 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi11_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    if(Spi.ChipSelectStretch)
    	SPI_CHIP_SELECT = 0U;
    else
    	SPI_CHIP_SELECT = 1U;
    Spi.Status = SPI_IDLE;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi11_callback_error
* Description  : This function is a callback function when CSI11 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_csi11_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
    SPI_CHIP_SELECT = 1U;
    DISABLE_SPI_CHIP_SELECT_STRETCH;
    Spi.Status = SPI_IDLE;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_csi11_callback_sendend
* Description  : This function is a callback function when CSI11 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_csi11_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    if(Spi.ChipSelectStretch)
    	SPI_CHIP_SELECT = 0U;
    else
    	SPI_CHIP_SELECT = 1U;
    Spi.Status = SPI_IDLE;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_interrupt
* Description  : This function is INTIICA0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void r_iica0_interrupt(void)
{
    if ((IICS0 & _80_IICA_STATUS_MASTER) == 0U)
    {
        iica0_slave_handler();
    }
}

/***********************************************************************************************************************
* Function Name: iica0_slave_handler
* Description  : This function is IICA0 slave handler.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void iica0_slave_handler(void)
{
    /* Control for stop condition */
    if (1U == SPD0)
    {    
        /* Get stop condition */
        SPIE0 = 0U;
        g_iica0_slave_status_flag = 1U;
    }
    else
    {
        if ((g_iica0_slave_status_flag & _80_IICA_ADDRESS_COMPLETE) == 0U)
        {
            if (1U == COI0)
            {
                SPIE0 = 1U;
                g_iica0_slave_status_flag |= _80_IICA_ADDRESS_COMPLETE;
                
                if (1U == TRC0)
                {
                    WTIM0 = 1U;
                    
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_slave_sendend();
                        WREL0 = 1U;
                    }
                }
                else
                {
                    ACKE0 = 1U;
                    WTIM0 = 0U;
                    WREL0 = 1U;
                }
            }
            else
            {
                r_iica0_callback_slave_error(MD_ERROR);
            }
        }
        else
        {
            if (1U == TRC0)
            {
                if ((0U == ACKD0) && (g_iica0_tx_cnt != 0U))
                {
                    r_iica0_callback_slave_error(MD_NACK);
                }
                else
                {
                    if (g_iica0_tx_cnt > 0U)
                    {
                        IICA0 = *gp_iica0_tx_address;
                        gp_iica0_tx_address++;
                        g_iica0_tx_cnt--;
                    }
                    else
                    {
                        r_iica0_callback_slave_sendend();
                        WREL0 = 1U;
                    }
                }
            }
            else
            {
                if (g_iica0_rx_cnt < g_iica0_rx_len)
                {
                    *gp_iica0_rx_address = IICA0;
                    gp_iica0_rx_address++;
                    g_iica0_rx_cnt++;
                    
                    if (g_iica0_rx_cnt == g_iica0_rx_len)
                    {
                        WTIM0 = 1U;
                        WREL0 = 1U;
                        r_iica0_callback_slave_receiveend();
                    }
                    else
                    {
                        WREL0 = 1U;
                    }
                }
                else
                {
                    WREL0 = 1U;
                }
            }
        }
    }
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_slave_error
* Description  : This function is a callback function when IICA0 slave error occurs.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_slave_error(MD_STATUS flag)
{
    /* Start user code. Do not edit comment generated here */
	ACKE0 = 1U;
	WTIM0 = 0U;
        WREL0 = 1U;
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_slave_receiveend
* Description  : This function is a callback function when IICA0 finishes slave reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_slave_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
    IICA_Beacon_Receive_Handler(IICA0);
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_slave_sendend
* Description  : This function is a callback function when IICA0 finishes slave transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_iica0_callback_slave_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    if(I2C.TxState == IICA_IN_TRANSMISSION)
    	I2C.TxState = IICA_TX_IDLE;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: r_iica0_interrupt
* Description  : This function is INTIICA0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void r_iica0_data_direction_handler(void)
{
	if(!I2C.Direction)
	{
		I2C.Direction = 1U;
		if(TRC0)
		{
			if(I2C.TxState == IICA_READY_TO_TRANSMIT)
			{
				R_IICA0_Slave_Send(I2C.Tx.Buffer, I2C.Tx.Length);
				I2C.TxState = IICA_IN_TRANSMISSION;
			}
			else
			{
				const uint8_t nack[] = {0x49, 0x44, 0x44, 0x44, 0x01, 0x00, 0x15};
				R_IICA0_Slave_Send(nack, sizeof(nack));
			}
		}
		else
		{
			if((I2C.RxState == IICA_RX_IDLE) && (I2C.TxState == IICA_TX_IDLE))
				R_IICA0_Slave_Receive(I2C.Rx.Buffer, 6U);
		}
	}
	else if(SPD0)
	{
		I2C.Direction = 0U;
	}
}

/***********************************************************************************************************************
* Function Name: r_iica0_callback_slave_sendend
* Description  : This function is a callback function when IICA0 finishes slave transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void SPI_Send_Receive(uint8_t *sendbuffer, uint8_t count, uint8_t *receivebuffer)
{
	SPI_CHIP_SELECT = 0U;
	Spi.Status = SPI_BUSY;
	R_CSI11_Send_Receive(sendbuffer, count, receivebuffer);
}

/* End user code. Do not edit comment generated here */
