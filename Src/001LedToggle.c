/**
 ******************************************************************************
 * @file           : 001LedToggle.c
 * @author         : Jibreal Khan
 * @brief          : Toggle inbuilt LED
 ******************************************************************************
 */

#include <stdint.h>
#include "Stm32F303VC.h"

// Software Delay
void delay(void)
{
	for(uint32_t i = 0; i < 50000; i++);
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

	// Enabling Clock and Initializing
	GPIO_PeriClockControl(GPIOE, ENABLE);
	GPIO_Init(&GpioLed);

	// Toggling loop
	while(1)
	{
		GPIO_ToggleOutputPin(GPIOE, GPIO_PIN_NO_8);
		delay();
	}
	return 0;
}
