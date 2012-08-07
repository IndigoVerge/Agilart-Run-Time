################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/Mappings/MappingManager.cpp \
../Common/Mappings/PinoutMapping.cpp \
../Common/Mappings/ProgramMapping.cpp 

OBJS += \
./Common/Mappings/MappingManager.o \
./Common/Mappings/PinoutMapping.o \
./Common/Mappings/ProgramMapping.o 

CPP_DEPS += \
./Common/Mappings/MappingManager.d \
./Common/Mappings/PinoutMapping.d \
./Common/Mappings/ProgramMapping.d 


# Each subdirectory must supply rules for building sources it contributes
Common/Mappings/%.o: ../Common/Mappings/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	arm-none-linux-gnueabi-g++ -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm1176jz-s -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


