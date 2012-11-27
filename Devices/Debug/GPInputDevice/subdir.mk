################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../GPInputDevice/GPIDevice.cpp 

OBJS += \
./GPInputDevice/GPIDevice.o 

CPP_DEPS += \
./GPInputDevice/GPIDevice.d 


# Each subdirectory must supply rules for building sources it contributes
GPInputDevice/%.o: ../GPInputDevice/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


