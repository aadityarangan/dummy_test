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
const uint8_t Nfc_RF_Field_Off[]			= {0x00, 0x02, 0x02, 0x00, 0x00};

const uint8_t ISO14443_Protocol_Select_Command[]	= {0x00, 0x02, 0x05, 0x02, 0x00, 0x00, 0x21, 0x00};
const uint8_t ISO14443_Request_For_Answer_Command[]	= {0x00, 0x04, 0x02, 0x26, 0x07};
const uint8_t ISO14443_Anti_Collision_1_Command[]	= {0x00, 0x04, 0x03, 0x93, 0x20, 0x08};
const uint8_t ISO14443_Select_1_Command_Head[]		= {0x00, 0x04, 0x00, 0x93, 0x70};
const uint8_t ISO14443_Select_1_Command_Tail[]		= {0x28};
const uint8_t ISO14443_Anti_Collision_2_Command[]	= {0x00, 0x04, 0x03, 0x95, 0x20, 0x08};
const uint8_t ISO14443_Select_2_Command_Head[]		= {0x00, 0x04, 0x00, 0x95, 0x70};
const uint8_t ISO14443_Select_2_Command_Tail[]		= {0x28};
const uint8_t ISO14443_Halt_Command[]			= {0x00, 0x04, 0x03, 0x50, 0x00, 0x28};



const uint8_t Mifare_UL_EV1_Password_Autheticate_Head[]	= {0x00, 0x04, 0x06, 0x1B};
const uint8_t Mifare_UL_EV1_Password_Autheticate_Tail[] = {0x28};
const uint8_t Mifare_UL_EV1_Memory_Read[]		= {0x00, 0x04, 0x04, 0x3A, 0x00, 0x13, 0x28};
const uint8_t Mifare_UL_EV1_Get_Version[]		= {0x00, 0x04, 0x02, 0x60, 0x28};
const uint8_t Mifare_UL_EV1_Page_Write[]		= {0x00, 0x04, 0x07, 0xA2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x28};
const uint8_t Mifare_UL_EV1_Sample_Write[]		= {0x00, 0x04, 0x07, 0xA2, 0x11, 0x80, 0x05, 0x00, 0x00, 0x28};
const uint8_t Mifare_UL_EV1_Bulk_Configure[]		= {0x00, 0x04, 0x07, 0xA2,		//Command head
							0x10,				//Start page address
							0x00, 0x00, 0x00, 0x00,		//Conguration page 0
							0x80, 0x05, 0x00, 0x00,		//Conguration page 1
							0x49, 0x4E, 0x54, 0x4C,		//Password page
							0x41, 0x44, 0x00, 0x00,		//Password acknowledge page
							0x28				//Command Tail
							};
							
							
uint8_t iso14443_sub_profile_handler(uint8_t *buffer, uint8_t length);
							
uint8_t mifare_ultraligth_ev1_handler(uint8_t *buffer, uint8_t length);
uint8_t mifare_ultraligth_ev1_parser(uint8_t *buffer, uint8_t length);
uint8_t mifare_desfire_ev2_handler(uint8_t *buffer, uint8_t length);
uint8_t mifare_desfire_ev1_parser(uint8_t *buffer, uint8_t length);



