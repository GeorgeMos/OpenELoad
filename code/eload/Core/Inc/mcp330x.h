/*
 * mcp330x.h
 *
 *  Created on: Jul 4, 2024
 *      Author: georg
 */

#ifndef INC_MCP330X_H_
#define INC_MCP330X_H_

//#ifdef __cplusplus
//extern "C"
//{
//#endif

#include "stdint.h"
#include "stm32f4xx_hal.h"
#define SPI_TRANSFER_LENGTH 3

enum READ_MODE
{
	SINGLE = 0,
	DIFFERENTIAL = 1
};

class mcp3302
{
private:
	SPI_HandleTypeDef hspi;
	uint16_t CS;
	GPIO_TypeDef* CSB;
	//Data
	uint8_t data[SPI_TRANSFER_LENGTH];
	uint8_t spiBuff[SPI_TRANSFER_LENGTH];
	int channels = 4;
	//Instructions
	const uint8_t startBit = 0b10000000;
	const uint8_t differentialMode = 0b00000000;
	const uint8_t singleCMode = 0b01000000;
	const uint8_t NUM_BITS = 12;


public:
	mcp3302(GPIO_TypeDef* CSB, uint16_t CS, SPI_HandleTypeDef hspi);
	uint16_t read(int channel, int mode);
	uint16_t readDMA(int channel, int mode);

};

//#ifdef __cplusplus
//}
//#endif

#endif /* INC_MCP330X_H_ */
