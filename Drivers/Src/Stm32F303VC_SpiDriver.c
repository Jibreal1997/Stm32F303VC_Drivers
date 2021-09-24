/*
 * 	File:Stm32F303VC_SpiDriver.c
 *  Created on: Sep 14, 2021
 *  Author: Jibreal Khan
 *  Contact: Jibrealkhan1997@gmail.com
 *  MCU: Stm32F303VC
 */
#include "Stm32F303VC_SpiDriver.h"

// Helper Function prototypes
static void spi_txe_interrupt_handle(SPI_Handle_t * pSPIHandle);			// Handler for TXE based interrupt
static void spi_rxne_interrupt_handle(SPI_Handle_t * pSPIHandle);			// Handler for RXNE based interrupt
static void spi_ovrerr_interrupt_handle(SPI_Handle_t * pSPIHandle);			// Handler for OVERR based interrupt


/***********************************************************API IMPLEMENTATIONS*******************************************************************/
/*
 * @Description: Enables of Disables the Clock to a SPI peripheral via the RCC register
 * @Parameters: SPI peripheral address , Enable or Disable Value
 * @Return: NONE
 */
// SPI Clock
void SPI_PeriClockControl(SPI_RegDef_t * pSPIx, uint8_t ENorDI)
{
	// For Enable
	if(ENorDI == ENABLE)
	{
		if(pSPIx == SPI1)
			SPI1_PCLK_EN();

		else if(pSPIx == SPI2)
			SPI2_PCLK_EN();

		else if(pSPIx == SPI3)
			SPI3_PCLK_EN();
	}

	// For Disable
	else
	{
		if(pSPIx == SPI1)
			SPI1_PCLK_DI();

		else if(pSPIx == SPI2)
			SPI2_PCLK_DI();

		else if(pSPIx == SPI3)
			SPI3_PCLK_DI();
	}
}


/*
 * @Description: Initializes the SPI CR registers with the user configurations (MODE, BUS, SCLK SPEED, DFF, CPOL, CPHA)
 * @Parameters: SPI handle pointer
 * @Return: NONE
 */
// SPI Initialize
void SPI_Init(SPI_Handle_t * pSPIHandle)
{

	//Enable the Peripheral Clock
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//First lets configure the SPI_CR1 register
	uint32_t tempreg = 0;

	//1. Configuring the mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2. Configure the Bus Mode
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		// BIDI Mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);
	}

	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		// BIDI Mode should be set
		tempreg |= (1 << SPI_CR1_BIDI_MODE);
	}

	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		// BIDI Mode should be cleared
		tempreg &= ~(1 << SPI_CR1_BIDI_MODE);

		// RXONLY bit must be set (to enable clock)
		tempreg |= (1 << SPI_CR1_RX_ONLY);
	}

	//3. Configuring the SPI Serial Clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR;

	//4. Configuring the DFF
	pSPIHandle->pSPIx->CR2 |= pSPIHandle->SPIConfig.SPI_DFF << SPI_CR2_DS;

	//5. Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL;

	//6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA;

	//7. Configure the SSM
	tempreg |= pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM;

	//8. Configure the SSOE bit
	pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_SSOE);


	// Placing the value in the Register
	pSPIHandle->pSPIx->CR1 = tempreg;



}

/*
 * @Description: Resets all the Registers of the SPI peripheral.
 * @Parameters: SPI peripheral address
 * @Return: NONE
 */
// SPI De-Initialize
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if(pSPIx == SPI1)
		SPI1_REG_RESET();

	else if(pSPIx == SPI2)
		SPI2_REG_RESET();

	else if(pSPIx == SPI3)
		SPI3_REG_RESET();
}



/*
 * @Description: Sends data via SPI communication, data could either be 8 or 16 bits (Blocking)
 * @Parameters: SPI peripheral address, Pointer to TxBuffer, Length of Data
 * @Return: NONE
 */
// Data Send (Blocking Call)
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t * pTxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait until TXE Flag is set
		while(SPI_GetFlagStatus(pSPIx, SPI_FLAG_TXE) == FLAG_RESET);

		//2. Check the DSS bits in CR2 (for 8 bits)
		if(pSPIx->CR2 & (SPI_DFF_16Bits < SPI_CR2_DS))
		{
			 //16 bits DFF
			//1. Load the data into DR
			pSPIx->DR = *((uint16_t *)pTxBuffer);

			//2. Decrease Length
			Len--;
			Len--;

			//3. Increment the address
			(uint16_t *)pTxBuffer++;
		}

		else
		{
			 //b bits DFF
			//1. Load the data into DR
			pSPIx->DR = *(pTxBuffer);

			//2. Decrease Length
			Len--;

			//3. Increment the address
			pTxBuffer++;

		}

	}
}


