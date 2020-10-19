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
* File Name    : r_cg_userdefine.h
* Version      : CodeGenerator for RL78/G13 V2.05.04.02 [20 Nov 2019]
* Device(s)    : R5F100FJ
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 19-10-2020
***********************************************************************************************************************/

#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */

/***************************************** Default Macros *****************************************/
#define PASSWORD_32_SIZE		4U
#define PASSWORD_ACK_32_SIZE		2U
#define VERIFICATION_CODE_SIZE		4U
#define MASK_64_SIZE			8U

/***************************************** Maximum Macros *****************************************/
#define MAXIMUM_UID_SIZE		10U
#define MAXIMUM_ACCESS_TABLE_ENTRIES	20U
#define MAXIMUM_ACTIONS_PER_ENTRY	20U
#define MAXIMUM_READ_WRITE_BUFFER_SIZE	256U
#define MAXIMUM_AES_KEY_RECORDS		4U

#define MAXIMUM_TOKEN_BUFFER_SIZE	1000U
#define MAXIMUM_ACL_ENTRIES		10U
#define MAXIMUM_TAG_ID_SIZE		10U
#define AUTH_CODE_SIZE			4U

#define MAXIMUM_USERS			5U
#define MAXIMUM_ACTIONS			25U
#define BROADCAST_ACTION_DEBOUCE_TIME	5U

/* End user code. Do not edit comment generated here */
#endif