/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void ISO14443_Handler(void)
{	
	if(!ISO14443_Parser(Spi.Rx.Buffer, Spi.Rx.Length))
	{
		if(Nfc.ISO14443Operation == NFC_ISO14443_SEQUENCE_IDLE)
		{
			Nfc.ISO14443Operation = NFC_ISO14443_PROTOCOL_SELECT;
			NFC_Request(ISO14443_Protocol_Select_Command, SIZE_OF_ARRAY(ISO14443_Protocol_Select_Command));
		}
		else if(Nfc.ISO14443Operation == NFC_ISO14443_PROTOCOL_SELECT)
		{
			Nfc.ISO14443Operation = NFC_ISO14443_REQUEST_FOR_ANSWER;
			NFC_Request(ISO14443_Request_For_Answer_Command, SIZE_OF_ARRAY(ISO14443_Request_For_Answer_Command));
		}
		else if(Nfc.ISO14443Operation == NFC_ISO14443_REQUEST_FOR_ANSWER)
		{
			Nfc.ISO14443Operation = NFC_ISO14443_ANTI_COLLISION_LEVEL_1;
			NFC_Request(ISO14443_Anti_Collision_1_Command, SIZE_OF_ARRAY(ISO14443_Anti_Collision_1_Command));
		}
		else if(Nfc.ISO14443Operation == NFC_ISO14443_ANTI_COLLISION_LEVEL_1)
		{
			uint8_t templength = 0;
			
			templength = Spi.Rx.Length - 3U;
			memcpy(Spi.Tx.Buffer, ISO14443_Select_1_Command_Head, SIZE_OF_ARRAY(ISO14443_Select_1_Command_Head));
			memcpy(Spi.Tx.Buffer+SIZE_OF_ARRAY(ISO14443_Select_1_Command_Head), Spi.Rx.Buffer+2U, templength);
			memcpy(Spi.Tx.Buffer+SIZE_OF_ARRAY(ISO14443_Select_1_Command_Head)+templength, ISO14443_Select_1_Command_Tail, SIZE_OF_ARRAY(ISO14443_Select_1_Command_Tail));
			templength += (SIZE_OF_ARRAY(ISO14443_Select_1_Command_Head)+SIZE_OF_ARRAY(ISO14443_Select_1_Command_Tail));
			memcpy(&Spi.Tx.Length, &templength, 1U);
			templength -= 3U;
			memcpy(Spi.Tx.Buffer+2U, &templength, 1U);
			
			Nfc.ISO14443Operation = NFC_ISO14443_SELECT_TAG_1;
			NFC_Request(Spi.Tx.Buffer, Spi.Tx.Length);
		}
		else if(Nfc.ISO14443Operation == NFC_ISO14443_SELECT_TAG_1)
		{
			Nfc.ISO14443Operation = NFC_ISO14443_ANTI_COLLISION_LEVEL_2;
			NFC_Request(ISO14443_Anti_Collision_2_Command, SIZE_OF_ARRAY(ISO14443_Anti_Collision_2_Command));
		}
		else if(Nfc.ISO14443Operation == NFC_ISO14443_ANTI_COLLISION_LEVEL_2)
		{
			uint8_t templength = 0;
			
			templength = Spi.Rx.Length - 3U;
			memcpy(Spi.Tx.Buffer, ISO14443_Select_2_Command_Head, SIZE_OF_ARRAY(ISO14443_Select_2_Command_Head));
			memcpy(Spi.Tx.Buffer+SIZE_OF_ARRAY(ISO14443_Select_2_Command_Head), Spi.Rx.Buffer+2U, templength);
			memcpy(Spi.Tx.Buffer+SIZE_OF_ARRAY(ISO14443_Select_2_Command_Head)+templength, ISO14443_Select_2_Command_Tail, SIZE_OF_ARRAY(ISO14443_Select_2_Command_Tail));
			templength += (SIZE_OF_ARRAY(ISO14443_Select_2_Command_Head)+SIZE_OF_ARRAY(ISO14443_Select_2_Command_Tail));
			memcpy(&Spi.Tx.Length, &templength, 1U);
			templength -= 3U;
			memcpy(Spi.Tx.Buffer+2U, &templength, 1U);
			Nfc.ISO14443Operation = NFC_ISO14443_SELECT_TAG_2;
			NFC_Request(Spi.Tx.Buffer, Spi.Tx.Length);
		}
		else if(Nfc.ISO14443Operation >= NFC_ISO14443_SELECT_TAG_2)
		{
			if(iso14443_sub_profile_handler(Spi.Rx.Buffer, Spi.Rx.Length))
				Nfc.ISO14443Operation = NFC_ISO14443_SEQUENCE_IDLE;
		}
	}
	else
	{
		Nfc.TagAbsenceCount++;
		if(Nfc.TagAbsenceCount == Nfc.Settings.TagAbsenceRate)
		{
			Nfc.ISO14443Operation = NFC_ISO14443_SEQUENCE_IDLE;
			Nfc.TagAbsenceCount = 0U;
			Exit_ISO14443_Handler();
		}
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t ISO14443_Parser(uint8_t *buffer, uint8_t length)
{
	uint8_t status = 0U, updateflag = 0;
	
	union
	{
		struct
		{
			uint8_t firstbytesignificantbits:4;
			uint8_t parityerror:1;
			uint8_t crcerror:1;
			uint8_t rfu:1;
			uint8_t collisionflag:1;
		}specific;
		uint8_t whole;
	}receiveflag;
	union
	{
		struct
		{
			uint8_t value:4;
			uint8_t rfu:4;
		}specific;
		uint8_t whole;
	}bitcollisionindex;
	uint8_t bytecollisionindex = 0;
	
	receiveflag.whole = 0U;
	bitcollisionindex.whole = 0U;
	if(Nfc.ISO14443Operation == NFC_ISO14443_SEQUENCE_IDLE)
	{
		Nfc.ResultCode = 0U;
	}
	else if(Nfc.ISO14443Operation == NFC_ISO14443_REQUEST_FOR_ANSWER)
	{
		if(Nfc.ResultCode == 0x80)
		{
			union
			{
				struct
				{
					uint16_t bitframeanticollision:5;
					uint16_t rfu1:1;
					uint16_t uidsizebitframe:2;
					uint16_t proprietarycoding:4;
					uint16_t rfu4:4;
				}specific;
				uint16_t whole;
			}atqa;
			
			atqa.whole = 0U;
			memcpy(&atqa.whole, buffer+NFC_HEAD_SIZE, sizeof(atqa));
			if(atqa.specific.uidsizebitframe == 0U)
				Nfc.Information.UIDSize = 4U;
			else if(atqa.specific.uidsizebitframe == 1U)
				Nfc.Information.UIDSize = 7U;
			else if(atqa.specific.uidsizebitframe == 3U)
				Nfc.Information.UIDSize = 10U;
				
			if(atqa.whole == 0x0044)
			{
				Nfc.Information.Profile = NFC_ISO14443;
				Nfc.Information.SubProfile = MIFARE_ULTRALIGTH_EV1;
			}
			else if(atqa.whole == 0x0344)
			{
				Nfc.Information.Profile = NFC_ISO14443;
				Nfc.Information.SubProfile = MIFARE_DESFIRE_EV2;
			}
			else
				status = 1U;
			updateflag = 1U;
		}
		else
			status = 1U;
	}
	else if(Nfc.ISO14443Operation == NFC_ISO14443_ANTI_COLLISION_LEVEL_1)
	{
		if(Nfc.ResultCode == 0x80)
		{
			memcpy(Nfc.Information.UID, buffer+3U, 3U);
			updateflag = 1U;
		}
		else
			status = 1U;
	}
	else if(Nfc.ISO14443Operation == NFC_ISO14443_SELECT_TAG_1)
	{
		if(Nfc.ResultCode == 0x80)
		{
			uint16_t selectacknowledgement = 0U, crc16 = 0U;
			
			memcpy(&selectacknowledgement, buffer+2U, 1U);
			memcpy(&crc16, buffer+3U, 2U);
			
			updateflag = 1U;
		}
		else
			status = 1U;
	}
	
	else if(Nfc.ISO14443Operation == NFC_ISO14443_ANTI_COLLISION_LEVEL_2)
	{
		if(Nfc.ResultCode == 0x80)
		{
			memcpy(Nfc.Information.UID+3U, buffer+2U, 4U);
			updateflag = 1U;
		}
		else
			status = 1U;
	}
	else if(Nfc.ISO14443Operation == NFC_ISO14443_SELECT_TAG_2)
	{
		if(Nfc.ResultCode == 0x80)
		{
			uint16_t selectacknowledgement = 0U, crc16 = 0U;
			
			memcpy(&selectacknowledgement, buffer+2U, 1U);
			memcpy(&crc16, buffer+3U, 2U);
			
			updateflag = 1U;
		}
		else
			status = 1U;
	}
	
	if(updateflag)
	{
		memcpy(&receiveflag.whole, buffer+2U+length-3U, 1U);
		memcpy(&bytecollisionindex, buffer+2U+length-2U, 1U);
		memcpy(&bitcollisionindex.whole, buffer+2U+length-1U, 1U);
	}
	
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Exit_ISO14443_Handler(void)
{
	volatile uint8_t index = 0U;
	
	CLEAR_DATA(&Nfc.Information, sizeof(Nfc.Information));
	
	for(index=0U; index<MAXIMUM_ACL_ENTRIES; index++)
	{
		ACLTable[index].Status.Auth = 0U;
		ACLTable[index].Status.Presence = 0U;
	}
	
	Nfc.ISO14443Operation = NFC_ISO14443_SEQUENCE_IDLE;
	Nfc_Profile_Switch();
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t iso14443_sub_profile_handler(uint8_t *buffer, uint8_t length)
{
	uint8_t status = 0U;
	
	if(Nfc.Information.SubProfile == MIFARE_ULTRALIGTH_EV1)
	{
		status = mifare_ultraligth_ev1_handler(buffer, length);
	}
	else if(Nfc.Information.SubProfile == MIFARE_DESFIRE_EV2)
	{
		status = mifare_desfire_ev2_handler(buffer, length);
	}
	
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t mifare_ultraligth_ev1_handler(uint8_t *buffer, uint8_t length)
{
	uint8_t status = 0U;
	if(!mifare_ultraligth_ev1_parser(buffer, length))
	{
		if(Nfc.ISO14443Operation == NFC_ISO14443_SELECT_TAG_2)
		{
			Nfc.ISO14443Operation = NFC_ISO14443_MULEV1_GET_VERSION;
			NFC_Request(Mifare_UL_EV1_Get_Version, SIZE_OF_ARRAY(Mifare_UL_EV1_Get_Version));
		}
		else if(Nfc.ISO14443Operation == NFC_ISO14443_MULEV1_GET_VERSION)
		{
			/*if(Nfc.ProcessStatus == NFC_PROCESS_REQUESTED)
			{
				Nfc.ProcessStatus = NFC_PROCESS_IN_EXECUTION;
				Nfc.Write.CurrentPage = Nfc.Read.CurrentPage = 0U;
			}
			if(Nfc.ProcessStatus == NFC_PROCESS_IN_EXECUTION)*/
			
			//volatile dfilter filter;
			volatile aclseacrhfilter filter;
			volatile entrytoken temptoken;
			memset(&temptoken, 0U, sizeof(entrytoken));
			
			
			filter.Type.Whole = 0U;
			filter.Info.Whole = 0U;
			filter.Token.Whole = 0U;
			
			filter.Type.Specific.First = 1U;
			filter.Type.Specific.Configured = 1U;
			
			filter.Type.Specific.UseInfo = 1U;
			filter.Info.Specific.Absolute = 1U;
			
			filter.Type.Specific.UseToken = 1U;
			filter.Token.Specific.AuthFalse = 1U;
			
			
			if(ACL_Search(Nfc.Information, temptoken, filter, &Nfc.ACLSearchResult, &Nfc.ACLSearchCount))
			{
				uint8_t templength = 0U;
				
				ACLTable[Nfc.ACLSearchResult].Status.Presence = 1U;
				if(ACLTable[Nfc.ACLSearchResult].Status.Expiry)
					goto skip_auth;
			
				memcpy(Spi.Tx.Buffer, Mifare_UL_EV1_Password_Autheticate_Head, SIZE_OF_ARRAY(Mifare_UL_EV1_Password_Autheticate_Head));
				templength += SIZE_OF_ARRAY(Mifare_UL_EV1_Password_Autheticate_Head);
				memcpy(Spi.Tx.Buffer+templength, ACLTable[Nfc.ACLSearchResult].Settings.Token.Guide.Password32, SIZE_OF_ARRAY(ACLTable[Nfc.ACLSearchResult].Settings.Token.Guide.Password32));
				templength += SIZE_OF_ARRAY(ACLTable[Nfc.ACLSearchResult].Settings.Token.Guide.Password32);
				memcpy(Spi.Tx.Buffer+templength, Mifare_UL_EV1_Password_Autheticate_Tail, SIZE_OF_ARRAY(Mifare_UL_EV1_Password_Autheticate_Tail));
				templength += SIZE_OF_ARRAY(Mifare_UL_EV1_Password_Autheticate_Tail);

				
				Nfc.ISO14443Operation = NFC_ISO14443_MULEV1_AUTHENTICATE;
				Spi.Tx.Length = templength;
				NFC_Request(Spi.Tx.Buffer, Spi.Tx.Length);				
			}
			else
			{
				skip_auth:
				status = 1U;
				NFC_Request(Nfc_RF_Field_Off, SIZE_OF_ARRAY(Nfc_RF_Field_Off));
			}
		}
		/*else if((Nfc.ISO14443Operation == NFC_ISO14443_MULEV1_AUTHENTICATE) && (Nfc.SelectedOperation >= NFC_ISO14443_MULEV1_READ))
		{	
			if(Nfc.SelectedOperation == NFC_ISO14443_MULEV1_READ)
			{
				Nfc.ISO14443Operation = NFC_ISO14443_MULEV1_READ;
				NFC_Request(Mifare_UL_EV1_Memory_Read, SIZE_OF_ARRAY(Mifare_UL_EV1_Memory_Read));
			}
			else if(Nfc.SelectedOperation == NFC_ISO14443_MULEV1_WRITE)
			{
				if(Nfc.Write.PageCount)
				{
					uint8_t bufferindex = 4U, temppagenumber = 0U, temppage[4];
					
					memset(temppage, 0x00, 4U);
					memcpy(Spi.Tx.Buffer, Mifare_UL_EV1_Page_Write, SIZE_OF_ARRAY(Mifare_UL_EV1_Page_Write));
					Spi.Tx.Length = SIZE_OF_ARRAY(Mifare_UL_EV1_Page_Write);
					temppagenumber = 0x04 + Nfc.Write.CurrentPage;
					memcpy(Spi.Tx.Buffer+4U, &temppagenumber, 1U);
					bufferindex *= Nfc.Write.CurrentPage;
					memcpy(Spi.Tx.Buffer+5U, Nfc.Write.Buffer+bufferindex, 4U);
					Nfc.Write.CurrentPage++;
					Nfc.Write.PageCount--;
					NFC_Request(Spi.Tx.Buffer, Spi.Tx.Length);
				}
				else
				{
					Nfc.Write.PageCount = Nfc.Write.CurrentPage = 0U;
					Nfc.SelectedOperation = NFC_ISO14443_MULEV1_READ;
				}
			}
			else if(Nfc.SelectedOperation == NFC_ISO14443_MULEV1_CHANGE_PASSWORD)
			{
				uint8_t temppage = 0x12;
				
				memcpy(Spi.Tx.Buffer, Mifare_UL_EV1_Page_Write, SIZE_OF_ARRAY(Mifare_UL_EV1_Page_Write));
				Spi.Tx.Length = SIZE_OF_ARRAY(Mifare_UL_EV1_Page_Write);
				memcpy(Spi.Tx.Buffer+5U, Nfc.ISO14443Parameter.MULEV1NewPassword, 4U);
				memcpy(Spi.Tx.Buffer+4U, &temppage, 1U);
				memcpy(Nfc.ISO14443Parameter.MULEV1CurrentPassword, Nfc.ISO14443Parameter.MULEV1NewPassword, 4U);
				Nfc.ISO14443Operation = NFC_ISO14443_MULEV1_GET_VERSION;
				Nfc.SelectedOperation = NFC_ISO14443_MULEV1_CHANGE_PACK;
				NFC_Request(Spi.Tx.Buffer, Spi.Tx.Length);
			}
			else if(Nfc.SelectedOperation == NFC_ISO14443_MULEV1_CHANGE_PACK)
			{
				uint8_t temppage = 0x13, temppagecontents[4];
				
				memset(temppagecontents, 0x00, 4U);
				memcpy(temppagecontents, Nfc.ISO14443Parameter.MULEV1NewPACK, 2U);
				memcpy(Spi.Tx.Buffer, Mifare_UL_EV1_Page_Write, SIZE_OF_ARRAY(Mifare_UL_EV1_Page_Write));
				Spi.Tx.Length = SIZE_OF_ARRAY(Mifare_UL_EV1_Page_Write);
				memcpy(Spi.Tx.Buffer+5U, temppagecontents, 4U);
				memcpy(Spi.Tx.Buffer+4U, &temppage, 1U);
				memcpy(Nfc.ISO14443Parameter.MULEV1CurrentPACK, Nfc.ISO14443Parameter.MULEV1NewPACK, 2U);
				Nfc.ISO14443Operation = NFC_ISO14443_MULEV1_GET_VERSION;
				Nfc.SelectedOperation = NFC_ISO14443_SEQUENCE_IDLE;
				NFC_Request(Spi.Tx.Buffer, Spi.Tx.Length);
			}
		}*/
		else
		{
			Nfc.SelectedOperation = NFC_ISO14443_SEQUENCE_IDLE;
			Nfc.ProcessStatus = NFC_PROCESS_COMPLETE;
			status = 1U;
			NFC_Request(Nfc_RF_Field_Off, SIZE_OF_ARRAY(Nfc_RF_Field_Off));
		}
	}
	else
	{
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
uint8_t mifare_ultraligth_ev1_parser(uint8_t *buffer, uint8_t length)
{
	uint8_t status = 0U;
	
	
	if(Nfc.ISO14443Operation == NFC_ISO14443_MULEV1_GET_VERSION)
	{
		if(Nfc.ResultCode == 0x80)
		{
			uint8_t storageversion = 0;
			
			memcpy(&storageversion, buffer+2U+6U, 1U);
			
			/*if(storageversion == 0x0B)
				Nfc.Information.AvailableMemory = 48U;
			else if(storageversion == 0x0E)
				Nfc.Information.AvailableMemory = 128U;
			else if(storageversion == 0x0F)
				Nfc.Information.AvailableMemory = 144U;
			else if(storageversion == 0x11)
				Nfc.Information.AvailableMemory = 504U;
			else if(storageversion == 0x13)
				Nfc.Information.AvailableMemory = 888U;*/
			if(storageversion == 0x13)
			{
				Nfc.Information.AvailableMemory = 888U;
				status = 0U;
			}
			else
				status = 1U;
		}
		else
			status = 0U;
	}
	else if(Nfc.ISO14443Operation == NFC_ISO14443_MULEV1_AUTHENTICATE)
	{
		/*if(Nfc.ResultCode == 0x80)
		{
			uint8_t passwordacknowledgement[2];	
			memset(passwordacknowledgement, 0x00, 2U);
			memcpy(passwordacknowledgement, buffer+2U, 2U);
			if(memcmp(passwordacknowledgement, Nfc.ISO14443Parameter.MULEV1CurrentPACK, 2U))
			{
				Nfc.ProcessStatus = NFC_PROCESS_AUTHENTICATION_ERROR;
				status = 1U;
			}
		}
		else
		{
			Nfc.ProcessStatus = NFC_PROCESS_AUTHENTICATION_ERROR;
			status = 1U;
		}*/
		
		if((Nfc.ResultCode == 0x80) && (Spi.Rx.Length == 0x07))
		{
			ACLTable[Nfc.ACLSearchResult].Status.Auth = 1U;
		}
		else
		{
			ACLTable[Nfc.ACLSearchResult].Status.Auth = 0U;
			status = 1U;
		}
		
		
		
	}
	else if(Nfc.ISO14443Operation == NFC_ISO14443_MULEV1_READ)
	{
		if(Nfc.ResultCode == 0x80)
		{
			Nfc.Read.PageSize = 4U;
			Nfc.Read.PageCount = (uint8_t)((Nfc.Information.AvailableMemory/Nfc.Read.PageSize));
			memcpy(Nfc.Read.Buffer, buffer+2U, Nfc.Information.AvailableMemory);
		}
		else
		{
			Nfc.ProcessStatus = NFC_PROCESS_EXECUTION_ERROR;
			status = 1U;
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
uint8_t mifare_desfire_ev2_handler(uint8_t *buffer, uint8_t length)
{
	uint8_t status = 0U;
	
	//
	
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t mifare_desfire_ev1_parser(uint8_t *buffer, uint8_t length)
{
	uint8_t status = 0U;
	
	//
	
	return status;
}


/* End user code. Do not edit comment generated here */