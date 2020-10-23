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
        R_WDT_Restart();
	Scheduler_Routine(Device.UpTime);
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
    
    Run_Scheduler();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
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
void Run_Scheduler(void)
{	
	Device.Status.Specific.SchedulerRun = 1U;
	R_TAU0_Channel4_Start();
	Scheduler_Routine(Device.UpTime);
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Scheduler_Routine(uint32_t timereference)
{
	if(Device.Status.Specific.SchedulerRun)
	{
		if(Device.Status.Specific.Reset) while(1U){}
		
		ACL_Book_Keeping(timereference);
		Nfc_Communication_Handler(timereference);
		Nfc_Handler(timereference);
		
		IICA_Beacon_Parser_Handler(timereference);
		
		Check_Edge_Action_Notifications(timereference);
		Check_Broadcast_Action_Notifications(timereference);
		
		RGB_Hallucinate_Handler(timereference);
	}
}

/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void Stop_Scheduler(void)
{
	Device.Status.Specific.SchedulerRun = 0U;
	R_TAU0_Channel4_Stop();
}

/* End user code. Do not edit comment generated here */