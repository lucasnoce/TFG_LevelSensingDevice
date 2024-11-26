################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MyDriversSrc/aht20.c \
../Core/Src/MyDriversSrc/ajsr04m.c \
../Core/Src/MyDriversSrc/le910r1br.c \
../Core/Src/MyDriversSrc/leds.c \
../Core/Src/MyDriversSrc/lsd_callbacks.c \
../Core/Src/MyDriversSrc/lsd_common.c 

OBJS += \
./Core/Src/MyDriversSrc/aht20.o \
./Core/Src/MyDriversSrc/ajsr04m.o \
./Core/Src/MyDriversSrc/le910r1br.o \
./Core/Src/MyDriversSrc/leds.o \
./Core/Src/MyDriversSrc/lsd_callbacks.o \
./Core/Src/MyDriversSrc/lsd_common.o 

C_DEPS += \
./Core/Src/MyDriversSrc/aht20.d \
./Core/Src/MyDriversSrc/ajsr04m.d \
./Core/Src/MyDriversSrc/le910r1br.d \
./Core/Src/MyDriversSrc/leds.d \
./Core/Src/MyDriversSrc/lsd_callbacks.d \
./Core/Src/MyDriversSrc/lsd_common.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/MyDriversSrc/%.o Core/Src/MyDriversSrc/%.su Core/Src/MyDriversSrc/%.cyclo: ../Core/Src/MyDriversSrc/%.c Core/Src/MyDriversSrc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-MyDriversSrc

clean-Core-2f-Src-2f-MyDriversSrc:
	-$(RM) ./Core/Src/MyDriversSrc/aht20.cyclo ./Core/Src/MyDriversSrc/aht20.d ./Core/Src/MyDriversSrc/aht20.o ./Core/Src/MyDriversSrc/aht20.su ./Core/Src/MyDriversSrc/ajsr04m.cyclo ./Core/Src/MyDriversSrc/ajsr04m.d ./Core/Src/MyDriversSrc/ajsr04m.o ./Core/Src/MyDriversSrc/ajsr04m.su ./Core/Src/MyDriversSrc/le910r1br.cyclo ./Core/Src/MyDriversSrc/le910r1br.d ./Core/Src/MyDriversSrc/le910r1br.o ./Core/Src/MyDriversSrc/le910r1br.su ./Core/Src/MyDriversSrc/leds.cyclo ./Core/Src/MyDriversSrc/leds.d ./Core/Src/MyDriversSrc/leds.o ./Core/Src/MyDriversSrc/leds.su ./Core/Src/MyDriversSrc/lsd_callbacks.cyclo ./Core/Src/MyDriversSrc/lsd_callbacks.d ./Core/Src/MyDriversSrc/lsd_callbacks.o ./Core/Src/MyDriversSrc/lsd_callbacks.su ./Core/Src/MyDriversSrc/lsd_common.cyclo ./Core/Src/MyDriversSrc/lsd_common.d ./Core/Src/MyDriversSrc/lsd_common.o ./Core/Src/MyDriversSrc/lsd_common.su

.PHONY: clean-Core-2f-Src-2f-MyDriversSrc

