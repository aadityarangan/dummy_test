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
* File Name    : r_extern_glb_var.h
* Version      : 
* Device(s)    : R5F100FE
* Tool-Chain   : CA78K0R
* Description  : This file includes user definition.
* Creation Date: 27-04-2019
***********************************************************************************************************************/

#ifndef _EXTERN_GLOBAL_VARIABLE_H
#define _EXTERN_GLOBAL_VARIABLE_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
#include "r_cg_userdefine.h"
/******************************************* Variables *******************************************/
/******************************************** Device ********************************************/
extern volatile struct device
{
	struct devicesettings
	{
		uint8_t AESKey[MAXIMUM_AES_KEY_RECORDS][16U];
	}Settings;
	struct
	{
		uint8_t ID;
		uint32_t UpTime;
	}Information;
	struct
	{
		uint8_t Flag:1;
		uint8_t Dummy:7;
		uint16_t Ticks;
	}Operation;
}Device;

extern const uint8_t PrimaryAESKey[16];
extern const uint8_t SecondaryAESKey[16];

typedef struct
{
	uint32_t LSB;
	uint32_t MSB;
}uint64_t;

/********************************************* IICA *********************************************/
typedef enum
{
	 IICA_RX_IDLE
	,IICA_IN_RECEPTION
	,IICA_BEACON_RECEIVED
}iicarxstate;

typedef enum
{
	 IICA_TX_IDLE
	,IICA_READY_TO_TRANSMIT
	,IICA_RESPONSE_HEAD_SENT
	,IICA_RESPONSE_DATA_SENT
	,IICA_IN_TRANSMISSION
}iicatxstate;

extern volatile struct
{
	iicatxstate TxState;
	iicarxstate RxState;
	uint8_t TimeOut;
	uint8_t Direction;
	struct iicadir
	{
		uint16_t SequenceID;
		uint8_t Checksum;
		uint8_t Buffer[250];
		uint16_t Length;
	}Tx, Rx;
}I2C;

/*********************************************** SPI ***********************************************/
typedef enum
{
	SPI_IDLE,
	SPI_BUSY
}spistatus;

extern volatile struct spi
{
	spistatus Status;
	uint8_t ChipSelectStretch;
	struct spidir
	{
		uint8_t Length;
		uint8_t Buffer[500];
	}Tx, Rx;
}Spi;

/********************************************************** NFC **********************************************************/
typedef enum
{
	 NFC_COM_IDLE
	,NFC_SEND_COMMAND
	,NFC_SEND_READ_COMMAND
	,NFC_READ_RESPONSE_HEAD
	,NFC_READ_DATA
}nfcstatus;

typedef enum
{
	 NFC_PROCESS_IDLE
	,NFC_PROCESS_REQUESTED
	,NFC_PROCESS_IN_EXECUTION
	,NFC_PROCESS_AUTHENTICATION_ERROR
	,NFC_PROCESS_EXECUTION_ERROR
	,NFC_PROCESS_COMPLETE
}nfcprocessstatus;

typedef enum
{
	 NFC_PROFILE_NONE
	,NFC_ISO14443
	,NFC_ISO15693
}nfcprofiles;

typedef enum
{
	 NFC_SUB_PROFILE_NONE
	,ICODE_SLIX
	,MIFARE_ULTRALIGTH_EV1
	,MIFARE_DESFIRE_EV2
	,MIFARE_PLUS_EV1
	,NTAG_216
}nfcsubprofiles;

typedef struct 
{
	uint8_t UIDSize;
	uint8_t UID[MAXIMUM_TAG_ID_SIZE];
	nfcprofiles Profile;
	nfcsubprofiles SubProfile;
	uint16_t AvailableMemory;
}taginfo;

typedef enum
{
	 NFC_ISO14443_SEQUENCE_IDLE
	,NFC_ISO14443_PROTOCOL_SELECT
	,NFC_ISO14443_REQUEST_FOR_ANSWER
	,NFC_ISO14443_ANTI_COLLISION_LEVEL_1
	,NFC_ISO14443_SELECT_TAG_1
	,NFC_ISO14443_ANTI_COLLISION_LEVEL_2
	,NFC_ISO14443_SELECT_TAG_2
	,NFC_ISO14443_MULEV1_GET_VERSION
	,NFC_ISO14443_MULEV1_AUTHENTICATE
	,NFC_ISO14443_MULEV1_READ
	,NFC_ISO14443_MULEV1_WRITE
	,NFC_ISO14443_MULEV1_CHANGE_PASSWORD
	,NFC_ISO14443_MULEV1_CHANGE_PACK
}nfciso14443operations;

typedef enum
{
	 NFC_ISO15693_SEQUENCE_IDLE
	,NFC_ISO15693_PROTOCOL_SELECT
	,NFC_ISO15693_ADJUST_INDEXMOD_GAIN
	,NFC_ISO15693_INVENTORY_REQUEST
	,NFC_ISO15693_GET_SYSTEM_INFO
}nfciso15693operations;

