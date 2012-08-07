################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GPOutputDevice/GPOutputDevice.cpp 

OBJS += \
./GPOutputDevice/GPOutputDevice.o 

CPP_DEPS += \
./GPOutputDevice/GPOutputDevice.d 


# Each subdirectory must supply rules for building sources it contributes
GPOutputDevice/%.o: ../GPOutputDevice/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	arm-none-linux-gnueabi-g++ -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm1176jz-s -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


