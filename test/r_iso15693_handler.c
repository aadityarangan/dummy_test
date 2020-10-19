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
* File Name    : r_iso14443_handler.c
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
const uint8_t Request_Inventory_Command[] = {0x00, 0x04, 0x03, 0x26, 0x01, 0x00};
const uint8_t Get_System_Information_Command[] = {0x00, 0x04, 0x02, 0x02, 0x2B};
const uint8_t ISO15693_Protocol_Select_Command[] = {0x00, 0x02, 0x02, 0x01, 0x05};
const uint8_t ISO15693_Adjust_Index_Mod_Gain_Command[] = {0x00, 0x09, 0x04, 0x68, 0x01, 0x01, 0x50};

/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void ISO15693_Handler(void)
{
	if(!ISO15693_Parser(Spi.Rx.Buffer, Spi.Rx.Length))
	{
		if(Nfc.ISO15693Operation == NFC_ISO15693_SEQUENCE_IDLE)
		{
			Nfc.ISO15693Operation = NFC_ISO15693_PROTOCOL_SELECT;
			NFC_Request(ISO15693_Protocol_Select_Command, SIZE_OF_ARRAY(ISO15693_Protocol_Select_Command));
		}
		else if(Nfc.ISO15693Operation == NFC_ISO15693_PROTOCOL_SELECT)
		{
			Nfc.ISO15693Operation = NFC_ISO15693_ADJUST_INDEXMOD_GAIN;
			NFC_Request(ISO15693_Adjust_Index_Mod_Gain_Command, SIZE_OF_ARRAY(ISO15693_Adjust_Index_Mod_Gain_Command));
		}
		else if(Nfc.ISO15693Operation == NFC_ISO15693_ADJUST_INDEXMOD_GAIN)
		{
			Nfc.ISO15693Operation = NFC_ISO15693_INVENTORY_REQUEST;
			NFC_Request(Request_Inventory_Command, SIZE_OF_ARRAY(Request_Inventory_Command));
		}
		else if(Nfc.ISO15693Operation == NFC_ISO15693_INVENTORY_REQUEST)
		{
			Nfc.ISO15693Operation = NFC_ISO15693_GET_SYSTEM_INFO;
			NFC_Request(Get_System_Information_Command, SIZE_OF_ARRAY(Get_System_Information_Command));
		}
		else if(Nfc.ISO15693Operation == NFC_ISO15693_GET_SYSTEM_INFO)
			Nfc.ISO15693Operation = NFC_ISO15693_SEQUENCE_IDLE;
	}
	else
	{
		Nfc.TagAbsenceCount++;
		if(Nfc.TagAbsenceCount == Nfc.Settings.TagAbsenceRate)
		{
			Nfc.TagAbsenceCount = 0U;
			Exit_ISO15693_Handler();
		}		
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t ISO15693_Parser(uint8_t *buffer, uint8_t length)
{
	uint8_t status = 0U;
	
	if(Nfc.ISO15693Operation == NFC_ISO15693_INVENTORY_REQUEST)
	{
		if(Nfc.ResultCode == 0x80)
		{
			status = 0U;
		}
		else
			status = 1U;
	}
	else if(Nfc.ISO15693Operation == NFC_ISO15693_GET_SYSTEM_INFO)
	{
		if(Nfc.ResultCode == 0x80)
		{
			memcpy(Nfc.Information.UID, buffer, 6U);
			Nfc.Information.UIDSize = 6U;
			Nfc.Information.Profile = NFC_ISO15693;
			Nfc.Information.SubProfile = ICODE_SLIX;
		}
		else
			status = 1U;
	}
	
	return status;
}


/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Exit_ISO15693_Handler(void)
{
	CLEAR_DATA(&Nfc.Information, sizeof(Nfc.Information));
	Nfc.ISO15693Operation = NFC_ISO15693_SEQUENCE_IDLE;
	Nfc_Profile_Switch();
}

/* End user code. Do not edit comment generated here */