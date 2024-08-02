#include <serial.hpp>

SerialPort::SerialPort() : hCom(INVALID_HANDLE_VALUE) {}

SerialPort::~SerialPort() {
    close();
}

void SerialPort::close() {
    if (hCom != INVALID_HANDLE_VALUE) {
        CloseHandle(hCom);
        hCom = INVALID_HANDLE_VALUE;
    }
}

std::vector<std::string> SerialPort::enumerateComPorts() {
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

bool SerialPort::open(const std::string& comPortName, DWORD baudRate) {
    hCom = CreateFile(comPortName.c_str(),
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      NULL,
                      OPEN_EXISTING,
                      0,
                      NULL);

    if (hCom == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening COM port: " << comPortName << std::endl;
        return false;
    }
    if (!configureComPort(baudRate)) {
            CloseHandle(hCom);
            hCom = INVALID_HANDLE_VALUE;
            return false;
        }
     return setTimeouts();
}

bool SerialPort::write(const std::string& data) {
    if (hCom == INVALID_HANDLE_VALUE) {
        std::cerr << "COM port not opened" << std::endl;
        return false;
    }

    DWORD bytesWritten;
    if (!WriteFile(hCom, data.c_str(), data.size(), &bytesWritten, NULL)) {
        std::cerr << "Error writing to COM port" << std::endl;
        return false;
    }
    return true;
}

std::string SerialPort::read(DWORD bytesToRead) {
    if (hCom == INVALID_HANDLE_VALUE) {
        std::cerr << "COM port not opened" << std::endl;
        return "";
    }

    char buffer[256] = {0};
    DWORD bytesRead;
    if (!ReadFile(hCom, buffer, bytesToRead, &bytesRead, NULL)) {
        std::cerr << "Error reading from COM port" << std::endl;
        return "";
    }
    return std::string(buffer, bytesRead);
}

bool SerialPort::findSpecificDevice(const std::string& command, const std::string& expectedResponse) {
    if (!write(command)) {
        return false;
    }

    Sleep(1000);  // Wait for the response

    std::string response = read();
    return response.find(expectedResponse) != std::string::npos;
}

bool SerialPort::configureComPort(DWORD baudRate) {
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hCom, &dcbSerialParams)) {
        std::cerr << "Error getting COM state" << std::endl;
        return false;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hCom, &dcbSerialParams)) {
        std::cerr << "Error setting COM state" << std::endl;
        return false;
    }

    return true;
}

bool SerialPort::setTimeouts(DWORD readIntervalTimeout, DWORD readTotalTimeoutConstant, DWORD readTotalTimeoutMultiplier,
                     DWORD writeTotalTimeoutConstant, DWORD writeTotalTimeoutMultiplier) {
        COMMTIMEOUTS timeouts = {0};
        timeouts.ReadIntervalTimeout = readIntervalTimeout;
        timeouts.ReadTotalTimeoutConstant = readTotalTimeoutConstant;
        timeouts.ReadTotalTimeoutMultiplier = readTotalTimeoutMultiplier;
        timeouts.WriteTotalTimeoutConstant = writeTotalTimeoutConstant;
        timeouts.WriteTotalTimeoutMultiplier = writeTotalTimeoutMultiplier;

        if (!SetCommTimeouts(hCom, &timeouts)) {
            std::cerr << "Error setting COM timeouts" << std::endl;
            return false;
        }

        return true;
    }