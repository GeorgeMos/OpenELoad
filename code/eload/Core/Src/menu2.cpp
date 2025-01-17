/*
 * menu2.cpp
 *
 *  Created on: Jul 19, 2024
 *      Author: georg
 */

#include "menu2.h"
#include "liquidcrystal_i2c.h"
#include "stdio.h"


Menu::Menu()
{
	this->currState = STATES::MODE_SEL;
	this->hwInterface = new hw();
	this->count = 0;
	this->open = 0;
	this->fWire = 0;
	this->dir = 0;
	this->menuPage = 0;
	this->cursorIndex = 0;
	this->setC = 0;
	this->setR = 0;
	this->setW = 0;
	HD44780_Init(4);
	HD44780_Clear();
}

void Menu::run()
{
	switch(this->currState)
	{
	case STATES::MODE_SEL:
		ModeSel();
		break;
	case STATES::CC_MODE:
		CC_Mode();
		break;
	case STATES::CR_MODE:
		CR_Mode();
		break;
	case STATES::CP_MODE:
		CP_Mode();
		break;
	case STATES::TRANS_MODE:
		TRANS_Mode();
		break;
	case STATES::CAL_MODE:
		CAL_Mode();
		break;
	case STATES::C_CAL:
		cCal_Mode();
		break;
	case STATES::V_CAL:
		vCal_Mode();
		break;
	case STATES::PID_CAL:
		pidCal_Mode();
		break;

	default:
		break;
	}
	//this->prevBtn = !HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);
}

void Menu::update(bool dir)
{
	HD44780_Clear();
	//this->currState = this->nextState;
	this->dir = dir;
	if(dir){this->count++;}
	else{this->count--;}
}

void Menu::ModeSel()
{

	int btn = !HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);
	this->open = 0;

	if(this->count < 0){this->count = 3; this->menuPage--;}
	if(this->count > 3){this->count = 0; this->menuPage++;}

	if(this->menuPage > 1){this->menuPage = 0;}
	if(this->menuPage < 0){this->menuPage = 1;}

	if(this->menuPage == 0)
	{
		HD44780_SetCursor(1,0);
		HD44780_PrintStr("Constant Current");

		HD44780_SetCursor(1,1);
		HD44780_PrintStr("Constant Resistance");

		HD44780_SetCursor(1,2);
		HD44780_PrintStr("Constant Power");

		HD44780_SetCursor(1,3);
		HD44780_PrintStr("Battery Testing");
	}

	if(this->menuPage == 1)
	{
		HD44780_SetCursor(1,0);
		HD44780_PrintStr("Transient");

		HD44780_SetCursor(1,1);
		HD44780_PrintStr("Self-Calibration");
	}
	HD44780_SetCursor(0,this->count);
	HD44780_PrintStr(">");


	if(this->count == 0 && this->menuPage == 0)
	{
		this->nextState = STATES::CC_MODE;
	}
	else if(this->count == 1 && this->menuPage == 0)
	{
		this->nextState = STATES::CR_MODE;
	}
	else if(this->count == 2 && this->menuPage == 0)
	{
		this->nextState = STATES::CP_MODE;
	}
	else if(this->count == 3 && this->menuPage == 0)
	{
		this->nextState = STATES::MODE_SEL;
	}
	else if(this->count == 0 && this->menuPage == 1)
	{
		this->nextState = STATES::TRANS_MODE;
	}
	else if(this->count == 1 && this->menuPage == 1)
	{
		this->nextState = STATES::CAL_MODE;
	}
	if(btn && !this->prevBtn)
	{
		this->prevBtn = btn;
		HD44780_Clear();
		this->count = 0; //Prepare the counter for the next state
		this->currState = this->nextState;
		this->run();
	}
	this->prevBtn = btn;




}

