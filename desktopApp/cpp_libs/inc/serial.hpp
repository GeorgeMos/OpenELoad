#ifndef __SERIAL__
#define __SERIAL__

#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>


class SerialPort {
public:
    SerialPort();

    ~SerialPort();

    void close();

    std::vector<std::string> enumerateComPorts();

    bool open(const std::string& comPortName, DWORD baudRate = CBR_115200);

    bool write(const std::string& data);

    std::string read(DWORD bytesToRead = 256);

    bool findSpecificDevice(const std::string& command, const std::string& expectedResponse);

private:
    HANDLE hCom;
    bool configureComPort(DWORD baudRate);
    bool setTimeouts(DWORD readIntervalTimeout = 50, DWORD readTotalTimeoutConstant = 50, DWORD readTotalTimeoutMultiplier = 10,
                     DWORD writeTotalTimeoutConstant = 50, DWORD writeTotalTimeoutMultiplier = 10);
};


#endif