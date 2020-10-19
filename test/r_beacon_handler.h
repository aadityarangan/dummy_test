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
* File Name    : 
* Version      : 
* Device(s)    : 
* Tool-Chain   : 
* Description  : 
* Creation Date: 
***********************************************************************************************************************/
#ifndef _R_BEACON_HANDLER_H
#define _R_BEACON_HANDLER_H
/* Start user code for function. Do not edit comment generated here */
/********************************************* Macros *********************************************/

//Main Modules
#define NFC_SERVER_TO_READER_MODULE_CODE		60040U
#define NFC_READER_TO_SERVER_MODULE_CODE		60041U
#define NFC_READER_TO_KLE_MODULE_CODE			60042U
#define NFC_KLE_TO_READER_MODULE_CODE			60043U

//Outgoing Response
#define NFC_COMMAND_RESPONSE_MODULE_CODE		10U
#define NFC_STATUS_RESPONSE_MODULE_CODE			11U

//Incoming Requests
#define NFC_ADD_ACL_ENTRY_MODULE_CODE			30U
#define NFC_DELETE_ACL_ENTRY_MODULE_CODE		31U
#define NFC_CLEAR_ACL_TABLE_MODULE_CODE			32U
#define NFC_BULK_UPDATE_ACL_TABLE_MOCULE_CODE		33U
#define NFC_PERMISSIONS_NOTIFY_MODULE_CODE		34U
#define NFC_DEVICE_STATUS_REQUEST_MODULE_CODE		35U
#define NFC_ROUTINE_REQUEST_MODULE_CODE			36U
#define NFC_CONFIGURE_TAG_MODULE_CODE			37U

//Outgoing Requests
#define NFC_ACTION_NOTIFY_MODULE_CODE			130U

//Settings Requests
#define NFC_AES_KEY_CHANGE_MODULE_CODE			170U

//Device Requests
#define NFC_BOOT_UP_MODULE_CODE				210U
#define NFC_ECHO_MODULE_CODE				211U
#define NFC_REBOOT_DEVICE_MODULE_CODE			212U
#define NFC_FACTORY_RESET_MODULE_CODE			213U

#define MINIMUM_ONE_BYTE_NFC_MODULE_CODE_VALUE		00U
#define MAXIMUM_ONE_BYTE_NFC_MODULE_CODE_VALUE		255U

#define IICA_BEACON_HEADER_SIZE				6U
#define IICA_BEACON_LENGTH_POSITION			4U
#define IICA_BEACON_LENGTH_SIZE				2U

/********************************************* Function Prototypes *********************************************/
uint8_t Parse_I2C_Beacon(uint8_t *request, uint16_t *requestlength, uint8_t *response, uint16_t *responselength);
uint8_t Validate_IICA_Beacon_Header(uint8_t *buffer);
void IICA_Beacon_Receive_Handler(uint8_t checksum);
void IICA_Beacon_Parser_Handler(void);

/****************************************** Module Handler Functions ******************************************/
void Module_Handler(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite);
void Attach_Command_Response(uint8_t modulecode, uint8_t *response, uint8_t *elapsedwrite, uint8_t status, uint8_t reason);
void Status_Response(uint8_t *response, uint16_t *elapsedwrite);

void Test_Module(uint8_t modulecode, uint8_t *request, uint8_t *requestlength, uint8_t *response, uint8_t *elapsedwrite);

/* End user code. Do not edit comment generated here */
#endif