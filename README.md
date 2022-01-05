# Drivers for Stm32F303VC Microcontroller

This repository consists of the following drivers:
* GPIO Driver
* SPI communication Driver
* USART Driver
* RCC Driver
* MCU Specific Header File (Stm32F303VC)

The drivers mentioned above can be found in the **Drivers** folder of the repository. Each of the drivers comes with a header ( .h) and source file ( .s) , which can be found in the Inc and Src folders respectively.

Note: The drivers can be used for blocking and interrupt based protocols.


## Example Programs

The drivers can be tested by the sample program provided in the Src folder of the repository. The folder contains the following programs:
* 001LedToggle.c - This program uses the GPIO driver to toggle a led.

* 002PushButtonLedToggle.c - This program uses the GPIO driver to toggle a led with the push of button.

* 003ButtonInterruptLedToggle.c - This program uses an interrupt to toggle the led.

* 004SPI_Tx_Blocking.c - This program transmits data to an Arduino using the SPI Driver in blocking mode.

* 005SPI_Tx_Arduino.c - This program transmits data to an Arduino using the SPI driver in interrupt mode.

* 006SPI_SendCommand.c - This program sends a command to the Arduino to make it perform certain operations.

* 007USART_Tx_Arduino.c - This program transmits data to an Arduino using the USART Driver in blocking mode.

* 008USART_Interrupt.c - This program transmits data to an Arduino using the USART Driver in interrupt mode.

Note: Add a bidirectional voltage converter between the Arduino and Stm32F303VC MCU before running the communication programs. Since they operate of different logic levels.
