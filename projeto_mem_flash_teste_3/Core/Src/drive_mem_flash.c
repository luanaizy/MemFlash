/*
 *  drive_mem_flash.c
 *
 *  Created on: 28 de jun de 2023
 *      Author: luana
 */

#include "drive_mem_flash.h"
#include "stm32f4xx_hal.h"
#include "stdint.h"


/*defines-----------------------------------------------------------------------------------------*/

#define MEM_CMD_WRITE_ENABLE          (0x06)/* @brief comando write enable na memória */
#define MEM_CMD_SECTOR_ERASE          (0x20)/* @brief comando sector erase na memória */
#define MEM_CMD_PAGE_PROGRAM          (0x02)
#define MEM_CMD_READ_DATA             (0x03)/* @brief comando read data na memória*/
#define MEM_CMD_JEDEC_ID              (0x9f)
#define MEM_CMD_READ_STATUS_REGISTER1 (0x05)

#define SPI_TIMEOUT                  (1000)/* @brief Timeout das funções de leitura e escrita SPI em ms*/

#define GET_BYTE(addr, num_byte) (addr & (0x000000FF << 8*num_byte))

void SectorErase(mem_hw_t *mem, uint32_t addr)
{

	uint8_t writeenable[1]={MEM_CMD_WRITE_ENABLE};
	uint8_t readstatusregister1[1]={MEM_CMD_READ_STATUS_REGISTER1};
	uint8_t sectorerase[4];
	sectorerase[0] = MEM_CMD_SECTOR_ERASE;
	sectorerase[1] = GET_BYTE(addr, 3 - 1);
	sectorerase[2] = GET_BYTE(addr, 3 - 2);
	sectorerase[3] = GET_BYTE(addr, 3 - 3);
	uint8_t buffer[1];

	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	 hw_spi_transmit(mem->hspi, writeenable, 1, SPI_TIMEOUT);
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	 hw_spi_transmit(mem->hspi, sectorerase, 4, SPI_TIMEOUT);
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);


	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	 hw_spi_transmit(mem->hspi, readstatusregister1, 1, SPI_TIMEOUT);
	 hw_spi_receive(mem->hspi, buffer, 1, SPI_TIMEOUT);
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);


	 uint8_t busy_erase = *(buffer) & 0x01;

	while (busy_erase == 1)
	{
		hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
		hw_spi_transmit(mem->hspi, readstatusregister1, 1, SPI_TIMEOUT);
		hw_spi_receive(mem->hspi, buffer, 1, SPI_TIMEOUT);
		hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);
		busy_erase = *(buffer) & 0x01;
	}
}


void PageProgram(mem_hw_t *mem, uint32_t addr, uint8_t *pdata,  uint16_t bytes_length )
{

	uint8_t writeenable[]={MEM_CMD_WRITE_ENABLE};
	uint8_t pageprogram[4];
	pageprogram[0] = MEM_CMD_PAGE_PROGRAM;
	pageprogram[1] = GET_BYTE(addr, 3-1);
	pageprogram[2] = GET_BYTE(addr, 3-2);
	pageprogram[3] = GET_BYTE(addr, 3-3);

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, writeenable, 1, SPI_TIMEOUT); //WRITE ENABLE
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, pageprogram, sizeof(pageprogram), SPI_TIMEOUT); //PAGE PROGRAM endereço 5 = AA BB
	hw_spi_transmit(mem->hspi, pdata, bytes_length , SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);
}



void ReadData(mem_hw_t *mem, uint32_t addr, uint8_t *received ,uint16_t size)
{
	uint8_t readdata[4];
	readdata[0] = MEM_CMD_PAGE_PROGRAM;
	readdata[1] = (GET_BYTE(addr,3-1))>>16;
	readdata[2] = (GET_BYTE(addr,3-2))>>8;
	readdata[3] = (GET_BYTE(addr,3-3));

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, readdata, 4, SPI_TIMEOUT);
	hw_spi_receive(mem->hspi, received, size, SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

}

uint32_t JedecId(mem_hw_t *mem)
{
	uint8_t jedecid[1]={MEM_CMD_JEDEC_ID};
	uint8_t received[3];
	uint32_t data;
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, jedecid,1, SPI_TIMEOUT);
	hw_spi_receive(mem->hspi, received, 3, SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	data = received[0] & (received[1]<<8) & (received[2]<<16);

	return data;
}
