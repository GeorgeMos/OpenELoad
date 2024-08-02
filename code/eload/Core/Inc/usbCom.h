/*
 * usbCom.h
 *
 *  Created on: Jul 24, 2024
 *      Author: georg
 */

#ifndef INC_USBCOM_H_
#define INC_USBCOM_H_

#define TX_BUF_LEN 64

#include "usbd_cdc_if.h"
#include "string.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "menu2.h"

extern uint8_t usbBuffer[64];

enum USB_STATES
{
	UNCONNECTED,
	CONNECTED,
	DISCOVER_DEV,
	USB_CC_MODE,
	USB_CR_MODE,
	USB_CP_MODE,
	SET_C_VAL,
	SET_R_VAL,
	SET_P_VAL
};


//You can copy and paste this enum in to the c++ code of the desktop app
enum USB_COMMANDS
{
	NOP,
	DISCOVER,
	CONNECT,
	DISCONNECT,
	HOME,
	SET_CC_MODE,
	SET_CR_MODE,
	SET_CP_MODE,
	EN_SEND_DATA,	//Toggle send data
	EN_GATE			//Toggle the MOSFET gate
};

void usbSend(uint8_t* txBuf, uint32_t len);
//void send(uint8_t* txBuf, uint32_t len);
//void recv(uint8_t* rxBuf, uint32_t len);

/*
class UsbCom
{
public:
	UsbCom();
	void send(uint8_t* txBuf, uint32_t len);
	void recv(uint8_t* rxBuf, uint32_t len);

};
*/

class UsbManager
{
private:
	USB_STATES currState;
	USB_COMMANDS currCommand;

	uint8_t txBuf[TX_BUF_LEN]{0};

	bool commandSent;

	Menu* menuHandle;

	double setCVal;
	double setRVal;
	double setPVal;

	bool sendCVPTData;	//Send current, voltage, power, temperature data
	//You can copy and paste this in to the c++ code of the desktop app
	std::map<std::string, USB_COMMANDS> commandMap;

	//Private Functions
	//Processes the buffer and outputs the recieved command
	void getCommand();
	void sendOnce(uint8_t* txBuf, uint32_t len);
	void rstSendOnce();
	void rstCommandBuf();
	void rstSetValues();
	void rstTxBuf();
	void sendData();
public:
	UsbManager();
	UsbManager(Menu* menuHandle);
	void run();
};

#endif /* INC_USBCOM_H_ */
