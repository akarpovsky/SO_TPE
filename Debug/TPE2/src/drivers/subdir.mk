################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE2/src/drivers/keyboard.c \
../TPE2/src/drivers/mouse.c \
../TPE2/src/drivers/video.c 

OBJS += \
./TPE2/src/drivers/keyboard.o \
./TPE2/src/drivers/mouse.o \
./TPE2/src/drivers/video.o 

C_DEPS += \
./TPE2/src/drivers/keyboard.d \
./TPE2/src/drivers/mouse.d \
./TPE2/src/drivers/video.d 


# Each subdirectory must supply rules for building sources it contributes
TPE2/src/drivers/%.o: ../TPE2/src/drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


