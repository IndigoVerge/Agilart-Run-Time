################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include TimerDevice/subdir.mk
-include ScreenButton/subdir.mk
-include Logical/subdir.mk
-include GPOutputDevice/subdir.mk
-include GPInputDevice/subdir.mk
-include DelayTimer/subdir.mk
-include ButtonDevice/subdir.mk
-include ADCDevice/subdir.mk
-include CosmDevice/subdir.mk
-include FeedDevice/subdir.mk
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
libDevices.so \


# All Target
all: libDevices.so

dependents:
	-cd ../../Common/Debug && $(MAKE) all

# Tool invocations
libDevices.so: $(OBJS) $(USER_OBJS) ../../Common/Debug/libCommon.so
	@echo 'Building target: $@'
	@echo 'Invoking: Sourcery CodeBench C++ Linker'
	g++ -L"../../Common/Debug" -shared -Xlinker -Map="libDevices.map" -g -O2 -o "libDevices.so" "@objs.rsp" "@user_objs.rsp" "@libs.rsp"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(LIBRARIES)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)
	-$(RM) "./CosmDevice/CosmDevice.o" "./TimerDevice/TimerDevice.o" "./ScreenButton/ScreenButton.o" "./Logical/AdnWidget.o" "./Logical/ComparatorWidget.o" "./Logical/ConstantWidget.o" "./Logical/CounterDevice.o" "./Logical/InverseDevice.o" "./Logical/OrDevice.o" "./Logical/RSTriggerDevice.o" "./Logical/SwitchBlock.o" "./Logical/XorDevice.o" "./GPOutputDevice/GPOutputDevice.o" "./GPInputDevice/GPIDevice.o" "./EntryPoint.o" "./DelayTimer/DelayTimer.o" "./FeedDevice/FeedDevice.o"  
	-$(RM) "./ButtonDevice/ButtonDevice.o" "./ADCDevice/ADCDevice.o"  libDevices.so libDevices.so
.PHONY: all clean dependents
.SECONDARY:
../../Common/Debug/libCommon.so:

-include ../makefile.targets
