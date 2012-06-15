################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include TPE3/src/subdir.mk
-include TPE3/src/drivers/subdir.mk
-include TPE3/bin/subdir.mk
-include TPE2/src/subdir.mk
-include TPE2/src/drivers/subdir.mk
-include TPE2/bin/subdir.mk
-include TPE1/src/utils/subdir.mk
-include TPE1/src/transport/sockets/subdir.mk
-include TPE1/src/transport/shmm/subdir.mk
-include TPE1/src/transport/fifos/subdir.mk
-include TPE1/src/transport/MQ/subdir.mk
-include TPE1/src/server/subdir.mk
-include TPE1/src/marshalling/subdir.mk
-include TPE1/src/client/subdir.mk
-include TPE1/src/IO/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: GIT\ -\ SO

# Tool invocations
GIT\ -\ SO: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MacOS X C Linker'
	gcc  -o "GIT - SO" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) "GIT - SO"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
