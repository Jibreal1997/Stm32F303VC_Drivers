/*
 * 	File Name: Stm32F303VC.h
 *  Created on: September 5, 2021
 * 	Author: Jibreal Khan
 *  Description: This is the MCU specific driver file. Primarily contains Macros of various addresses , register definitions , clock macros , reset macros, etc.
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */

#ifndef INC_STM32F303VC_H_
#define INC_STM32F303VC_H_

// Libraries
#include<stddef.h>
#include<stdint.h>


#define __vo								volatile
#define __weak								__attribute__((weak))		// For weak implementation of functions

/*
 * ********************************************************************************* M4 PROCESSOR SPECIFIC DETAILS ************************************************************************************************
 */

// For NVIC_ISER (Enabling Interrupts)
#define NVIC_ISER0							((__vo uint32_t *)0xE000E100)
#define NVIC_ISER1							((__vo uint32_t *)0xE000E104)
#define NVIC_ISER2							((__vo uint32_t *)0xE000E108)
#define NVIC_ISER3							((__vo uint32_t *)0xE000E10C)
//#define NVIC_ISER4							((__vo uint32_t *)0xE000E100)
//#define NVIC_ISER5							((__vo uint32_t *)0xE000E100)
//#define NVIC_ISER6							((__vo uint32_t *)0xE000E100)
#define NVIC_ISER7							((__vo uint32_t *)0xE000E11C)

// For NVIC_ICER (Disabling Interrupts)
#define NVIC_ICER0							((__vo uint32_t *)0xE000E180)
#define NVIC_ICER1							((__vo uint32_t *)0xE000E184)
#define NVIC_ICER2							((__vo uint32_t *)0xE000E188)
#define NVIC_ICER3							((__vo uint32_t *)0xE000E18C)
//#define NVIC_ICER4							((__vo uint32_t *)0xE000E180)
//#define NVIC_ICER5							((__vo uint32_t *)0xE000E180)
//#define NVIC_ICER6							((__vo uint32_t *)0xE000E180)
#define NVIC_ICER7							((__vo uint32_t *)0xE000E19C)

// For NVIC_IPR (Setting Priority)
#define NVIC_IPR_BASEADDR					((__vo uint32_t *)0xE000E400)


/************************************************************************************ MACROS FOR VARIOUS ADDRESSES ****************************************************************************************/

// MEMORY
#define FLASH_BASEADDR						0x08000000U			// Program memory base address
#define SRAM_BASEADDR						0x20000000U			// Data memory base address
#define SYSTEM_MEMORY						0x1FFFD800U			// ROM memory base address
#define	CCM_RAM								0x10000000U

// BUS
#define PERIPH_BASEADDR						0x40000000U			// Starting address of all the peripherals
#define APB1_PERI_BASEADDR					0x40000000U
#define APB2_PERI_BASEADDR					0x40010000U
#define AHB1_PERI_BASEADDR					0x40020000U
#define AHB2_PERI_BASEADDR					0x48000000U
#define AHB3_PERI_BASEADDR					0x50000000U

// GPIO Peripherals
#define GPIOA_BASEADDR						0x48000000U
#define GPIOB_BASEADDR						0x48000400U
#define GPIOC_BASEADDR						0x48000800U
#define GPIOD_BASEADDR						0x48000C00U
#define GPIOE_BASEADDR						0x48001000U
#define GPIOF_BASEADDR						0x48001400U

// SPI Peripherals
#define SPI1_BASEADDR						0x40013000U
#define SPI2_BASEADDR						0x40003800U
#define SPI3_BASEADDR						0x40003C00U

// I2C Peripherals
#define I2C1_BASEADDR						0x40005400U
#define I2C2_BASEADDR						0x40005800U

// UART & USART Peripherals
#define USART1_BASEADDR						0x40013800U
#define USART2_BASEADDR						0x40004400U
#define USART3_BASEADDR						0x40004800U
#define UART4_BASEADDR						0x40004C00U
#define UART5_BASEADDR						0x40005000U

