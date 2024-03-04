#pragma once
#include <iostream>
#include <string>

/*
CRC 16 Modbus implementation
*/


class CRC16
{ 
private:
    static const std::uint16_t mod16CRCTable[];

public:
    static bool compareCRC16(std::string ltpString, std::string crcString);
};

