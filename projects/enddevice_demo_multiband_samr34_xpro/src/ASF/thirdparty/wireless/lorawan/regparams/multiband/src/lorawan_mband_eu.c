/********************************************************************
* Copyright (C) 2016 Microchip Technology Inc. and its subsidiaries
* (Microchip).  All rights reserved.
*
* You are permitted to use the software and its derivatives with Microchip
* products. See the license agreement accompanying this software, if any, for
* more info about your rights and obligations.
*
* SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
* MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
* PURPOSE. IN NO EVENT SHALL MICROCHIP, SMSC, OR ITS LICENSORS BE LIABLE OR
* OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH
* OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY FOR ANY DIRECT OR INDIRECT
* DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
* INDIRECT OR CONSEQUENTIAL DAMAGES, OR OTHER SIMILAR COSTS. To the fullest
* extend allowed by law, Microchip and its licensors liability will not exceed
* the amount of fees, if any, that you paid directly to Microchip to use this
* software.
*************************************************************************
*
*                           lorawan_eu.c
*
* LoRaWAN eu file
*
******************************************************************************/
#include "lorawan_multiband.h"

#if (EU_BAND == 1)
static void InitDefault868Channels (void);
static void InitDefault433Channels (void);


static const ChannelParams_t DefaultChannels868[] =
{
	LC0_868,
	LC1_868,
	LC2_868,
};

static const ChannelParams_t DefaultChannels433[] =
{
	LC0_433,
	LC1_433,
	LC2_433,
};

static const OthChannelParams_t AdvChannels868[] =
{
	Adv_LC0_868,
	Adv_LC1_868,
	Adv_LC2_868,
};

static const OthChannelParams_t AdvChannels433[] =
{
	Adv_LC0_433,
	Adv_LC1_433,
	Adv_LC2_433,
};

static const SubBandParams_t SubBandParams433[] =
{
	SB0_433
};

static const SubBandParams_t SubBandParams868[] =
{
	SB0_868	,
	SB1_868	,
	SB2_868	,
	SB3_868	,
	SB4_868 ,
	SB5_868
};

static const uint16_t SubBandDutyCycle868[] =
{
	SB0_DT,
	SB1_DT,
	SB2_DT,
	SB3_DT,
	SB4_DT,
	SB5_DT
};

static const DRParams_t DefaultDrparamsEU[] =
{
	DR0_PARAMS_EU,
	DR1_PARAMS_EU,
	DR2_PARAMS_EU,
	DR3_PARAMS_EU,
	DR4_PARAMS_EU,
	DR5_PARAMS_EU,
	DR6_PARAMS_EU,
	DR7_PARAMS_EU,
};

#if (ENABLE_PDS == 1)
#define PDS_REG_EU868_CH_PARAM_1_ADDR                      ((uint8_t *)&(RegParams.cmnParams.paramsType2.chParams[MAX_CHANNELS_T2 - MAX_CHANNELS_T2]))
#define PDS_REG_EU868_CH_PARAM_2_ADDR                      ((uint8_t *)&(RegParams.cmnParams.paramsType2.othChParams[MAX_CHANNELS_T2 - MAX_CHANNELS_T2]))
#define PDS_REG_EU868_SB_DUTY_PRESCLAR_ADDR			       ((uint8_t *)&(RegParams.cmnParams.paramsType2.subBandDutyCycle[MAX_CHANNELS_T2 - MAX_CHANNELS_T2]))

#define PDS_REG_EU868_CH_PARAM_1_SIZE					    sizeof(RegParams.cmnParams.paramsType2.chParams)
#define PDS_REG_EU868_CH_PARAM_2_SIZE					    sizeof(RegParams.cmnParams.paramsType2.othChParams)
#define PDS_REG_EU868_SB_DUTY_PRESCLAR_SIZE                 sizeof(RegParams.cmnParams.paramsType2.subBandDutyCycle)

#define PDS_REG_EU868_CH_PARAM_1_OFFSET                    (PDS_FILE_START_OFFSET)
#define PDS_REG_EU868_SB_DUTY_PRESCLAR_OFFSET              (PDS_REG_EU868_CH_PARAM_1_OFFSET + PDS_SIZE_OF_ITEM_HDR + PDS_REG_EU868_CH_PARAM_1_SIZE)
#define PDS_REG_EU868_CH_PARAM_2_OFFSET                    (PDS_FILE_START_OFFSET)

/* PDS Reg Params NA Item declaration */

