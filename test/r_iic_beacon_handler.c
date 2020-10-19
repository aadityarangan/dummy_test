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
volatile uint8_t flag = 0;
/* End user code. Do not edit comment generated here */

/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
void IICA_Beacon_Parser_Handler(void)
{
	if(I2C.RxState == IICA_BEACON_RECEIVED)
	{
		if(!Parse_I2C_Beacon(I2C.Rx.Buffer+IICA_BEACON_HEADER_SIZE, &I2C.Rx.Length, I2C.Tx.Buffer, &I2C.Tx.Length))
				I2C.TxState = IICA_READY_TO_TRANSMIT;
		I2C.RxState = IICA_RX_IDLE;
	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint8_t Validate_IICA_Beacon_Header(uint8_t *buffer)
{
	uint8_t status = 0;
	uint16_t length = 0;
	
	if(buffer[0] != 'I')
		status |= 0x01;
	if(memcmp((buffer+1), "AAA", 3))
		status |= 0x02;
	memcpy(&length, (buffer+4), 2);
	if((length<(AES_BLOCK_SIZE*2)) || (length%AES_BLOCK_SIZE))
		status |= 0x04;
		
	return status;
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
void IICA_Beacon_Receive_Handler(uint8_t checksum)
{
	volatile uint8_t dummy = 0U;
	if(I2C.RxState == IICA_RX_IDLE)
    	{
    		if(!(Validate_IICA_Beacon_Header(I2C.Rx.Buffer)))
		{
			I2C.RxState = IICA_IN_RECEPTION;
			memcpy(&I2C.Rx.Length, I2C.Rx.Buffer+IICA_BEACON_LENGTH_POSITION, IICA_BEACON_LENGTH_SIZE);
			R_IICA0_Slave_Receive(I2C.Rx.Buffer+IICA_BEACON_HEADER_SIZE, I2C.Rx.Length+CHECKSUM_EIGTH_SIZE);
		}
		else
		{
			I2C.RxState = IICA_RX_IDLE;
		}
    	}
   	else if(I2C.RxState == IICA_IN_RECEPTION)
    	{
		I2C.Rx.Checksum = checksum;
		dummy = Checksum8(I2C.Rx.Buffer+IICA_BEACON_HEADER_SIZE, I2C.Rx.Length);
		if(Checksum8(I2C.Rx.Buffer+IICA_BEACON_HEADER_SIZE, I2C.Rx.Length) == I2C.Rx.Checksum)
			I2C.RxState = IICA_BEACON_RECEIVED;
     		else
			I2C.RxState = IICA_RX_IDLE;
    	}
}

/***********************************************************************************************************************
* Function Name: 
* Description  : 
* Arguments    : 
* Return Value : 
***********************************************************************************************************************/
uint8_t Parse_I2C_Beacon(uint8_t *request, uint16_t *requestlength, uint8_t *response, uint16_t *responselength)
{
	volatile uint8_t requestiv[AES_BLOCK_SIZE],
			 responseiv[AES_BLOCK_SIZE];
	volatile uint8_t status = 0,
			 aeskeynumber = 0,
			 statusresponseflag = 0,
			 numberofresponsemodules = 0,
			 responsechecksum8 = 0,
			 *beaconlengthposition,
			 *numberofmodulesposition,
			 *encrypteddatastartposition;
	volatile uint16_t requestcrc16 = 0,
			  responsecrc16 = 0,
			  sequenceid = 0,
			  randseed = 0,
			  tempresponselength = 0,
			  directionmodulecode = 0;
	
	memcpy(requestiv, request+(*requestlength)-AES_BLOCK_SIZE, AES_BLOCK_SIZE);	//Decrypt beacon
	aeskeynumber = 0U;
	AES_CBC_decrypt(request, (*requestlength)-AES_BLOCK_SIZE, &Device.Settings.AESKey[aeskeynumber][0], requestiv);
	memcpy(&requestcrc16, request+(*requestlength)-AES_BLOCK_SIZE-CRC16_SIZE, CRC16_SIZE);	//CRC-16 copy
	if(requestcrc16 == Compute_CRC16(0U, request, (*requestlength)-AES_BLOCK_SIZE-CRC16_SIZE))	//CRC-16 check
	{
		volatile uint8_t numberofmodules = 0;
		
		
		
		memcpy(response, "I", 1U);		//Response: Added sub ID
		response++;
		memcpy(response, "AAA", 3U);		//Response: Added device ID
		response += 3U;
		beaconlengthposition = response;	//Resposne: Marked beacon length position
		response += 2U;
		(*responselength) = 0;
		encrypteddatastartposition = response;	//Response: Marked encrypted data start position
		memcpy(&I2C.Rx.SequenceID, request, 2U);	//Request: Copied sequence ID
		request += 2U;
		memcpy(response, &I2C.Rx.SequenceID, 2U);	//Response: Added sequence id
		response += 2U;
		(*responselength) += 2U;
		
		I2C.Tx.SequenceID = I2C.Rx.SequenceID;
		
		memcpy(&directionmodulecode, request, sizeof(directionmodulecode));	//Request: Check direction module code
		request += sizeof(directionmodulecode);
		if(directionmodulecode == NFC_SERVER_TO_READER_MODULE_CODE)
		{
			directionmodulecode = NFC_READER_TO_SERVER_MODULE_CODE;
			memcpy(response, &directionmodulecode, sizeof(directionmodulecode));	//Response: Check direction module code
			(*responselength) += sizeof(directionmodulecode);
			response += sizeof(directionmodulecode);
		}
		
		
		if(directionmodulecode == NFC_KLE_TO_READER_MODULE_CODE)
		{
			directionmodulecode = NFC_READER_TO_KLE_MODULE_CODE;
			memcpy(response, &directionmodulecode, sizeof(directionmodulecode));	//Response: Check direction module code
			response += sizeof(directionmodulecode);
			(*responselength) += sizeof(directionmodulecode);
		}
		
		
		memcpy(&numberofmodules, request, 1U);	//Request: Copied number of modules
		request += 1U;
		numberofmodulesposition = response;	//Response: Marked number of modules position
		response++;
		(*responselength)++;
		
		while(numberofmodules)	//Parsing modules
		{
			uint8_t modulecode = 0U,
				modulelength = 0U,
				elapsedwrite = 0U;
			
			memcpy(&modulecode, request,1U);	//Request: Coppied module code
			request += 1U;
			memcpy(&modulelength, request,1U);	//Request: Copied module length
			request += 1U;
			if(modulecode == NFC_DEVICE_STATUS_REQUEST_MODULE_CODE)
				statusresponseflag = 1U;
			else
				Module_Handler(modulecode, request, &modulelength, response, &elapsedwrite);	//Module handler
			
			request += modulelength;	//Request: Move request pointer by request module length
			response += elapsedwrite;	//Response: Move response pointer by elapsed write length
			(*responselength) += elapsedwrite;	//Response: Count reposne module write length
			numberofresponsemodules++;
			numberofmodules--;
		}
		
		if(statusresponseflag)
		{
			volatile uint16_t elapsedwrite = 0U;
			
			statusresponseflag  = 0U;
			Status_Response(response, &elapsedwrite);
			response += elapsedwrite;
			(*responselength) += elapsedwrite;
			numberofresponsemodules++;
		}
		
		memcpy(numberofmodulesposition, &numberofresponsemodules, 1U);
		(*responselength) += 2U;
		(*responselength) = (((*responselength)+AES_BLOCK_SIZE-1)/AES_BLOCK_SIZE)*AES_BLOCK_SIZE;	//Block adjusted length
		responsecrc16 = Compute_CRC16(0U, encrypteddatastartposition, (*responselength)-CRC16_SIZE);
		memcpy(encrypteddatastartposition+(*responselength)-CRC16_SIZE, &responsecrc16, CRC16_SIZE);
		randseed = Create_Rand_Seed(requestiv, AES_BLOCK_SIZE);
		Get_Random_Iv(responseiv, randseed);
		AES_CBC_encrypt(encrypteddatastartposition, (*responselength), &Device.Settings.AESKey[aeskeynumber][0], responseiv);
		
		memcpy(encrypteddatastartposition+(*responselength), responseiv, AES_BLOCK_SIZE);
		(*responselength) += AES_BLOCK_SIZE;
		memcpy(beaconlengthposition, responselength, 2U);
		responsechecksum8 = Checksum8(encrypteddatastartposition, (*responselength));
		memcpy(encrypteddatastartposition+(*responselength), &responsechecksum8, 1U);
		(*responselength) += IICA_BEACON_HEADER_SIZE;
		(*responselength)++;
		NOP();
	}
	else
		status = 1U;
	
	return status;
}

/* End user code. Do not edit comment generated here */
