#include <vcp_api.hpp>


std::vector<std::string> enumerateComPorts() {
    std::vector<std::string> comPorts;
    for (int i = 1; i <= 256; ++i) {
        std::string comPort = "COM" + std::to_string(i);
        HANDLE hCom = CreateFile(comPort.c_str(), GENERIC_READ | GENERIC_WRITE,
                                 0, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hCom != INVALID_HANDLE_VALUE) {
            comPorts.push_back(comPort);
            CloseHandle(hCom);
        }
    }
    return comPorts;
}

// Function to find a specific device by sending a command and checking the response
bool findSpecificDevice(const std::string& comPort, const std::string& command, const std::string& expectedResponse) {
    HANDLE hCom = CreateFile(comPort.c_str(), GENERIC_READ | GENERIC_WRITE,
                             0, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hCom == INVALID_HANDLE_VALUE) {
        return false;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hCom, &dcbSerialParams)) {
        CloseHandle(hCom);
        return false;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hCom, &dcbSerialParams)) {
        CloseHandle(hCom);
        return false;
    }

    DWORD bytesWritten;
    if (!WriteFile(hCom, command.c_str(), command.length(), &bytesWritten, nullptr)) {
        CloseHandle(hCom);
        return false;
    }

    Sleep(100);  // Wait for the response

    char buffer[256] = {0};
    DWORD bytesRead;
    if (!ReadFile(hCom, buffer, sizeof(buffer), &bytesRead, nullptr)) {
        CloseHandle(hCom);
        return false;
    }

    CloseHandle(hCom);
    return std::string(buffer, bytesRead).find(expectedResponse) != std::string::npos;
}

int openPort(const std::string& comPort)
{
    VCP_API::devPort.open(comPort, O_RDWR);
    if(devPort.is_open())
    {
        return 0;
    }
    else
    {
        return 1;   //Error
    }
}

void closePort()
{
    VCP_API::devPort.close();
}

void readData(uint8_t* rxBuf, uint32_t len)
{
    VCP_API::devPort.read((char*)rxBuf, len);
}

void writeData(uint8_t* txBuf, uint32_t len)
{
    VCP_API::devPort.write((char*)txBuf, len);
}