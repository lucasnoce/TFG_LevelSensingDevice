################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MyServicesSrc/data_analysis.c \
../Core/Src/MyServicesSrc/device_sleep.c \
../Core/Src/MyServicesSrc/distance_measurements.c \
../Core/Src/MyServicesSrc/logs_management.c \
../Core/Src/MyServicesSrc/lte_transmissions.c \
../Core/Src/MyServicesSrc/state_machine.c 

OBJS += \
./Core/Src/MyServicesSrc/data_analysis.o \
./Core/Src/MyServicesSrc/device_sleep.o \
./Core/Src/MyServicesSrc/distance_measurements.o \
./Core/Src/MyServicesSrc/logs_management.o \
./Core/Src/MyServicesSrc/lte_transmissions.o \
./Core/Src/MyServicesSrc/state_machine.o 

C_DEPS += \
./Core/Src/MyServicesSrc/data_analysis.d \
./Core/Src/MyServicesSrc/device_sleep.d \
./Core/Src/MyServicesSrc/distance_measurements.d \
./Core/Src/MyServicesSrc/logs_management.d \
./Core/Src/MyServicesSrc/lte_transmissions.d \
./Core/Src/MyServicesSrc/state_machine.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/MyServicesSrc/%.o Core/Src/MyServicesSrc/%.su Core/Src/MyServicesSrc/%.cyclo: ../Core/Src/MyServicesSrc/%.c Core/Src/MyServicesSrc/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-MyServicesSrc

clean-Core-2f-Src-2f-MyServicesSrc:
	-$(RM) ./Core/Src/MyServicesSrc/data_analysis.cyclo ./Core/Src/MyServicesSrc/data_analysis.d ./Core/Src/MyServicesSrc/data_analysis.o ./Core/Src/MyServicesSrc/data_analysis.su ./Core/Src/MyServicesSrc/device_sleep.cyclo ./Core/Src/MyServicesSrc/device_sleep.d ./Core/Src/MyServicesSrc/device_sleep.o ./Core/Src/MyServicesSrc/device_sleep.su ./Core/Src/MyServicesSrc/distance_measurements.cyclo ./Core/Src/MyServicesSrc/distance_measurements.d ./Core/Src/MyServicesSrc/distance_measurements.o ./Core/Src/MyServicesSrc/distance_measurements.su ./Core/Src/MyServicesSrc/logs_management.cyclo ./Core/Src/MyServicesSrc/logs_management.d ./Core/Src/MyServicesSrc/logs_management.o ./Core/Src/MyServicesSrc/logs_management.su ./Core/Src/MyServicesSrc/lte_transmissions.cyclo ./Core/Src/MyServicesSrc/lte_transmissions.d ./Core/Src/MyServicesSrc/lte_transmissions.o ./Core/Src/MyServicesSrc/lte_transmissions.su ./Core/Src/MyServicesSrc/state_machine.cyclo ./Core/Src/MyServicesSrc/state_machine.d ./Core/Src/MyServicesSrc/state_machine.o ./Core/Src/MyServicesSrc/state_machine.su

.PHONY: clean-Core-2f-Src-2f-MyServicesSrc

