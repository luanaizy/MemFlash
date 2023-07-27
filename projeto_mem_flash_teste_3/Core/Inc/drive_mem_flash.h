/*
 * drive_mem_flash.h
 *
 *  Created on: 28 de jun de 2023
 *      Author: luana
 */

#include "stdint.h"
#include "hw_spi.h"
#include "hw_gpio.h"
#include "hw_errorcode.h"

#ifndef INC_DRIVE_MEM_FLASH_H_
#define INC_DRIVE_MEM_FLASH_H_


typedef struct
{
	void *hspi;
	void *gpio_port;
	uint16_t gpio_pin;
}mem_hw_t;

static void writeEnable(mem_hw_t *mem);

void sectorErase(mem_hw_t *mem, uint32_t addr);

void pageProgram(mem_hw_t *mem, uint32_t addr, uint8_t *pdata, uint16_t bytes_lenght );

void readData(mem_hw_t *mem, uint32_t addr, uint8_t *received, uint16_t size);

uint32_t jedecId(mem_hw_t *mem);


#endif /* INC_DRIVE_MEM_FLASH_H_ */
