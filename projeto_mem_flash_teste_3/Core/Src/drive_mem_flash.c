/*
 *  drive_mem_flash.c
 *
 *  Created on: 28 de jun de 2023
 *      Author: luana
 */

#include "drive_mem_flash.h"



/*defines-----------------------------------------------------------------------------------------*/

#define MEM_CMD_WRITE_ENABLE          (0x06)/* @brief código do comando write enable na memória */
#define MEM_CMD_SECTOR_ERASE          (0x20)/* @brief código do comando sector erase na memória */
#define MEM_CMD_PAGE_PROGRAM          (0x02)/* @brief código do comando page program na memória*/
#define MEM_CMD_READ_DATA             (0x03)/* @brief código do comando read data na memória*/
#define MEM_CMD_JEDEC_ID              (0x9f)/* @brief código do comando jedec id na memória*/
#define MEM_CMD_READ_STATUS_REGISTER1 (0x05)/* @brief código do comando read status register 1 na memória*/

#define SPI_TIMEOUT                  (1000)/* @brief Timeout das funções de leitura e escrita SPI em ms*/

#define GET_BYTE(addr, num_byte) (addr & (0x000000FF << (8*num_byte)))

static void writeEnable(mem_hw_t *mem)
{
	uint8_t writeenable[1]={MEM_CMD_WRITE_ENABLE};
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, writeenable,sizeof(writeenable), SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);
}

void sectorErase(mem_hw_t *mem, uint32_t addr)
{



	uint8_t sectorerase[4];
	sectorerase[0] = MEM_CMD_SECTOR_ERASE;
	sectorerase[1] = GET_BYTE(addr, 2)>>16;
	sectorerase[2] = GET_BYTE(addr, 1)>>8;
	sectorerase[3] = GET_BYTE(addr, 0);


	writeEnable(mem);

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, sectorerase, 4, SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	uint8_t readstatusregister1[1]={MEM_CMD_READ_STATUS_REGISTER1};
	uint8_t busy_erase;
	uint8_t buffer[1];

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, readstatusregister1, 1, SPI_TIMEOUT);
	do
	{
		hw_spi_receive(mem->hspi, buffer, 1, SPI_TIMEOUT);
		busy_erase = *(buffer) & 0x01;
	}while (busy_erase == 1);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

}


void pageProgram(mem_hw_t *mem, uint32_t addr, uint8_t *pdata,  uint16_t bytes_length )
{

	uint8_t pageprogram[4];
	pageprogram[0] = MEM_CMD_PAGE_PROGRAM;
	pageprogram[1] = GET_BYTE(addr, 2)>>16;
	pageprogram[2] = GET_BYTE(addr, 1)>>8;
	pageprogram[3] = GET_BYTE(addr, 0);

	writeEnable(mem);

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, pageprogram, sizeof(pageprogram), SPI_TIMEOUT);
	hw_spi_transmit(mem->hspi, pdata, bytes_length , SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	uint8_t buffer[1];
	uint8_t busy_write;
	uint8_t readstatusregister1[1]={MEM_CMD_READ_STATUS_REGISTER1};
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, readstatusregister1, 1, SPI_TIMEOUT);
	do
	{
		hw_spi_receive(mem->hspi, buffer, 1, SPI_TIMEOUT);
		busy_write = *(buffer) & 0x01;
	}while (busy_write == 1);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

}



void readData(mem_hw_t *mem, uint32_t addr, uint8_t *received ,uint16_t size)
{
	uint8_t readdata[4];
	readdata[0] = MEM_CMD_READ_DATA;
	readdata[1] = (GET_BYTE(addr,2))>>16;
	readdata[2] = (GET_BYTE(addr,1))>>8;
	readdata[3] = (GET_BYTE(addr,0));

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, readdata, sizeof(readdata), SPI_TIMEOUT);
	hw_spi_receive(mem->hspi, received, size, SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

}

uint32_t jedecId(mem_hw_t *mem)
{
	uint8_t jedecid[1]={MEM_CMD_JEDEC_ID};
	uint8_t received[3];
	uint32_t data;
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, jedecid,sizeof(jedecid), SPI_TIMEOUT);
	hw_spi_receive(mem->hspi, received, sizeof(received), SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	data = received[2] | (received[1]<<8) | (received[0]<<16);

	return data;
}
