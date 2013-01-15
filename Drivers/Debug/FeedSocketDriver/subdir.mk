################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../FeedSocketDriver/FeedSocketDriver.cpp \
../FeedSocketDriver/FeedsSocketClient.cpp 

OBJS += \
./FeedSocketDriver/FeedSocketDriver.o \
./FeedSocketDriver/FeedsSocketClient.o 

CPP_DEPS += \
./FeedSocketDriver/FeedSocketDriver.d \
./FeedSocketDriver/FeedsSocketClient.d 


# Each subdirectory must supply rules for building sources it contributes
FeedSocketDriver/%.o: ../FeedSocketDriver/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-unknown-linux-gnueabi-g++ -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