typedef struct
{
	uint8_t AFI;
	uint8_t DSFI;
	uint8_t BlockSize;
	uint8_t ICReference;
}nfciso15693parameters;

extern volatile struct nfc
{
	struct nfcsettings
	{
		uint8_t TagAbsenceRate;
	}Settings;
	uint8_t ResultCode;
	nfcstatus Status;
	nfcprocessstatus ProcessStatus;
	uint8_t TagAbsenceCount;
	nfcprofiles SearchProfile;
	uint8_t SelectedOperation;
	nfciso14443operations ISO14443Operation;
	nfciso15693operations ISO15693Operation;
	taginfo Information;
	uint8_t ACLSearchResult;
	uint8_t ACLSearchCount;
	struct readwrite
	{
		uint8_t PageSize;
		uint8_t PageCount;
		uint8_t CurrentPage;
		uint8_t Buffer[MAXIMUM_READ_WRITE_BUFFER_SIZE];
	}Read, Write;
}Nfc;

/******************************************************* Lock Handler *******************************************************/
typedef enum
{
	 ACTION_NONE
	,ACTION_VEHICLE_LOCK
	,ACTION_VEHICLE_UNLOCK
	,ACTION_VEHICLE_IGNITION_ON
	,ACTION_VEHICLE_IGNITION_OFF
	,ACTION_VEHICLE_MOBILIZE
	,ACTION_VEHICLE_IMMOBILIZE
	,ACTION_BOOT_LOCK
	,ACTION_BOOT_UNLOCK
	,ACTION_BATTERY_LOCK
	,ACTION_BATTERY_UNLOCK
	,ACTION_IGNITION_BOOT_BACKLIGTH
}accessactions;

typedef struct
{
	uint8_t UserID;
	uint64_t GenerationTime;
	uint16_t PresenceTime;
	accessactions PresenceAction;
	uint16_t AbsenceTime;
	accessactions AbsenceAction;
	union actionrule
	{
		struct
		{
			uint8_t Broadcast:1;
			uint8_t Dummy:7;
		}Specific;
		uint8_t Whole;
	}ActionRule;
	uint8_t AuthCode[AUTH_CODE_SIZE];
	struct guiderule
	{
		uint8_t Password32:1;
		uint8_t Mask64:1;
		uint8_t Dummy:6;
	}GuideRule;
	struct accessguide
	{
		uint8_t Password32[PASSWORD_32_SIZE];
		uint8_t PasswordACK16[PASSWORD_ACK_32_SIZE];
		uint8_t Mask64[MASK_64_SIZE];
	}Guide;
}entrytoken;

typedef struct
{
	union
	{
		struct
		{
			uint8_t Index:1;
			uint8_t First:1;
			uint8_t Last:1;
			uint8_t FreeSlot:1;
			uint8_t UseInfo:1;
			uint8_t UseToken:1;
			uint8_t Configured:1;
			uint8_t UnConfigured:1;
		}Specific;
		uint8_t Whole;
	}Type;
	union
	{
		struct
		{
			uint8_t Absolute:1;
			uint8_t UIDSize:1;
			uint8_t UID:1;
			uint8_t Profile:1;
			uint8_t SubProfile:1;
			uint8_t Dummy:3;
		}Specific;
		uint8_t Whole;
	}Info;
	union
	{
		struct
		{
			uint16_t Absolute:1;
			uint16_t UserID:1;
			uint16_t Expired:1;
			uint16_t Valid:1;
			uint16_t GenerationTime:1;
			uint16_t PresenceAction:1;
			uint16_t AbsenceAction:1;
			uint16_t PresenceTime:1;
			uint16_t AbsenceTime:1;
			uint16_t AuthCode:1;
			uint16_t AuthTrue:1;
			uint16_t AuthFalse:1;
			uint16_t Dummy:4;
		}Specific;
		uint16_t Whole;
	}Token;
}aclseacrhfilter;

extern volatile struct acltable
{
	struct aclsettings
	{
		taginfo Info;
		entrytoken Token;
	}Settings;
	struct aclstatus
	{
		uint8_t Presence:1;
		uint8_t Auth:1;
		uint8_t Expiry:1;
		uint8_t Config:1;
		uint8_t Active:1;
		uint8_t AbsenceTrigger:1;
		uint8_t Dummy:2;
		uint16_t PresenceCount;
		uint16_t AbsenceCount;
		uint16_t BroadcastCount;
	}Status;
}ACLTable[MAXIMUM_ACL_ENTRIES];

extern volatile struct permissions
{
	uint64_t GenerationTime;
	accessactions ActionQueue[MAXIMUM_ACTIONS];
}Permissions[MAXIMUM_USERS];

/********************************************** Include Function protoypes DATA ******************************************/
#include "r_function_prototype.h"

/* End user code. Do not edit comment generated here */
#endif