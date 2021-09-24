/*
 *  File: 004SPI_Tx_Arduino.c
 *  Created on: Sep 17, 2021
 *  Author: Jibreal Khan
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */

//Pinout
/*
 *PB12----------> SPI2 NSS
 *PB14----------> SPI2 MISO
 *PB13----------> SPI2 SCK
 *PB15----------> SPI2 MOSI
 * */

//Libraries
#include<stdio.h>
#include "Stm32F303VC.h"
#include<string.h>


void delay(void)  													// Creating software delay
{

 for(uint32_t i = 0; i < 500000; i++);

}

void SPI2_GPIOInits(void)
{
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	//For SCLK pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

	//For MOSI pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

//	//For MISO pin
//	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
//	GPIO_Init(&SPIPins);

	//For NSS pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

}

void SPI2_Inits(void)
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICEMODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8Bits;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;		// SSM disabled for hardware slave management

	SPI_Init(&SPI2Handle);


}

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GpioBtn;

	// For Button
	GpioBtn.pGPIOx = GPIOA;											//Address of the GPIO port
	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;			// Filling in the structure members
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;

	// Initializing the GPIO pins
	GPIO_Init(&GpioBtn);
}


int main(void)
{
	char user_data[] = "Hello World you are wonderful and full of";

	GPIO_ButtonInit();
	SPI2_GPIOInits();
	SPI2_Inits();

	//Enable the SSI, this makes NSS internally high to avoid MODF error
	SPI_SSIConfig(SPI2, ENABLE);

	while(1)
	{
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));	// Runs the next line only when the button is pressed , else it pauses.

		delay();

		// Enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		// Before sending the actual data , you gotta send the length of the data to the slave.
		uint8_t dataLen = strlen(user_data);
		SPI_SendData(SPI2, &dataLen,1);

		// Sending the actual data
		SPI_SendData(SPI2, (uint8_t *)user_data, strlen(user_data));

		//Check if SPI is busy or not
		while( SPI_GetFlagStatus(SPI2, SPI_FLAG_BUSY));

		// Disable the peripheral after the last byte has been sent
		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;

}
