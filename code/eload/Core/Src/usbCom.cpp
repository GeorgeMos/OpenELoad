/*
 * usbCom.cpp
 *
 *  Created on: Jul 24, 2024
 *      Author: georg
 */


#include "usbCom.h"



void usbSend(uint8_t* txBuf, uint32_t len)
{
	CDC_Transmit_FS(txBuf, len);
}

/*
void send(uint8_t* txBuf, uint32_t len)
{
	CDC_Transmit_FS(txBuf, len);
}
*/
/*
void recv(uint8_t* rxBuf, uint32_t len)
{

}
*/

void UsbManager::sendOnce(uint8_t* txBuf, uint32_t len)
{
	if(!this->commandSent)
	{
		usbSend(txBuf, len);
		this->commandSent = 1;
	}
}

void UsbManager::rstSendOnce()
{
	this->commandSent = 0;
}

void UsbManager::getCommand()
{
	char* buf = (char*)usbBuffer;
	std::string strBuf(buf);
	std::string first_five = strBuf.substr(0,5);
	bool found = 0;
	//std::string strBuf = "DISCOVER";
	if(this->commandMap.find(strBuf) != this->commandMap.end())
	{
		//Command found
		this->currCommand = this->commandMap.at(strBuf);
		found = 1;
	}
	else if(first_five.compare("SET_C") == 0 && !found)
	{
		//Set Current value command found (SET_C 00.000)
		std::string value = strBuf.substr(6, 6);
		this->setCVal = std::stod(value);
		this->currState = USB_STATES::SET_C_VAL;

	}
	else if(first_five.compare("SET_R") == 0 && !found)
	{
		//Set Resistance value command found (SET_R 0000)
		std::string value = strBuf.substr(6, 4);
		this->setRVal = std::stod(value);
		this->currState = USB_STATES::SET_R_VAL;

	}
	else if(first_five.compare("SET_P") == 0 && !found)
	{
		//Set Power value command found (SET_P 00.00)
		std::string value = strBuf.substr(6, 5);
		this->setPVal = std::stod(value);
		this->currState = USB_STATES::SET_P_VAL;

	}
	else //if(this->commandMap.find(strBuf) == this->commandMap.end())
	{
		//Command does not exist
		this->currCommand = USB_COMMANDS::NOP;
		found = 0;
	}

}

UsbManager::UsbManager(Menu* menuHandle)
{
	this->menuHandle = menuHandle;
	rstCommandBuf();
	rstTxBuf();

	this->commandSent = 0;

	this->sendCVPTData = 0;

	this->rstSetValues();

	this->currState = USB_STATES::UNCONNECTED;
	this->currCommand = USB_COMMANDS::NOP;

	this->commandMap["NOP"] = USB_COMMANDS::NOP;
	this->commandMap["DISCOVER"] = USB_COMMANDS::DISCOVER;
	this->commandMap["CONNECT"] = USB_COMMANDS::CONNECT;
	this->commandMap["DISCONNECT"] = USB_COMMANDS::DISCONNECT;
	this->commandMap["HOME"] = USB_COMMANDS::HOME;
	this->commandMap["SET_CC_MODE"] = USB_COMMANDS::SET_CC_MODE;
	this->commandMap["SET_CR_MODE"] = USB_COMMANDS::SET_CR_MODE;
	this->commandMap["SET_CP_MODE"] = USB_COMMANDS::SET_CP_MODE;
	this->commandMap["EN_SEND_DATA"] = USB_COMMANDS::EN_SEND_DATA;
	this->commandMap["EN_GATE"] = USB_COMMANDS::EN_GATE;

}