/*
 * @Description: Receives data via SPI communication, data could either be 8 or 16 bits (Blocking)
 * @Parameters: SPI peripheral address, Pointer to RxBuffer, Length of Data
 * @Return: NONE
 */
// Receive Data
void SPI_RecieveData(SPI_RegDef_t *pSPIx, uint8_t * pRxBuffer, uint32_t Len)
{
	while(Len > 0)
	{
		//1. Wait until RXNE Flag is set
		while(SPI_GetFlagStatus(pSPIx, SPI_FLAG_RXNE) == FLAG_RESET);

		//2. Check the DSS bits in CR2 (for 8 bits)
		if(pSPIx->CR2 & (SPI_DFF_16Bits < SPI_CR2_DS))
		{
			 //16 bits DFF
			//1. Load the data into DR
			*((uint16_t *)pRxBuffer) = pSPIx->DR ;

			//2. Decrease Length
			Len--;
			Len--;

			//3. Increment the address
			(uint16_t *)pRxBuffer++;
		}

		else
		{
			 //b bits DFF
			//1. Load the data into DR
			*(pRxBuffer) = pSPIx->DR;

			//2. Decrease Length
			Len--;

			//3. Increment the address
			pRxBuffer++;
		}

	}

}


/*
 * @Description: Configures the NVIC side register ISER and ICER , enables interrupt reception
 * @Parameters: IRQ Number , Enable or Disable Value
 * @Return: NONE
 */
// IRQ Configurations and Handling
void SPI_IRQITConfig(uint8_t IRQNumber, uint8_t ENorDI)
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
 * @Description: Configures the NVIC side register IPR, sets the priority of the interrupts
 * @Parameters: IRQ Number , Priority
 * @Return: NONE
 */
void SPI_IRQPriorityConfig(uint32_t IRQNumber, uint8_t IRQPriority)
{
	//1. First lets find the correct Register and section
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section =  (8 * IRQNumber % 4) + 4;
	*(NVIC_IPR_BASEADDR + iprx) |= IRQPriority << (iprx_section);
}


/*
 * @Description: Checks which flag has caused the interrupt and calls the appropriate function (TXE flag, RXNE flag, OVR flag)
 * @Parameters: Pointer to Handle Structure
 * @Return: NONE
 */
void SPI_IRQHandling(SPI_Handle_t * pHandle)
{
	uint8_t temp1,temp2;

	// First check the Status register for Txe Flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 && temp2)
	{
		// handle Txe
		spi_txe_interrupt_handle(pHandle);
	}

	// Check the Status register for RXNE Flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 && temp2)
	{
		// handle Txe
		spi_rxne_interrupt_handle(pHandle);
	}

	//Check the Status register for OVR Flag
	temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 && temp2)
	{
		// handle Txe
		spi_ovrerr_interrupt_handle(pHandle);
	}



}


/*
 * @Description: Gets the data from the user and stores it inside the Handle Structure, enables the TXEIE bit
 * @Parameters: Pointer to Handle structure, pointer to Tx buffer, Length of data
 * @Return: State of the SPI Tx
 */
// Data Send (Interrupt)
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle, uint8_t * pTxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_TX )
	{
		//1. Save the Tx buffer address and Len Information in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = Len;

		//2. Mark the SPI state as busy in Transmission so that no other code can take over
		// the same SPI peripheral until transmission is over
		pSPIHandle->TxState = SPI_BUSY_TX;

		//3. Enable the TXEIE control bit to get interrupt when the TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);

		//4. Data Transmission will be handled by the ISR code
	}

	return state;

}


/*
 * @Description: Gets the pRxBuffer and Len from the user and stores it in a handle structure, enables the RXNEIE bit
 * @Parameters: Pointer to Handle structure, pointer to Rx buffer, Length of data
 * @Return: State of the SPI Rx
 */
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle, uint8_t * pRxBuffer, uint32_t Len)
{
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_RX )
	{
		//1. Save the Rx buffer address and Len Information in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = Len;

		//2. Mark the SPI state as busy in Reception so that no other code can take over
		// the same SPI peripheral until Reception is over
		pSPIHandle->RxState = SPI_BUSY_RX;

		//3. Enable the RXNEIE control bit to get interrupt when the RXNE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);

		//4. Data Reception will be handled by the ISR code
	}

	return state;

}

