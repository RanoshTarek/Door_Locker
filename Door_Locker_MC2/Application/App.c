/******************************************************************************
 *
 * Module: Application
 *
 * File Name: App.c
 *
 * Description: Source file for Application Tasks.
 *
 * Author: Rana Tarek
 ******************************************************************************/
#include "../Util/micro_config.h"
#include "../i2C/external_eeprom.h"
#include "../UART/UART.h"
/*******************************************************************************
 *                      Preprocessor Macros         	                       *
 *******************************************************************************/
#define PASSWORD_SIZE 8
#define PASSWORD_WRONG_COUNTER 2
#define ADDRESS_OF_EEPROM  0x0311
#define MOTOR_DIRECTION DDRC
#define MOTOR_PORT PORTC

#define MOTOR_PIN1 PC2
#define MOTOR_PIN2 PC3
#define MOTOR_PIN3 PC4
#define MOTOR_PIN4 PC5

#define LED_DIRECTION DDRC
#define LED_PORT PORTC
#define LED PC7
/*******************************************************************************
 *                      Variable (Private)                                     *
 *******************************************************************************/
static uint8 wrongPasswordCount = 0; //counter to handle wrong password count size
static bool isTimerSet = FALSE; //just flag to check if Timer on or not
static bool isDoorOpen = FALSE; //just flag to check if Timer on or not
static bool flashing = FALSE; //just flag to check if flash on or not
static unsigned char seconds = 0; //counter to count second
static uint8 count = 0; //counter to handle password size
static uint8 operation = 0; //Make default operation = '-' it used to add new  or change password,'+' it used to open Door
/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/
static void INT_TIMER_Init(void); // responsible to initial timer
static bool checkPassword(uint16 address, uint8 password[PASSWORD_SIZE]); //responsible to check password and password saved in ROM matched or not
static void flashingLEd(); //responsible for flashing LEd
static void openDoor(); //responsible for openDoor
static void closeDoor(); //responsible for closeDoor

