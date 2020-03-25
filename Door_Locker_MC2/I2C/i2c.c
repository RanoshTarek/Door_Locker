#include "i2c.h"

void TWI_init(void) {
	TWBR = 0x02;
	TWSR = 0x00;
	TWCR = (1 << TWEN);
	TWAR = 0b00000010;

}
void TWI_start(void) {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;

}
void TWI_stop(void) {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO);
}
void TWI_write(uint8 data) {
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (BIT_IS_CLEAR(TWCR, TWINT))
		;
}
uint8 TWI_readWithACK(void) {
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
	while (BIT_IS_CLEAR(TWCR, TWINT));
	return TWDR;

}
uint8 TWI_readWithNACK(void) {

	TWCR = (1 << TWEN) | (1 << TWINT);
	while (BIT_IS_CLEAR(TWCR, TWINT));
	return TWDR;

}
uint8 TWI_getStatus(void){
	uint8 status=(TWSR&0xF8);
	return status;
}
