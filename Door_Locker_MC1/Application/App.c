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
#include "../KeyPad_Driver/KeyPad.h"
#include "../UART/uart.h"
#include "../LCD_Driver/lcd.h"
#include "../LCD_Driver/LCDMsg.h"
/*******************************************************************************
 *                      Preprocessor Macros         	                       *
 *******************************************************************************/
#define PASSWORD_SIZE 8
/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/
static bool checkPassword(uint8 password[], uint8 re_password[]);/*responsible to check password and re-password matched or not*/
static void sendDataToSecondMircoController(uint8 key);/*responsible to send data to Second MircoController*/
static void displayCharacter(uint8 key, uint8 count);/* responsible to display pressed key and replace it with '*' */
static void restLcd();/*responsible the rest LCD and display choose to the user*/
/*******************************************************************************
 *                      Variable (Private)                                     *
 *******************************************************************************/
static bool firstTime = TRUE; //just flag to check if first time or not  used to handle LCD Message
static uint8 count = 0; //counter to handle password size
static uint8 operation = '-'; //Make default operation = '-' it used to add new  or change password,'+' it used to open Door

int main(void) {
	uint8 password[PASSWORD_SIZE]; //Array to save the password
	uint8 re_password[PASSWORD_SIZE]; //Array to save re-enter the password
	bool isPassword = TRUE; //boolean to check where save password and with initial value true to start with password
	bool isRePassword = FALSE; //boolean to check where save password and with initial value false to start with password
	uint8 pressedKey; //variable to save pressed key from keypad
	UART_init(); //Initialize UART
	LCD_init(); //Initialize LCD
	LCD_displayStringRowColumn(0, 0, ENTER_YOUR_PASSWORD); //display message into LCD
	while (1) {
		pressedKey = KeyPad_getPressedNumber(); //get the pressed pressedKey
		_delay_ms(500);
		/* 1-check if pressed key is number and Counter small than Password size
		 * 2-check operation if '-'
		 * 	 a.get key and saved in password array
		 * 	 b.then get key and saved in re-password array
		 * 	 c.compare two arrays if matched saved password not go back to step a
		 * 3-reset operation to make user choose operation
		 * 	 a.'+' Open Door
		 * 	 b.'-' Change password
		 */
		if ((pressedKey >= 0) && (pressedKey <= 9) && count < PASSWORD_SIZE) {
			if (operation == '-') {
				if (isPassword) {
					password[count] = pressedKey; //add element to array
					displayCharacter(pressedKey, count); //display char into LCD
					count++; //move to next position in array
					if (count == PASSWORD_SIZE) { // if array full move to re-password
						isPassword = FALSE; //rest flag
						isRePassword = TRUE; //change re-password flag
						count = 0; //rest counter
						LCD_clearScreen(); //clear LCD
						LCD_displayStringRowColumn(0, 0, ENTER_YOUR_REPASSWORD); //display message into LCD
						continue; //stop this iteration and start over
					}
				} else if (isRePassword) {
					re_password[count] = pressedKey; //add element to array
					displayCharacter(pressedKey, count); //display char into LCD
					count++; //move to next position in array
					if (count == PASSWORD_SIZE) { // if array full check matched between two array
						isRePassword = FALSE; //rest flag
						count = 0; //rest counter
						isPassword = TRUE; //rest flag
						isRePassword = FALSE; //rest flag
						/* 1-check if password match,save password
						 * 2-if not go to step 1
						 * */
						if (checkPassword(password, re_password)) {
							_delay_ms(150);
							LCD_clearScreen(); //display message into LCD
							LCD_displayStringRowColumn(0, 0, PASSWORD_MATCHED); //display message into LCD
							LCD_displayStringRowColumn(1, 0, LOADING); //display message into LCD
							for (int i = 0; i < PASSWORD_SIZE; ++i) {
								sendDataToSecondMircoController(password[i]); //send char to second Mirco-control
							}
							sendDataToSecondMircoController(13); //notify second Mirco-control to save data it's like enter
							operation = 0; //rest operation
							continue; //stop this iteration and start over
						} else {
							LCD_clearScreen(); //clear LCD
							LCD_displayStringRowColumn(0, 0,
							NOT_PASSWORD_MATCHED); //display message into LCD
							_delay_ms(200);
							LCD_clearScreen(); //clear LCD
							LCD_displayStringRowColumn(0, 0,
							ENTER_YOUR_PASSWORD); //display message into LCD
						}
					} else {

					}
				} else {

				}
			} else {

			}
		}
		//Check operation if 0 get operation or if '+' get pressed password and send it to second mirco-control
		if (operation == 0) {
			//Check operation if between + or - send operation not display error massage to user
			if (pressedKey == '+' || pressedKey == '-') {
				UART_sendByte(pressedKey); //Send operation to second mirco-control
				operation = pressedKey; //assign operation
				LCD_clearScreen(); //clear LCD
				LCD_displayStringRowColumn(0, 0, ENTER_YOUR_PASSWORD); //display message into LCD
			} else if ((pressedKey >= 0) && (pressedKey <= 9)) {
				LCD_clearScreen(); //clear LCD
				LCD_displayStringRowColumn(0, 0, CHOOSE_RIGHT); //display message into LCD
				LCD_displayStringRowColumn(1, 0, OPERTAION); //display message into LCD
			} else {

			}
		} else if (operation == '+') {
			if ((pressedKey >= 0) && (pressedKey <= 9) && count < PASSWORD_SIZE) { //check if pressed key is number and Counter small than Password size
				displayCharacter(pressedKey, count); //display char into LCD
			} else {

			}
			sendDataToSecondMircoController(pressedKey); //send char to second Mirco-control
		} else {

		}
	}
}
/************************************************************************************
 * Function Name: sendDataToSecondMircoController
 * Parameters (in): pressedKey-uint8
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: responsible to send data to Second MircoController
 **********************************************************************************/