void Menu::CC_Mode()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Const Current");

	HD44780_SetCursor(17,0);
	if(open){HD44780_PrintStr("ON");}
	else{HD44780_PrintStr("OFF");}

	char voltage[10];
	//float v;
	//if(this->fWire){v = hwInterface->readVoltageFilteredF(); this->hwInterface->fWire = 1;}
	//else{v = hwInterface->readVoltageFiltered(); this->hwInterface->fWire = 0;}
	this->hwInterface->fWire = this->fWire;
	sprintf(voltage, "%.3f", this->hwInterface->cvolt);

	HD44780_SetCursor(0,1);
	HD44780_PrintStr(voltage);
	HD44780_PrintStr("V ");

	char current[10];
	//float c = hwInterface->readC();
	sprintf(current, "%.3f", this->hwInterface->ccur);

	HD44780_SetCursor(7,1);
	HD44780_PrintStr(current);
	HD44780_PrintStr("A ");

	current[5] = '\n';
	//usbSend((uint8_t*)current, 6);

	char power[10];
	float p = hwInterface->readW();
	sprintf(power, "%.2f", p);

	HD44780_SetCursor(15,1);
	HD44780_PrintStr(power);
	HD44780_PrintStr("W");


	HD44780_SetCursor(0,2);
	HD44780_PrintStr("Set I=");

	char setCurrent[10];
	sprintf(setCurrent, "%.3f", this->setC);

	HD44780_SetCursor(7,2);
	HD44780_PrintStr(setCurrent);
	HD44780_PrintStr("A");
	HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);

	HD44780_SetCursor(0,3);
	HD44780_PrintStr("4-Wire:");

	HD44780_SetCursor(7,3);
	if(this->fWire){HD44780_PrintStr("ON");}
	else{HD44780_PrintStr("OFF");}

	HD44780_SetCursor(11,3);
	HD44780_PrintStr("B");

	char pw[10];
	sprintf(pw, "%.1f", this->hwInterface->pwm);

	HD44780_SetCursor(13,3);
	HD44780_PrintStr(pw); //Dummy

	HD44780_SetCursor(19,3);
	HD44780_PrintStr("C");

	//testing
	//this->hwInterface->setGateVoltage((int)this->setC*10);
	//Controll logic
	//Cursor logic
	int btn = !HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);

	if(this->cursorIndex < 0){this->cursorIndex = 0;}
	if(this->cursorIndex > 7){this->cursorIndex = 0;}
	if(btn && !this->prevBtn && cursorIndex != 6){this->cursorIndex++; this->count = 0; this->dir = 0;}

	//Setting the cursor
	HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);

	//Cursor Functionality
	switch(cursorIndex)
	{
	case 6:
		if(this->dir){this->open = 1; cursorIndex++;}
		else{this->open = 0;}

		HD44780_SetCursor(17,0);
		if(open){HD44780_PrintStr("ON "); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);}
		else{HD44780_PrintStr("OFF"); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);}
		HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);

		break;
	case 0:
		this->open = 0;
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setC += this->count;
		if(this->setC < 0){this->setC = 0; this->count = 0;}
		//if(this->dir){this->setC += 1;}
		//else{this->setC -= 1;}
		//if(this->setC < 0){this->setC = 0;}

		sprintf(setCurrent, "%.3f", this->setC);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setCurrent);
		HD44780_PrintStr("A ");

		HD44780_SetCursor(17,0);
		if(open){HD44780_PrintStr("ON "); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);}
		else{HD44780_PrintStr("OFF"); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);}

		HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);
		break;
	case 1:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setC += (float)this->count/10;
		if(this->setC < 0){this->setC = 0; this->count = 0;}

		sprintf(setCurrent, "%.3f", this->setC);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setCurrent);
		HD44780_PrintStr("A ");
		HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);
		break;
	case 2:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setC += (float)this->count/100;
		if(this->setC < 0){this->setC = 0; this->count = 0;}

		sprintf(setCurrent, "%.3f", this->setC);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setCurrent);
		HD44780_PrintStr("A ");
		HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);
		break;
	case 3:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setC += (float)this->count/1000;
		if(this->setC < 0){this->setC = 0; this->count = 0;}

		sprintf(setCurrent, "%.3f", this->setC);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setCurrent);
		HD44780_PrintStr("A");
		HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);
		break;
	case 4:
		if(this->dir){this->fWire = 1;}
		else{this->fWire = 0;}

		HD44780_SetCursor(7,3);
		if(this->fWire){HD44780_PrintStr("ON ");}
		else{HD44780_PrintStr("OFF");}
		HD44780_SetCursor(this->ccMap[cursorIndex][0], this->ccMap[cursorIndex][1]);
		break;
	case 5:
		if(this->count != 0)
		{
			this->nextState = STATES::MODE_SEL;
			HD44780_Clear();
			HD44780_NoCursor();
			HD44780_NoBlink();
			this->count = 0; //Prepare the counter for the next state
			this->currState = this->nextState;
			this->cursorIndex = 0;
			this->run();
		}
		break;
	case 7:
		HD44780_NoCursor();
		HD44780_NoBlink();
		break;
	default:
		break;
	}
	this->prevBtn = btn;
	//this->updateHardware();


}

