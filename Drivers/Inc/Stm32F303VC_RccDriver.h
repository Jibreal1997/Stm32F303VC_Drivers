/*
 * Stm32F303VC_RccDriver.h
 *
 *  Created on: Sep 26, 2021
 *      Author: 15f14
 */

#ifndef INC_STM32F303VC_RCCDRIVER_H_
#define INC_STM32F303VC_RCCDRIVER_H_

#include "Stm32F303VC.h"

//This returns the APB1 clock value
uint32_t RCC_GetPCLK1Value(void);

//This returns the APB2 clock value
uint32_t RCC_GetPCLK2Value(void);


uint32_t  RCC_GetPLLOutputClock(void);


#endif /* INC_STM32F303VC_RCCDRIVER_H_ */