static void sendDataToSecondMircoController(uint8 pressedKey) {
	static uint8 responeCode;
	if ((pressedKey >= 0) && (pressedKey <= 9) && count < PASSWORD_SIZE) {
		UART_sendByte(pressedKey); //send the pressed pressedKey to the second MC using uart
		_delay_ms(500);
		count++; //move to next position in array
	} else if (pressedKey == 13) {
		if (count < PASSWORD_SIZE) {
			LCD_displayStringRowColumn(0, 0, PASSWORD_ERROR_MSSAGE); //display message into LCD
		} else if (count == PASSWORD_SIZE) {
			UART_sendByte(pressedKey); //send the pressed pressedKey to the second MC using uart
			responeCode = UART_recieveByte(); //Response from second mirco-control
			LCD_clearScreen(); //clear LCD
			if (responeCode == 1 && firstTime) { //password saved
				LCD_displayStringRowColumn(0, 0, DATA_SAVED); //display message into LCD
				restLcd(); //rest LCD
			} else if (responeCode == 1 && operation == '+') { //Correct password
				LCD_displayStringRowColumn(0, 0, WECLOME); //display message into LCD
				if (UART_recieveByte()) { //door open for 1 minute so waiting until second  mirco-control send flag
					restLcd(); //rest LCD
				}
			} else if (responeCode == 1 && operation == '-') { //password changed successfully
				LCD_displayStringRowColumn(0, 0, CHANAGE_PASSWORD); //display message into LCD
				restLcd(); //rest LCD
			} else if (responeCode == 0 && operation == '+') { //Wrong password
				LCD_displayStringRowColumn(0, 0, WRONG_TRY_AGAIN); //display message into LCD
				count = 0; //rest count
			} else if (responeCode == 2 && operation == '+') { //Password enter wrong 2 times
				_delay_ms(500);
				restLcd(); //rest LCD
			} else {

			}
			if (firstTime)
				firstTime = FALSE; //rest flag
			else {
			}
		}

	} else {
	}

}
/************************************************************************************
 * Function Name: restLcd
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: responsible the rest LCD and display choose to the user
 **********************************************************************************/
static void restLcd() {
	_delay_ms(500);
	LCD_clearScreen(); //rest LCD
	count = 0; //rest count
	operation = 0; //rest operation
	LCD_displayStringRowColumn(0, 0, OPEN_DOOR); //display message into LCD
	LCD_displayStringRowColumn(1, 0, CHANGE_PASSWORD); //display message into LCD
}
/************************************************************************************
 * Function Name: checkPassword
 * Parameters (in): Password
 * 					Re-password
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: boolean
 * Description: responsible to check password and re-password matched or not
 **********************************************************************************/
static bool checkPassword(uint8 password[], uint8 re_password[]) {
	for (int i = 0; i < PASSWORD_SIZE; ++i) {
		if (password[i] != re_password[i])
			return FALSE;
		else {

		}
	}
	return TRUE;
}
/************************************************************************************
 * Function Name: displayCharacter
 * Parameters (in): pressedKey - uint8
 * 					count 		-uint8
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: none
 * Description: responsible to display pressed key and replace it with *
 **********************************************************************************/
static void displayCharacter(uint8 pressedKey, uint8 count) {
	LCD_goToRowColumn(1, count);
	LCD_intgerToString(pressedKey); //display integer into LCD
	_delay_ms(150);
	LCD_displayStringRowColumn(1, count, "*"); //display the '*'
}