void Menu::CR_Mode()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Const Resistance");

	HD44780_SetCursor(17,0);
	if(open){HD44780_PrintStr("ON");}
	else{HD44780_PrintStr("OFF");}

	char voltage[10];
	//float v;
	//if(this->fWire){v = hwInterface->readVoltageFilteredF(); this->hwInterface->fWire = 1;}
	//else{v = hwInterface->readVoltageFiltered(); this->hwInterface->fWire = 0;}
	this->hwInterface->fWire = this->fWire;
	sprintf(voltage, "%.3f", this->hwInterface->cvolt);

	HD44780_SetCursor(0,1);
	HD44780_PrintStr(voltage);
	HD44780_PrintStr("V ");

	char current[10];
	//float c = hwInterface->readC();
	sprintf(current, "%.3f", this->hwInterface->ccur);

	HD44780_SetCursor(7,1);
	HD44780_PrintStr(current);
	HD44780_PrintStr("A ");

	current[5] = '\n';
	//usbSend((uint8_t*)current, 6);

	char power[10];
	float p = hwInterface->readW();
	sprintf(power, "%.2f", p);

	HD44780_SetCursor(15,1);
	HD44780_PrintStr(power);
	HD44780_PrintStr("W");


	HD44780_SetCursor(0,2);
	HD44780_PrintStr("Set R=");

	char setResistance[10];
	sprintf(setResistance, "%.4d", (int)this->setR);

	HD44780_SetCursor(7,2);
	HD44780_PrintStr(setResistance);
	HD44780_PrintStr("R");
	HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);

	HD44780_SetCursor(0,3);
	HD44780_PrintStr("4-Wire:");

	HD44780_SetCursor(7,3);
	if(this->fWire){HD44780_PrintStr("ON");}
	else{HD44780_PrintStr("OFF");}

	HD44780_SetCursor(11,3);
	HD44780_PrintStr("B");

	char pw[10];
	sprintf(pw, "%.1f", this->hwInterface->pwm);

	HD44780_SetCursor(13,3);
	HD44780_PrintStr(pw); //Dummy

	HD44780_SetCursor(19,3);
	HD44780_PrintStr("C");

	//testing
	//this->hwInterface->setGateVoltage((int)this->setC*10);
	//Controll logic
	//Cursor logic
	int btn = !HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);

	if(this->cursorIndex < 0){this->cursorIndex = 0;}
	if(this->cursorIndex > 7){this->cursorIndex = 0;}
	if(btn && !this->prevBtn && cursorIndex != 6){this->cursorIndex++; this->count = 0; this->dir = 0;}

	//Setting the cursor
	HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);

	//Cursor Functionality
	switch(cursorIndex)
	{
	case 6:
		if(this->dir){this->open = 1; cursorIndex++;}
		else{this->open = 0;}

		HD44780_SetCursor(17,0);
		if(open){HD44780_PrintStr("ON "); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);}
		else{HD44780_PrintStr("OFF"); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);}
		HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);

		break;
	case 0:
		this->open = 0;
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setR += this->count*1000;
		if(this->setR < 0){this->setR = 0; this->count = 0;}
		//if(this->dir){this->setC += 1;}
		//else{this->setC -= 1;}
		//if(this->setC < 0){this->setC = 0;}

		sprintf(setResistance, "%.4d", (int)this->setR);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setResistance);
		HD44780_PrintStr("R ");

		HD44780_SetCursor(17,0);
		if(open){HD44780_PrintStr("ON "); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);}
		else{HD44780_PrintStr("OFF"); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);}

		HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);
		break;
	case 1:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setR += (float)this->count*100;
		if(this->setR < 0){this->setR = 0; this->count = 0;}

		sprintf(setResistance, "%.4d", (int)this->setR);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setResistance);
		HD44780_PrintStr("R ");
		HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);
		break;
	case 2:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setR += (float)this->count*10;
		if(this->setR < 0){this->setR = 0; this->count = 0;}

		sprintf(setResistance, "%.4d", (int)this->setR);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setResistance);
		HD44780_PrintStr("R ");
		HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);
		break;
	case 3:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setR += (float)this->count;
		if(this->setR < 0){this->setR = 0; this->count = 0;}

		sprintf(setResistance, "%.4d", (int)this->setR);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setResistance);
		HD44780_PrintStr("R ");
		HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);
		break;
	case 4:
		if(this->dir){this->fWire = 1;}
		else{this->fWire = 0;}

		HD44780_SetCursor(7,3);
		if(this->fWire){HD44780_PrintStr("ON ");}
		else{HD44780_PrintStr("OFF");}
		HD44780_SetCursor(this->crMap[cursorIndex][0], this->crMap[cursorIndex][1]);
		break;
	case 5:
		if(this->count != 0)
		{
			this->nextState = STATES::MODE_SEL;
			HD44780_Clear();
			HD44780_NoCursor();
			HD44780_NoBlink();
			this->count = 0; //Prepare the counter for the next state
			this->currState = this->nextState;
			this->cursorIndex = 0;
			this->run();
		}
		break;
	case 7:
		HD44780_NoCursor();
		HD44780_NoBlink();
		break;
	default:
		break;
	}
	this->prevBtn = btn;

}
void Menu::CP_Mode()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Const Power");

	HD44780_SetCursor(17,0);
	if(open){HD44780_PrintStr("ON");}
	else{HD44780_PrintStr("OFF");}

	char voltage[10];
	//float v;
	//if(this->fWire){v = hwInterface->readVoltageFilteredF(); this->hwInterface->fWire = 1;}
	//else{v = hwInterface->readVoltageFiltered(); this->hwInterface->fWire = 0;}
	this->hwInterface->fWire = this->fWire;
	sprintf(voltage, "%.3f", this->hwInterface->cvolt);

	HD44780_SetCursor(0,1);
	HD44780_PrintStr(voltage);
	HD44780_PrintStr("V ");

	char current[10];
	//float c = hwInterface->readC();
	sprintf(current, "%.3f", this->hwInterface->ccur);

	HD44780_SetCursor(7,1);
	HD44780_PrintStr(current);
	HD44780_PrintStr("A ");

	current[5] = '\n';
	//usbSend((uint8_t*)current, 6);

	char power[10];
	sprintf(power, "%.2f", this->hwInterface->cpow);

	HD44780_SetCursor(15,1);
	HD44780_PrintStr(power);
	HD44780_PrintStr("W");


	HD44780_SetCursor(0,2);
	HD44780_PrintStr("Set W=");

	char setPower[10];
	sprintf(setPower, "%.2f", this->setW);

	HD44780_SetCursor(7,2);
	HD44780_PrintStr(setPower);
	HD44780_PrintStr("W");
	HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);

	HD44780_SetCursor(0,3);
	HD44780_PrintStr("4-Wire:");

	HD44780_SetCursor(7,3);
	if(this->fWire){HD44780_PrintStr("ON");}
	else{HD44780_PrintStr("OFF");}

	HD44780_SetCursor(11,3);
	HD44780_PrintStr("B");

	char pw[10];
	sprintf(pw, "%.1f", this->hwInterface->pwm);

	HD44780_SetCursor(13,3);
	HD44780_PrintStr(pw); //Dummy

	HD44780_SetCursor(19,3);
	HD44780_PrintStr("C");

	//testing
	//this->hwInterface->setGateVoltage((int)this->setC*10);
	//Controll logic
	//Cursor logic
	int btn = !HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);

	if(this->cursorIndex < 0){this->cursorIndex = 0;}
	if(this->cursorIndex > 6){this->cursorIndex = 0;}
	if(btn && !this->prevBtn && cursorIndex != 5){this->cursorIndex++; this->count = 0; this->dir = 0;}

	//Setting the cursor
	HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);

	//Cursor Functionality
	switch(cursorIndex)
	{
	case 5:
		if(this->dir){this->open = 1; cursorIndex++;}
		else{this->open = 0;}

		HD44780_SetCursor(17,0);
		if(open){HD44780_PrintStr("ON "); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);}
		else{HD44780_PrintStr("OFF"); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);}
		HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);

		break;
	case 0:
		this->open = 0;
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setW += this->count;
		if(this->setW < 0){this->setW = 0; this->count = 0;}
		//if(this->dir){this->setC += 1;}
		//else{this->setC -= 1;}
		//if(this->setC < 0){this->setC = 0;}

		sprintf(setPower, "%.2f", this->setW);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setPower);
		HD44780_PrintStr("W ");

		HD44780_SetCursor(17,0);
		if(open){HD44780_PrintStr("ON "); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);}
		else{HD44780_PrintStr("OFF"); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);}

		HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);
		break;
	case 1:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setW += (float)this->count/10;
		if(this->setW < 0){this->setW = 0; this->count = 0;}


		sprintf(setPower, "%.2f", this->setW);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setPower);
		HD44780_PrintStr("W ");		HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);
		break;
	case 2:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setW += (float)this->count/100;
		if(this->setW < 0){this->setW = 0; this->count = 0;}


		sprintf(setPower, "%.2f", this->setW);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setPower);
		HD44780_PrintStr("W ");
		HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);
		break;
		/*
	case 3:
		if(this->count >= 1){this->count = 1;}
		if(this->count <= -1){this->count = -1;}
		this->setW += (float)this->count;
		if(this->setW < 0){this->setW = 0; this->count = 0;}

		sprintf(setResistance, "%.4d", (int)this->setR);

		HD44780_SetCursor(7,2);
		HD44780_PrintStr(setResistance);
		HD44780_PrintStr("R ");
		HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);
		break;
		*/
	case 3:
		if(this->dir){this->fWire = 1;}
		else{this->fWire = 0;}

		HD44780_SetCursor(7,3);
		if(this->fWire){HD44780_PrintStr("ON ");}
		else{HD44780_PrintStr("OFF");}
		HD44780_SetCursor(this->cpMap[cursorIndex][0], this->cpMap[cursorIndex][1]);
		break;
	case 4:
		if(this->count != 0)
		{
			this->nextState = STATES::MODE_SEL;
			HD44780_Clear();
			HD44780_NoCursor();
			HD44780_NoBlink();
			this->count = 0; //Prepare the counter for the next state
			this->currState = this->nextState;
			this->cursorIndex = 0;
			this->run();
		}
		break;
	case 6:
		HD44780_NoCursor();
		HD44780_NoBlink();
		break;
	default:
		break;
	}
	this->prevBtn = btn;
}

