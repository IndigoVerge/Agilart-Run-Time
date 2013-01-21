################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FeedDevice/FeedDevice.cpp 

OBJS += \
./FeedDevice/FeedDevice.o 

CPP_DEPS += \
./FeedDevice/FeedDevice.d 


# Each subdirectory must supply rules for building sources it contributes
FeedDevice/%.o: ../FeedDevice/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CC_PREFIX)g++ -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -mcpu=arm926ej-s -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