int main(void) {
	MOTOR_DIRECTION |= (1 << MOTOR_PIN1) | (1 << MOTOR_PIN2) | (1 << MOTOR_PIN3)
			| (1 << MOTOR_PIN4); //set motor direction to be output
	MOTOR_PORT &= (~(1 << MOTOR_PIN1)) & (~(1 << MOTOR_PIN2))
			& (~(1 << MOTOR_PIN3)) & (~(1 << MOTOR_PIN4)); //initial value
	LED_DIRECTION |= (1 << LED); //set motor direction to be output
	LED_PORT |= ((1 << LED)); //make led off
	uint16 address = ADDRESS_OF_EEPROM; //Specify address of eeprom
	uint8 password[PASSWORD_SIZE]; //Array to save the password
	bool firstTime = TRUE; //just flag to check if first time or not  used to handle LCD Message
	UART_init(); //Initialize UART
	EEPROM_init(); //Initialize EEPROM
	INT_TIMER_Init(); //Initialize Timer
	while (1) {
		/* 1-check if timer is set
		 * 	 a.wrong password flash led
		 * 	 b.waiting till timer overflow ,rest timer and seconds++
		 * 	 c.if seconds equal 60
		 * 	 	-if door open close door and rest flag
		 * 	 	-if wrong password  stop flashing
		 * 	 d-rests flags
		 * 2-check operation if '-'
		 * 	 a.get key and saved in password array
		 * 	 b.then EEPROM_write data in EEPROM
		 * 3-check operation if '+'
		 * 	 a.get key and compare with saved  password EEPROM
		 * 	 b.then send response to second mirco-controller
		 * */
		if (isTimerSet) {
			if (wrongPasswordCount == PASSWORD_WRONG_COUNTER) {
				flashing = TRUE; //set flashing true
				flashingLEd(); //flashing led
			}
			// wait till the timer overflow flag is SET
			while ((TIFR & (1 << OCF1A)) == 0)
				;
			seconds++; //Increment seconds
			TCNT1 = 0; //rest timer
			TIFR |= (1 << OCF1A); //clear timer1 overflow flag
			if (seconds == 60) {
				if (isDoorOpen) {
					closeDoor(); //close door
					UART_sendByte(1); //send response to second mirco-controller
					isDoorOpen = FALSE; //rest flag
				} else if (wrongPasswordCount == PASSWORD_WRONG_COUNTER) {
					UART_sendByte(2); //send response to second mirco-controller
					LED_PORT |= (1 << LED); //stop flashing
					wrongPasswordCount = 0; //rest wrong password counter
					flashing = FALSE; //rest flag
				}
				seconds = 0; //rest wrong seconds counter
				operation = 0; //rest operation
				isTimerSet = FALSE; //rest flag
			}
			else {

			}
		}
		else if (operation == 0 && !firstTime) {
			operation = UART_recieveByte();
			count = 0; //rest counter
		} else if (operation == '-' || firstTime) { //change password door or add new password
			password[count] = UART_recieveByte();//add received key into password
			EEPROM_writeByte(address + count, password[count]); //add data to ROM
			count++; //move to next position in array
			if (count == PASSWORD_SIZE && UART_recieveByte() == 13) {
				{
					UART_sendByte(1); //send response to second mirco-controller
					if (firstTime)
						firstTime = FALSE; //rest flag
					operation = 0; //rest operation
				}
			} else {

			}
		}
		else if (operation == '+') {//open door
			password[count] = UART_recieveByte(); //save password in array
			count++; //move to next position in array
			if (count == PASSWORD_SIZE && UART_recieveByte() == 13) {
				/* 1-check if password match,open door and waiting 60 second then send response 1
				 * 2-if not send response 0 if user enter password twice flashing
				 * */
				if (checkPassword(address, password)) {
					wrongPasswordCount = 0;
					openDoor();//open door
					isDoorOpen = TRUE;//set isDoorOpen flag
					UART_sendByte(1); //send response to second mirco-controller
					if (!isTimerSet) {
						_delay_ms(200);
						isTimerSet = TRUE;//set timer
					} else {

					}
				} else {
					wrongPasswordCount++;//increment counter
					if (wrongPasswordCount == PASSWORD_WRONG_COUNTER) {
						seconds = 0; //rest seconds
						isTimerSet = TRUE; //set timer flag
						flashingLEd(); //flashing led
					} else {
						UART_sendByte(0);//send response to second mirco-controller
						count = 0; //rest count
					}
				}
			} else {

			}
		} else {

		}
	}
	return 0;
}
/************************************************************************************
 * Function Name: flashingLEd
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: void
 * Description: responsible for flashing LEd
 **********************************************************************************/
static void flashingLEd() {
	if (flashing) {
		LED_PORT = LED_PORT ^ (1 << LED);//toggle led
		_delay_ms(700);
	} else {
		LED_PORT |= (1 << LED);//off led
	}
}
/************************************************************************************
 * Function Name: openDoor
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: void
 * Description: responsible for openDoor
 **********************************************************************************/
static void openDoor() {
	MOTOR_PORT = ((12 << 2) & 0x3C) | (MOTOR_PORT & 0xC3);//rotate motor right
}
/************************************************************************************
 * Function Name: closeDoor
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: void
 * Description: responsible for closeDoor
 **********************************************************************************/
static void closeDoor() {
	MOTOR_PORT = ((6 << 2) & 0x3C) | (MOTOR_PORT & 0xC3);//rotate motor left
}
/************************************************************************************
 * Function Name: INT_TIMER_Init
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: void
 * Description: responsible to initial timer
 **********************************************************************************/
static void INT_TIMER_Init(void) {
	TCCR1B |= ((1 << CS10) | (1 << CS11));
	OCR1A = 15624;
	TCNT1 = 0;
}
/************************************************************************************
 * Function Name: checkPassword
 * Parameters (in): Password
 * 					address
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: boolean
 * Description: responsible to check password and password saved in rom matched or not
 **********************************************************************************/
static bool checkPassword(uint16 address, uint8 password[PASSWORD_SIZE]) {
	uint8 val = 0;
	for (int var = 0; var < count; ++var) {
		EEPROM_readByte(address + var, &val);
		if (password[var] != val)
			return FALSE;
	}
	return TRUE;
}