void Menu::TRANS_Mode()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Transient Mode");
}

void Menu::CAL_Mode()
{

	int btn = !HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);

	HD44780_NoCursor();
	HD44780_NoBlink();

	if(this->count < 0){this->count = 3;}
	if(this->count > 3){this->count = 0;}

	HD44780_SetCursor(1, 0);
	HD44780_PrintStr("Calibrate C Offset");

	HD44780_SetCursor(1, 1);
	HD44780_PrintStr("Calibrate V Offset");

	HD44780_SetCursor(1, 2);
	HD44780_PrintStr("Calibrate PID Gains");

	HD44780_SetCursor(1, 3);
	HD44780_PrintStr("Back");

	HD44780_SetCursor(0,this->count);
	HD44780_PrintStr(">");

	if(this->count == 0)
	{
		this->nextState = STATES::C_CAL;
	}
	else if(this->count == 1)
	{
		this->nextState = STATES::V_CAL;
	}
	else if(this->count == 2)
	{
		this->nextState = STATES::PID_CAL;
	}
	else if(this->count == 3)
	{
		this->nextState = STATES::MODE_SEL;
	}

	if(btn && !this->prevBtn)
	{
		this->prevBtn = btn;
		HD44780_Clear();
		this->count = 0; //Prepare the counter for the next state
		this->currState = this->nextState;
		this->run();
	}
	this->prevBtn = btn;
}

