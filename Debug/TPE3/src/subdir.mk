################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE3/src/auxMem.c \
../TPE3/src/exceptions.c \
../TPE3/src/fs.c \
../TPE3/src/io.c \
../TPE3/src/kernel.c \
../TPE3/src/libc.c \
../TPE3/src/memo.c \
../TPE3/src/memorym.c \
../TPE3/src/proc.c \
../TPE3/src/scheduler.c \
../TPE3/src/shell.c \
../TPE3/src/strings.c 

ASM_SRCS += \
../TPE3/src/kstart.asm \
../TPE3/src/kstart_backup.asm \
../TPE3/src/libasm.asm \
../TPE3/src/loader.asm \
../TPE3/src/loader_backup.asm \
../TPE3/src/paging.asm 

OBJS += \
./TPE3/src/auxMem.o \
./TPE3/src/exceptions.o \
./TPE3/src/fs.o \
./TPE3/src/io.o \
./TPE3/src/kernel.o \
./TPE3/src/kstart.o \
./TPE3/src/kstart_backup.o \
./TPE3/src/libasm.o \
./TPE3/src/libc.o \
./TPE3/src/loader.o \
./TPE3/src/loader_backup.o \
./TPE3/src/memo.o \
./TPE3/src/memorym.o \
./TPE3/src/paging.o \
./TPE3/src/proc.o \
./TPE3/src/scheduler.o \
./TPE3/src/shell.o \
./TPE3/src/strings.o 

C_DEPS += \
./TPE3/src/auxMem.d \
./TPE3/src/exceptions.d \
./TPE3/src/fs.d \
./TPE3/src/io.d \
./TPE3/src/kernel.d \
./TPE3/src/libc.d \
./TPE3/src/memo.d \
./TPE3/src/memorym.d \
./TPE3/src/proc.d \
./TPE3/src/scheduler.d \
./TPE3/src/shell.d \
./TPE3/src/strings.d 


# Each subdirectory must supply rules for building sources it contributes
TPE3/src/%.o: ../TPE3/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

TPE3/src/%.o: ../TPE3/src/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: GCC Assembler'
	as  -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


