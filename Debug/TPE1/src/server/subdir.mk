################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE1/src/server/execute.c \
../TPE1/src/server/manageClient.c \
../TPE1/src/server/server.c 

OBJS += \
./TPE1/src/server/execute.o \
./TPE1/src/server/manageClient.o \
./TPE1/src/server/server.o 

C_DEPS += \
./TPE1/src/server/execute.d \
./TPE1/src/server/manageClient.d \
./TPE1/src/server/server.d 


# Each subdirectory must supply rules for building sources it contributes
TPE1/src/server/%.o: ../TPE1/src/server/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


