################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EEPROM/External_EEPROM_I2C.c \
../EEPROM/external_eeprom.c \
../EEPROM/i2c.c 

OBJS += \
./EEPROM/External_EEPROM_I2C.o \
./EEPROM/external_eeprom.o \
./EEPROM/i2c.o 

C_DEPS += \
./EEPROM/External_EEPROM_I2C.d \
./EEPROM/external_eeprom.d \
./EEPROM/i2c.d 


# Each subdirectory must supply rules for building sources it contributes
EEPROM/%.o: ../EEPROM/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


