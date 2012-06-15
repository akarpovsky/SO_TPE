################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TPE1/src/transport/sockets/socket_c.c \
../TPE1/src/transport/sockets/socket_s.c \
../TPE1/src/transport/sockets/sockets_contact_serialization.c 

OBJS += \
./TPE1/src/transport/sockets/socket_c.o \
./TPE1/src/transport/sockets/socket_s.o \
./TPE1/src/transport/sockets/sockets_contact_serialization.o 

C_DEPS += \
./TPE1/src/transport/sockets/socket_c.d \
./TPE1/src/transport/sockets/socket_s.d \
./TPE1/src/transport/sockets/sockets_contact_serialization.d 


# Each subdirectory must supply rules for building sources it contributes
TPE1/src/transport/sockets/%.o: ../TPE1/src/transport/sockets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


