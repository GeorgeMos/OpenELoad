#include <iostream>
#include <usbCom.hpp>
#include <vector>

UsbCom com = UsbCom();

int main()
{
    std::vector<std::string> ports = com.listPorts();
    for(int i = 0; i < ports.size(); i++)
    {
        if(com.checkPort(ports[i]))
        {
            std::cout<<"Found Port: " << ports[i] << "\n";
            com.setPort(ports[i]);
            break;
        }
    }

    
    if(com.connect())
    {
        std::cout << "Connected to Port: " << com.getPort() << "\n";
        com.sendCommand(USB_COMMANDS::SET_CR_MODE);
        system("pause");
        com.setR(999);
        system("pause");
    }
    else
    {
        std::cout << "Failed to connect to port: " << com.getPort() << "\n";
        return 0;
    }
    com.closePort();
    
    return 0;
}