// Helper function Implementations
/*
 * @Description: Actually performs the data transmission when in interrupt mode.
 * @Parameters: Pointer to handle structure.
 * @Return: NONE
 */
static void spi_txe_interrupt_handle(SPI_Handle_t * pSPIHandle)
{
	//2. Check the DSS bits in CR2 (for 8 bits)
	if(pSPIHandle->pSPIx->CR2 & (SPI_DFF_16Bits < SPI_CR2_DS))
	{
		 //16 bits DFF
		//1. Load the data into DR
		pSPIHandle->pSPIx->DR = *((uint16_t *)pSPIHandle->pTxBuffer);

		//2. Decrease Length
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;

		//3. Increment the address
		(uint16_t *)pSPIHandle->pTxBuffer++;
	}

	else
	{
		//8 bits DFF
		pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);

		//2. Decrease Length
		pSPIHandle->TxLen--;

		//3. Increment the address
		pSPIHandle->pTxBuffer++;
	}

	if(! pSPIHandle->TxLen)
	{
		// Close the SPI communication and inform the application that TX is over

		// Disable TXEIE bit, this prevents interrupts from TXE flag
		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_TX_CMPLT);

	}


}


/*
 * @Description:  Actually performs the data reception when in interrupt mode.
 * @Parameters: Pointer to Handle Structure.
 * @Return: NONE
 */
static void spi_rxne_interrupt_handle(SPI_Handle_t * pSPIHandle)
{
	//2. Check the DSS bits in CR2 (for 8 bits)
	if(pSPIHandle->pSPIx->CR2 & (SPI_DFF_16Bits < SPI_CR2_DS))
	{
		 //16 bits DFF
		//1. Load the data into DR
		*((uint16_t *)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;

		//2. Decrease Length
		pSPIHandle->RxLen--;
		pSPIHandle->RxLen--;

		//3. Increment the address
		(uint16_t *)pSPIHandle->pRxBuffer++;
	}

	else
	{
		//8 bits DFF
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;

		//2. Decrease Length
		pSPIHandle->RxLen--;

		//3. Increment the address
		pSPIHandle->pRxBuffer++;
	}

	if(! pSPIHandle->RxLen)
	{
		// Close the SPI communication and inform the application that RX is over

		// Disable RXNEIE bit, this prevents interrupts from RXNE flag
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_RX_CMPLT);

	}

}


/*
 * @Description: Clears the OVR flag and informs the application about the error.
 * @Parameters: Pointer to Handle Structure
 * @Return: NONE
 */
static void spi_ovrerr_interrupt_handle(SPI_Handle_t * pSPIHandle)
{
	uint8_t temp;
	//1. Clear the OVR Flag (Read the DR , followed by reading the SR register)
	if(pSPIHandle->TxState != SPI_BUSY_TX)			// Check if its not busy in Tx when the OVR error happens
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;

	}
	(void)temp;

	//2. Inform the Application
	SPI_ApplicationEventCallBack(pSPIHandle, SPI_EVENT_OVR_ERR);

}


/*
 * @Description: Disables the TXEIE bit and clear the TX buffer and length.
 * @Parameters: Pointer to handle structure
 * @Return: NONE
 */
// Close Transmission
void SPI_CloseTransmission(SPI_Handle_t * pSPIHandle)
{
	// Disable TXEIE bit, this prevents interrupts from TXE flag
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;


}

/*
 * @Description: Disables the RXNEIE bit and clear the RX buffer and length.
 * @Parameters: Pointer to handle structure
 * @Return: NONE
 */
// Close Reception
void SPI_CloseReception(SPI_Handle_t * pSPIHandle)
{
	// Disable RXNEIE bit, this prevents interrupts from RXNE flag
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;

}


/*
 * @Description: Clears the OVR flag by reading from the Data and Status register.
 * @Parameters: Pointer to SPI peripheral address
 * @Return: NONE
 */
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp;
	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}

// Enable the SPI peripheral by setting the SPE bit
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}

	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

// To Enable the SSI to avoid the MODF bit set
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, uint8_t ENorDI)
{
	if(ENorDI == ENABLE)
	{
		pSPIx->CR1 |= (1 << SPI_CR1_SSI);
	}

	else
	{
		pSPIx->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

// Return the FLAG status
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*
 * @Description: Weak Implementation of application callback function
 * @Parameters: Pointer to Handle Structure, Application Event
 * @Return: NONE
 */
// Weak implementation of application call back
__weak void SPI_ApplicationEventCallBack(SPI_Handle_t * pSPIHandle, uint8_t AppEv)
{

}


