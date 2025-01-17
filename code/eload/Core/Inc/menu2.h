/*
 * menu2.h
 *
 *  Created on: Jul 19, 2024
 *      Author: georg
 */

#ifndef INC_MENU2_H_
#define INC_MENU2_H_

#define ENC_BTN_P GPIOA
#define ENC_BTN GPIO_PIN_3
#include "hw_interface.h"
//#include "usbCom.h"

enum STATES
{
	MODE_SEL,
	CC_MODE,
	CR_MODE,
	CP_MODE,
	TRANS_MODE,
	CAL_MODE,
	C_CAL,
	PID_CAL,
	V_CAL
};

class Menu
{
private:
	STATES currState;
	STATES nextState;

	int count;
	bool dir;
	int menuPage = 0;



	void ModeSel();
	void CC_Mode();
	void CR_Mode();
	void CP_Mode();
	void TRANS_Mode();
	void CAL_Mode();
	void cCal_Mode();
	void vCal_Mode();
	void pidCal_Mode();

	//Column, Row of lcd
	int ccMap[7][2] = {{7,2}, {9,2}, {10,2}, {11,2}, {7,3}, {11,3}, {17, 0}};
	int crMap[7][2] = {{7,2}, {8,2}, {9,2}, {10,2}, {7,3}, {11,3}, {17, 0}};
	int cpMap[7][2] = {{7,2}, {9,2}, {10,2}, {7,3}, {11,3}, {17, 0}};
	int cursorIndex;

	//Functionality variables
	bool open;	//ON/OFF for the current(I) channel
	bool fWire;
	float setC;
	float setR;
	float setW;
public:
	hw* hwInterface;
	int prevBtn = 0;
	Menu();
	void run();
	void update(bool dir);
	bool selecting();
	void updateHardware();
	void setState(STATES state);
	void SETC(double c);
	void SETR(double r);
	void SETP(double p);
	void toggleGate();
};


#endif /* INC_MENU2_H_ */
