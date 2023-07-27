/*
 * drive_especifico_mem_flash.c
 *
 *  Created on: 28 de jun de 2023
 *      Author: luana
 */


#include "hw_spi.h"


status_hw hw_spi_receive(void*hspi, uint8_t *pdata, uint16_t size, uint32_t timeout){
	int errorcode;
	errorcode = HAL_SPI_Receive((SPI_HandleTypeDef *)hspi, pdata, size, timeout);
	if (errorcode == HAL_TIMEOUT){
				return error_timeout;
			}
		return error_ok;
}


status_hw hw_spi_transmit(void *hspi, uint8_t*pdata, uint16_t size, uint32_t timeout){
	int errorcode;
	errorcode = HAL_SPI_Transmit((SPI_HandleTypeDef *)hspi, pdata, size, timeout);
	if (errorcode == HAL_TIMEOUT){
			return error_timeout;
		}
	return error_ok;
}
