################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../EEPROM_I2C/External_EEPROM_I2C.c \
../EEPROM_I2C/external_eeprom.c \
../EEPROM_I2C/i2c.c 

OBJS += \
./EEPROM_I2C/External_EEPROM_I2C.o \
./EEPROM_I2C/external_eeprom.o \
./EEPROM_I2C/i2c.o 

C_DEPS += \
./EEPROM_I2C/External_EEPROM_I2C.d \
./EEPROM_I2C/external_eeprom.d \
./EEPROM_I2C/i2c.d 


# Each subdirectory must supply rules for building sources it contributes
EEPROM_I2C/%.o: ../EEPROM_I2C/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


