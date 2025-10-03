/*
 * hwInterface.h
 *
 *  Created on: Jul 5, 2024
 *      Author: georg
 */

#ifndef INC_HWINTERFACE_H_
#define INC_HWINTERFACE_H_
#include <FIRFilter.h>
#include "pid.h"
#include "mcp330x.h"
#include "stm32f4xx_hal.h"
#include "flashMem.h"

#define COFFSET_FLASH_ADDR 0x0803FFC0
#define KP_FLASH_ADDR 0x0803FFD0
#define KI_FLASH_ADDR 0x0803FFE0
#define KD_FLASH_ADDR 0x0803FFF0

#define PWM_BASELINE 650

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;

class hw
{
private:
	mcp3302 *adc;
	FIRFilter mavfVoltage;
	FIRFilter mavfVoltageF;
	FIRFilter mavfCurrent;
	PID *cPid;
	float vPoints;
	float adcVREF;
	double Kp, Ki, Kd;
	float cMargin = 0.01;

	float integral = 0;
	float prevError = 0;



public:
	hw();
	double pwm; //0-TIM1->ARR
	bool fWire;
	bool sampling;
	int samples;
	float cOffset;
	double cvolt;
	double ccur;
	double cpow;
	double scur;
	//Reads the differentioal voltage on CH0(+) - CH1(-)
	//float readVoltageF();
	float readVoltage();
	float readVoltageFiltered();
	float readVoltageFilteredF();
	float readW();
	//Reads the voltage on CH3, connected to c sensor (Current Sensor Voltage)
	float readC();
	float readCFiltered();
	void setGateVoltage(int pwm);
	void setC(float c);
	void calC(int times);
	void loadCalibratedValues();
	void saveCalibratedValues();

	void setKp(double Kp);
	void setKi(double Ki);
	void setKd(double Kd);

	double getKp();
	double getKi();
	double getKd();
};



#endif /* INC_HWINTERFACE_H_ */
