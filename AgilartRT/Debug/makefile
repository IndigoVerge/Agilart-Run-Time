################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include WebServer/subdir.mk
-include DaemonUtils/subdir.mk
-include Common/subdir.mk
-include Common/Mappings/subdir.mk
-include Common/Drivers/subdir.mk
-include Common/Devices/subdir.mk
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
EXECUTABLES += \
agilart-runtime \


# All Target
all: agilart-runtime secondary-outputs

dependents:
	-cd ../../Common/Debug && $(MAKE) clean all
	-cd ../../Devices/Debug && $(MAKE) clean all
	-cd ../../Drivers/Debug && $(MAKE) clean all

# Tool invocations
agilart-runtime: $(OBJS) $(USER_OBJS) ../../Common/Debug/libCommon.so ../../Devices/Debug/libDevices.so ../../Drivers/Debug/libDrivers.so
	@echo 'Building target: $@'
	@echo 'Invoking: Sourcery CodeBench C++ Linker'
	g++ -L"../../Drivers/Debug" -L"../../Devices/Debug" -L"../../Common/Debug" -I"/usr/include" -pthread -ldl -lcurl -lpthread -lrt -Xlinker -Map="agilart-runtime.map" -g -O2 -o "agilart-runtime" "@objs.rsp" "@user_objs.rsp" "@libs.rsp"
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS)
	-$(RM) "./agilartRT.o" "./DaemonUtils/DaemonService.o" "./Common/ProgramManager.o" "./Common/UpcbJsonParser.o" 
	-$(RM) "./Common/Mappings/MappingManager.o" "./Common/Mappings/PinoutMapping.o" "./Common/Mappings/ProgramMapping.o" "./Common/Drivers/BaseDriversProvider.o" "./Common/Drivers/DriversManager.o" "./Common/Drivers/PluggableDriversProvider.o" "./Common/Devices/BaseDevicesProvider.o" "./Common/Devices/DeviceManager.o" "./Common/Devices/PluggableDevicesProvider.o"  agilart-runtime agilart-runtime
post-build:
	-@echo 'Im ready!'

secondary-outputs:

.PHONY: all clean dependents
.SECONDARY: post-build
../../Common/Debug/libCommon.so:
../../Devices/Debug/libDevices.so:
../../Drivers/Debug/libDrivers.so:

-include ../makefile.targets
