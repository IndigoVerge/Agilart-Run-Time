################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Common/ProgramManager.cpp \
../Common/UpcbJsonParser.cpp 

OBJS += \
./Common/ProgramManager.o \
./Common/UpcbJsonParser.o 

CPP_DEPS += \
./Common/ProgramManager.d \
./Common/UpcbJsonParser.d 


# Each subdirectory must supply rules for building sources it contributes
Common/%.o: ../Common/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	arm-none-linux-gnueabi-g++ -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm1176jz-s -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


