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
* Version      : CodeGenerator for RL78/G14 V2.05.04.02 [20 Nov 2019]
* Device(s)    : R5F104BC
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Serial module.
* Creation Date: 31-01-2020
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
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */



/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Check_User_Permissions(void)
{
	volatile uint8_t index = 0U;
	
	for(index=0U; index<MAXIMUM_ACL_ENTRIES; index++)
	{
		if((ACLTable[index].Settings.Token.UserID < MAXIMUM_USERS) && (ACLTable[index].Settings.Info.UIDSize))
		{
			volatile uint8_t pi = 0;
			
			pi = (ACLTable[index].Settings.Token.UserID);
			if(memcmp(&Permissions[pi].GenerationTime.LSB, &ACLTable[index].Settings.Token.GenerationTime.LSB, sizeof(ACLTable[index].Settings.Token.GenerationTime.LSB)))
				ACLTable[index].Status.Expiry = 1U;
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Delete_Expired_Entries(void)
{
	volatile uint8_t result = 0U, count = 0U;
	volatile aclseacrhfilter filter;
	volatile taginfo info;
	volatile entrytoken token;
	
	memset(&info, 0U, sizeof(taginfo));
	memset(&token, 0U, sizeof(entrytoken));
	
	filter.Type.Whole = 0U;
	filter.Info.Whole = 0U;
	filter.Token.Whole = 0U;
	
	filter.Type.Specific.First = 1U;
	filter.Type.Specific.UseToken = 1U;
	filter.Token.Specific.Expired = 1U;
	
	while(ACL_Search(info, token, filter, &result, &count))
	{
		memset(&ACLTable[result].Settings.Info, 0U, sizeof(taginfo));
		memset(&ACLTable[result].Settings.Token, 0U, sizeof(entrytoken));
		memset(&ACLTable[result].Status, 0U, sizeof(ACLTable[result].Status));
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void ACL_Book_Keeping(void)
{
	Check_User_Permissions();
	Delete_Expired_Entries();
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t confirm_acl_search_filter(uint8_t index, taginfo info, entrytoken token, aclseacrhfilter filter)
{
	volatile uint8_t status = 0;
	
	if(filter.Type.Specific.Configured)
	{
		if(ACLTable[index].Status.Config)
			status = 1U;
	}
	
	if(filter.Type.Specific.UnConfigured)
	{
		if(!ACLTable[index].Status.Config)
			status = 1U;
	}
	
	if(filter.Type.Specific.UseInfo)
	{
		if(filter.Info.Specific.Absolute)
		{
			if(memcmp(&ACLTable[index].Settings.Info, &info, sizeof(taginfo)))
				status = 1U;
		}
		else
		{
			if(filter.Info.Specific.UIDSize)
			{
				if(ACLTable[index].Settings.Info.UIDSize != info.UIDSize)
					status = 1U;
			}
			if(filter.Info.Specific.UID)
			{
				if(memcmp(&ACLTable[index].Settings.Info.UID, &info.UID, info.UIDSize))
					status = 1U;
			}
			if(filter.Info.Specific.Profile)
			{
				if(ACLTable[index].Settings.Info.Profile != info.Profile)
					status = 1U;
			}
			if(filter.Info.Specific.SubProfile)
			{
				if(ACLTable[index].Settings.Info.SubProfile != info.SubProfile)
					status = 1U;
			}
		}
	}
	
	if(filter.Type.Specific.UseToken)
	{
		if(filter.Token.Specific.Absolute)
		{
			if(memcmp(&ACLTable[index].Settings.Token, &token, sizeof(entrytoken)))
				status = 1U;
		}
		else
		{
			if(filter.Token.Specific.UserID)
			{
				if(memcmp(&ACLTable[index].Settings.Token.UserID, &token.UserID, sizeof(token.UserID)))
					status = 1U;
			}
			if(filter.Token.Specific.Expired)
			{
				if(!ACLTable[index].Status.Expiry)
					status = 1U;
			}
			if(filter.Token.Specific.Valid)
			{
				if(ACLTable[index].Status.Expiry)
					status = 1U;
			}
			if(filter.Token.Specific.GenerationTime)
			{
				if(memcmp(&ACLTable[index].Settings.Token.GenerationTime, &token.UserID, sizeof(token.GenerationTime)))
					status = 1U;
			}
			if(filter.Token.Specific.PresenceAction)
			{
				if(ACLTable[index].Settings.Token.PresenceAction != token.PresenceAction)
					status = 1U;
			}
			if(filter.Token.Specific.PresenceTime)
			{
				if(ACLTable[index].Settings.Token.PresenceTime != token.PresenceTime)
					status = 1U;
			}
			if(filter.Token.Specific.AbsenceAction)
			{
				if(ACLTable[index].Settings.Token.AbsenceAction != token.AbsenceAction)
					status = 1U;
			}
			if(filter.Token.Specific.AbsenceTime)
			{
				if(ACLTable[index].Settings.Token.AbsenceTime != token.AbsenceTime)
					status = 1U;
			}
			if(filter.Token.Specific.AuthCode)
			{
				if(memcmp(&ACLTable[index].Settings.Token.AuthCode, &token.UserID, sizeof(token.AuthCode)))
					status = 1U;
			}
			if(filter.Token.Specific.AuthTrue)
			{
				if(!ACLTable[index].Status.Auth)
					status = 1U;
			}
			if(filter.Token.Specific.AuthFalse)
			{
				if(ACLTable[index].Status.Auth)
					status = 1U;
			}
		}
	}
	
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t ACL_Search(taginfo tag, entrytoken token, aclseacrhfilter filter, uint8_t *searchresult, uint8_t *resultcount)
{
	uint8_t status = 0U, index = 0U, searchtype = 0U;
	
	(*resultcount) = 0U;
	
	for(index=0U; index<MAXIMUM_ACL_ENTRIES; index++)
	{
		if(filter.Type.Specific.FreeSlot)
		{
			if(!ACLTable[index].Settings.Info.UIDSize)
			{
				(*searchresult) = index;
				(*resultcount)++;
				status = 1U;
				break;
			}
		}
		else
		{
			if(!confirm_acl_search_filter(index, tag, token, filter))
			{
				(*searchresult) = index;
				(*resultcount)++;
				status = 1U;
				if(filter.Type.Specific.First)
					break;
			}
		}
	}
	
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t Add_ACL_Entry(uint8_t *entrydata, uint16_t length, uint16_t *consumedlength)
{
	volatile uint8_t status = 0U;
	volatile taginfo tempinfo;
	volatile entrytoken temptoken;
	volatile aclseacrhfilter tempfilter;
	volatile uint8_t searchresult = 0U;
	volatile uint8_t searchcount = 0U;
	
	(*consumedlength) = 0U;
	
	//Tag Info
	memset(&tempinfo, 0U, sizeof(tempinfo));
	memset(&temptoken, 0U, sizeof(temptoken));
	
	memcpy(&tempinfo.UIDSize, entrydata, sizeof(tempinfo.UIDSize));
	entrydata += sizeof(tempinfo.UIDSize);
	(*consumedlength) += sizeof(tempinfo.UIDSize);
	
	
	memcpy(&tempinfo.UID, entrydata, tempinfo.UIDSize);
	entrydata += tempinfo.UIDSize;
	(*consumedlength) += tempinfo.UIDSize;
	
	
	memcpy(&tempinfo.Profile, entrydata, sizeof(tempinfo.Profile));
	entrydata += sizeof(tempinfo.Profile);
	(*consumedlength) += sizeof(tempinfo.Profile);
	
	memcpy(&tempinfo.SubProfile, entrydata, sizeof(tempinfo.SubProfile));
	entrydata += sizeof(tempinfo.SubProfile);
	(*consumedlength) += sizeof(tempinfo.SubProfile);
	
	memcpy(&tempinfo.AvailableMemory, entrydata, sizeof(tempinfo.AvailableMemory));
	entrydata += sizeof(tempinfo.AvailableMemory);
	(*consumedlength) += sizeof(tempinfo.AvailableMemory);
	
	//Token Data
	memcpy(&temptoken.UserID, entrydata, sizeof(temptoken.UserID));
	entrydata += sizeof(temptoken.UserID);
	(*consumedlength) += sizeof(temptoken.UserID);
	
	memcpy(&temptoken.GenerationTime, entrydata, sizeof(temptoken.GenerationTime));
	entrydata += sizeof(temptoken.GenerationTime);
	(*consumedlength) += sizeof(temptoken.GenerationTime);
	
	//New
	
	memcpy(&temptoken.PresenceTime, entrydata, sizeof(temptoken.PresenceTime));
	entrydata += sizeof(temptoken.PresenceTime);
	(*consumedlength) += sizeof(temptoken.PresenceTime);
	
	memcpy(&temptoken.PresenceAction, entrydata, sizeof(temptoken.PresenceAction));
	entrydata += sizeof(temptoken.PresenceAction);
	(*consumedlength) += sizeof(temptoken.PresenceAction);
	
	memcpy(&temptoken.AbsenceTime, entrydata, sizeof(temptoken.AbsenceTime));
	entrydata += sizeof(temptoken.AbsenceTime);
	(*consumedlength) += sizeof(temptoken.AbsenceTime);
	
	memcpy(&temptoken.AbsenceAction, entrydata, sizeof(temptoken.AbsenceAction));
	entrydata += sizeof(temptoken.AbsenceAction);
	(*consumedlength) += sizeof(temptoken.AbsenceAction);
	
	memcpy(&temptoken.ActionRule.Whole, entrydata, sizeof(temptoken.ActionRule.Whole));
	entrydata += sizeof(temptoken.ActionRule.Whole);
	(*consumedlength) += sizeof(temptoken.ActionRule.Whole);
	
	//New
	
	/*memcpy(&temptoken.Action, entrydata, sizeof(temptoken.Action));
	entrydata += sizeof(temptoken.Action);
	(*consumedlength) += sizeof(temptoken.Action);
	
	memcpy(&temptoken.ActionRule.Whole, entrydata, sizeof(temptoken.ActionRule.Whole));
	entrydata += sizeof(temptoken.ActionRule.Whole);
	(*consumedlength) += sizeof(temptoken.ActionRule.Whole);
	
	memcpy(&temptoken.Timeout, entrydata, sizeof(temptoken.Timeout));
	entrydata += sizeof(temptoken.Timeout);
	(*consumedlength) += sizeof(temptoken.Timeout);*/
	
	memcpy(&temptoken.AuthCode, entrydata, sizeof(temptoken.AuthCode));
	entrydata += sizeof(temptoken.AuthCode);
	(*consumedlength) += sizeof(temptoken.AuthCode);
	
	memcpy(&temptoken.GuideRule, entrydata, sizeof(temptoken.GuideRule));
	entrydata += sizeof(temptoken.GuideRule);
	(*consumedlength) += sizeof(temptoken.GuideRule);
	
	if(temptoken.GuideRule.Password32)
	{
		memcpy(&temptoken.Guide.Password32, entrydata, sizeof(temptoken.Guide.Password32));
		entrydata += sizeof(temptoken.Guide.Password32);
		(*consumedlength) += sizeof(temptoken.Guide.Password32);
		memcpy(&temptoken.Guide.PasswordACK16, entrydata, sizeof(temptoken.Guide.PasswordACK16));
		entrydata += sizeof(temptoken.Guide.PasswordACK16);
		(*consumedlength) += sizeof(temptoken.Guide.PasswordACK16);
	}
	else if(temptoken.GuideRule.Mask64)
	{
		memcpy(&temptoken.Guide.Mask64, entrydata, sizeof(temptoken.Guide.Mask64));
		entrydata += sizeof(temptoken.Guide.Mask64);
		(*consumedlength) += sizeof(temptoken.Guide.Mask64);
	}
	
	tempfilter.Type.Whole = 0U;
	tempfilter.Info.Whole = 0U;
	tempfilter.Token.Whole = 0U;
	
	tempfilter.Type.Specific.First = 1U;
		
	tempfilter.Type.Specific.UseInfo = 1U;
	tempfilter.Info.Specific.Absolute = 1U;
	
	tempfilter.Type.Specific.UseToken = 1U;
	tempfilter.Token.Specific.UserID = 1U;
	tempfilter.Token.Specific.PresenceAction = 1U;
	tempfilter.Token.Specific.AbsenceAction = 1U;
	tempfilter.Type.Specific.Configured = 1U;
	
	if(ACL_Search(tempinfo, temptoken, tempfilter, &searchresult, &searchcount))
	{
		memcpy(&ACLTable[searchresult].Settings.Token, &temptoken, sizeof(entrytoken));
	}
	else
	{
		searchresult = 0U;
		searchcount = 0U;
		
		tempfilter.Type.Whole = 0U;
		tempfilter.Info.Whole = 0U;
		tempfilter.Token.Whole = 0U;
		
		tempfilter.Type.Specific.FreeSlot = 1U;
		
		if(ACL_Search(tempinfo, temptoken, tempfilter, &searchresult, &searchcount))
		{
			memcpy(&ACLTable[searchresult].Settings.Info, &tempinfo, sizeof(taginfo));
			memcpy(&ACLTable[searchresult].Settings.Token, &temptoken, sizeof(entrytoken));
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
uint8_t Add_Configure_Tag_Entry(uint8_t *entrydata, uint16_t length)
{
	volatile uint8_t status = 0U;
	volatile taginfo tempinfo;
	volatile entrytoken temptoken;
	volatile aclseacrhfilter tempfilter;
	volatile uint8_t searchresult = 0U;
	volatile uint8_t searchcount = 0U;
	
	
	//Tag Info
	memset(&tempinfo, 0U, sizeof(tempinfo));
	memset(&temptoken, 0U, sizeof(temptoken));
	
	memcpy(&tempinfo.UIDSize, entrydata, sizeof(tempinfo.UIDSize));
	entrydata += sizeof(tempinfo.UIDSize);	
	
	memcpy(&tempinfo.UID, entrydata, tempinfo.UIDSize);
	entrydata += tempinfo.UIDSize;
	
	
	memcpy(&tempinfo.Profile, entrydata, sizeof(tempinfo.Profile));
	entrydata += sizeof(tempinfo.Profile);
	
	memcpy(&tempinfo.SubProfile, entrydata, sizeof(tempinfo.SubProfile));
	entrydata += sizeof(tempinfo.SubProfile);
	
	memcpy(&tempinfo.AvailableMemory, entrydata, sizeof(tempinfo.AvailableMemory));
	entrydata += sizeof(tempinfo.AvailableMemory);
	
	//Token Data
	memcpy(&temptoken.UserID, entrydata, sizeof(temptoken.UserID));
	entrydata += sizeof(temptoken.UserID);
	
	memcpy(&temptoken.GenerationTime, entrydata, sizeof(temptoken.GenerationTime));
	entrydata += sizeof(temptoken.GenerationTime);
	
	//New
		
	memcpy(&temptoken.PresenceTime, entrydata, sizeof(temptoken.PresenceTime));
	entrydata += sizeof(temptoken.PresenceTime);
		
	memcpy(&temptoken.PresenceAction, entrydata, sizeof(temptoken.PresenceAction));
	entrydata += sizeof(temptoken.PresenceAction);
		
	memcpy(&temptoken.AbsenceTime, entrydata, sizeof(temptoken.AbsenceTime));
	entrydata += sizeof(temptoken.AbsenceTime);
		
	memcpy(&temptoken.AbsenceAction, entrydata, sizeof(temptoken.AbsenceAction));
	entrydata += sizeof(temptoken.AbsenceAction);
	
	memcpy(&temptoken.ActionRule.Whole, entrydata, sizeof(temptoken.ActionRule.Whole));
	entrydata += sizeof(temptoken.ActionRule.Whole);
	
	//New
	
	
	/*memcpy(&temptoken.Action, entrydata, sizeof(temptoken.Action));
	entrydata += sizeof(temptoken.Action);
	
	memcpy(&temptoken.ActionRule.Whole, entrydata, sizeof(temptoken.ActionRule.Whole));
	entrydata += sizeof(temptoken.ActionRule.Whole);
	
	memcpy(&temptoken.Timeout, entrydata, sizeof(temptoken.Timeout));
	entrydata += sizeof(temptoken.Timeout);*/
	
	memcpy(&temptoken.AuthCode, entrydata, sizeof(temptoken.AuthCode));
	entrydata += sizeof(temptoken.AuthCode);
	
	memcpy(&temptoken.GuideRule, entrydata, sizeof(temptoken.GuideRule));
	entrydata += sizeof(temptoken.GuideRule);
	
	if(temptoken.GuideRule.Password32)
	{
		memcpy(&temptoken.Guide.Password32, entrydata, sizeof(temptoken.Guide.Password32));
		entrydata += sizeof(temptoken.Guide.Password32);
		memcpy(&temptoken.Guide.PasswordACK16, entrydata, sizeof(temptoken.Guide.PasswordACK16));
		entrydata += sizeof(temptoken.Guide.PasswordACK16);
	}
	else if(temptoken.GuideRule.Mask64)
	{
		memcpy(&temptoken.Guide.Mask64, entrydata, sizeof(temptoken.Guide.Mask64));
		entrydata += sizeof(temptoken.Guide.Mask64);
	}
	
	searchresult = 0U;
	searchcount = 0U;
			
	tempfilter.Type.Whole = 0U;
	tempfilter.Info.Whole = 0U;
	tempfilter.Token.Whole = 0U;
	
	tempfilter.Type.Specific.UseInfo = 1U;
	tempfilter.Type.Specific.UseToken = 1U;
	tempfilter.Type.Specific.UnConfigured = 1U;
	tempfilter.Info.Specific.Absolute = 1U;
	tempfilter.Token.Specific.Absolute = 1U;
	
	if(!ACL_Search(tempinfo, temptoken, tempfilter, &searchresult, &searchcount))
	{
		searchresult = 0U;
		searchcount = 0U;
			
		tempfilter.Type.Whole = 0U;
		tempfilter.Info.Whole = 0U;
		tempfilter.Token.Whole = 0U;
	
		tempfilter.Type.Specific.FreeSlot = 1U;
	
		if(ACL_Search(tempinfo, temptoken, tempfilter, &searchresult, &searchcount))
		{
			memcpy(&ACLTable[searchresult].Settings.Info, &tempinfo, sizeof(taginfo));
			memcpy(&ACLTable[searchresult].Settings.Token, &temptoken, sizeof(entrytoken));
			ACLTable[searchresult].Status.Config = 1U;
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
uint8_t Delete_ACL_Entry(aclseacrhfilter filter, taginfo info, entrytoken token, uint8_t index)
{
	volatile uint8_t status = 0U;
	volatile uint8_t searchresult = 0U;
	volatile uint8_t searchcount = 0U;
	
	if(filter.Type.Specific.Index)
	{
		memset(&ACLTable[index].Settings.Info, 0U, sizeof(taginfo));
		memset(&ACLTable[index].Settings.Token, 0U, sizeof(entrytoken));
	}
	else
	{
		filter.Type.Specific.First = 1U;
		filter.Type.Specific.Last = 0U;
		
		while(ACL_Search(info, token, filter, &searchresult, &searchcount))
		{
			memset(&ACLTable[searchresult].Settings.Info, 0U, sizeof(taginfo));
			memset(&ACLTable[searchresult].Settings.Token, 0U, sizeof(entrytoken));
		}
	}
	
	status = 1U;
	
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Clear_ACL_Table(void)
{
	memset(ACLTable, 0U, sizeof(ACLTable));
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t Update_Permissions(uint8_t *data)
{
	volatile uint8_t status = 0U, index = 0U, users = 0U, userid = 0U;
	
	memcpy(&users, data, sizeof(users));
	data += sizeof(users);
	
	if(users<=MAXIMUM_USERS)
	{
		for(index=0U; index<MAXIMUM_USERS; index++)
		{
			memcpy(&userid, data, sizeof(userid));
			data += sizeof(userid);
			if(userid < MAXIMUM_USERS)
			{
				memcpy(&Permissions[index].GenerationTime, data, sizeof(Permissions[index].GenerationTime));
				data += sizeof(Permissions[index].GenerationTime);
			}
		}
	}
	else
		status = 1U;
		
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Update_Action_Notification_Queue(uint8_t userid, accessactions action)
{
	volatile uint8_t index = 0;
	
	for(index=0U; index<MAXIMUM_ACTIONS; index++)
	{
		if(Permissions[userid].ActionQueue[index] == action)
			break;
		else if(Permissions[userid].ActionQueue[index] == ACTION_NONE)
		{
			Permissions[userid].ActionQueue[index] = action;
			break;
		}
			
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Check_Edge_Action_Notifications(void)
{	
	volatile uint8_t index = 0;
	
	for(index=0; index<MAXIMUM_ACL_ENTRIES; index++)
	{
		if((!ACLTable[index].Status.AbsenceTrigger) && (!ACLTable[index].Status.Presence) && (!ACLTable[index].Status.Auth))
		{
			ACLTable[index].Status.PresenceCount = 0U;
			if(ACLTable[index].Status.Active)
			{
				if(ACLTable[index].Settings.Token.AbsenceTime)
				{
					ACLTable[index].Status.AbsenceCount = ACLTable[index].Settings.Token.AbsenceTime;
					ACLTable[index].Status.AbsenceTrigger = 1U;
				}
				else
				{
					ACLTable[index].Status.Active = 0U;
					ACLTable[index].Status.AbsenceCount = 0U;
					ACLTable[index].Status.AbsenceTrigger = 0U;
					Update_Action_Notification_Queue(ACLTable[index].Settings.Token.UserID, ACLTable[index].Settings.Token.AbsenceAction);
				}
			}
		}
		
		else if((ACLTable[index].Status.PresenceCount == 0U) && (ACLTable[index].Status.Presence) && (ACLTable[index].Status.Auth))
		{
			if(ACLTable[index].Settings.Token.PresenceTime)
				ACLTable[index].Status.PresenceCount = ACLTable[index].Settings.Token.PresenceTime;
			else
			{
				ACLTable[index].Status.PresenceCount = 1U;
				ACLTable[index].Status.Active = 1U;
				Update_Action_Notification_Queue(ACLTable[index].Settings.Token.UserID, ACLTable[index].Settings.Token.PresenceAction);
			}
		}
				
		else if((ACLTable[index].Status.PresenceCount > 1) && (ACLTable[index].Status.Presence) && (ACLTable[index].Status.Auth))
			ACLTable[index].Status.PresenceCount--;
			
		else if((ACLTable[index].Status.PresenceCount == 1) && (ACLTable[index].Status.Presence) && (ACLTable[index].Status.Auth))
		{
			if(!ACLTable[index].Status.Active)
			{
				ACLTable[index].Status.Active = 1U;
				if(ACLTable[index].Settings.Token.ActionRule.Specific.Broadcast)
					ACLTable[index].Status.BroadcastCount = ACLTable[index].Settings.Token.PresenceTime;
				else
					Update_Action_Notification_Queue(ACLTable[index].Settings.Token.UserID, ACLTable[index].Settings.Token.PresenceAction);
			}
		}
			
		else if((ACLTable[index].Status.AbsenceCount > 1) && (!ACLTable[index].Status.Presence) && (!ACLTable[index].Status.Auth))
			ACLTable[index].Status.AbsenceCount--;
			
		else if((ACLTable[index].Status.AbsenceCount == 1) && (!ACLTable[index].Status.Presence) && (!ACLTable[index].Status.Auth))
		{
			ACLTable[index].Status.Active = 0U;
			ACLTable[index].Status.AbsenceCount = 0U;
			ACLTable[index].Status.AbsenceTrigger = 0U;
			Update_Action_Notification_Queue(ACLTable[index].Settings.Token.UserID, ACLTable[index].Settings.Token.AbsenceAction);
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Check_Broadcast_Action_Notifications(void)
{
	volatile uint8_t index = 0;
	
	for(index=0; index<MAXIMUM_ACL_ENTRIES; index++)
	{
		if(ACLTable[index].Settings.Token.ActionRule.Specific.Broadcast)
		{
			if(ACLTable[index].Status.BroadcastCount)
			{
				ACLTable[index].Status.BroadcastCount--;
				if(!ACLTable[index].Status.BroadcastCount)
					Update_Action_Notification_Queue(ACLTable[index].Settings.Token.UserID, ACLTable[index].Settings.Token.PresenceAction);
			}
			else
			{
				if((ACLTable[index].Status.Active) && (!ACLTable[index].Status.AbsenceTrigger))
					ACLTable[index].Status.BroadcastCount = ACLTable[index].Settings.Token.PresenceTime;
				else
					ACLTable[index].Status.BroadcastCount = 0;
			}
		}
	}
}


/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Attach_Action_Notifications(uint8_t *data, uint8_t *consumed)
{
	volatile uint8_t permissionindex = 0U,
			 actionindex = 0U,
			 modulecode = 0U,
			 modulelength = 0U,
			 numberofusers = 0U,
			 *modulelengthposition = 0U,
			 *numberofusersposition = 0U;
	
	(*consumed) = 0U;
	
	modulecode = NFC_ACTION_NOTIFY_MODULE_CODE;		//Attach module code
	memcpy(data, &modulecode, sizeof(modulecode));
	data += sizeof(modulecode);
	
	modulelengthposition = data;				//Mark module length position
	data += sizeof(modulelength);
	
	numberofusersposition = data;				//Mark number of users position
	data += sizeof(numberofusers);
	(*consumed) += sizeof(numberofusers);
	
	for(permissionindex=0U; permissionindex<MAXIMUM_USERS; permissionindex++)
	{
		if(Permissions[permissionindex].ActionQueue[0] != ACTION_NONE)
		{
			volatile uint8_t numberofactions = 0U, *numberofactionsposition = 0U;
			
			numberofusers++;	//Increament number of users
			
			memcpy(data, &permissionindex, sizeof(permissionindex));	//Copy User ID
			data += sizeof(numberofusers);
			(*consumed) += sizeof(numberofusers);
			
			numberofactionsposition = data;
			data += sizeof(numberofactions);
			(*consumed) += sizeof(numberofactions);
			
			numberofactions = 0U;
			
			for(actionindex=0U; actionindex<MAXIMUM_ACTIONS; actionindex++)
			{
				if(Permissions[permissionindex].ActionQueue[actionindex] != ACTION_NONE)
				{
					numberofactions++;
					
					memcpy(data, &Permissions[permissionindex].ActionQueue[actionindex], sizeof(Permissions[permissionindex].ActionQueue[actionindex]));
					Permissions[permissionindex].ActionQueue[actionindex] = 0U;
					data += sizeof(Permissions[permissionindex].ActionQueue[actionindex]);
					(*consumed) += sizeof(Permissions[permissionindex].ActionQueue[actionindex]);
				}
				else
					break;
			}
			memcpy(numberofactionsposition, &numberofactions, sizeof(numberofactions));
		}
	}
	memcpy(numberofusersposition, &numberofusers, sizeof(numberofusers));
	memcpy(modulelengthposition, &(*consumed), sizeof((*consumed)));
	
	(*consumed) += 2U;
}


/* End user code. Do not edit comment generated here */