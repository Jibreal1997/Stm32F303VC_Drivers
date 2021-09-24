/*
 * 	File: 002PushButtonLedToggle.c
 *  Created on: Sep 12, 2021
 *  Author: Jibreal Khan
 *  Description: This programs Toggle the internal Led each time
 *  the inbuilt push button is pressed. This is to checkout the
 *  functionality of the GPIO drivers.
 */

#include <stdint.h>
#include "Stm32F303VC.h"

// Software Delay
void delay(void)
{
	for(uint32_t i = 0; i < 500000; i++);
}


int main(void)
{
	// GPIO LED pin configurations
	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOE;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_8;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;

	// Enabling Clock for GPIOE and Initializing
	GPIO_PeriClockControl(GPIOE, ENABLE);
	GPIO_Init(&GpioLed);

	//GPIO Button Pin configurations
	GPIO_Handle_t GpioButton;

	GpioButton.pGPIOx = GPIOA;
	GpioButton.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioButton.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;

	// Enabling Clock for GPIOA and Initializing
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GpioButton);


	// Toggling loop
	while(1)
	{
		// Checking if the button has been pressed
		if(GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == 1)
		{
			delay();	// To prevent button debouncing
			GPIO_ToggleOutputPin(GPIOE, GPIO_PIN_NO_8);

		}

	}
	return 0;
}


