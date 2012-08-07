################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Logging/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LIBRARIES += \
libCommon.so \


# All Target
all: libCommon.so

# Tool invocations
libCommon.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Sourcery CodeBench C++ Linker'
	arm-none-linux-gnueabi-g++ -shared -Xlinker -Map="libCommon.map" -mcpu=arm1176jz-s -o "libCommon.so" "@objs.rsp" "@user_objs.rsp" "@libs.rsp"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)
	-$(RM) "./BaseDevice.o" "./BaseDriver.o" "./Command.o" "./Configuration.o" "./ConnectionPoint.o" "./Convert.o" "./DataSerializationManager.o" "./Event.o" "./InConnectionPoint.o" "./JsonParameters.o" "./LogEntry.o" "./OutConnectionPoint.o" "./Pinout.o" "./Process.o" "./Program.o" "./Wire.o" "./Logging/BaseLogProvider.o" 
	-$(RM) "./Logging/EventLogger.o" "./Logging/JsonLogProvider.o" "./Logging/SysLogProvider.o"  libCommon.so libCommon.so
.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
