/*
 * @file: hw_spi.c
 * @brief funções de transmissão e recepção spi
 *  Created on: 28 de jun de 2023
 *      Author: luana
 */


#include "hw_spi.h"

/**
 * @brief função hw_spi_receive - função bloqueante que recebe dados via spi.
 * @param ponteiro para estrutura hspi do tipo void.
 * @param ponteiro para vetor para recepção dos dados do tipo uint8_t.
 * @param uint16_t com o tamanho do dado a ser recebido em bytes.
 * @param uint32_t com o tempo de timeout da função.
 * @retval status_hw de erro.
 */
status_hw hw_spi_receive(void*hspi, uint8_t *pdata, uint16_t size, uint32_t timeout){
	int errorcode;
	errorcode = HAL_SPI_Receive((SPI_HandleTypeDef *)hspi, pdata, size, timeout);
	if (errorcode == HAL_TIMEOUT){
				return error_timeout;
			}
		return error_ok;
}

/**
 * @brief função hw_spi_transmit - função bloqueante que transmite dados via spi.
 * @param ponteiro para estrutura hspi do tipo void.
 * @param ponteiro para vetor com os dados a serem transmitidos do tipo uint8_t
 * @param uint16_t com o tamanho do dado a ser transmitido em bytes.
 * @param uint32_t com o tempo de timeout da função.
 * @retval status hw de erro.
 */
status_hw hw_spi_transmit(void *hspi, uint8_t*pdata, uint16_t size, uint32_t timeout){
	int errorcode;
	errorcode = HAL_SPI_Transmit((SPI_HandleTypeDef *)hspi, pdata, size, timeout);
	if (errorcode == HAL_TIMEOUT){
			return error_timeout;
		}
	return error_ok;
}