//Miscellaneous Peripherals
#define EXTI_BASEADDR						0x40010400U			// Used in interrupt configuration
#define SYSCFG_BASEADDR						0x40010000U			// Used in interrupt configuration (selecting the port for the EXTI line)
#define RCC_BASEADDR						0x40021000U			// Used in clocks and Reset



/************************************************************************************ REGISTER DEFINITIONS **********************************************************************************************/


// GPIO register definition
typedef struct
{
	__vo uint32_t MODER;					// GPIO port mode register
	__vo uint32_t OTYPER;					// GPIO port output type register
	__vo uint32_t OSPEEDR;					// GPIO port output speed register
	__vo uint32_t PUPDR;					// GPIO port pull-up/pull-down register
	__vo uint32_t IDR;						// GPIO port input data register
	__vo uint32_t ODR;						// GPIO port output data register
	__vo uint32_t BSRR;						// GPIO port bit set/reset register
	__vo uint32_t LCKR;						// GPIO port configuration lock register
	__vo uint32_t AFR[2];					// GPIO alternate function register
	__vo uint32_t BRR;						// GPIO port bit reset register

}GPIO_RegDef_t;

// GPIO Structure Access definitions
#define GPIOA 								((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB 								((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC 								((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD 								((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE 								((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOF 								((GPIO_RegDef_t *)GPIOF_BASEADDR)



// RCC Register Definition
typedef struct
{
	__vo uint32_t CR;							//Clock control register
	__vo uint32_t CFGR;							//Clock configuration register
	__vo uint32_t CIR;							//Clock interrupt register
	__vo uint32_t APB2RSTR;						//APB2 peripheral reset register
	__vo uint32_t APB1RSTR;						//APB1 peripheral reset register
	__vo uint32_t AHBENR;						//AHB peripheral clock enable register
	__vo uint32_t APB2ENR;						//APB2 peripheral clock enable register
	__vo uint32_t APB1ENR;						//APB1 peripheral clock enable register
	__vo uint32_t BDCR;							//RTC domain control register
	__vo uint32_t CSR;							//Control/status register
	__vo uint32_t AHBRSTR;						//AHB peripheral reset register
	__vo uint32_t CFGR2;						//Clock configuration register 2
	__vo uint32_t CFGR3;						//Clock configuration register 3
}RCC_RegDef_t;

// RCC Structure Access definitions
#define RCC 									((RCC_RegDef_t *)RCC_BASEADDR)


// EXTI register Definition
typedef struct
{
	__vo uint32_t IMR1; 								//Interrupt mask register
	__vo uint32_t EMR1; 								//Event mask register
	__vo uint32_t RTSR1; 								//Rising trigger selection register
	__vo uint32_t FTSR1; 								//Falling trigger selection register
	__vo uint32_t SWIER1; 								//Software interrupt event register
	__vo uint32_t PR1; 									//Pending register
	__vo uint32_t IMR2; 								//Interrupt mask register
	__vo uint32_t EMR2; 								//Event mask register
	__vo uint32_t RTSR2; 								//Rising trigger selection register
	__vo uint32_t FTSR2; 								//Falling trigger selection register
	__vo uint32_t SWIER2; 								//Software interrupt event register
	__vo uint32_t PR2; 									//Pending register
}EXTI_RegDef_t;

// EXTI Structure Access definitions
#define EXTI 									((EXTI_RegDef_t *)EXTI_BASEADDR)


// SYSCFG register Definition
typedef struct
{
	__vo uint32_t CFGR1; 								//SYSCFG configuration register 1
	__vo uint32_t RCR; 									//SYSCFG CCM SRAM
	__vo uint32_t EXTICR[4]; 							//SYSCFG external interrupt configuration register
	__vo uint32_t CFGR[2]; 								//SYSCFG configuration register
}SYSCFG_RegDef_t;

