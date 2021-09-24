/*
 * 	File:Stm32F303VC_GpioDriver.c
 *  Created on: Sep 7, 2021
 *  Author: Jibreal Khan
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */

#include "Stm32F303VC_GpioDriver.h"

/*
 * @Description: Enables of Disables the Clock to a GPIO port via the RCC register
 * @Parameters: GPIO port address , Enable or Disable Value
 * @Return: NONE
 */
// Peripheral clock control
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t ENorDI)
{
	// For Enable
	if(ENorDI == ENABLE)
	{
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_EN();

		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_EN();

		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_EN();

		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_EN();

		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_EN();

		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_EN();
	}

	// For Disable
	else
	{
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_DI();

		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_DI();

		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_DI();

		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_DI();

		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_DI();

		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_DI();
	}
}

/*
 * @Description: Setting user defined configurations into the actual registers
 * @Parameters: GPIO Handle Variable (provides access to user configurations and registers)
 * @Return: NONE
 */
// Initialize
void GPIO_Init(GPIO_Handle_t * pGPIOHandle)
{
	uint32_t temp = 0;		// create a temporary variable

	// Enabling the Peripheral Clock
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);


	//1. Configure the mode of the GPIO pin

	// Checking for non-interrupt mode.
	if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		// Aligning
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);		// Clearing those bits before setting them
		pGPIOHandle->pGPIOx->MODER |= temp;		// Setting
		temp = 0;
	}

	// This is interrupt mode
	else
	{
		//1. Configure the RTSR , FTSR or both.
		if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_IT_FT)
		{
			//FTSR
			EXTI->FTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the RTSR
			EXTI->RTSR1 &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);


		}

		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_IT_RT)
		{
			//RTSR
			EXTI->RTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the FTSR
			EXTI->FTSR1 &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_IT_RFT)
		{
			//FTSR and RTSR
			EXTI->RTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->FTSR1 |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. Configure the GPIO port selection using SYSCFG_EXTICR
		uint32_t temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4);
		uint32_t temp2 = (4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4));
		uint32_t portcode = GPIO_BASEADDR_TO_PORTCODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = (portcode << temp2);


		//3. Enable the EXTI interrupt delivery using IMR
		EXTI->IMR1 |= 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;



	}

	//2. Configure the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);		// Clearing those bits before setting them
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;	//Setting
	temp = 0;

	//3. Configure the PUPD settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);		// Clearing those bits before setting them
	pGPIOHandle->pGPIOx->PUPDR |= temp;		//Setting
	temp = 0;

	//4. Configure the OP Type
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);		// Clearing those bits before setting them
	pGPIOHandle->pGPIOx->OTYPER |= temp;	//Setting
	temp = 0;

	//5. Configure the alternate function
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		//configure the alternate function register mode
		uint32_t temp1, temp2;

		temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;	// Finding the register
		temp2 = 4 * (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8);	// Finding the right bit position
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xF << temp2);		// Clearing
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << temp2);	// Setting

	}

}

/*
 * @Description: Clearing the registers of the GPIO via the RCC
 * @Parameters: GPIO port address
 * @Return: NONE
 */
// De-Initialize
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
		if(pGPIOx == GPIOA)
			GPIOA_REG_RESET();

		else if(pGPIOx == GPIOB)
			GPIOB_REG_RESET();

		else if(pGPIOx == GPIOC)
			GPIOC_REG_RESET();

		else if(pGPIOx == GPIOD)
			GPIOD_REG_RESET();

		else if(pGPIOx == GPIOE)
			GPIOE_REG_RESET();

		else if(pGPIOx == GPIOF)
			GPIOF_REG_RESET();

}

/*
 * @Description: Reading from the Register IDR
 * @Parameters: GPIO port address, Pin Number
 * @Return: Value (which is 0 or 1 )
 */
// Data Read pin
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t Value;
	Value = (uint8_t)(pGPIOx->IDR >> PinNumber) & 0x00000001;
	return Value;
}

/*
 * @Description: Reading from the Register IDR
 * @Parameters: GPIO port address
 * @Return: Value (16 bits)
 */
// Data Read port
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t Value;
	Value = (uint16_t)pGPIOx->IDR;
	return Value;

}

/*
 * @Description: Writing to the Register ODR for output of 1 pin
 * @Parameters: GPIO port address, Pin Number, Value
 * @Return: NONE
 */
// Data write pin
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	//if Value is 1 write 1
	if(Value == GPIO_PIN_SET)
	{
		//write 1 to the output data register
		pGPIOx->ODR |= (1 << PinNumber);
	}

	else
	{
		//write 0 to the output data register
		pGPIOx->ODR &= ~(1 << PinNumber);
	}

}

/*
 * @Description: Writing to the Register ODR for output of entire port
 * @Parameters: GPIO port address, Value
 * @Return: NONE
 */
// Data write port
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}


/*
 * @Description: Writing to the Register ODR to make it opposite
 * @Parameters: GPIO port address, Pin Number
 * @Return: NONE
 */
// Data write toggle
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);

}


/*
 * @Description: ENABLE or DISABLE the Interrupt from the NVIC side via ISER and ICER registers
 * @Parameters: IRQ Number, Enable or Disable Value
 * @Return: NONE
 */
// IRQ NVIC settings
void GPIO_IRQITConfig(uint8_t IRQNumber, uint8_t ENorDI)
{
	// For enabling the Interrupt
	if(ENorDI == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			// ISER0 register
			*NVIC_ISER0 |= (1 << IRQNumber);
		}

		else if(IRQNumber > 31 && IRQNumber <  64)
		{
			// ISER1 register
			*NVIC_ISER1 |= (1 << IRQNumber % 32);
		}

		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// ISER2 register
			*NVIC_ISER2 |= (1 << IRQNumber % 64);
		}
	}

	// For Disabling the interrupt
	else
	{
		if(IRQNumber <= 31)
		{
			// ICER0 register
			*NVIC_ICER0 |= (1 << IRQNumber);

		}

		else if(IRQNumber > 31 && IRQNumber <  64)
		{
			// ICER1 register
			*NVIC_ICER1 |= (1 << IRQNumber % 32);
		}

		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			// ICER2 register
			*NVIC_ICER2 |= (1 << IRQNumber % 64);

		}
	}
}

/*
 * @Description: Setting the Priority of the Interrupt from the NVIC side via IPR registers
 * @Parameters: IRQ Number, IRQPriority
 * @Return: NONE
 */
// IRQ NVIC Priority setting
void GPIO_IRQPriorityConfig(uint32_t IRQNumber,uint8_t IRQPriority)
{
	//1. First lets find the correct Register and section
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section =  (8 * IRQNumber % 4) + 4;
	*(NVIC_IPR_BASEADDR + iprx) |= IRQPriority << (iprx_section);
}

/*
 * @Description: Clears the Pending Register once the ISR has ran its course , the ISR from the application side must call this function
 * @Parameters: Pin Number
 * @Return: NONE
 *
 */
void GPIO_IRQHandling(uint8_t PinNumber)
{
	if(EXTI->PR1 & (1 << PinNumber))
	{
		// Weird clearing, actually got to write 1 to clear it.
		EXTI->PR1 |= (1 << PinNumber);
	}
}