const ItemMap_t pds_reg_eu868_fid1_item_list[] = {
	DECLARE_ITEM(PDS_REG_EU868_CH_PARAM_1_ADDR,
	PDS_FILE_REG_EU868_04_IDX,
	(uint8_t)PDS_REG_EU868_CH_PARAM_1,
	PDS_REG_EU868_CH_PARAM_1_SIZE,
	PDS_REG_EU868_CH_PARAM_1_OFFSET),
	DECLARE_ITEM(PDS_REG_EU868_SB_DUTY_PRESCLAR_ADDR,
	PDS_FILE_REG_EU868_04_IDX,
	(uint8_t)PDS_REG_EU868_SB_DUTY_PRESCLAR,
	PDS_REG_EU868_SB_DUTY_PRESCLAR_SIZE,
	PDS_REG_EU868_SB_DUTY_PRESCLAR_OFFSET)	
};

const ItemMap_t pds_reg_eu868_fid2_item_list[] = {
	DECLARE_ITEM(PDS_REG_EU868_CH_PARAM_2_ADDR,
	PDS_FILE_REG_EU868_12_IDX,
	(uint8_t)PDS_REG_EU868_CH_PARAM_2,
	PDS_REG_EU868_CH_PARAM_2_SIZE,
	PDS_REG_EU868_CH_PARAM_2_OFFSET)	
};

PdsOperations_t aRegEu868Fid1PdsOps[PDS_REG_EU868_FID1_MAX_VALUE];
PdsOperations_t aRegEu868Fid2PdsOps[PDS_REG_EU868_FID2_MAX_VALUE];

/* PDS Callback */
void LorawanReg_EU868_Pds_Cb(void);

#endif
#endif

/****************************** FUNCTIONS *************************************/

StackRetStatus_t LORAReg_InitEU(IsmBand_t ismBand)
{
	StackRetStatus_t status = LORAWAN_SUCCESS;
#if (EU_BAND == 1)
	
	RegParams.TxCurDataRate = MAC_DEF_TX_CURRENT_DATARATE_EU;
	RegParams.maxChannels = MAX_CHANNELS_T2;
	RegParams.MacTxPower = MAC_DEF_TX_POWER_EU;
	RegParams.pChParams = &RegParams.cmnParams.paramsType2.chParams[0];
	RegParams.pDrParams = &RegParams.cmnParams.paramsType2.DRParams[0];
	RegParams.pSubBandParams = &RegParams.cmnParams.paramsType2.SubBands[0];
	RegParams.pOtherChParams = &RegParams.cmnParams.paramsType2.othChParams[0];
	RegParams.pDutyCycleTimer = &RegParams.cmnParams.paramsType2.DutyCycleTimer;
	RegParams.MinNewChIndex = 3;
	RegParams.FeaturesSupport = DUTY_CYCLE_SUPPORT | PA_SUPPORT;
	RegParams.minDataRate = MAC_DATARATE_MIN_EU;
	RegParams.maxDataRate = MAC_DATARATE_MAX_EU;
	RegParams.Rx1DrOffset = 5;
	RegParams.maxTxPwrIndx = 7;
	RegParams.cmnParams.paramsType2.minNonDefChId = 3;
	RegParams.maxTxPwr = 16;
	RegParams.pDutyCycleTimer->timerId = regTimerId;
	
	RegParams.band = ismBand;
	
	if(ismBand == ISM_EU868)
	{
		InitDefault868Channels ();
		RegParams.DefRx1DataRate = MAC_868_RX1_WINDOW_DATARATE;
		RegParams.DefRx2DataRate = MAC_868_RX2_WINDOW_DATARATE;
		RegParams.DefRx2Freq = MAC_868_RX2_WINDOW_FREQ;

#if (ENABLE_PDS == 1)

		/*Fill PDS item id in RegParam Structure */
		RegParams.regParamItems.fileid = PDS_FILE_REG_EU868_04_IDX;
		RegParams.regParamItems.alt_ch_item_id = 0;
		RegParams.regParamItems.ch_param_1_item_id = PDS_REG_EU868_CH_PARAM_1;
		RegParams.regParamItems.ch_param_2_item_id = PDS_REG_EU868_CH_PARAM_2;
		RegParams.regParamItems.sb_dc_prescalr_item_id = PDS_REG_EU868_SB_DUTY_PRESCLAR;
		RegParams.regParamItems.band_item_id = 0;

		PdsFileMarks_t filemarks_fid1;
		/* File ID EU868 - Register */
		filemarks_fid1.fileMarkListAddr = aRegEu868Fid1PdsOps;
		filemarks_fid1.numItems =  (uint8_t)(PDS_REG_EU868_FID1_MAX_VALUE & 0x00FF);
		filemarks_fid1.itemListAddr = (ItemMap_t *)&pds_reg_eu868_fid1_item_list;
		filemarks_fid1.fIDcb = LorawanReg_EU868_Pds_Cb;
		PDS_RegFile(PDS_FILE_REG_EU868_04_IDX,filemarks_fid1);
		
		PdsFileMarks_t filemarks_fid2;
		/* File ID EU868 - Register */
		filemarks_fid2.fileMarkListAddr = aRegEu868Fid2PdsOps;
		filemarks_fid2.numItems =  (uint8_t)(PDS_REG_EU868_FID2_MAX_VALUE & 0x00FF);
		filemarks_fid2.itemListAddr = (ItemMap_t *)&pds_reg_eu868_fid2_item_list;
		filemarks_fid2.fIDcb = LorawanReg_EU868_Pds_Cb;
		PDS_RegFile(PDS_FILE_REG_EU868_12_IDX,filemarks_fid2);
		
#endif		
	}
	else if(ismBand == ISM_EU433)
	{
		InitDefault433Channels ();
		RegParams.DefRx1DataRate = MAC_433_RX1_WINDOW_DATARATE;
		RegParams.DefRx2DataRate = MAC_433_RX2_WINDOW_DATARATE;
		RegParams.DefRx2Freq = MAC_433_RX2_WINDOW_FREQ;

	}
	else
	{
		return UNSUPPORTED_BAND;
	}
	memcpy (RegParams.pDrParams, DefaultDrparamsEU, sizeof(DefaultDrparamsEU) );

    LORAREG_InitGetAttrFnPtrsEU();
	LORAREG_InitValidateAttrFnPtrsEU();
	LORAREG_InitSetAttrFnPtrsEU();
#else
    status = UNSUPPORTED_BAND;
#endif
	return status;
}


