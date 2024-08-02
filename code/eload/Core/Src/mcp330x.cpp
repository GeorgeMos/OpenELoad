/*
 * mcp330x.cpp
 *
 *  Created on: Jul 4, 2024
 *      Author: georg
 */
//#ifdef __cplusplus
//extern "C"
//{
//#endif

#include <mcp330x.h>
#include "stdint.h"
#include "stm32f4xx_hal.h"

mcp3302::mcp3302(GPIO_TypeDef* CSB, uint16_t CS, SPI_HandleTypeDef hspi)
{
	this->hspi = hspi;
	this->CS = CS;
	this->CSB = CSB;
	HAL_GPIO_WritePin(CSB, CS, GPIO_PIN_SET);
}

uint16_t mcp3302::read(int channel, int mode)
{
	spiBuff[0] = 0;
	spiBuff[1] = 0;
	spiBuff[2] = 0;
	this->data[0] = (this->startBit) | ((!mode) << 6) | (channel << 3);
	this->data[1] = 0;
	this->data[2] = 0;
	HAL_GPIO_WritePin(CSB, CS, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi, this->data, this->spiBuff, 3, 100);
	HAL_GPIO_WritePin(CSB, CS, GPIO_PIN_SET);
	unsigned sb = spiBuff[0] & 0x01;
	return (sb ?
			(~((spiBuff[1]) << 4 | (spiBuff[2] & 0xF0) >> 4))+1 :
			((spiBuff[1]) << 4 | (spiBuff[2] & 0xF0) >> 4));
}

uint16_t mcp3302::readDMA(int channel, int mode)
{
	spiBuff[0] = 0;
	spiBuff[1] = 0;
	spiBuff[2] = 0;
	this->data[0] = (this->startBit) | ((!mode) << 6) | (channel << 3);
	this->data[1] = 0;
	this->data[2] = 0;
	HAL_GPIO_WritePin(CSB, CS, GPIO_PIN_RESET);
	HAL_StatusTypeDef err = HAL_SPI_TransmitReceive_DMA(&hspi, this->data, this->spiBuff, 3);
	//HAL_SPI_TransmitReceive(&hspi, this->data, this->spiBuff, 3, 100);
	while(err != HAL_OK){}
	HAL_GPIO_WritePin(CSB, CS, GPIO_PIN_SET);
	unsigned sb = spiBuff[0] & 0x01;
	return (sb ?
			(~((spiBuff[1]) << 4 | (spiBuff[2] & 0xF0) >> 4))+1 :
			((spiBuff[1]) << 4 | (spiBuff[2] & 0xF0) >> 4));
}

//#ifdef __cplusplus
//}
//#endif


