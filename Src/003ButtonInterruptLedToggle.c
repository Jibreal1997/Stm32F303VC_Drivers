/*
 * 	File: 003ButtonInterruptLedToggle.c
 *  Created on: Sep 12, 2021
 *  Author: Jibreal Khan
 *  Description: This programs Toggle the internal Led each time
 *  the inbuilt push button is pressed. This is to checkout the
 *  functionality of the Interrupt modes of the GPIO drivers.
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
	GpioButton.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GpioButton.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;
	GpioButton.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioButton.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;

	// Enabling Clock for GPIOA and Initializing
	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GpioButton);

	// Interrupt Part
	GPIO_IRQITConfig(IRQ_NO_EXTI0, ENABLE);		// Enabling Interrupt NVIC side
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, 15);	// Setting the priority of the Interrupt

	while(1);	// Pausing the program until interrupt takes place

	return 0;
}

// Interrupt Service Routine
void EXTI0_IRQHandler(void)
{
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	GPIO_ToggleOutputPin(GPIOE, GPIO_PIN_NO_8);
}



