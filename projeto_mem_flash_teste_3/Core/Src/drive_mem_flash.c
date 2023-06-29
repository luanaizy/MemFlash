/*
 *  drive_mem_flash.c
 *
 *  Created on: 28 de jun de 2023
 *      Author: luana
 */

#include "drive_mem_flash.h"
#include "stm32f4xx_hal.h"

/*defines-----------------------------------------------------------------------------------------*/

#define MEM_CMD_WRITE_ENABLE          (0x06)/* @brief comando write enable na memória */
#define MEM_CMD_SECTOR_ERASE          (0x20)/* @brief comando sector erase na memória */
#define MEM_CMD_PAGE_PROGRAM          (0x02)
#define MEM_CMD_READ_DATA             (0x03)/* @brief comando read data na memória*/
#define MEM_CMD_JEDEC_ID              (0x9f)
#define MEM_CMD_READ_STATUS_REGISTER1 (0x05)

#define SPI_TIMEOUT                  (1000)/* @brief Timeout das funções de leitura e escrita SPI em ms*/



void SectorErase(mem_hw_t *mem, uint32_t addr){

	uint8_t writeenable[1]={MEM_CMD_WRITE_ENABLE};
	uint8_t readstatusregister1[1]={MEM_CMD_READ_STATUS_REGISTER1};
	uint8_t sectorerase[4];
	sectorerase[0] = MEM_CMD_SECTOR_ERASE;
	sectorerase[1] = addr & 0x00FF0000;
	sectorerase[2] = addr & 0X0000FF00;
	sectorerase[3] = addr & 0X000000FF;
	uint8_t buffer[1];

	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	 hw_spi_transmit(mem->hspi, writeenable, 1, SPI_TIMEOUT); //WRITE ENABLE
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	 hw_spi_transmit(mem->hspi, sectorerase, 4, SPI_TIMEOUT); //SECTOR ERASE endereço 5
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);


	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	 hw_spi_transmit(mem->hspi, readstatusregister1, 1, SPI_TIMEOUT);
	 hw_spi_receive(mem->hspi, buffer, 1, SPI_TIMEOUT);
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);


	 uint8_t busy_erase = *(buffer) & 0x01;

	while (busy_erase == 1){
		hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
		hw_spi_transmit(mem->hspi, readstatusregister1, 1, SPI_TIMEOUT);
		hw_spi_receive(mem->hspi, buffer, 1, SPI_TIMEOUT);
		hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);
		busy_erase = *(buffer) & 0x01;
}


void PageProgram(mem_hw_t *mem, uint32_t addr, uint8_t *pdata ){

	uint8_t writeenable[]={MEM_CMD_WRITE_ENABLE};
	uint8_t pageprogram[sizeof(pdata)+4];
	pageprogram[0] = MEM_CMD_PAGE_PROGRAM;
	pageprogram[1] = addr & 0x00FF0000;
	pageprogram[2] = addr & 0X0000FF00;
	pageprogram[3] = addr & 0X000000FF;

	for (int i=0;i<sizeof(pdata);i++){
		pageprogram[i+4] = pdata[i];
	}

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, writeenable, 1, SPI_TIMEOUT); //WRITE ENABLE
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	 hw_spi_transmit(mem->hspi, pageprogram, sizeof(pageprogram), SPI_TIMEOUT); //PAGE PROGRAM endereço 5 = AA BB
	 hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);
}



uint8_t ReadData(mem_hw_t *mem, uint32_t addr, uint16_t size){
	uint8_t readdata[4];
	readdata[0] = MEM_CMD_PAGE_PROGRAM;
	readdata[1] = (addr & 0x00FF0000)>>4;
	readdata[2] = (addr & 0X0000FF00)>>2;
	readdata[3] = addr & 0X000000FF;
	uint8_t received[size];

	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, readdata, 4, SPI_TIMEOUT);
	hw_spi_receive(mem->hspi, received, sizeof(pageprogram), SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	return received;
}

uint8_t JedecId(mem_hw_t *mem){
	uint8_t jedecid[1]={MEM_CMD_JEDEC_ID};
	uint8_t received[3];
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 0);
	hw_spi_transmit(mem->hspi, readdata, 4, SPI_TIMEOUT);
	hw_spi_receive(mem->hspi, received, 3, SPI_TIMEOUT);
	hw_gpio_write_pin(mem->gpio_port, mem->gpio_pin, 1);

	return received;
}
