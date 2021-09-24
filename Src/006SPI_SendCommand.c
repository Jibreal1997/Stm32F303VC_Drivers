/*
 *  File: 006SPI_SendCommand.c
 *  Created on: Sep 19, 2021
 *  Author: Jibreal Khan
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */

/* PIN DESCRIPTION
 *PB12----------> SPI2 NSS
 *PB14----------> SPI2 MISO
 *PB13----------> SPI2 SCK
 *PB15----------> SPI2 MOSI
 */

#include<stdio.h>
#include "Stm32F303VC.h"
#include<string.h>

// Semi hosting
extern void initialise_monitor_handles(void);


// Command Codes that the Arduino Slave recognizes
#define COMMAND_LED_CTRL	0x50
#define COMMAND_SENSOR_READ	0x51
#define COMMAND_LED_READ	0x52
#define COMMAND_PRINT		0x53
#define COMMAND_ID_READ		0x54

#define LED_ON				1
#define LED_OFF				0

// Arduino analog pins
#define ANALOG_PIN0 		1
#define ANALOG_PIN1 		2
#define ANALOG_PIN2 		3
#define ANALOG_PIN3 		4
#define ANALOG_PIN4 		5
#define ANALOG_PIN5 		6

// Arguments
#define LED_PIN				9
#define LED_ON				1
#define LED_OFF				0

// Creating software delay
void delay(void)
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
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//For MOSI pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//For MISO pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	//For NSS pin
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
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

// Verifying the Response from the Arduino Slave
uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if(ackbyte == 0xF5)
	{
		//ACK
		return 1;
	}
	else
	{
		return 0;
	}

}


int main(void)
{

	uint8_t dummy_write	= 0xff;						// Dummy data to be sent
	uint8_t dummy_read;

	initialise_monitor_handles();					// Semi hosting command

	printf("Application is running \n");

	GPIO_ButtonInit();								// Initializing GPIO pins and SPI configurations
	SPI2_GPIOInits();
	SPI2_Inits();

	printf("SPI Init, done \n");

	//Enable the SSI, this makes NSS internally high to avoid MODF error
	SPI_SSIConfig(SPI2, ENABLE);


	while(1)
	{
		// Sending command one
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));	// Runs the next line only when the button is pressed , else it pauses.

		delay();

		// Enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		//1. CMD_LED_CTRL  <pin no> <value>
		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte;
		uint8_t args[2];

		//Send Command
		SPI_SendData(SPI2, &commandcode, 1);

		//do dummy read to clear RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		// Now send some dummy data to fetch response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// Read the ack byte recieved
		SPI_RecieveData(SPI2, &ackbyte, 1);

		// Now check if you received ack or nack
		if(SPI_VerifyResponse(ackbyte))
		{
			// send arguments
			args[0] = LED_PIN;
			args[1] = LED_ON;
			SPI_SendData(SPI2,args, 2);
			printf("COMMAND_LED_CTRL Executed \n");


		}// End of command 1

//-------------------------------------------------------

		// Sending command two
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));	// Runs the next line only when the button is pressed , else it pauses.

		//2. CMD_SENSOR_READ  <analog pin number(1)>
		delay();

		//command code
		commandcode = COMMAND_SENSOR_READ;

		//Send Command
		SPI_SendData(SPI2, &commandcode, 1);

		//do dummy read to clear RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		// Now send some dummy data to fetch response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// Read the ack byte recieved
		SPI_RecieveData(SPI2, &ackbyte, 1);

		// Now check if you received ack or nack
		if(SPI_VerifyResponse(ackbyte))
		{
			// send arguments
			args[0] = ANALOG_PIN0;
			SPI_SendData(SPI2,args, 1);

			//do dummy read to clear RXNE
			SPI_RecieveData(SPI2, &dummy_read, 1);

			// Insert some delay so that the slave can be ready with the data
			delay();

			// Now send some dummy data to fetch response from the slave
			SPI_SendData(SPI2, &dummy_write, 1);

			uint8_t analog_read;

			SPI_RecieveData(SPI2, &analog_read, 1);

			printf("COMMAND_SENSOR_READ %d \n", analog_read);
			// End of command 2

		}

