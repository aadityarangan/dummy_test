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
* File Name    : r_st95hf_handler.h
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100FE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for RTC module.
* Creation Date: 10-05-2019
***********************************************************************************************************************/

#ifndef _R_NFC_HANDLER_H
#define _R_NFC_HANDLER_H
/* Start user code for function. Do not edit comment generated here */


/*************************************** Macro Definitions ***************************************/
#define MIFARE_UL_EV1_DEFAULT_PASSWORD "INTL"
#define MIFARE_UL_EV1_DEFAULT_PASSWORD_ACKNOWLEDGEMENT "ID"

/*************************************** Macro Definitions ***************************************/
#define NFC_IRQ_IN P2.7
#define NFC_IRQ_OUT P1.5
#define NFC_HEAD_SIZE 2U
#define NFC_RESULT_CODE_SIZE 1U
#define NFC_RESPONSE_LENGTH_SIZE 1U

/*************************************** Function Prototypes ***************************************/
void NFC_Init(void);
void Nfc_Handler(void);
void Query_NFC_ID(void);
void NFC_Request(uint8_t *command, uint8_t length);

void Nfc_Communication_Handler(void);
void Nfc_Process_Handler(void);
void Nfc_Profile_Switch(void);

void ISO14443_Handler(void);
void Exit_ISO14443_Handler(void);
uint8_t ISO14443_Parser(uint8_t *buffer, uint8_t length);
void Update_ISO14443_Flags(uint8_t *buffer, uint8_t length);

void ISO15693_Handler(void);
uint8_t ISO15693_Parser(uint8_t *buffer, uint8_t length);
void Exit_ISO15693_Handler(void);

void Test_Command(void);


/*************************************** Function Prototypes ***************************************/
uint8_t Tag_Handler(uint8_t *buffer, uint8_t length);




/* End user code. Do not edit comment generated here */
#endif