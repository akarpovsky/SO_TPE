################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE1/src/marshalling/marshalling.c \
../TPE1/src/marshalling/message_serialization.c 

OBJS += \
./TPE1/src/marshalling/marshalling.o \
./TPE1/src/marshalling/message_serialization.o 

C_DEPS += \
./TPE1/src/marshalling/marshalling.d \
./TPE1/src/marshalling/message_serialization.d 


# Each subdirectory must supply rules for building sources it contributes
TPE1/src/marshalling/%.o: ../TPE1/src/marshalling/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