//--------------------------------------------------------------
		// Sending command three
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));	// Runs the next line only when the button is pressed , else it pauses.

		//3. COMMAND_LED_READ  <pin number(1)>
		delay();

		//command code
		commandcode = COMMAND_LED_READ;

		//Send Command
		SPI_SendData(SPI2, &commandcode, 1);

		//do dummy read to clear RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		// Now send some dummy data to fetch response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// Read the ack byte recieved
		SPI_RecieveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))
		{
			// send arguments
			args[0] = LED_PIN;
			SPI_SendData(SPI2,args, 1);

			//do dummy read to clear RXNE
			SPI_RecieveData(SPI2, &dummy_read, 1);

			// Insert some delay so that the slave can be ready with the data
			delay();

			SPI_SendData(SPI2,&dummy_write,1);

			uint8_t led_status;
			SPI_RecieveData(SPI2,&led_status,1);
			printf("COMMAND_READ_LED %d\n",led_status);
		}
		// End of command 3
//----------------------------------------------------------------------------------------
		// Sending command four
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));	// Runs the next line only when the button is pressed , else it pauses.

		//4. COMMAND_PRINT  <len(2)> <message(len)>
		delay();

		//command code
		commandcode = COMMAND_PRINT;

		//Send Command
		SPI_SendData(SPI2, &commandcode, 1);

		//do dummy read to clear RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		// Now send some dummy data to fetch response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// Read the ack byte recieved
		SPI_RecieveData(SPI2, &ackbyte, 1);

		uint8_t message[] = "Hello ! How are you ??";

		if(SPI_VerifyResponse(ackbyte))
		{
			// send arguments
			args[0] = strlen((char *)message);
			SPI_SendData(SPI2,args, 1);	// Sending length

			//do dummy read to clear RXNE
			SPI_RecieveData(SPI2, &dummy_read, 1);

			// Insert some delay so that the slave can be ready with the data
			delay();

			// send message

			for(int i = 0; i < args[0]; i++)
			{
				SPI_SendData(SPI2,&message[i],1);
				SPI_RecieveData(SPI2,&dummy_read,1);
			}

			printf("COMMAND_PRINT Executed \n");
		}
			// End of command four
//------------------------------------------------------------------------------
		// Sending command five
		while( ! GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));	// Runs the next line only when the button is pressed , else it pauses.

		//5. COMMAND_ID_READ
		delay();

		//command code
		commandcode = COMMAND_ID_READ;

		//Send Command
		SPI_SendData(SPI2, &commandcode, 1);

		//do dummy read to clear RXNE
		SPI_RecieveData(SPI2, &dummy_read, 1);

		// Now send some dummy data to fetch response from the slave
		SPI_SendData(SPI2, &dummy_write, 1);

		// Read the ack byte recieved
		SPI_RecieveData(SPI2, &ackbyte, 1);

		uint8_t id[11];
		uint32_t i=0;

		if( SPI_VerifyResponse(ackbyte))
		{
			// read 10 bytes from the slave
			for(i = 0 ; i < 10 ; i++)
			{
				//send dummy byte to fetch data from slave
				SPI_SendData(SPI2,&dummy_write,1);
				SPI_RecieveData(SPI2,&id[i],1);
			}

			id[10] = '\0';

			printf("COMMAND_ID : %s \n",id);

		}
			// End of Command five
//--------------------------------------------------------------------------

		//Check if SPI is busy or not
		while( SPI_GetFlagStatus(SPI2, SPI_FLAG_BUSY));

		// Disable the peripheral after the last byte has been sent
		SPI_PeripheralControl(SPI2, DISABLE);

		printf("SPI Communication Closed\n");

	}

	return 0;

}


