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
/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint16_t Compute_CRC16(uint16_t calculated_crc, uint8_t *data, uint16_t size)
{
	uint16_t temps;
	
	R_WDT_Restart();
	
	for (temps = 0 ; temps < size ; temps ++)
	{
		uint8_t i = 8;
		
		calculated_crc = calculated_crc ^ (unsigned short)*data++ << 8;
		
		do
		{
			if (calculated_crc & 0x8000)
				calculated_crc = calculated_crc << 1 ^ 0x1021;
			else
				calculated_crc = calculated_crc << 1;
		}
		while (--i);
	}
	
	return calculated_crc;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
uint8_t Checksum8(uint8_t *datapointer, uint16_t length)
{
	uint8_t checksum = 0U;
	uint16_t ckcount = 0U;

	R_WDT_Restart();
	
	checksum = 0;
	for(ckcount=0; ckcount<length; ckcount++)
	{
		checksum += *(datapointer+ckcount);
	}
	
	return (uint8_t)checksum;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint16_t Create_Rand_Seed(uint8_t *buffer, uint16_t length)
{
	volatile uint8_t count = 0;
	volatile uint16_t seed = 0;
	
	if(!(length%2U))
	{
		for(count=1; count<length; count+=2)
		{
			seed += (((uint16_t)*(buffer+(count-1)) << 8U) | ((uint16_t)*(buffer+count)));
		}
	}
	
	return seed;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint8_t Check_In_Range(uint8_t element, uint8_t *buffer, uint8_t buffersize)
{
	volatile uint8_t status = 0U;
	
	while(buffersize--)
	{
		if(element == (*(buffer+buffersize)))
		{
			status = 1U;
			break;
		}
	}
	
	return status;
}

/* End user code. Do not edit comment generated here */
