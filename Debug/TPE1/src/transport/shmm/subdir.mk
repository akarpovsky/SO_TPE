################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE1/src/transport/shmm/shmm_c.c \
../TPE1/src/transport/shmm/shmm_contact_serialization.c \
../TPE1/src/transport/shmm/shmm_s.c 

OBJS += \
./TPE1/src/transport/shmm/shmm_c.o \
./TPE1/src/transport/shmm/shmm_contact_serialization.o \
./TPE1/src/transport/shmm/shmm_s.o 

C_DEPS += \
./TPE1/src/transport/shmm/shmm_c.d \
./TPE1/src/transport/shmm/shmm_contact_serialization.d \
./TPE1/src/transport/shmm/shmm_s.d 


# Each subdirectory must supply rules for building sources it contributes
TPE1/src/transport/shmm/%.o: ../TPE1/src/transport/shmm/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


