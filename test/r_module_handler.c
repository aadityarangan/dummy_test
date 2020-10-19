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
typedef enum
{
	 NFC_ACK	= 0x06
	,NFC_NACK	= 0x15
}responsestatus;

typedef enum
{
	 NFC_NO_REASON			= 0U
	,NFC_NO_TAG_DETECTED		= 1U
	,NFC_REQUEST_FAILED		= 2U
	,NFC_NO_SPACE_AVAILABLE		= 3U
	,NFC_INVALID_PERMISSIONS	= 4U
}responsereason;

/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_reader_echo_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_device_reboot_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	//Initiate reboot sequence
	Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_factory_reset_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	//Initiate factory reset equence
	Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_change_aes_key_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	volatile status;
	//Initiate aes key change
	if(status)
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_NACK, NFC_REQUEST_FAILED);
	else
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_add_acl_entry_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	volatile uint16_t consumed = 0U;
	
	if(Add_ACL_Entry(request, (*requestlength), &consumed))
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_NACK, NFC_NO_SPACE_AVAILABLE);
	else
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_delete_acl_entry_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	volatile uint8_t index = 0U;
	volatile aclseacrhfilter tempfilter;
	volatile taginfo tempinfo;
	volatile entrytoken temptoken;
	
	tempfilter.Type.Whole = 0U;
	tempfilter.Info.Whole = 0U;
	tempfilter.Token.Whole = 0U;
		
	memset(&tempinfo, 0U, sizeof(taginfo));
	memset(&temptoken, 0U, sizeof(entrytoken));
	
	memcpy(&tempfilter, request, sizeof(aclseacrhfilter));
	request += sizeof(aclseacrhfilter);
	
	if(tempfilter.Type.Specific.Index)
	{
		memcpy(&index, request, sizeof(index));
		request += sizeof(index);
	}
	else
	{
		//Info
		memcpy(&tempinfo.UIDSize, request, sizeof(tempinfo.UIDSize));
		request += sizeof(tempinfo.UIDSize);
		
		memcpy(tempinfo.UID, request, tempinfo.UIDSize);
		request += tempinfo.UIDSize;
		
		memcpy(&tempinfo.Profile, request, sizeof(tempinfo.Profile));
		request += sizeof(tempinfo.Profile);
		
		memcpy(&tempinfo.SubProfile, request, sizeof(tempinfo.SubProfile));
		request += sizeof(tempinfo.SubProfile);
		
		memcpy(&tempinfo.AvailableMemory, request, sizeof(tempinfo.AvailableMemory));
		request += sizeof(tempinfo.AvailableMemory);
		
		//Token
		memcpy(&temptoken.UserID, request, sizeof(temptoken.UserID));
		request += sizeof(temptoken.UserID);
		
		memcpy(&temptoken.GenerationTime, request, sizeof(temptoken.GenerationTime));
		request += sizeof(temptoken.GenerationTime);
		
		//New
		
		memcpy(&temptoken.PresenceTime, request, sizeof(temptoken.PresenceTime));
		request += sizeof(temptoken.PresenceTime);
			
		memcpy(&temptoken.PresenceAction, request, sizeof(temptoken.PresenceAction));
		request += sizeof(temptoken.PresenceAction);
		
		memcpy(&temptoken.AbsenceTime, request, sizeof(temptoken.AbsenceTime));
		request += sizeof(temptoken.AbsenceTime);
		
		memcpy(&temptoken.AbsenceAction, request, sizeof(temptoken.AbsenceAction));
		request += sizeof(temptoken.AbsenceAction);
		
		memcpy(&temptoken.ActionRule.Whole, request, sizeof(temptoken.ActionRule.Whole));
		request += sizeof(temptoken.ActionRule.Whole);
		
		//New
		
		/*memcpy(&temptoken.Action, request, sizeof(temptoken.Action));
		request += sizeof(temptoken.Action);
		
		memcpy(&temptoken.ActionRule, request, sizeof(temptoken.ActionRule));
		request += sizeof(temptoken.ActionRule);
		
		memcpy(&temptoken.Timeout, request, sizeof(temptoken.Timeout));
		request += sizeof(temptoken.Timeout);*/
		
		memcpy(temptoken.AuthCode, request, sizeof(temptoken.AuthCode));
		request += sizeof(temptoken.AuthCode);
		
		memcpy(&temptoken.GuideRule, request, sizeof(temptoken.GuideRule));
		request += sizeof(temptoken.GuideRule);
		
		if(temptoken.GuideRule.Password32)
		{
			memcpy(temptoken.Guide.Password32, request, sizeof(temptoken.Guide.Password32));
			request += sizeof(temptoken.Guide.Password32);
			
			memcpy(temptoken.Guide.PasswordACK16, request, sizeof(temptoken.Guide.PasswordACK16));
			request += sizeof(temptoken.Guide.PasswordACK16);
		}
		else if(temptoken.GuideRule.Mask64)
		{
			memcpy(temptoken.Guide.Mask64, request, sizeof(temptoken.Guide.Mask64));
			request += sizeof(temptoken.Guide.Mask64);
		}
		
		
	}
		
	
	if(Delete_ACL_Entry(tempfilter, tempinfo, temptoken, index))
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
	else
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_NACK, NFC_REQUEST_FAILED);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_clear_acl_table_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	Clear_ACL_Table();
	
	Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_bulk_update_acl_table_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	volatile uint8_t numberofentries = 0U, status = 0U;
	volatile uint16_t consumedlength = 0U;
	Clear_ACL_Table();
	
	memcpy(&numberofentries, request, sizeof(numberofentries));
	request += sizeof(numberofentries);
	
	while(numberofentries)
	{
		if(Add_ACL_Entry(request, (*requestlength), &consumedlength))
		{
			status = 1U;
			break;
		}
		request += consumedlength;
		numberofentries--;
	}
	
	if(status)
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_NACK, NFC_NO_SPACE_AVAILABLE);
	else
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_permissions_notify_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	//	Check gen time expiry
	if(Update_Permissions(request))
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_NACK, NFC_INVALID_PERMISSIONS);
	else
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_NO_REASON);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_configure_tag_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	if(Add_Configure_Tag_Entry(request, (*requestlength)))
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_ACK, NFC_INVALID_PERMISSIONS);
	else
		Attach_Command_Response(modulecode, response, elapsedwrite, NFC_NACK, NFC_NO_SPACE_AVAILABLE);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void nfc_routine_request_module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	Attach_Action_Notifications(response, elapsedwrite);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Module_Handler(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{
	switch (modulecode)
	{
		case NFC_ROUTINE_REQUEST_MODULE_CODE:
		nfc_routine_request_module(modulecode, request, requestlength, response, elapsedwrite);
		break;
		
		case NFC_ECHO_MODULE_CODE:
		nfc_reader_echo_module(modulecode, request, requestlength, response, elapsedwrite);
		break;
			
		case NFC_REBOOT_DEVICE_MODULE_CODE:
		nfc_device_reboot_module(modulecode, request, requestlength, response, elapsedwrite);
		break;
		
		case NFC_FACTORY_RESET_MODULE_CODE:
		nfc_factory_reset_module(modulecode, request, requestlength, response, elapsedwrite);
		break;
			
		case NFC_AES_KEY_CHANGE_MODULE_CODE:
		nfc_change_aes_key_module(modulecode, request, requestlength, response, elapsedwrite);
		break;
		
		case NFC_ADD_ACL_ENTRY_MODULE_CODE:
		nfc_add_acl_entry_module(modulecode, request, requestlength, response, elapsedwrite);
		break;

		case NFC_DELETE_ACL_ENTRY_MODULE_CODE:
		nfc_delete_acl_entry_module(modulecode, request, requestlength, response, elapsedwrite);
		break;

		case NFC_CLEAR_ACL_TABLE_MODULE_CODE:
		nfc_clear_acl_table_module(modulecode, request, requestlength, response, elapsedwrite);
		break;

		case NFC_BULK_UPDATE_ACL_TABLE_MOCULE_CODE:
		nfc_bulk_update_acl_table_module(modulecode, request, requestlength, response, elapsedwrite);
		break;

		case NFC_PERMISSIONS_NOTIFY_MODULE_CODE:
		nfc_permissions_notify_module(modulecode, request, requestlength, response, elapsedwrite);
		break;
		
		case NFC_CONFIGURE_TAG_MODULE_CODE:
		nfc_permissions_notify_module(modulecode, request, requestlength, response, elapsedwrite);
		break;
		
		default:
		break;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Attach_Command_Response(uint8_t modulecode, uint8_t *response, uint8_t *elapsedwrite, uint8_t status, uint8_t reason)
{
	volatile uint8_t offset = 0U;
	
	struct
	{
		uint8_t modulecode;
		uint8_t modulelength;
		uint16_t sequenceid;
		uint8_t responsemodulecode;
		uint8_t status;
		uint8_t reason;
	}commandresponse;
	
	memset(&commandresponse, 0U, sizeof(commandresponse));
	
	commandresponse.modulecode = NFC_COMMAND_RESPONSE_MODULE_CODE;
	commandresponse.modulelength = 5U;
	commandresponse.sequenceid = I2C.Tx.SequenceID;
	commandresponse.responsemodulecode = modulecode;
	commandresponse.status = status;
	commandresponse.reason = reason;
	
	memcpy(response+offset, &commandresponse.modulecode, sizeof(commandresponse.modulecode));
	(*elapsedwrite) += sizeof(commandresponse.modulecode);
	offset += sizeof(commandresponse.modulecode);
	
	memcpy(response+offset, &commandresponse.modulelength, sizeof(commandresponse.modulelength));
	(*elapsedwrite) += sizeof(commandresponse.modulelength);
	offset += sizeof(commandresponse.modulelength);
	
	memcpy(response+offset, &commandresponse.sequenceid, sizeof(commandresponse.sequenceid));
	(*elapsedwrite) += sizeof(commandresponse.sequenceid);
	offset += sizeof(commandresponse.sequenceid);
	
	memcpy(response+offset, &commandresponse.responsemodulecode, sizeof(commandresponse.responsemodulecode));
	(*elapsedwrite) += sizeof(commandresponse.responsemodulecode);
	offset += sizeof(commandresponse.responsemodulecode);
	
	memcpy(response+offset, &commandresponse.status, sizeof(commandresponse.status));
	(*elapsedwrite) += sizeof(commandresponse.status);
	offset += sizeof(commandresponse.status);
	
	memcpy(response+offset, &commandresponse.reason, sizeof(commandresponse.reason));
	(*elapsedwrite) += sizeof(commandresponse.reason);
	offset += sizeof(commandresponse.reason);
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Status_Response(uint8_t *response, uint16_t *elapsedwrite)
{
	*response = 0x01;
	response++;
	*response = 0x02;
	response++;
	*response = 0x03;
	response++;
	*response = 0x04;
	response++;
	*response = 0x05;
	response++;
	(*elapsedwrite) = 5U;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Test_Module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite)
{	
	//Test Module
	if(modulecode == NFC_ADD_ACL_ENTRY_MODULE_CODE)
		nfc_add_acl_entry_module(modulecode, request, requestlength, response, elapsedwrite);
	else if(modulecode == NFC_DELETE_ACL_ENTRY_MODULE_CODE)
		nfc_delete_acl_entry_module(modulecode, request, requestlength, response, elapsedwrite);
	else if(modulecode == NFC_CLEAR_ACL_TABLE_MODULE_CODE)
		nfc_clear_acl_table_module(modulecode, request, requestlength, response, elapsedwrite);
	else if(modulecode == NFC_BULK_UPDATE_ACL_TABLE_MOCULE_CODE)
		nfc_bulk_update_acl_table_module(modulecode, request, requestlength, response, elapsedwrite);
}

/* End user code. Do not edit comment generated here */