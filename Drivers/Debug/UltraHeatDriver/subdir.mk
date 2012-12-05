################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../UltraHeatDriver/UltraHeatDriver.cpp 

OBJS += \
./UltraHeatDriver/UltraHeatDriver.o 

CPP_DEPS += \
./UltraHeatDriver/UltraHeatDriver.d 


# Each subdirectory must supply rules for building sources it contributes
UltraHeatDriver/%.o: ../UltraHeatDriver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I/usr/local/armboost/include -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


