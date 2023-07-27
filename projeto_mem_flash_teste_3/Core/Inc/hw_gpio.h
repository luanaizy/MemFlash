/*
 * gpio.h
 *
 *  Created on: 24 de jul de 2023
 *      Author: luana
 */

#include "stm32f4xx_hal.h"
#include "hw_errorcode.h"

#ifndef INC_HW_GPIO_H_
#define INC_HW_GPIO_H_


void hw_gpio_write_pin(void*gpio_port, uint16_t gpio_pin, uint32_t value);

#endif /* INC_HW_GPIO_H_ */
