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
* File Name    : r_main.c
* Version      : CodeGenerator for RL78/G13 V2.05.04.02 [20 Nov 2019]
* Device(s)    : R5F100FJ
* Tool-Chain   : CA78K0R
* Description  : This file implements main function.
* Creation Date: 19-10-2020
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
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
#include "r_glb_var.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
void Debug_Init(void);
void Debug_Routine(void);
volatile uint8_t reset = 0U;
volatile uint8_t debug = 0U;
/* End user code. Do not edit comment generated here */
void R_MAIN_UserInit(void);

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void main(void)
{
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    while (1U)
    { 
        //R_WDT_Restart();
	if(reset)
	{
		reset = 0U;
		while(1U){}
	}
	R_WDT_Restart();
	if(Device.Operation.Flag)
	{
		Device.Operation.Flag = 0U;
		Operation_Routine();
	}
	Untimed_Functions();
    }
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_MAIN_UserInit(void)
{
    /* Start user code. Do not edit comment generated here */
    
    R_Boot_Up();
    EI();
    Debug_Init();
    Nfc.SearchProfile = 1U;
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Operation_Routine(void)
{
	Device.Operation.Ticks++;
		
	R_WDT_Restart();
	
	if(!(Device.Operation.Ticks % 1U))	//5ms operations
	{
		ACL_Book_Keeping();
		Nfc_Communication_Handler();
		Nfc_Handler();
	}
	if(!(Device.Operation.Ticks % 2U))	//10ms operations
	{
		IICA_Beacon_Parser_Handler();
	}
	if(!(Device.Operation.Ticks % 10U))	//50ms operations
	{
		//
	}
	if(!(Device.Operation.Ticks % 20U))	//100ms operations
	{
		//Debug_Routine();
		Check_Edge_Action_Notifications();
		Check_Broadcast_Action_Notifications();
	}
	if(!(Device.Operation.Ticks % 200U))	//1s operations
	{
		Device.Operation.Ticks = 0U;
		
	}
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Untimed_Functions(void)
{
	//
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Device_Init(void)
{
	memcpy(&Device.Settings.AESKey[0][0], DefaultPrimaryAESKey, 16U);
	memcpy(&Device.Settings.AESKey[1][0], DefaultSecondaryAESKey, 16U);
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Debug_Init(void)
{
	volatile uint8_t i = 0U;
    	const uint8_t uid[] = {0x04, 0x87, 0x67, 0x0a, 0xec, 0x64, 0x81};
	
	for(i=0U; i<2; i++)
    	{
    		ACLTable[i].Settings.Info.UIDSize = 7U;
    		memcpy(ACLTable[i].Settings.Info.UID, uid, sizeof(uid));
    		ACLTable[i].Settings.Info.Profile = NFC_ISO14443;
    		ACLTable[i].Settings.Info.SubProfile = MIFARE_ULTRALIGTH_EV1;
    		ACLTable[i].Settings.Info.AvailableMemory = 888U;
		memset(ACLTable[i].Settings.Token.Guide.Password32, 0xff, 4U);
	}
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Debug_Routine(void)
{
	if(debug)
	{	
		debug = 0U;
		/*
		Permissions[1].ActionQueue[0] = 2;
		Permissions[1].ActionQueue[1] = 3;
		Permissions[1].ActionQueue[2] = 4;
		Permissions[1].ActionQueue[3] = 5;
		
		Permissions[0].ActionQueue[0] = 1;
		Permissions[0].ActionQueue[1] = 2;
		
		Permissions[2].ActionQueue[0] = 2;
		Permissions[2].ActionQueue[1] = 5;
		
		Permissions[3].ActionQueue[0] = 4;
		Permissions[3].ActionQueue[1] = 5;
		Permissions[3].ActionQueue[2] = 6;
		Permissions[3].ActionQueue[3] = 7;
		
		Attach_Action_Notifications(test, &testlength);
		
		/*volatile aclseacrhfilter filter;
		volatile uint8_t request[100], response[100], numberofentries = 0U;
		volatile uint8_t requestlength = 0U, responselength = 0U, modulecode = 0U;
		volatile taginfo tempinfo;
		volatile entrytoken temptoken;
		
		//Info
		memset(&tempinfo, 0U, sizeof(taginfo));
		memset(&temptoken, 0U, sizeof(entrytoken));
		
		tempinfo.UIDSize = 7U;
		memcpy(request+requestlength, &tempinfo.UIDSize, sizeof(tempinfo.UIDSize));
		requestlength += sizeof(tempinfo.UIDSize);
		
		memset(tempinfo.UID, 0xab, tempinfo.UIDSize);
		memcpy(request+requestlength, &tempinfo.UID, tempinfo.UIDSize);
		requestlength += tempinfo.UIDSize;
		
		tempinfo.Profile = NFC_ISO14443;
		memcpy(request+requestlength, &tempinfo.Profile, sizeof(tempinfo.Profile));
		requestlength += sizeof(tempinfo.Profile);
		
		tempinfo.SubProfile = MIFARE_DESFIRE_EV2;
		memcpy(request+requestlength, &tempinfo.SubProfile, sizeof(tempinfo.SubProfile));
		requestlength += sizeof(tempinfo.SubProfile);
		
		tempinfo.AvailableMemory = 888U;
		memcpy(request+requestlength, &tempinfo.AvailableMemory, sizeof(tempinfo.AvailableMemory));
		requestlength += sizeof(tempinfo.AvailableMemory);
		
		//Token
		temptoken.UserID = 0x33;
		memcpy(request+requestlength, &temptoken.UserID, sizeof(temptoken.UserID));
		requestlength += sizeof(temptoken.UserID);
		
		memset(&temptoken.GenerationTime, 0xdd, sizeof(temptoken.GenerationTime));
		memcpy(request+requestlength, &temptoken.GenerationTime, sizeof(temptoken.GenerationTime));
		requestlength += sizeof(temptoken.GenerationTime);
		
		temptoken.Action = ACTION_VEHICLE_LOCK;
		memcpy(request+requestlength, &temptoken.Action, sizeof(temptoken.Action));
		requestlength += sizeof(temptoken.Action);
		
		temptoken.ActionRule.Specific.RisingEdge = 1U;
		memcpy(request+requestlength, &temptoken.ActionRule.Whole, sizeof(temptoken.ActionRule.Whole));
		requestlength += sizeof(temptoken.ActionRule.Whole);
		
		temptoken.Timeout = 9U;
		memcpy(request+requestlength, &temptoken.Timeout, sizeof(temptoken.Timeout));
		requestlength += sizeof(temptoken.Timeout);
		
		memset(temptoken.AuthCode, 0x65, sizeof(temptoken.AuthCode));
		memcpy(request+requestlength, &temptoken.AuthCode, sizeof(temptoken.AuthCode));
		requestlength += sizeof(temptoken.AuthCode);
		
		temptoken.GuideRule.Password32 = 1U;
		memcpy(request+requestlength, &temptoken.GuideRule, sizeof(temptoken.GuideRule));
		requestlength += sizeof(temptoken.GuideRule);
		
		memset(temptoken.Guide.Password32, 0xcc, 4U);	
		memcpy(request+requestlength, temptoken.Guide.Password32, sizeof(temptoken.Guide.Password32));
		requestlength += sizeof(temptoken.Guide.Password32);
		
		memset(temptoken.Guide.PasswordACK16, 0x22, 2U);
		memcpy(request+requestlength, temptoken.Guide.PasswordACK16, sizeof(temptoken.Guide.PasswordACK16));
		requestlength += sizeof(temptoken.Guide.PasswordACK16);
		
		
		debug = 0U;
		modulecode = NFC_ADD_ACL_ENTRY_MODULE_CODE;
		Test_Module(modulecode, request, &requestlength, response, &responselength);
		
		NOP();
		//////////////////////////////////////////////////////////////////////////
		
		filter.Type.Whole = 0U;
		filter.Info.Whole = 0U;
		filter.Token.Whole = 0U;
		
		
		memset(request, 0U, sizeof(request));
		memset(response, 0U, sizeof(response));
		responselength = 0U;
		requestlength = 0U;
		memset(&tempinfo, 0U, sizeof(taginfo));
		memset(&temptoken, 0U, sizeof(entrytoken));
		
		filter.Type.Specific.UseInfo = 0U;
		filter.Type.Specific.UseToken = 1U;
		filter.Token.Specific.Action = 1U;
		
		memcpy(request+requestlength, &filter, sizeof(filter));
		requestlength += sizeof(filter);
		
		tempinfo.UIDSize = 7U;
		memcpy(request+requestlength, &tempinfo.UIDSize, sizeof(tempinfo.UIDSize));
		requestlength += sizeof(tempinfo.UIDSize);
		
		memset(tempinfo.UID, 0xab, tempinfo.UIDSize);
		memcpy(request+requestlength, &tempinfo.UID, tempinfo.UIDSize);
		requestlength += tempinfo.UIDSize;
		
		tempinfo.Profile = NFC_ISO14443;
		memcpy(request+requestlength, &tempinfo.Profile, sizeof(tempinfo.Profile));
		requestlength += sizeof(tempinfo.Profile);
		
		tempinfo.SubProfile = MIFARE_DESFIRE_EV2;
		memcpy(request+requestlength, &tempinfo.SubProfile, sizeof(tempinfo.SubProfile));
		requestlength += sizeof(tempinfo.SubProfile);
		
		tempinfo.AvailableMemory = 888U;
		memcpy(request+requestlength, &tempinfo.AvailableMemory, sizeof(tempinfo.AvailableMemory));
		requestlength += sizeof(tempinfo.AvailableMemory);
		
		//Token
		temptoken.UserID = 0x33;
		memcpy(request+requestlength, &temptoken.UserID, sizeof(temptoken.UserID));
		requestlength += sizeof(temptoken.UserID);
		
		memset(&temptoken.GenerationTime, 0xdd, sizeof(temptoken.GenerationTime));
		memcpy(request+requestlength, &temptoken.GenerationTime, sizeof(temptoken.GenerationTime));
		requestlength += sizeof(temptoken.GenerationTime);
		
		temptoken.Action = ACTION_VEHICLE_LOCK;
		memcpy(request+requestlength, &temptoken.Action, sizeof(temptoken.Action));
		requestlength += sizeof(temptoken.Action);
		
		temptoken.ActionRule.Specific.RisingEdge = 1U;
		memcpy(request+requestlength, &temptoken.ActionRule.Whole, sizeof(temptoken.ActionRule.Whole));
		requestlength += sizeof(temptoken.ActionRule.Whole);
		
		temptoken.Timeout = 9U;
		memcpy(request+requestlength, &temptoken.Timeout, sizeof(temptoken.Timeout));
		requestlength += sizeof(temptoken.Timeout);
		
		memset(temptoken.AuthCode, 0x65, sizeof(temptoken.AuthCode));
		memcpy(request+requestlength, &temptoken.AuthCode, sizeof(temptoken.AuthCode));
		requestlength += sizeof(temptoken.AuthCode);
		
		temptoken.GuideRule.Password32 = 1U;
		memcpy(request+requestlength, &temptoken.GuideRule, sizeof(temptoken.GuideRule));
		requestlength += sizeof(temptoken.GuideRule);
		
		memset(temptoken.Guide.Password32, 0xcc, 4U);	
		memcpy(request+requestlength, temptoken.Guide.Password32, sizeof(temptoken.Guide.Password32));
		requestlength += sizeof(temptoken.Guide.Password32);
		
		memset(temptoken.Guide.PasswordACK16, 0x22, 2U);
		memcpy(request+requestlength, temptoken.Guide.PasswordACK16, sizeof(temptoken.Guide.PasswordACK16));
		requestlength += sizeof(temptoken.Guide.PasswordACK16);
		
		
		modulecode = NFC_DELETE_ACL_ENTRY_MODULE_CODE;
		Test_Module(modulecode, request, &requestlength, response, &responselength);
		
		NOP();
		
		///////////////////////////////////
		
		filter.Type.Whole = 0U;
		filter.Info.Whole = 0U;
		filter.Token.Whole = 0U;
		
		memset(request, 0U, sizeof(request));
		memset(response, 0U, sizeof(response));
		responselength = 0U;
		requestlength = 0U;
		memset(&tempinfo, 0U, sizeof(taginfo));
		memset(&temptoken, 0U, sizeof(entrytoken));
		
		modulecode = NFC_CLEAR_ACL_TABLE_MODULE_CODE;
		Test_Module(modulecode, request, &requestlength, response, &responselength);
		NOP();
		
		
		//////////////////////////////////
		
		filter.Type.Whole = 0U;
		filter.Info.Whole = 0U;
		filter.Token.Whole = 0U;
		
		memset(request, 0U, sizeof(request));
		memset(response, 0U, sizeof(response));
		responselength = 0U;
		requestlength = 0U;
		
		numberofentries = 2U;
		memcpy(request+requestlength, &numberofentries, sizeof(numberofentries));
		requestlength += sizeof(numberofentries);
		
		//------------------------------------------------------------------------------
		
		memset(&tempinfo, 0U, sizeof(taginfo));
		memset(&temptoken, 0U, sizeof(entrytoken));
		
		tempinfo.UIDSize = 7U;
		memcpy(request+requestlength, &tempinfo.UIDSize, sizeof(tempinfo.UIDSize));
		requestlength += sizeof(tempinfo.UIDSize);
		
		memset(tempinfo.UID, 0x69, tempinfo.UIDSize);
		memcpy(request+requestlength, &tempinfo.UID, tempinfo.UIDSize);
		requestlength += tempinfo.UIDSize;
		
		tempinfo.Profile = NFC_ISO14443;
		memcpy(request+requestlength, &tempinfo.Profile, sizeof(tempinfo.Profile));
		requestlength += sizeof(tempinfo.Profile);
		
		tempinfo.SubProfile = MIFARE_DESFIRE_EV2;
		memcpy(request+requestlength, &tempinfo.SubProfile, sizeof(tempinfo.SubProfile));
		requestlength += sizeof(tempinfo.SubProfile);
		
		tempinfo.AvailableMemory = 888U;
		memcpy(request+requestlength, &tempinfo.AvailableMemory, sizeof(tempinfo.AvailableMemory));
		requestlength += sizeof(tempinfo.AvailableMemory);
		
		//Token
		temptoken.UserID = 0x33;
		memcpy(request+requestlength, &temptoken.UserID, sizeof(temptoken.UserID));
		requestlength += sizeof(temptoken.UserID);
		
		memset(&temptoken.GenerationTime, 0xdd, sizeof(temptoken.GenerationTime));
		memcpy(request+requestlength, &temptoken.GenerationTime, sizeof(temptoken.GenerationTime));
		requestlength += sizeof(temptoken.GenerationTime);
		
		temptoken.Action = ACTION_VEHICLE_LOCK;
		memcpy(request+requestlength, &temptoken.Action, sizeof(temptoken.Action));
		requestlength += sizeof(temptoken.Action);
		
		temptoken.ActionRule.Specific.RisingEdge = 1U;
		memcpy(request+requestlength, &temptoken.ActionRule.Whole, sizeof(temptoken.ActionRule.Whole));
		requestlength += sizeof(temptoken.ActionRule.Whole);
		
		temptoken.Timeout = 9U;
		memcpy(request+requestlength, &temptoken.Timeout, sizeof(temptoken.Timeout));
		requestlength += sizeof(temptoken.Timeout);
		
		memset(temptoken.AuthCode, 0x65, sizeof(temptoken.AuthCode));
		memcpy(request+requestlength, &temptoken.AuthCode, sizeof(temptoken.AuthCode));
		requestlength += sizeof(temptoken.AuthCode);
		
		temptoken.GuideRule.Password32 = 1U;
		memcpy(request+requestlength, &temptoken.GuideRule, sizeof(temptoken.GuideRule));
		requestlength += sizeof(temptoken.GuideRule);
		
		memset(temptoken.Guide.Password32, 0xcc, 4U);	
		memcpy(request+requestlength, temptoken.Guide.Password32, sizeof(temptoken.Guide.Password32));
		requestlength += sizeof(temptoken.Guide.Password32);
		
		memset(temptoken.Guide.PasswordACK16, 0x22, 2U);
		memcpy(request+requestlength, temptoken.Guide.PasswordACK16, sizeof(temptoken.Guide.PasswordACK16));
		requestlength += sizeof(temptoken.Guide.PasswordACK16);
		
		//------------------------------------------------------------------------------

		memset(&tempinfo, 0U, sizeof(taginfo));
		memset(&temptoken, 0U, sizeof(entrytoken));
		
		tempinfo.UIDSize = 7U;
		memcpy(request+requestlength, &tempinfo.UIDSize, sizeof(tempinfo.UIDSize));
		requestlength += sizeof(tempinfo.UIDSize);
		
		memset(tempinfo.UID, 0xcd, tempinfo.UIDSize);
		memcpy(request+requestlength, &tempinfo.UID, tempinfo.UIDSize);
		requestlength += tempinfo.UIDSize;
		
		tempinfo.Profile = NFC_ISO14443;
		memcpy(request+requestlength, &tempinfo.Profile, sizeof(tempinfo.Profile));
		requestlength += sizeof(tempinfo.Profile);
		
		tempinfo.SubProfile = MIFARE_DESFIRE_EV2;
		memcpy(request+requestlength, &tempinfo.SubProfile, sizeof(tempinfo.SubProfile));
		requestlength += sizeof(tempinfo.SubProfile);
		
		tempinfo.AvailableMemory = 888U;
		memcpy(request+requestlength, &tempinfo.AvailableMemory, sizeof(tempinfo.AvailableMemory));
		requestlength += sizeof(tempinfo.AvailableMemory);
		
		//Token
		temptoken.UserID = 0x33;
		memcpy(request+requestlength, &temptoken.UserID, sizeof(temptoken.UserID));
		requestlength += sizeof(temptoken.UserID);
		
		memset(&temptoken.GenerationTime, 0xdd, sizeof(temptoken.GenerationTime));
		memcpy(request+requestlength, &temptoken.GenerationTime, sizeof(temptoken.GenerationTime));
		requestlength += sizeof(temptoken.GenerationTime);
		
		temptoken.Action = ACTION_VEHICLE_LOCK;
		memcpy(request+requestlength, &temptoken.Action, sizeof(temptoken.Action));
		requestlength += sizeof(temptoken.Action);
		
		temptoken.ActionRule.Specific.RisingEdge = 1U;
		memcpy(request+requestlength, &temptoken.ActionRule.Whole, sizeof(temptoken.ActionRule.Whole));
		requestlength += sizeof(temptoken.ActionRule.Whole);
		
		temptoken.Timeout = 9U;
		memcpy(request+requestlength, &temptoken.Timeout, sizeof(temptoken.Timeout));
		requestlength += sizeof(temptoken.Timeout);
		
		memset(temptoken.AuthCode, 0x65, sizeof(temptoken.AuthCode));
		memcpy(request+requestlength, &temptoken.AuthCode, sizeof(temptoken.AuthCode));
		requestlength += sizeof(temptoken.AuthCode);
		
		temptoken.GuideRule.Password32 = 1U;
		memcpy(request+requestlength, &temptoken.GuideRule, sizeof(temptoken.GuideRule));
		requestlength += sizeof(temptoken.GuideRule);
		
		memset(temptoken.Guide.Password32, 0xcc, 4U);	
		memcpy(request+requestlength, temptoken.Guide.Password32, sizeof(temptoken.Guide.Password32));
		requestlength += sizeof(temptoken.Guide.Password32);
		
		memset(temptoken.Guide.PasswordACK16, 0x22, 2U);
		memcpy(request+requestlength, temptoken.Guide.PasswordACK16, sizeof(temptoken.Guide.PasswordACK16));
		requestlength += sizeof(temptoken.Guide.PasswordACK16);
		
		//------------------------------------------------------------------------------
		
		
		modulecode = NFC_BULK_UPDATE_ACL_TABLE_MOCULE_CODE;
		Test_Module(modulecode, request, &requestlength, response, &responselength);
		NOP();*/
	}
}

/* End user code. Do not edit comment generated here */
