################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/Stm32F303VC_GpioDriver.c \
../Drivers/Src/Stm32F303VC_SpiDriver.c 

OBJS += \
./Drivers/Src/Stm32F303VC_GpioDriver.o \
./Drivers/Src/Stm32F303VC_SpiDriver.o 

C_DEPS += \
./Drivers/Src/Stm32F303VC_GpioDriver.d \
./Drivers/Src/Stm32F303VC_SpiDriver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/Stm32F303VC_GpioDriver.o: ../Drivers/Src/Stm32F303VC_GpioDriver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F303VCTx -DSTM32 -DSTM32F3 -DSTM32F3DISCOVERY -DDEBUG -c -I"C:/Users/15f14/Desktop/Jibreal/Embedded Projects/Stm32F303VC_Drivers/Drivers/Inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Src/Stm32F303VC_GpioDriver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/Src/Stm32F303VC_SpiDriver.o: ../Drivers/Src/Stm32F303VC_SpiDriver.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32F303VCTx -DSTM32 -DSTM32F3 -DSTM32F3DISCOVERY -DDEBUG -c -I"C:/Users/15f14/Desktop/Jibreal/Embedded Projects/Stm32F303VC_Drivers/Drivers/Inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/Src/Stm32F303VC_SpiDriver.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