void Menu::cCal_Mode()
{
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("Calibrating...");

	HD44780_SetCursor(0, 1);
	HD44780_PrintStr("Keep inputs free");
	this->hwInterface->calC(50000);
	//this->hwInterface->saveCalibratedValues();

	HD44780_Clear();
	this->count = 0; //Prepare the counter for the next state
	this->currState = STATES::CAL_MODE;
	this->run();
}

void Menu::vCal_Mode()
{
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("Calibrate V Offset");
}

void Menu::pidCal_Mode()
{
	HD44780_SetCursor(0, 0);
	HD44780_PrintStr("Calibrate PID Gains");
}

bool Menu::selecting()
{
	if(this->cursorIndex >= 7 && (this->currState == STATES::CC_MODE || this->currState == STATES::CR_MODE))
	{
		//this->open = 0;
		HD44780_NoCursor();
		HD44780_NoBlink();

		return 0;
	}
	else if(this->cursorIndex >= 6 && this->currState == STATES::CP_MODE)
	{
		//this->open = 0;
		HD44780_NoCursor();
		HD44780_NoBlink();

		return 0;
	}
	else if(this->currState == STATES::CAL_MODE)
	{
		HD44780_NoCursor();
		HD44780_NoBlink();
		return 0;
	}
	else
	{
		//this->open = 0;
		if(this->currState != STATES::MODE_SEL)
		{
			HD44780_Cursor();
			HD44780_Blink();
		}
		return 1;
	}
}

