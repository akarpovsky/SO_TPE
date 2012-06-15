################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE1/src/utils/LinkedList.c \
../TPE1/src/utils/hashmap.c \
../TPE1/src/utils/printColor.c 

OBJS += \
./TPE1/src/utils/LinkedList.o \
./TPE1/src/utils/hashmap.o \
./TPE1/src/utils/printColor.o 

C_DEPS += \
./TPE1/src/utils/LinkedList.d \
./TPE1/src/utils/hashmap.d \
./TPE1/src/utils/printColor.d 


# Each subdirectory must supply rules for building sources it contributes
TPE1/src/utils/%.o: ../TPE1/src/utils/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


