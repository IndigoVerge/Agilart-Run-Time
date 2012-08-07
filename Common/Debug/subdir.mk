################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BaseDevice.cpp \
../BaseDriver.cpp \
../Command.cpp \
../Configuration.cpp \
../ConnectionPoint.cpp \
../Convert.cpp \
../DataSerializationManager.cpp \
../Event.cpp \
../InConnectionPoint.cpp \
../JsonParameters.cpp \
../LogEntry.cpp \
../OutConnectionPoint.cpp \
../Pinout.cpp \
../Process.cpp \
../Program.cpp \
../Wire.cpp 

OBJS += \
./BaseDevice.o \
./BaseDriver.o \
./Command.o \
./Configuration.o \
./ConnectionPoint.o \
./Convert.o \
./DataSerializationManager.o \
./Event.o \
./InConnectionPoint.o \
./JsonParameters.o \
./LogEntry.o \
./OutConnectionPoint.o \
./Pinout.o \
./Process.o \
./Program.o \
./Wire.o 

CPP_DEPS += \
./BaseDevice.d \
./BaseDriver.d \
./Command.d \
./Configuration.d \
./ConnectionPoint.d \
./Convert.d \
./DataSerializationManager.d \
./Event.d \
./InConnectionPoint.d \
./JsonParameters.d \
./LogEntry.d \
./OutConnectionPoint.d \
./Pinout.d \
./Process.d \
./Program.d \
./Wire.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	arm-none-linux-gnueabi-g++ -O0 -g3 -Wall -c -fmessage-length=0 -fPIC -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm1176jz-s -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


