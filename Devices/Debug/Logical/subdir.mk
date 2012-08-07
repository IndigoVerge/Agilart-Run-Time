################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Logical/AdnWidget.cpp \
../Logical/ComparatorWidget.cpp \
../Logical/ConstantWidget.cpp \
../Logical/CounterDevice.cpp \
../Logical/InverseDevice.cpp \
../Logical/OrDevice.cpp \
../Logical/RSTriggerDevice.cpp \
../Logical/SwitchBlock.cpp \
../Logical/XorDevice.cpp 

OBJS += \
./Logical/AdnWidget.o \
./Logical/ComparatorWidget.o \
./Logical/ConstantWidget.o \
./Logical/CounterDevice.o \
./Logical/InverseDevice.o \
./Logical/OrDevice.o \
./Logical/RSTriggerDevice.o \
./Logical/SwitchBlock.o \
./Logical/XorDevice.o 

CPP_DEPS += \
./Logical/AdnWidget.d \
./Logical/ComparatorWidget.d \
./Logical/ConstantWidget.d \
./Logical/CounterDevice.d \
./Logical/InverseDevice.d \
./Logical/OrDevice.d \
./Logical/RSTriggerDevice.d \
./Logical/SwitchBlock.d \
./Logical/XorDevice.d 


# Each subdirectory must supply rules for building sources it contributes
Logical/%.o: ../Logical/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	arm-none-linux-gnueabi-g++ -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm1176jz-s -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


