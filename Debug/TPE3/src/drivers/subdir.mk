################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE3/src/drivers/ata_disk.c \
../TPE3/src/drivers/keyboard.c \
../TPE3/src/drivers/mouse.c \
../TPE3/src/drivers/video.c 

OBJS += \
./TPE3/src/drivers/ata_disk.o \
./TPE3/src/drivers/keyboard.o \
./TPE3/src/drivers/mouse.o \
./TPE3/src/drivers/video.o 

C_DEPS += \
./TPE3/src/drivers/ata_disk.d \
./TPE3/src/drivers/keyboard.d \
./TPE3/src/drivers/mouse.d \
./TPE3/src/drivers/video.d 


# Each subdirectory must supply rules for building sources it contributes
TPE3/src/drivers/%.o: ../TPE3/src/drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