void Menu::updateHardware()
{
	if(this->open)
	{
		if(this->currState == STATES::CC_MODE)
		{
			this->hwInterface->setC(this->setC);
		}
		else if(this->currState == STATES::CR_MODE)
		{
			float i = this->hwInterface->cvolt / this->setR;
			this->hwInterface->setC(i);
		}
		if(this->currState == STATES::CP_MODE)
		{
			float i = this->setW / this->hwInterface->cvolt;
			this->hwInterface->setC(i);
		}

	}
	else
	{
		this->hwInterface->pwm = PWM_BASELINE;
		this->hwInterface->setGateVoltage(this->hwInterface->pwm);
	}
}


void Menu::setState(STATES state)
{
	this->currState = state;
	HD44780_Clear();
	this->run();
}

void Menu::SETC(double c)
{
	this->count = 0;
	this->setC = c;
	HD44780_Clear();
	this->run();
}

void Menu::SETR(double r)
{
	this->count = 0;
	this->setR = r;
	HD44780_Clear();
	this->run();
}

void Menu::SETP(double p)
{
	this->count = 0;
	this->setW = p;
	HD44780_Clear();
	this->run();
}

void Menu::toggleGate()
{
	if(this->currState == STATES::CC_MODE || this->currState == STATES::CR_MODE)
	{
		this->dir = !this->dir;
		//this->open = !this->open;
		if(this->dir){this->cursorIndex = 6;}
		else{this->cursorIndex = 0;}
	}
	else if(this->currState == STATES::CP_MODE)
	{
		this->dir = !this->dir;
		//this->open = !this->open;
		if(this->dir){this->cursorIndex = 5;}
		else{this->cursorIndex = 0;}
	}
	HD44780_Clear();
	this->run();

}
