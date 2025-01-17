/*
 * hwInterface.cpp
 *
 *  Created on: Jul 5, 2024
 *      Author: georg
 */

#include "hw_Interface.h"

hw::hw()
{
	//HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//TIM1->CCR1 = 0;
	this->adc = new mcp3302(GPIOA, GPIO_PIN_2, hspi1);
	this->adcVREF = 3.3;
	this->fWire = 0;
	this->pwm = PWM_BASELINE;
	this->sampling = 1;	//TODO: Implement eeprom storing for sampling and samples values.
	this->samples = 450;
	this->cOffset = 0;
	//this->loadCalibratedValues();
	this->ccur = 0;
	this->cvolt = 0;
	this->scur = 0;
	this->Kp = 0.5;
	this->Ki = 5000;
	this->Kd = 0.00001;
	FIRFilter_Init(&this->mavfVoltage);
	FIRFilter_Init(&this->mavfVoltageF);
	FIRFilter_Init(&this->mavfCurrent);

	this->cPid = new PID(&this->ccur, &this->pwm, &this->scur, this->Kp, this->Ki, this->Kd, _PID_CD_DIRECT);
	this->cPid->SetMode(_PID_MODE_AUTOMATIC);
	this->cPid->SetSampleTime(1);
	this->cPid->SetOutputLimits(PWM_BASELINE, 1000);
	//this->cMargin = 0.01;
}

void hw::saveCalibratedValues()
{
	uint32_t cOffsetBITS = 0;
	memcpy(&cOffsetBITS, &this->cOffset, sizeof(cOffsetBITS));
	Flash_Write_Data(COFFSET_FLASH_ADDR, &cOffsetBITS, 1);

	//TODO::Store pid values also
}

void hw::loadCalibratedValues()
{
	static_assert(sizeof(float) == sizeof this->cOffset, "`float` has a weird size.");
	uint32_t cOffsetBITS = 0;
	Flash_Read_Data(COFFSET_FLASH_ADDR, &cOffsetBITS, 1);
	memcpy(&this->cOffset, &cOffsetBITS, sizeof(float));

	//TODO::Read pid values also
}

/*
float hw::readVoltageFilteredF()
{
	int16_t value;
	float v = 0;
	value = (int16_t)this->adc->read(2, 0);
	v = value * (adcVREF / 4095)*10; //Divided to have a range of 0-33v

	//for(int i = 0; i < 10; i++){
		//value = (int16_t)this->adc->read(1, 0);
		//this->mavf->buf[i] =  value * (adcVREF / 4095)*10; //Divided to have a range of 0-33v
	//}

	this->cvolt = FIRFilter_Update(&this->mavfVoltageF, v);
	return cvolt;
}
*/

float hw::readVoltageFiltered()
{
	int16_t value;
	float v = 0;
	if(this->fWire)
	{
		value = (int16_t)this->adc->read(2, 0);
	}
	else
	{
		value = (int16_t)this->adc->read(1, 0);
	}

	v = value * (adcVREF / 4095)*10; //Divided to have a range of 0-33v

	//for(int i = 0; i < 10; i++){
		//value = (int16_t)this->adc->read(1, 0);
		//this->mavf->buf[i] =  value * (adcVREF / 4095)*10; //Divided to have a range of 0-33v
	//}

	this->cvolt = FIRFilter_Update(&this->mavfVoltage, v);
	return cvolt;
}

float hw::readW()
{
	this->cpow = this->cvolt * this->ccur;
	return this->cpow;
}

float hw::readC()
{
	int16_t value;
	float c = 0;
	if(sampling)
	{
		for(int i = 0; i < this->samples; i++){
			value = (int16_t)this->adc->read(3, 0);
			if(((value * (adcVREF / 4095) - 2.5)*10) > 0)
			{
				c += ((value * (adcVREF / 4095) - 2.5)*10) + this->cOffset;
			}

		}
		c = c/this->samples;
	}
	else
	{
		value = (int16_t)this->adc->read(3, 0);
		c = ((value * (adcVREF / 4095) - 2.5)*10) + this->cOffset;
		if(c < 0){c = 0;}
	}

	if(c < 0){c = 0;}
	return c;
}

float hw::readCFiltered()
{
	int16_t value;
	float c = 0;
	value = (int16_t)this->adc->read(3, 0);
	c = ((value * (adcVREF / 4095) - 2.5)*10) + this->cOffset;
	this->ccur = FIRFilter_Update(&this->mavfCurrent, c);

	return this->ccur;
}


void hw::setGateVoltage(int pwm)
{
	TIM1->CCR1 = pwm;
}

void hw::setC(float c)
{
	this->scur = c;
	this->ccur = this->readCFiltered();
	this->cPid->Compute();
	this->setGateVoltage(this->pwm);

}

void hw::calC(int times)
{
	float current;

	this->setC(0);
	for(int i = 0; i < times; i++)
	{
		current = this->readCFiltered();
		if(current > 0){this->cOffset -= 0.001;}
		else if(current < 0){this->cOffset += 0.001;}
	}

}
