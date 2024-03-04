#pragma once
#include "TriggerMessage.hpp"
#include "CRC16.hpp"
#include <algorithm>
/*
This file parses LTPC messages. After parsing, only trigger messages (type 0x00) stay in focus of this program.
 
Format of LTPC (=[L]ength, [T]ype, [P]ayload, [C]rc) Message is):
-----------------------------------------------------------------

+----------------+----------------+-----------------------+-----------------+
| Length         | Type of Message| Payload               | Modbus-CRC-16   |
| (1 byte)       | (1 byte)       | (Len > 1 Byte?)       | (2 bytes)       |
+----------------+----------------+-----------------------+-----------------+
Data is received as string:
+----------------+----------------+-----------------------+-----------------+
| Length         | Type of Message| Payload               | Modbus-CRC-16   |
| (2 char)       | (2 char)       | (Len > 2 char?)       | (4 char)        |
+----------------+----------------+-----------------------+-----------------+


LTPC Message-Types:
----------------------
+--------+--------------------+
| Type # | Description        |
+--------+--------------------+
|  0x00  | Trigger Message    |
+--------+--------------------+
|  0x01  | Status MSG (20ms)  |
+--------+--------------------+
|  0x02  | Status MSG (10ms)  |
+--------+--------------------+
|  0x03  | Status MSG (100ms) |
+--------+--------------------+
|  ...   | ...                |
+--------+--------------------+

*/
//using namespace std;
class TriggerMessage;
class LTPCMessageParser
{
private:
    
    std::uint16_t length;
    std::uint16_t type;
    TriggerMessage * lastBumpTrigger;
    CRC16 * crcCalculator;
public:
    LTPCMessageParser();
    ~LTPCMessageParser();
    void update(std::string ltpcMessage);

};

