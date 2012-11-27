################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/Devices/BaseDevicesProvider.cpp \
../Common/Devices/DeviceManager.cpp \
../Common/Devices/PluggableDevicesProvider.cpp 

OBJS += \
./Common/Devices/BaseDevicesProvider.o \
./Common/Devices/DeviceManager.o \
./Common/Devices/PluggableDevicesProvider.o 

CPP_DEPS += \
./Common/Devices/BaseDevicesProvider.d \
./Common/Devices/DeviceManager.d \
./Common/Devices/PluggableDevicesProvider.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Devices/%.o: ../Common/Devices/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