void UsbManager::run()
{
	this->getCommand();
	switch(this->currState)
	{
	case USB_STATES::UNCONNECTED:
		if(this->currCommand == USB_COMMANDS::DISCOVER)
		{
			const char* res = "ELOAD";
			memcpy(this->txBuf, (uint8_t*)res, 5);
			usbSend(txBuf, 5);
			rstTxBuf();
			rstCommandBuf();
			//this->currState = USB_STATES::DISCOVER_DEV;
		}

		if(this->currCommand == USB_COMMANDS::CONNECT)
		{
			this->currState = USB_STATES::CONNECTED;
		}

		break;
	case USB_STATES::DISCOVER_DEV:
		if(this->currCommand == USB_COMMANDS::CONNECT)
		{
			this->currState = USB_STATES::CONNECTED;
		}
		break;
	case USB_STATES::CONNECTED:
		if(this->currCommand == USB_COMMANDS::DISCONNECT)
		{
			this->menuHandle->setState(STATES::MODE_SEL);
			this->currState = USB_STATES::UNCONNECTED;
			rstCommandBuf();
		}
		if(this->currCommand == USB_COMMANDS::SET_CC_MODE)
		{
			this->currState = USB_STATES::USB_CC_MODE;
		}
		if(this->currCommand == USB_COMMANDS::SET_CR_MODE)
		{
			this->currState = USB_STATES::USB_CR_MODE;
		}
		if(this->currCommand == USB_COMMANDS::SET_CP_MODE)
		{
			this->currState = USB_STATES::USB_CP_MODE;
		}
		if(this->currCommand == USB_COMMANDS::HOME)
		{
			this->menuHandle->setState(STATES::MODE_SEL);
			this->currState = USB_STATES::CONNECTED;
			rstCommandBuf();
		}
		if(this->currCommand == USB_COMMANDS::EN_SEND_DATA)
		{
			this->sendCVPTData = !this->sendCVPTData;
			rstCommandBuf();
		}
		if(this->currCommand == USB_COMMANDS::EN_GATE)
		{
			this->menuHandle->toggleGate();
			rstCommandBuf();
		}
		break;
	case USB_STATES::USB_CC_MODE:
		this->rstSetValues();
		this->menuHandle->setState(STATES::CC_MODE);
		this->currState = USB_STATES::CONNECTED;
		rstCommandBuf();
		break;
	case USB_STATES::USB_CR_MODE:
		this->rstSetValues();
		this->menuHandle->setState(STATES::CR_MODE);
		this->currState = USB_STATES::CONNECTED;
		rstCommandBuf();
		break;
	case USB_STATES::USB_CP_MODE:
		this->rstSetValues();
		this->menuHandle->setState(STATES::CP_MODE);
		this->currState = USB_STATES::CONNECTED;
		rstCommandBuf();
		break;
	case USB_STATES::SET_C_VAL:
		this->menuHandle->SETC(this->setCVal);
		this->currState = USB_STATES::CONNECTED;
		rstCommandBuf();
		break;
	case USB_STATES::SET_R_VAL:
		this->menuHandle->SETR(this->setRVal);
		this->currState = USB_STATES::CONNECTED;
		rstCommandBuf();
		break;
	case USB_STATES::SET_P_VAL:
		this->menuHandle->SETP(this->setPVal);
		this->currState = USB_STATES::CONNECTED;
		rstCommandBuf();
		break;
	default:
		break;
	}

	this->sendData();
}

void UsbManager::rstCommandBuf()
{
	memset (usbBuffer, '\0', 64);  // clear the buffer
	usbBuffer[0] = 'N';
	usbBuffer[1] = 'O';
	usbBuffer[2] = 'P';
}

void UsbManager::rstSetValues()
{
	this->setCVal = 0.f;
	this->setRVal = 0.f;
	this->setPVal = 0.f;
}

void UsbManager::sendData()
{
	if(this->sendCVPTData)
	{
		//String to be sent to pc. Current+" "Voltage+" "+Power
		std::string txBufStr = std::to_string(this->menuHandle->hwInterface->ccur) //Currrent
				+ " " + std::to_string(this->menuHandle->hwInterface->cvolt)	//Voltage
				+ " " + std::to_string(this->menuHandle->hwInterface->cpow);	//Power
		const char* txBuf = txBufStr.c_str();
		usbSend((uint8_t*)txBuf, txBufStr.length());
	}
}

void UsbManager::rstTxBuf()
{
	memset (this->txBuf, '\0', TX_BUF_LEN);
}
