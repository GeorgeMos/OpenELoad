#ifndef USB_COM
#define USB_COM
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "serial.hpp"
#include <iomanip>
#include <sstream>

#define RX_BUF_SIZE 64

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


class UsbCom
{
    private:
    std::string vcpName;
    SerialPort* vcp;
    std::map<USB_COMMANDS, std::string> commandMap;

    uint8_t rxBuf[RX_BUF_SIZE];
    public:
    UsbCom();
    ~UsbCom();
    std::vector<std::string> listPorts();
    bool checkPort(std::string vcpName);
    void setPort(std::string vcpName);
    std::string getPort();
    bool connect();
    void closePort();
    void sendCommand(USB_COMMANDS command);
    void sendCommand(std::string command);
    void readData(uint8_t len);
    void setC(double c);
    void setR(int r);
    void setP(double p);

};

#endif