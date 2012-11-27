################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../gSOAP/Services/soapC.cpp \
../gSOAP/Services/soapUPCBWebServiceSoapProxy.cpp \
../gSOAP/Services/soapUPCBWebServiceSoapService.cpp \
../gSOAP/Services/stdsoap2.cpp 

C_SRCS += \
../gSOAP/Services/httpform.c \
../gSOAP/Services/httpget.c \
../gSOAP/Services/threads.c 

OBJS += \
./gSOAP/Services/httpform.o \
./gSOAP/Services/httpget.o \
./gSOAP/Services/soapC.o \
./gSOAP/Services/soapUPCBWebServiceSoapProxy.o \
./gSOAP/Services/soapUPCBWebServiceSoapService.o \
./gSOAP/Services/stdsoap2.o \
./gSOAP/Services/threads.o 

C_DEPS += \
./gSOAP/Services/httpform.d \
./gSOAP/Services/httpget.d \
./gSOAP/Services/threads.d 

CPP_DEPS += \
./gSOAP/Services/soapC.d \
./gSOAP/Services/soapUPCBWebServiceSoapProxy.d \
./gSOAP/Services/soapUPCBWebServiceSoapService.d \
./gSOAP/Services/stdsoap2.d 


# Each subdirectory must supply rules for building sources it contributes
gSOAP/Services/%.o: ../gSOAP/Services/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C Compiler'
	$(CC_PREFIX)gcc -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gSOAP/Services/%.o: ../gSOAP/Services/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Sourcery CodeBench C++ Compiler'
	$(CC_PREFIX)g++ -I"../../Drivers" -I"../../Devices" -I"../../Common" -O0 -g3 -Wall -c -fmessage-length=0 -fcommon -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" $(CFLAGS) -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


