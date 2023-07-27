/*
 * gpio.c
 *
 *  Created on: 24 de jul de 2023
 *      Author: luana
 */


#include "hw_gpio.h"

void hw_gpio_write_pin(void*gpio_port, uint16_t gpio_pin, uint32_t value){

	HAL_GPIO_WritePin((GPIO_TypeDef *)gpio_port, gpio_pin, value);
}
