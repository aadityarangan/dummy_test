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
* Copyright (C) 2011, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_st95hf_handler.c
* Version      : CodeGenerator for RL78/G14 V2.05.03.02 [06 Nov 2018]
* Device(s)    : R5F104BA
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for TAU module.
* Creation Date: 25-11-2019
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_extern_glb_var.h"
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t Flag[8];
const uint8_t NFC_ID_Query_Command[] = {0x00, 0x01, 0x00};
const uint8_t NFC_Read_Command[] = {0x02};

/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void NFC_Init(void)
{
	uint16_t count = 0U;
	
	/***************************** NFC Chip Init *****************************/
	count = 0x00ff;
	while(--count)
	{
		NFC_IRQ_IN = 0U;
	}
	count = 0x00ff;
	while(--count)
	{
		NFC_IRQ_IN = 0U;
	}
	count = 0xffff;
	while(--count)
	{
		R_WDT_Restart();
		NOP();
	}
	NFC_IRQ_IN = 1U;
	SPI_CHIP_SELECT = 1U;
	/***************************** Settings *****************************/
	
	
	/********************************** test **********************************/
	/*Nfc.Write.PageCount = 3U;
	memset(Nfc.Write.Buffer, 0x99, 4U);
	memset(Nfc.Write.Buffer+4U, 0x88, 4U);
	memset(Nfc.Write.Buffer+8U, 0x44, 4U);*/
	
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void NFC_Request(uint8_t *command, uint8_t length)
{
	Nfc.Status = NFC_SEND_COMMAND;
	SPI_Send_Receive(command, length, 0U);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Nfc_Communication_Handler(void)
{
	R_WDT_Restart();
	if((Spi.Status == SPI_IDLE) && (!NFC_IRQ_OUT))
	{
		if(Nfc.Status == NFC_SEND_COMMAND)
		{
			ENABLE_SPI_CHIP_SELECT_STRETCH;
			Nfc.Status = NFC_SEND_READ_COMMAND;
			SPI_Send_Receive(NFC_Read_Command, 1U, 0U);
		}
		else if(Nfc.Status == NFC_SEND_READ_COMMAND)
		{
			Nfc.Status = NFC_READ_RESPONSE_HEAD;
			SPI_Send_Receive(0U, NFC_HEAD_SIZE, Spi.Rx.Buffer);
		}
		else if(Nfc.Status == NFC_READ_RESPONSE_HEAD)
		{
			DISABLE_SPI_CHIP_SELECT_STRETCH;
			Nfc.Status = NFC_READ_DATA;
			memcpy(&Nfc.ResultCode, Spi.Rx.Buffer, NFC_RESULT_CODE_SIZE);
			memcpy(&Spi.Rx.Length, Spi.Rx.Buffer+NFC_RESULT_CODE_SIZE, NFC_RESPONSE_LENGTH_SIZE);
			
			if(Spi.Rx.Length)
				SPI_Send_Receive(0U, Spi.Rx.Length, Spi.Rx.Buffer+2U);
			else
				Nfc.Status = NFC_COM_IDLE;
		}
	}
	else if((Spi.Status == SPI_IDLE) && (NFC_IRQ_OUT))
	{
		if((Nfc.Status==NFC_READ_DATA) || (Nfc.Status==NFC_READ_RESPONSE_HEAD))
		{
			memcpy(&Nfc.ResultCode, Spi.Rx.Buffer, NFC_RESULT_CODE_SIZE);
			memcpy(&Spi.Rx.Length, Spi.Rx.Buffer+NFC_RESULT_CODE_SIZE, NFC_RESPONSE_LENGTH_SIZE);
			SPI_CHIP_SELECT = 1U;
			DISABLE_SPI_CHIP_SELECT_STRETCH;
			Nfc.Status = NFC_COM_IDLE;
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Nfc_Handler(void)
{
	if(Nfc.Status == NFC_COM_IDLE)
	{
		if(Nfc.SearchProfile == NFC_ISO14443)
			ISO14443_Handler();
		else if(Nfc.SearchProfile == NFC_ISO15693)
			ISO15693_Handler();
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Nfc_Profile_Switch(void)
{
	if(Nfc.SearchProfile == NFC_ISO14443)
		Nfc.SearchProfile = NFC_ISO15693;
	else if(Nfc.SearchProfile == NFC_ISO15693)
		Nfc.SearchProfile = NFC_ISO14443;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Nfc_Process_Handler(void)
{
	//
}

/* End user code. Do not edit comment generated here */