// SYSCFG Structure Access definitions
#define SYSCFG									((SYSCFG_RegDef_t *)SYSCFG_BASEADDR)


//SPI register Definition
typedef struct
{
	__vo uint32_t CR1;									//Control register 1
	__vo uint32_t CR2;									//Control register 2
	__vo uint32_t SR;									//Status register
	__vo uint32_t DR;									//Data register
	__vo uint32_t CRCPR;								//CRC polynomial register
	__vo uint32_t RXCRCR;								//Rx CRC register
	__vo uint32_t TXCRCR;								//Tx CRC register
	__vo uint32_t I2SCFGR;								//Configuration register
	__vo uint32_t I2SPR;								//Prescaler register

}SPI_RegDef_t;

// SPI Structure Access definitions
#define SPI1 										((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2 										((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3 										((SPI_RegDef_t *)SPI3_BASEADDR)



/********************************************************************************CLOCK ENABLE/DISABLE MACROS****************************************************************************************************/

//*****************For Enabling

// For GPIO peripherals
#define GPIOA_PCLK_EN()				(RCC->AHBENR |= (1 << 17))
#define GPIOB_PCLK_EN()				(RCC->AHBENR |= (1 << 18))
#define GPIOC_PCLK_EN()				(RCC->AHBENR |= (1 << 19))
#define GPIOD_PCLK_EN()				(RCC->AHBENR |= (1 << 20))
#define GPIOE_PCLK_EN()				(RCC->AHBENR |= (1 << 21))
#define GPIOF_PCLK_EN()				(RCC->AHBENR |= (1 << 22))


// For I2C peripherals
#define I2C1_PCLK_EN()				(RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()				(RCC->APB1ENR |= (1 << 22))


// For SPI peripherals
#define SPI1_PCLK_EN()				(RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()				(RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()				(RCC->APB1ENR |= (1 << 15))


// For USART and UART
#define USART1_PCLK_EN()			(RCC->APB2ENR |= (1 << 14))
#define USART2_PCLK_EN()			(RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()			(RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()				(RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()				(RCC->APB1ENR |= (1 << 20))


//For SYSCFG registers
#define SYSCFG_PCLK_EN()			(RCC->APB2ENR |= (1 << 0))


//******************For Disabling

// For GPIO peripherals
#define GPIOA_PCLK_DI()				(RCC->AHBENR &= ~(1 << 17))
#define GPIOB_PCLK_DI()				(RCC->AHBENR &= ~(1 << 18))
#define GPIOC_PCLK_DI()				(RCC->AHBENR &= ~(1 << 19))
#define GPIOD_PCLK_DI()				(RCC->AHBENR &= ~(1 << 20))
#define GPIOE_PCLK_DI()				(RCC->AHBENR &= ~(1 << 21))
#define GPIOF_PCLK_DI()				(RCC->AHBENR &= ~(1 << 22))


// For I2C peripherals
#define I2C1_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 22))


// For SPI peripherals
#define SPI1_PCLK_DI()				(RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 15))


// For USART and UART
#define USART1_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 14))
#define USART2_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()			(RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()				(RCC->APB1ENR &= ~(1 << 20))


//For SYSCFG registers
#define SYSCFG_PCLK_DI()			(RCC->APB2ENR &= ~(1 << 0))


/************************************************************************** GPIO RESET and OTHER MACROS *******************************************************************************************/

// GPIO Reset
#define GPIOA_REG_RESET()			do{RCC->AHBRSTR |= (1 << 17); RCC->AHBRSTR &= ~(1 << 17);} while(0)
#define GPIOB_REG_RESET()			do{RCC->AHBRSTR |= (1 << 18); RCC->AHBRSTR &= ~(1 << 18);} while(0)
#define GPIOC_REG_RESET()			do{RCC->AHBRSTR |= (1 << 19); RCC->AHBRSTR &= ~(1 << 19);} while(0)
#define GPIOD_REG_RESET()			do{RCC->AHBRSTR |= (1 << 20); RCC->AHBRSTR &= ~(1 << 20);} while(0)
#define GPIOE_REG_RESET()			do{RCC->AHBRSTR |= (1 << 21); RCC->AHBRSTR &= ~(1 << 21);} while(0)
#define GPIOF_REG_RESET()			do{RCC->AHBRSTR |= (1 << 22); RCC->AHBRSTR &= ~(1 << 22);} while(0)

