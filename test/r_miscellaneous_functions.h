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
* File Name    : r_miscellaneous_functions.h
* Version      : 
* Device(s)    : 
* Tool-Chain   : 
* Description  : 
* Creation Date: 
***********************************************************************************************************************/

#ifndef _R_MISCELLANEOUS_FUNCTIONS_H
#define _R_MISCELLANEOUS_FUNCTIONS_H
/* Start user code for function. Do not edit comment generated here */
/********************************************* Macros *********************************************/
#define CRC16_POLYNOMIAL_CCITT	0x1021
#define CHECKSUM_EIGTH_SIZE 1U
#define CRC16_SIZE 2U
#define SIZE_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))
#define CLEAR_DATA(array, size) (memset(array, 0x00, size))

/********************************************* Function Prototypes *********************************************/
uint8_t Checksum8(uint8_t *datapointer, uint16_t length);
uint16_t Compute_CRC16(uint16_t calculated_crc, uint8_t *data, uint16_t size);
uint16_t Create_Rand_Seed(uint8_t *buffer, uint16_t length);
uint8_t Check_In_Range(uint8_t element, uint8_t *buffer, uint8_t buffersize);

uint8_t Check_Scheduler_Run_Interval(uint32_t timereference, uint32_t interval);

/* End user code. Do not edit comment generated here */
#endif