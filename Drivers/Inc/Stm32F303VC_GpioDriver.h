/*
 *  File: Stm32F303VC_GpioDriver.h
 *  Created on: Sep 7, 2021
 *  Author: Jibreal Khan
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */

#ifndef INC_STM32F303VC_GPIODRIVER_H_
#define INC_STM32F303VC_GPIODRIVER_H_

#include "Stm32F303VC.h"

/*
 * ************************************************************ Configuration and Handle Structures *****************************************************
 * */

// GPIO pin configuration structure (User configurable data)
typedef struct
{
	uint8_t GPIO_PinNumber;					// possible values at @GPIO_Pin_NO
	uint8_t GPIO_PinMode;					// possible values at @GPIO_MODE
	uint8_t GPIO_PinSpeed;					// possible values at @GPIO_SPEED
	uint8_t GPIO_PinPuPdControl;			// possible values at @GPIO_PUPDR
	uint8_t GPIO_PinOPType;					// possible values at @GPIO_OP_TYPE
	uint8_t GPIO_PinAltFunMode;				// user has to figure it out

}GPIO_PinConfig_t;


// GPIO handle structure (Interaction with API to actually get the configurations and set the registers)
typedef struct
{
	GPIO_RegDef_t * pGPIOx;
	GPIO_PinConfig_t GPIO_PinConfig;

}GPIO_Handle_t;

/*
 * *********************************************************VARIOUS MACROS FOR USER CONFIGURABLE ITEMS*****************************************************
 */


 /* @GPIO_PIN_NO
 *********************** GPIO pin numbers ******************
 */
#define GPIO_PIN_NO_0  				0
#define GPIO_PIN_NO_1  				1
#define GPIO_PIN_NO_2  				2
#define GPIO_PIN_NO_3  				3
#define GPIO_PIN_NO_4  				4
#define GPIO_PIN_NO_5  				5
#define GPIO_PIN_NO_6  				6
#define GPIO_PIN_NO_7  				7
#define GPIO_PIN_NO_8  				8
#define GPIO_PIN_NO_9  				9
#define GPIO_PIN_NO_10  			10
#define GPIO_PIN_NO_11 				11
#define GPIO_PIN_NO_12  			12
#define GPIO_PIN_NO_13 				13
#define GPIO_PIN_NO_14 				14
#define GPIO_PIN_NO_15 				15


/*
 * @GPIO_MODE
 ************** GPIO pin possible modes *****************
 */
#define GPIO_MODE_IN		0			// Input
#define GPIO_MODE_OUT		1			// Output
#define GPIO_MODE_ALTFN 	2			// Alternate Functionality
#define GPIO_MODE_ANALOG 	3			// Analog
#define GPIO_MODE_IT_RT		4			// Rising Edge Trigger
#define GPIO_MODE_IT_FT		5			// Falling Edge Trigger
#define GPIO_MODE_IT_RFT	6			// Rising and Falling Edge Trigger

/*
 * @GPIO_OP_TYPE
 ************** GPIO pin possible output types *****************
 */
#define GPIO_OP_TYPE_PP		0			// Push Pull
#define GPIO_OP_TYPE_OD		1			// Open Drain

/*
 * @GPIO_SPEED
 ************** GPIO pin possible output speeds *****************
 */
#define GPIO_SPEED_LOW		0			// LOW
#define GPIO_SPEED_MEDIUM	1			// MEDIUM
#define GPIO_SPEED_HIGH		3			// HIGH

/*
 * @GPIO_PUPDR
 ************** GPIO pin Pull up Pull down *****************
 */
#define GPIO_PUPDR_NO		0			// No pull up and pull down
#define GPIO_PUPDR_PU		1			// Pull up
#define GPIO_PUPDR_PD		2			// Pull down

/*************************************************************************************************************************
 * 											GPIO API PROTOTYPES
 * ***********************************************************************************************************************
 */

// GPIO Peripheral clock control
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI);

// GPIO Initialize and De-initialize
void GPIO_Init(GPIO_Handle_t * pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

// GPIO Data Read
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);

// GPIO Data write
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);

// GPIO Data Toggle
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

// IRQ settings
void GPIO_IRQITConfig(uint8_t IRQNumber, uint8_t ENorDI);
void GPIO_IRQPriorityConfig(uint32_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* INC_STM32F303VC_GPIODRIVER_H_ */