// SPI Reset
#define SPI1_REG_RESET()			do{RCC->APB2RSTR |= (1 << 12); RCC->APB2RSTR &= ~(1 << 12);} while(0)
#define SPI2_REG_RESET()			do{RCC->APB1RSTR |= (1 << 14); RCC->APB1RSTR &= ~(1 << 14);} while(0)
#define SPI3_REG_RESET()			do{RCC->APB1RSTR |= (1 << 15); RCC->APB1RSTR &= ~(1 << 15);} while(0)


// Base address to port code
#define GPIO_BASEADDR_TO_PORTCODE(x)	((x == GPIOA) ? 0 :\
									(x == GPIOB) ? 1 :\
									(x == GPIOC) ? 2 :\
									(x == GPIOD) ? 3 :\
									(x == GPIOE) ? 4 :\
									(x == GPIOF) ? 5 : 0)


/******************************************************************************IRQ Numbers**********************************************************************************************/
//GPIO
#define IRQ_NO_EXTI0				6
#define IRQ_NO_EXTI1				7
#define IRQ_NO_EXTI2				8
#define IRQ_NO_EXTI3				9
#define IRQ_NO_EXTI4				10
#define IRQ_NO_EXTI9_5				23
#define IRQ_NO_EXTI15_10			40

// SPI
#define IRQ_NO_SPI1					35
#define IRQ_NO_SPI2					36
#define IRQ_NO_SPI3					51



/********************************************************************************MISCELLANEOUS MACROS*********************************************************************************************/
#define ENABLE						1
#define DISABLE						0
#define SET							ENABLE
#define RESET						DISABLE
#define GPIO_PIN_SET				SET
#define GPIO_PIN_RESET				RESET
#define FLAG_SET					SET
#define FLAG_RESET					RESET


/**********************************************************************************REGISTER BIT DEFINITIONS*******************************************************/
// Bit position definitions SPI_CR1
#define SPI_CR1_CPHA					0
#define SPI_CR1_CPOL					1
#define SPI_CR1_MSTR					2
#define SPI_CR1_BR						3
#define SPI_CR1_SPE						6
#define SPI_CR1_LSB_FIRST				7
#define SPI_CR1_SSI						8
#define SPI_CR1_SSM						9
#define SPI_CR1_RX_ONLY					10
#define SPI_CR1_CRCL					11
#define SPI_CR1_CRC_NEXT				12
#define SPI_CR1_CRC_EN					13
#define SPI_CR1_BIDI_OE					14
#define SPI_CR1_BIDI_MODE				15

// Bit position definitions SPI_CR2
#define SPI_CR2_RXDMAEN		 			0
#define SPI_CR2_TXDMAEN				 	1
#define SPI_CR2_SSOE				 	2
#define SPI_CR2_FRF						4
#define SPI_CR2_ERRIE					5
#define SPI_CR2_RXNEIE				 	6
#define SPI_CR2_TXEIE					7
#define SPI_CR2_DS						8

// Bit position definitions SPI_SR
#define SPI_SR_RXNE						0
#define SPI_SR_TXE				 		1
#define SPI_SR_CHSIDE				 	2
#define SPI_SR_UDR					 	3
#define SPI_SR_CRCERR				 	4
#define SPI_SR_MODF					 	5
#define SPI_SR_OVR					 	6
#define SPI_SR_BSY					 	7
#define SPI_SR_FRE					 	8


// Other Driver header files
#include "Stm32F303VC_GpioDriver.h"
#include "Stm32F303VC_SpiDriver.h"


#endif /* INC_STM32F303VC_H_ */
