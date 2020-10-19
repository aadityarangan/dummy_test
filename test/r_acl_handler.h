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
* File Name    : r_loc_handler.h
* Version      : CodeGenerator for RL78/G13 V2.05.01.04 [18 May 2018]
* Device(s)    : R5F100FE
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for RTC module.
* Creation Date: 10-05-2019
***********************************************************************************************************************/

#ifndef _R_ACL_HANDLER_H
#define _R_ACL_HANDLER_H
/* Start user code for function. Do not edit comment generated here */
/*************************************** Macro Definitions ***************************************/
//

/*************************************** Function Prototypes ***************************************/
void ACL_Book_Keeping(void);
void Clear_ACL_Table(void);
uint8_t Add_ACL_Entry(uint8_t *entrydata, uint16_t length, uint16_t *consumedlength);
uint8_t Delete_ACL_Entry(aclseacrhfilter filter, taginfo info, entrytoken token, uint8_t index);
uint8_t ACL_Search(taginfo tag, entrytoken token, aclseacrhfilter filter, uint8_t *searchresult, uint8_t *resultcount);
uint8_t Update_Permissions(uint8_t *data);
uint8_t Add_Configure_Tag_Entry(uint8_t *entrydata, uint16_t length);
void Check_Edge_Action_Notifications(void);
void Attach_Action_Notifications(uint8_t *data, uint8_t *consumed);
void Check_Broadcast_Action_Notifications(void);

/* End user code. Do not edit comment generated here */
#endif