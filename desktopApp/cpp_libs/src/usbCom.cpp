#include <usbCom.hpp>

UsbCom::UsbCom()
{
    this->vcp = new SerialPort();

    this->commandMap[USB_COMMANDS::NOP] = "NOP";
	this->commandMap[USB_COMMANDS::DISCOVER] = "DISCOVER";
	this->commandMap[USB_COMMANDS::CONNECT] = "CONNECT";
	this->commandMap[USB_COMMANDS::DISCONNECT] = "DISCONNECT";
	this->commandMap[USB_COMMANDS::HOME] = "HOME";
	this->commandMap[USB_COMMANDS::SET_CC_MODE] = "SET_CC_MODE";
	this->commandMap[USB_COMMANDS::SET_CR_MODE] = "SET_CR_MODE";
	this->commandMap[USB_COMMANDS::SET_CP_MODE] = "SET_CP_MODE";
	this->commandMap[USB_COMMANDS::EN_SEND_DATA] = "EN_SEND_DATA";
	this->commandMap[USB_COMMANDS::EN_GATE] = "EN_GATE";
}


std::vector<std::string> UsbCom::listPorts()
{
    return this->vcp->enumerateComPorts();
}

UsbCom::~UsbCom()
{
    this->vcp->close();
}

bool UsbCom::checkPort(std::string vcpName)
{
    if(this->vcp->open(vcpName))
    {
        bool found = this->vcp->findSpecificDevice("DISCOVER", "ELOAD");
        this->vcp->close();
        return found;
    }
    else 
    {
        this->vcp->close();
        return false;
    }
}

void UsbCom::setPort(std::string vcpName)
{
    this->vcpName = vcpName;
}

std::string UsbCom::getPort()
{
    return this->vcpName;
}

bool UsbCom::connect()
{
    bool opened = this->vcp->open(this->vcpName);
    if(opened){this->sendCommand(USB_COMMANDS::CONNECT);}
    return (opened);
}

void UsbCom::closePort()
{
    //this->sendCommand(USB_COMMANDS::HOME);
    //Sleep(10);
    this->sendCommand(USB_COMMANDS::DISCONNECT);
    Sleep(10);
    this->vcp->close();
}

void UsbCom::sendCommand(USB_COMMANDS command)
{
    this->vcp->write(this->commandMap.at(command));
}

void UsbCom::sendCommand(std::string command)
{
    this->vcp->write(command);
}

void UsbCom::readData(uint8_t len)
{
}

void UsbCom::setC(double c)
{
    std::stringstream stream;
    std::string command;
    stream << std::fixed << std::setprecision(3) << c;
    //std::string s = stream.str();
    if(c < 10)
    {
        command = "SET_C 0" + stream.str();
    }
    else
    {
        command = "SET_C " + stream.str();
    }
    sendCommand(command);

}

void UsbCom::setR(int r)
{
    std::string command = "SET_R ";
    if(r < 1000)
    {
        command += "0";
    }
    if(r < 100)
    {
        command += "0";
    }
    if(r < 10)
    {
        command += "0";
    }
    command += std::to_string(r);
    sendCommand(command);
}

void UsbCom::setP(double p)
{
    std::string command;
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << p;
    //std::string s = stream.str();
    if(p < 10)
    {
        command = "SET_P 0" + stream.str();
    }
    else
    {
        command = "SET_P " + stream.str();
    }
    sendCommand(command);
}
