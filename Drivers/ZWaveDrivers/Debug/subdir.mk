################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../EntryPoint.cpp \
../ZSwtichDriver.cpp 

OBJS += \
./EntryPoint.o \
./ZSwtichDriver.o 

CPP_DEPS += \
./EntryPoint.d \
./ZSwtichDriver.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/pi/bobi/agilart/Common" -I/home/pi/open-zwave-read-only/cpp/src -I/home/pi/open-zwave-read-only/cpp/src/command_classes -I/home/pi/open-zwave-read-only/cpp/src/platform -I/home/pi/open-zwave-read-only/cpp/src/platform/unix -I/home/pi/open-zwave-read-only/cpp/src/value_classes -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


