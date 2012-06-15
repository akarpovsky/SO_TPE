################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE1/src/client/client.c \
../TPE1/src/client/parseCommand.c 

OBJS += \
./TPE1/src/client/client.o \
./TPE1/src/client/parseCommand.o 

C_DEPS += \
./TPE1/src/client/client.d \
./TPE1/src/client/parseCommand.d 


# Each subdirectory must supply rules for building sources it contributes
TPE1/src/client/%.o: ../TPE1/src/client/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


