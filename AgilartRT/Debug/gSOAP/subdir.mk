################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gSOAP/DescriptorsUtil.cpp \
../gSOAP/WebServer.cpp 

OBJS += \
./gSOAP/DescriptorsUtil.o \
./gSOAP/WebServer.o 

CPP_DEPS += \
./gSOAP/DescriptorsUtil.d \
./gSOAP/WebServer.d 


# Each subdirectory must supply rules for building sources it contributes
gSOAP/%.o: ../gSOAP/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


