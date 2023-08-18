/*
 * @file: hw_gpio.c
 * @brief função de escrita no gpio.
 *  Created on: 24 de jul de 2023
 *      Author: luana
 */


#include "hw_gpio.h"

/**
 * @brief função hw_gpio_write_pin - função bloqueante que escreve no gpio.
 * @param ponteiro para porta do gpio do tipo void.
 * @param uint16_t pino do gpio.
 * @param uint32_t com o valor a ser escrito no gpio.
 * @retval void
 */
void hw_gpio_write_pin(void*gpio_port, uint16_t gpio_pin, uint32_t value){

	HAL_GPIO_WritePin((GPIO_TypeDef *)gpio_port, gpio_pin, value);
}
