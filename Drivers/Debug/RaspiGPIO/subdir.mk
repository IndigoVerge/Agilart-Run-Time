################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../RaspiGPIO/RaspiGPInputDriver.cpp \
../RaspiGPIO/RaspiGPOutputDriver.cpp 

C_SRCS += \
../RaspiGPIO/bcm2835.c 

OBJS += \
./RaspiGPIO/RaspiGPInputDriver.o \
./RaspiGPIO/RaspiGPOutputDriver.o \
./RaspiGPIO/bcm2835.o 

C_DEPS += \
./RaspiGPIO/bcm2835.d 

CPP_DEPS += \
./RaspiGPIO/RaspiGPInputDriver.d \
./RaspiGPIO/RaspiGPOutputDriver.d 


# Each subdirectory must supply rules for building sources it contributes
RaspiGPIO/%.o: ../RaspiGPIO/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I/usr/local/armboost/include -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

RaspiGPIO/%.o: ../RaspiGPIO/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C Compiler'
	$(CC_PREFIX)gcc -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


