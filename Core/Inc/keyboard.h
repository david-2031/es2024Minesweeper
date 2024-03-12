/*
 * keyboard.h
 *
 *  Created on: Mar 9, 2024
 *      Author: cedricpurwin
 */

#ifndef INC_KEYBOARD_H_
#define INC_KEYBOARD_H_

#include "stdint.h"
#include "spi.h"

void keyboard_init(SPI_HandleTypeDef *hspi, uint8_t ms_rate);
void get_key_1ms(void);
uint8_t is_key(void);

#endif /* INC_KEYBOARD_H_ */
