/*
 *  File: Stm32F303VC_SpiDriver.h
 *  Created on: Sep 14, 2021
 *  Author: Jibreal Khan
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */

#ifndef INC_STM32F303VC_SPIDRIVER_H_
#define INC_STM32F303VC_SPIDRIVER_H_

#include "Stm32F303VC.h"


/*
 ******************************************************* Configuration and Handling structure ******************************************************/

// SPI Configuration structure
typedef struct
{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;

}SPI_Config_t;

// SPI Handle Structure
typedef struct
{
	SPI_RegDef_t 	*pSPIx;
	SPI_Config_t   	SPIConfig;
	uint8_t 	  	*pTxBuffer;		// To store the app. Tx buffer address
	uint8_t 	  	*pRxBuffer;		// To store the app. Rx buffer address
	uint32_t 		TxLen;			// To store the Tx Len
	uint32_t 		RxLen;			// To store the Rx Len
	uint8_t 		TxState;		// To store the Tx state
	uint8_t 		RxState;		// To store the RX state

}SPI_Handle_t;

/*
 ***************************************************** Possible Options for Configuration ***********************************************************/
//@SPI_DEVICEMODE
#define SPI_DEVICEMODE_MASTER			1
#define SPI_DEVICEMODE_SLAVE			0

//@SPI_BUS_CONFIG
#define SPI_BUS_CONFIG_FD				1
#define SPI_BUS_CONFIG_HD				2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY	3

//@SPI_SCLK_SPEED
#define SPI_SCLK_SPEED_DIV2				0
#define SPI_SCLK_SPEED_DIV4				1
#define SPI_SCLK_SPEED_DIV8				2
#define SPI_SCLK_SPEED_DIV16			3
#define SPI_SCLK_SPEED_DIV32			4
#define SPI_SCLK_SPEED_DIV64			5
#define SPI_SCLK_SPEED_DIV128			6
#define SPI_SCLK_SPEED_DIV256			7

// @SPI_DFF
//#define SPI_DFF_4Bits					3
//#define SPI_DFF_5Bits					4
//#define SPI_DFF_6Bits					5
//#define SPI_DFF_7Bits					6
#define SPI_DFF_8Bits					7
//#define SPI_DFF_9Bits					8
//#define SPI_DFF_10Bits				9
//#define SPI_DFF_11Bits				10
//#define SPI_DFF_12Bits				11
//#define SPI_DFF_13Bits				12
//#define SPI_DFF_14Bits				13
//#define SPI_DFF_15Bits				14
#define SPI_DFF_16Bits					15

// @SPI_CPOL
#define SPI_CPOL_HIGH					1
#define SPI_CPOL_LOW					0

// @SPI_CPHA
#define SPI_CPHA_HIGH					1
#define SPI_CPHA_LOW					0

// @SPI_SSM
#define SPI_SSM_EN					1
#define SPI_SSM_DI					0


/****************************************************************** Additional MACROS ************************************************************/

// Possible SPI ApplicationCallBack Events
#define SPI_EVENT_TX_CMPLT		1
#define SPI_EVENT_RX_CMPLT		2
#define SPI_EVENT_OVR_ERR		3
#define SPI_EVENT_CRC_ERR		4

// SPI Application States
#define SPI_READY				0
#define SPI_BUSY_RX				1
#define SPI_BUSY_TX				2

// SPI related Flag definitions
#define SPI_FLAG_TXE				(1 << SPI_SR_TXE)
#define SPI_FLAG_RXNE				(1 << SPI_SR_RXNE)
#define SPI_FLAG_BUSY				(1 << SPI_SR_BSY)



/***********************************************************************SPI API Prototypes***************************************************************/

// SPI Clock
void SPI_PeriClockControl(SPI_RegDef_t * pSPIx, uint8_t ENorDI);

// SPI Initialize & De Initialize
void SPI_Init(SPI_Handle_t * pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// Data Send and Receive (Blocking)
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t * pTxBuffer, uint32_t Len);
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t * pRxBuffer, uint32_t Len);

// Data Send and Receive (Interrupt)
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t * pTxBuffer, uint32_t Len);
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle, uint8_t * pRxBuffer, uint32_t Len);

// IRQ Configurations and Handling
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t ENorDI);
void SPI_IRQPriorityConfig(uint32_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t * pHandle);

// Other APIs
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t ENorDI);	// Enable the SPI peripheral by setting the SPE bit
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);					// Clears the Overrun Error Flag
void SPI_CloseTransmission(SPI_Handle_t * pSPIHandle);		// Closes the Transmission by clearing the TXIE bit
void SPI_CloseReception(SPI_Handle_t * pSPIHandle);			// Closes the Reception by clearing the RXNEIE bit
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName); // Get the flag status


// Application CallBack
void SPI_ApplicationEventCallBack(SPI_Handle_t * pSPIHandle, uint8_t AppEv);	// This is to be implemented on the application side.


#endif /* INC_STM32F303VC_SPIDRIVER_H_ */
