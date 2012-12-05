################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Logical/SwitchBlock/LoopBlock.cpp 

OBJS += \
./Logical/SwitchBlock/LoopBlock.o 

CPP_DEPS += \
./Logical/SwitchBlock/LoopBlock.d 


# Each subdirectory must supply rules for building sources it contributes
Logical/SwitchBlock/%.o: ../Logical/SwitchBlock/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I"../../Common" -I/usr/local/armboost/include -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


