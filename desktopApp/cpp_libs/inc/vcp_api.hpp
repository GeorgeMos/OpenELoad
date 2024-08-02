
#ifndef VCP_API
#define VCP_API

#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


/**
 * @brief Enumerate all available COM ports
 * 
 * @return std::vector<std::string> 
 */
std::vector<std::string> enumerateComPorts();


/**
 * @brief Device VCP
 * 
 */
extern std::fstream devPort;

/**
 * @brief Opens the VCP
 * 
 * @param comPort 
 * @return int (0 = OK, 1 = ERR)
 */
int openPort(const std::string& comPort);


/**
 * @brief Closes the VCP
 *  
 * @return void 
 */
void closePort();

/**
 * @brief Finds a specific device by sending a command and recieving a specific responce
 * 
 * @param comPort 
 * @param command 
 * @param expectedResponse 
 * @return true 
 * @return false 
 */
bool findSpecificDevice(const std::string& comPort, const std::string& command, const std::string& expectedResponse);

/**
 * @brief Receives data from device
 * 
 * @param rxBuf 
 * @param len 
 * @return void
 */
void readData(uint8_t* rxBuf, uint32_t len);

/**
 * @brief Sends data to device
 * 
 * @param txBuf 
 * @param len 
 * @return void
 */
void writeData(uint8_t* txBuf, uint32_t len);

#endif
