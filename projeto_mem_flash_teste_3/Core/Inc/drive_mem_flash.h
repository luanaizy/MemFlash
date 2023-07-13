/*
 * drive_mem_flash.h
 *
 *  Created on: 28 de jun de 2023
 *      Author: luana
 */
#include "stdint.h"

#ifndef INC_DRIVE_MEM_FLASH_H_
#define INC_DRIVE_MEM_FLASH_H_

typedef struct
{
	void *hspi;
	void *gpio_port;
	uint16_t gpio_pin;
}mem_hw_t;

typedef enum{
	error_ok,
	error_timeout
}status_hw;

void hw_gpio_write_pin(void*gpio_port, uint16_t gpio_pin, uint32_t value);

status_hw hw_spi_transmit(void*hspi, uint8_t*pdata, uint16_t size, uint32_t timeout);

status_hw hw_spi_receive(void*hspi, uint8_t *pdata, uint16_t size, uint32_t timeout);

void SectorErase(mem_hw_t *mem, uint32_t addr);

void PageProgram(mem_hw_t *mem, uint32_t addr, uint8_t *pdata, uint16_t bytes_lenght );

void ReadData(mem_hw_t *mem, uint32_t addr, uint8_t *received, uint16_t size);

uint32_t JedecId(mem_hw_t *mem);


#endif /* INC_DRIVE_MEM_FLASH_H_ */
