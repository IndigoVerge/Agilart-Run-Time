################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 

CPP_SRCS += \
../WebServer/WebServer.cpp 

C_SRCS += \
../WebServer/mongoose.c 

OBJS += \
./WebServer/WebServer.o \
./WebServer/mongoose.o 

C_DEPS += \
./WebServer/mongoose.d 

CPP_DEPS += \
./WebServer/WebServer.d 


# Each subdirectory must supply rules for building sources it contributes
WebServer/%.o: ../WebServer/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	$(CC_PREFIX)g++ -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

WebServer/%.o: ../WebServer/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	$(CC_PREFIX)gcc -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
