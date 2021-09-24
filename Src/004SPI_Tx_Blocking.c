/*
 *  File: 004SPI_Tx_Blocking.c
 *  Created on: Sep 17, 2021
 *  Author: Jibreal Khan
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */

//Pin Out
/*
 * PA4 ---> SPI_NSS  (AF5)
 * PA5 ---> SPI_SCK	 (AF5)
 * PA6 ---> SPI_MISO (AF5)
 * PA7 ---> SPI_MOSI (AF5)
 */
#include"Stm32F303VC.h"
#include<string.h>

// GPIO configurations for SPI
void SPI1_GPIOInits(void)
{
	GPIO_Handle_t SPI_Pins;

	SPI_Pins.pGPIOx = GPIOA;
	SPI_Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPI_Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPI_Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPI_Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NO;
	SPI_Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_HIGH;

	//SCLK
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GPIO_Init(&SPI_Pins);

	//MOSI
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&SPI_Pins);

	//MISO
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&SPI_Pins);

	//NSS
	SPI_Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_4;
	GPIO_Init(&SPI_Pins);
}

// Configuring SPI settings
void SPI1_Inits(void)
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI1;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICEMODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8Bits;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;

	SPI_Init(&SPI2Handle);

}



int main(void)
{
	char user_data[] = "Hello World";
	SPI1_GPIOInits();		// GPIO configurations for SPI

	SPI1_Inits();			// Configuring SPI settings

	SPI_SSIConfig(SPI1, ENABLE);

	SPI_PeripheralControl(SPI1, ENABLE);  // Enable the SPI peripheral

	SPI_SendData(SPI1, (uint8_t *)user_data, strlen(user_data));

	while(1);

	return 0;
}

