################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CosmDevice/CosmDevice.cpp 

OBJS += \
./CosmDevice/CosmDevice.o 

CPP_DEPS += \
./CosmDevice/CosmDevice.d 


# Each subdirectory must supply rules for building sources it contributes
CosmDevice/%.o: ../CosmDevice/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I"../../Common" -I"/usr/include"  -O0 -g3 -Wall -lcurl -ldl -lpthread -lrt -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


