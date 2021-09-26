/*
 * Stm32F303VC_UartDriver.h
 *
 *  Created on: Sep 26, 2021
 *      Author: 15f14
 */

#ifndef INC_STM32F303VC_USARTDRIVER_H_
#define INC_STM32F303VC_USARTDRIVER_H_

#include <stdio.h>
#include <stdint.h>
#include "Stm32F303VC.h"

/*
 * Configuration and Handling Structures
 */

typedef struct
{
	uint8_t USART_Mode;
	uint32_t USART_Baud;
	uint8_t USART_NoOfStopBits;
	uint8_t USART_WordLength;
	uint8_t USART_ParityControl;
	uint8_t USART_HWFlowControl;
}USART_Config_t;

typedef struct
{
	USART_RegDef_t *pUSARTx;
	USART_Config_t USART_Config;
	uint8_t 	  	*pTxBuffer;		// To store the app. Tx buffer address
	uint8_t 	  	*pRxBuffer;		// To store the app. Rx buffer address
	uint32_t 		TxLen;			// To store the Tx Len
	uint32_t 		RxLen;			// To store the Rx Len
	uint8_t 		TxState;		// To store the Tx state
	uint8_t 		RxState;		// To store the RX state
	uint8_t 		TxBusyState;	//
	uint8_t 		RxBusyState;		//
}USART_Handle_t;

//******************************************************* Possible Modes ************************************
/*
 *@USART_Mode
 *Possible options for USART_Mode
 */
#define USART_MODE_ONLY_TX 0
#define USART_MODE_ONLY_RX 1
#define USART_MODE_TXRX  2

/*
 *@USART_Baud
 *Possible options for USART_Baud
 */
#define USART_STD_BAUD_1200					1200
#define USART_STD_BAUD_2400					400
#define USART_STD_BAUD_9600					9600
#define USART_STD_BAUD_19200 				19200
#define USART_STD_BAUD_38400 				38400
#define USART_STD_BAUD_57600 				57600
#define USART_STD_BAUD_115200 				115200
#define USART_STD_BAUD_230400 				230400
#define USART_STD_BAUD_460800 				460800
#define USART_STD_BAUD_921600 				921600
#define USART_STD_BAUD_2M 					2000000
#define SUART_STD_BAUD_3M 					3000000


/*
 *@USART_ParityControl
 *Possible options for USART_ParityControl
 */
#define USART_PARITY_EN_ODD   2
#define USART_PARITY_EN_EVEN  1
#define USART_PARITY_DISABLE   0

/*
 *@USART_WordLength
 *Possible options for USART_WordLength
 */
#define USART_WORDLEN_8BITS  0
#define USART_WORDLEN_9BITS  1

/*
 *@USART_NoOfStopBits
 *Possible options for USART_NoOfStopBits
 */
#define USART_STOPBITS_1     0
#define USART_STOPBITS_0_5   1
#define USART_STOPBITS_2     2
#define USART_STOPBITS_1_5   3

/*
 *@USART_HWFlowControl
 *Possible options for USART_HWFlowControl
 */
#define USART_HW_FLOW_CTRL_NONE    	0
#define USART_HW_FLOW_CTRL_CTS    	1
#define USART_HW_FLOW_CTRL_RTS    	2
#define USART_HW_FLOW_CTRL_CTS_RTS	3


// USART_SR Flags
#define USART_FLAG_RXNE		5
#define USART_FLAG_TXE		7
#define USART_FLAG_TC		6


//USART BIT DEFINITIONS
#define USART_ISR_PE		0
#define USART_ISR_FE		1
#define USART_ISR_NF		2
#define USART_ISR_ORE		3
#define USART_ISR_IDLE		4
#define USART_ISR_RXNE		5
#define USART_ISR_TC		6
#define USART_ISR_TXE		7
#define USART_ISR_LBDF		8
#define USART_ISR_CTSIF		9
#define USART_ISR_CTS		10
#define USART_ISR_RTOF		11
#define USART_ISR_EOBF		12
#define USART_ISR_ABRIE		14
#define USART_ISR_ABRF		15
#define USART_ISR_BUSY		16
#define USART_ISR_CMF		17
#define USART_ISR_SBKF		18
#define USART_ISR_RWU		19
#define USART_ISR_WUF		20
#define USART_ISR_TEACK		21
#define USART_ISR_REACK		22

#define USART_CR1_UE		0
#define USART_CR1_UESM		1
#define USART_CR1_RE		2
#define USART_CR1_TE		3
#define USART_CR1_IDLEIE	4
#define USART_CR1_RXNEIE	5
#define USART_CR1_TCIE		6
#define USART_CR1_TXEIE		7
#define USART_CR1_PEIE		8
#define USART_CR1_PS		9
#define USART_CR1_PCS		10
#define USART_CR1_WAKE		11
#define USART_CR1_M0		12
#define USART_CR1_MME		13
#define USART_CR1_CMIE		14
#define USART_CR1_OVER8		15
#define USART_CR1_DEDT		16
#define USART_CR1_DEAT		21
#define USART_CR1_RTOIE		26
#define USART_CR1_EOBIE		27
#define USART_CR1_M1		28

// SOME OTHER STUFF
/*
 * Application states
 */
#define USART_BUSY_IN_RX 1
#define USART_BUSY_IN_TX 2
#define USART_READY 0


#define USART_EVENT_TX_CMPLT   0
#define	USART_EVENT_RX_CMPLT   1
#define	USART_EVENT_IDLE      2
#define	USART_EVENT_CTS       3
#define	USART_EVENT_PE        4
#define	USART_ERR_FE     	5
#define	USART_ERR_NE    	 6
#define	USART_ERR_ORE    	7


/******************************************************************************************
 *								APIs supported by this driver
 *		 For more information about the APIs check the function definitions
 ******************************************************************************************/
/*
 * Peripheral Clock setup
 */
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi);

/*
 * Init and De-init
 */
void USART_Init(USART_Handle_t *pUSARTHandle);
void USART_DeInit(USART_RegDef_t *pUSARTx);


/*
 * Data Send and Receive
 */
void USART_SendData(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len);
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len);

/*
 * IRQ Configuration and ISR handling
 */
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void USART_IRQHandling(USART_Handle_t *pHandle);

/*
 * Other Peripheral Control APIs
 */
void USART_PeripheralControl(USART_RegDef_t *pUSARTx, uint8_t EnOrDi);
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx , uint32_t FlagName);
void USART_ClearFlag(USART_RegDef_t *pUSARTx, uint16_t StatusFlagName);
void USART_SetBaudRate(USART_RegDef_t *pUSARTx, uint32_t BaudRate);

/*
 * Application callback
 */
void USART_ApplicationEventCallback(USART_Handle_t *pUSARTHandle,uint8_t AppEv);







#endif /* INC_STM32F303VC_USARTDRIVER_H_ */
