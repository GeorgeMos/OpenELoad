/*
 * menu.h
 *
 *  Created on: Jul 3, 2024
 *      Author: georg
 */
/*
#ifndef INC_MENU_HPP_
#define INC_MENU_HPP_
#include "stdint.h"
#include "liquidcrystal_i2c.h"
#include "hw_interface.h"

#define ENC_BTN_P GPIOA
#define ENC_BTN GPIO_PIN_3

enum STATES
{
	MAIN,
	MODE_SEL,
	CC_MODE,
	CR_MODE,
	CW_MODE,
	CC,
	CR,
	CW
};

class Menu
{
private:
	int nextState;
	int currState;
	void mainMenu();
	void modeSelMenu();
	void cc_modeMenu();
	void cr_modeMenu();
	void cw_modeMenu();
	void voltMeter_modeMenu();
	void ampMeter_modeMenu();
	void powerMeter_modeMenu();

	void cc();
	void cr();
	void cw();

	hw *HW;
	int counterValue;
	int lastCounterValue;
	float setC;
	float setR;
	float setW;

	int numMode;

public:
	Menu();

	//Steps the menu state machine
	void update(uint16_t encDir, int encPos);
	void run();
	//Clears the screen
	void clear();
};




#endif /* INC_MENU_HPP_ */

