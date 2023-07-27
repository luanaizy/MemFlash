/*
 * spi.h
 *
 *  Created on: 24 de jul de 2023
 *      Author: luana
 */
#include "stm32f4xx_hal.h"
#include "hw_errorcode.h"

#ifndef INC_HW_SPI_H_
#define INC_HW_SPI_H_


status_hw hw_spi_transmit(void*hspi, uint8_t*pdata, uint16_t size, uint32_t timeout);

status_hw hw_spi_receive(void*hspi, uint8_t *pdata, uint16_t size, uint32_t timeout);

#endif /* INC_HW_SPI_H_ */
