/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) 2011, 2017 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_function_prototype.h
* Version      : 
* Device(s)    : R5F100FE
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 27-04-2019
***********************************************************************************************************************/

#ifndef _FUNC_PRO_H
#define _FUNC_PRO_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
/******************** MCU Libraries ********************/
#include "r_cg_userdefine.h"
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_serial.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"

/******************** C Compiler ********************/
#include "math.h"
#include "stdlib.h"
#include "string.h"

/******************** User Libraries ********************/
#include "r_acl_handler.h"
#include "r_nfc_handler.h"
#include "r_beacon_handler.h"
#include "r_aes_handler.h"
#include "r_miscellaneous_functions.h"
#include "r_rgb_handler.h"

/********************************** Function Prototypes **********************************/
void main(void);
void R_Boot_Up(void);
void Fault_Recovery_Handler(void);
void Device_Init(void);

void Run_Scheduler(void);
void Scheduler_Routine(uint32_t timereference);
void Stop_Scheduler(void);

/* End user code. Do not edit comment generated here */
#endif