/*
 * menu.c
 *
 *  Created on: Jul 3, 2024
 *      Author: georg
 */
/*
#include <menu.hpp>
#include "liquidcrystal_i2c.h"
#include "stdint.h"
#include "hw_interface.h"
#include "stdio.h"
Menu::Menu()
{
	this->currState = STATES::MAIN;
	this->HW = new hw();
	this->numMode = 0;
	HD44780_Init(4);
	HD44780_Clear();
	mainMenu();
}

void Menu::update(uint16_t encDir, int encPos)
{
	int btn = HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);
	this->counterValue = encPos;
	switch(this->currState)
	{
	case STATES::MAIN:
		//if(encDir){this->currState = STATES::MODE_SEL;}
		//else{this->currState = STATES::CW_MODE;}
		if(!btn){currState = STATES::CC;}
		break;

	//case STATES::MODE_SEL:
		//modeSelMenu();
		//if(encDir){this->currState = STATES::CC;}
		//else{this->currState = STATES::CW_MODE;}
		//break;

	case STATES::CC:
		if(!btn){this->currState = STATES::CC_MODE;}
		else if(encDir && btn){this->currState = STATES::CR;}
		else if(!encDir && btn){this->currState = STATES::CW;}
		break;

	case STATES::CR:
		if(!btn){this->currState = STATES::CR_MODE;}
		else if(encDir && btn){this->currState = STATES::CW;}
		else if(!encDir && btn){this->currState = STATES::CC;}
		break;

	case STATES::CW:
		if(!btn){this->currState = STATES::CW_MODE;}
		else if(encDir && btn){this->currState = STATES::CC;}
		else if(!encDir && btn){this->currState = STATES::CR;}
		break;

	case STATES::CC_MODE:

		break;

	case STATES::CR_MODE:

		break;

	case STATES::CW_MODE:

		break;

	default:
		break;
	}

}

void Menu::run()
{
	HD44780_Clear();
	switch(this->currState)
	{
	case STATES::MAIN:
		mainMenu();
		break;

	case STATES::CC:
		cc();
		break;

	case STATES::CR:
		cr();
		break;

	case STATES::CW:
		cw();
		break;

	case STATES::CC_MODE:
		cc_modeMenu();
		break;

	case STATES::CR_MODE:
		cr_modeMenu();
		break;

	case STATES::CW_MODE:
		cw_modeMenu();
		break;
	default:
		break;
	}
	this->lastCounterValue = this->counterValue;
}

void Menu::clear()
{
	HD44780_Clear();
}

void Menu::mainMenu()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Press to select");
	HD44780_SetCursor(0,1);
	HD44780_PrintStr("mode");
}

void Menu::cc()
{
	HD44780_Clear();
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Constant Current");
}

void Menu::cr()
{
	HD44780_Clear();
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Constant Resistance");
}

void Menu::cw()
{
	HD44780_Clear();
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("Constant Wattage");
}

void Menu::modeSelMenu()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("MODE");
}

void Menu::cc_modeMenu()
{
	int btn = HAL_GPIO_ReadPin(ENC_BTN_P, ENC_BTN);


	char current[10];
	float c = HW->readC();
	sprintf(current, "%.2f", c);

	HD44780_SetCursor(10,0);
	HD44780_PrintStr("MC:");
	HD44780_PrintStr(current);

	char setCurrent[10];
	float sc = this->setC;
	sprintf(setCurrent, "%.2f", sc);

	int val = 0.01;

	if(this->numMode == 0)
	{
		val = 0.01;
	}
	else if(this->numMode == 1)
	{
		val = 0.1;
	}
	else
	{
		val = 1;
	}

	if(this->counterValue > this->lastCounterValue)
	{
		this->setC -= val;
	}
	else if(this->counterValue < this->lastCounterValue)
	{
		this->setC += val;
	}
	if(this->setC <= 0)
	{
		this->setC = 0;
	}
	if(this->setC >= 20)
	{
		this->setC = 20;
	}


	if(btn)
	{
		this->numMode++;
		this->numMode = this->numMode%3;
	}


	HD44780_SetCursor(0,0);
	HD44780_PrintStr("SC:");
	HD44780_PrintStr(setCurrent);

	char voltage[10];
	float v = HW->readVoltage();
	sprintf(voltage, "%.3f", v);

	HD44780_SetCursor(0,1);
	HD44780_PrintStr("V:");
	HD44780_PrintStr(voltage);

	HD44780_SetCursor(0,2);
	HD44780_PrintStr("W:");
	HD44780_PrintStr("0.0");

	HD44780_SetCursor(0,3);
	HD44780_PrintStr("R:");
	HD44780_PrintStr("0.0 ");

}

void Menu::cr_modeMenu()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("R:");
	HD44780_PrintStr("0.0 ");

	HD44780_SetCursor(0,1);
	HD44780_PrintStr("C:");
	HD44780_PrintStr("0.0 ");

	HD44780_SetCursor(0,2);
	HD44780_PrintStr("V:");
	HD44780_PrintStr("0.0");

	HD44780_SetCursor(0,3);
	HD44780_PrintStr("W:");
	HD44780_PrintStr("0.0");
}

void Menu::cw_modeMenu()
{
	HD44780_SetCursor(0,0);
	HD44780_PrintStr("W:");
	HD44780_PrintStr("0.0 ");

	HD44780_SetCursor(0,1);
	HD44780_PrintStr("C:");
	HD44780_PrintStr("0.0");

	HD44780_SetCursor(0,2);
	HD44780_PrintStr("V:");
	HD44780_PrintStr("0.0");

	HD44780_SetCursor(0,3);
	HD44780_PrintStr("R:");
	HD44780_PrintStr("0.0 ");
}

*/
