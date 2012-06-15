################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE2/src/auxMem.c \
../TPE2/src/exceptions.c \
../TPE2/src/io.c \
../TPE2/src/kernel.c \
../TPE2/src/libc.c \
../TPE2/src/memo.c \
../TPE2/src/memorym.c \
../TPE2/src/proc.c \
../TPE2/src/scheduler.c \
../TPE2/src/shell.c \
../TPE2/src/strings.c 

ASM_SRCS += \
../TPE2/src/kstart.asm \
../TPE2/src/libasm.asm \
../TPE2/src/loader.asm \
../TPE2/src/paging.asm 

OBJS += \
./TPE2/src/auxMem.o \
./TPE2/src/exceptions.o \
./TPE2/src/io.o \
./TPE2/src/kernel.o \
./TPE2/src/kstart.o \
./TPE2/src/libasm.o \
./TPE2/src/libc.o \
./TPE2/src/loader.o \
./TPE2/src/memo.o \
./TPE2/src/memorym.o \
./TPE2/src/paging.o \
./TPE2/src/proc.o \
./TPE2/src/scheduler.o \
./TPE2/src/shell.o \
./TPE2/src/strings.o 

C_DEPS += \
./TPE2/src/auxMem.d \
./TPE2/src/exceptions.d \
./TPE2/src/io.d \
./TPE2/src/kernel.d \
./TPE2/src/libc.d \
./TPE2/src/memo.d \
./TPE2/src/memorym.d \
./TPE2/src/proc.d \
./TPE2/src/scheduler.d \
./TPE2/src/shell.d \
./TPE2/src/strings.d 


# Each subdirectory must supply rules for building sources it contributes
TPE2/src/%.o: ../TPE2/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

TPE2/src/%.o: ../TPE2/src/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


