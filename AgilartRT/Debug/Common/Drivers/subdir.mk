################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/Drivers/BaseDriversProvider.cpp \
../Common/Drivers/DriversManager.cpp \
../Common/Drivers/PluggableDriversProvider.cpp 

OBJS += \
./Common/Drivers/BaseDriversProvider.o \
./Common/Drivers/DriversManager.o \
./Common/Drivers/PluggableDriversProvider.o 

CPP_DEPS += \
./Common/Drivers/BaseDriversProvider.d \
./Common/Drivers/DriversManager.d \
./Common/Drivers/PluggableDriversProvider.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Drivers/%.o: ../Common/Drivers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


