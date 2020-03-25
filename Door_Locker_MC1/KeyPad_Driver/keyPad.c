#include "keyPad.h"

/*******************************************************************************
 *                     Private Function Prototype                              *
 *******************************************************************************/

#if(N_column==3)
/*
 * function responsible for mapping pressed number
 * */
static uint8 KeyPad_4x3_adjustSwithchNumber(uint8);
/*
 * function responsible for mapping pressed number
 * */
#elif (N_column==4)
static uint8 keypad_4x4_adjustSwitchNumber(uint8);
#endif

uint8 KeyPad_getPressedNumber(void) {
	uint8 column, row;
	while (1) {

	for (column = 0; column < N_column; ++column) {

		KEYPAD_PORT_DIR = (0b00010000 << column);
		KEYPAD_PORT_OUT = (~(0b00010000 << column));
		for (row = 0; row < N_column; ++row) {
			if (BIT_IS_CLEAR(KEYPAD_PORT_IN, row)) {
		#if(N_column==3)
					return KeyPad_4x3_adjustSwithchNumber((row * N_column) + column + 1);
		#else
					return keypad_4x4_adjustSwitchNumber((row * N_column) + column + 1);

		#endif

				}
			}

		}
	}
}

#if (N_column==3)
static uint8 KeyPad_4x3_adjustSwithchNumber(uint8 button_number) {
	{
		switch(button_number)
		{
			case 10: return '*'; // ASCII Code of *
					 break;
			case 11: return 0;
					 break;
			case 12: return '#'; // ASCII Code of #
					 break;
			default: return button_number;
		}
	}

}
#elif (N_column==4)
static uint8 keypad_4x4_adjustSwitchNumber(uint8 button_number) {
	switch(button_number)
	{
		case 1: return 7;
				break;
		case 2: return 8;
				break;
		case 3: return 9;
				break;
		case 4: return '%'; // ASCII Code of %
				break;
		case 5: return 4;
				break;
		case 6: return 5;
				break;
		case 7: return 6;
				break;
		case 8: return '*'; /* ASCII Code of '*' */
				break;
		case 9: return 1;
				break;
		case 10: return 2;
				break;
		case 11: return 3;
				break;
		case 12: return '-'; /* ASCII Code of '-' */
				break;
		case 13: return 13;  /* ASCII of Enter */
				break;
		case 14: return 0;
				break;
		case 15: return '='; /* ASCII Code of '=' */
				break;
		case 16: return '+'; /* ASCII Code of '+' */
				break;
		default: return button_number;
	}
}
#endif

