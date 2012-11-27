################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Logging/BaseLogProvider.cpp \
../Logging/EventLogger.cpp \
../Logging/JsonLogProvider.cpp \
../Logging/SysLogProvider.cpp 

OBJS += \
./Logging/BaseLogProvider.o \
./Logging/EventLogger.o \
./Logging/JsonLogProvider.o \
./Logging/SysLogProvider.o 

CPP_DEPS += \
./Logging/BaseLogProvider.d \
./Logging/EventLogger.d \
./Logging/JsonLogProvider.d \
./Logging/SysLogProvider.d 


# Each subdirectory must supply rules for building sources it contributes
Logging/%.o: ../Logging/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