/*
 * \brief This function initializes all the EU868 Channels to default values
 */
#if (EU_BAND == 1)
static void InitDefault868Channels (void)
{
    uint8_t i;

    memset (RegParams.pChParams, 0, sizeof(DefaultChannels868) );
    memcpy (RegParams.pChParams, DefaultChannels868, sizeof(DefaultChannels868) );
    memset (RegParams.pOtherChParams, 0, sizeof(AdvChannels868) );
    memcpy (RegParams.pOtherChParams, AdvChannels868, sizeof(AdvChannels868) );	
	memset (RegParams.pSubBandParams, 0, sizeof(SubBandParams868) );
	memcpy (RegParams.pSubBandParams, SubBandParams868, sizeof(SubBandParams868) );
	memset(RegParams.cmnParams.paramsType2.subBandDutyCycle,0,sizeof(SubBandDutyCycle868));
	memcpy(RegParams.cmnParams.paramsType2.subBandDutyCycle,SubBandDutyCycle868,sizeof(SubBandDutyCycle868));
    for (i = 3; i < RegParams.maxChannels; i++)
    {
        // for undefined channels the duty cycle should be a very big value, and the data range a not-valid value
        //duty cycle 0 means no duty cycle limitation, the bigger the duty cycle value, the greater the limitation
        RegParams.pChParams[i].dataRange.value = UINT8_MAX;		
    }
}

/*
 * \brief This function initializes all the EU433 Channels to default values
 */
static void InitDefault433Channels (void)
{
    uint8_t i;

    memset (RegParams.pChParams, 0, sizeof(DefaultChannels433) );
    memcpy (RegParams.pChParams, DefaultChannels433, sizeof(DefaultChannels433) );
    memset (RegParams.pOtherChParams, 0, sizeof(AdvChannels433) );
    memcpy (RegParams.pOtherChParams, AdvChannels433, sizeof(AdvChannels433) );
	memset (RegParams.pSubBandParams, 0, sizeof(SubBandParams433) );
	memcpy (RegParams.pSubBandParams, SubBandParams433, sizeof(SubBandParams433) );	
	
    for (i = 3; i < RegParams.maxChannels; i++)
    {
        // for undefined channels the duty cycle should be a very big value, and the data range a not-valid value
        //duty cycle 0 means no duty cycle limitation, the bigger the duty cycle value, the greater the limitation
        RegParams.pChParams[i].dataRange.value = UINT8_MAX;
    }
}

#if (ENABLE_PDS == 1)
void LorawanReg_EU868_Pds_Cb(void)
{
	
}
#endif
#endif
