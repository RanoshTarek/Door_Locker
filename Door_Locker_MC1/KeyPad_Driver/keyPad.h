/*
 * keyPad.h
 *
 *  Created on: Feb 4, 2020
 *      Author: Rana
 */

#ifndef KEYPAD_DRIVER_KEYPAD_H_
#define KEYPAD_DRIVER_KEYPAD_H_


#include "../Util/std_types.h"
#include "../Util/micro_config.h"
#include "../Util/common_macros.h"
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* keypad number of row and column configuration */
#define N_row 4
#define N_column 4
/*KeyPad Port Configuration*/
#define KEYPAD_PORT_OUT PORTA
#define KEYPAD_PORT_IN PINA
#define KEYPAD_PORT_DIR DDRA

/*******************************************************************************
 *                      Function Prototype                                   *
 *******************************************************************************/
uint8 KeyPad_getPressedNumber(void);

#endif /* KEYPAD_DRIVER_KEYPAD_H_ */
