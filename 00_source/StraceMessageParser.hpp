#pragma once
#include "LTPCMessageParser.hpp"

/*
Strace LTPC-Data is received as string:
---------------------------------------

+--------------------+----------------+------------------+-----------------------+-----------------+-----------------+------------------------------------+-----------------+--------------------+------------------+------------------+
| I/O Statement      | Num Param      | Strace MSG Delim | LTPC MSG Start Delim  | LTPC Length     | LTPC Type       | LTPC Payload                       | LTPC CRC        | LTPC MSG End Delim | Strace MSG Delim | Postamble        |
| (5/6 char)         | (>3 char)      | (1 char)         |     (4 char)          |   (4 char)      |   (4 char)      |  (>4 char)                         | (4 char)        | (4 char)           |  (1 char)        |  (>8  char)      |
| [read(] / [write(] | [19, ]         |    ["]           |      [\x3c]           |   [\x07]        |   [\x00]        | [\x00\x00\x00\xa7\x0d\x01\x00\x00] | [\x75\x4a]      | [\x3e]             |    ["]           | [, 15) = 15]     |
+--------------------+----------------+------------------+-----------------------+-----------------+-----------------+------------------------------------+-----------------+--------------------+------------------+------------------+

* Only messages from MCU are parsed (I/O-Statement  = read).
* LTPC Message is hex, all lowercase ASCII. 
* LTPC part of the message can be split across multiple strace messages, also multiple ltpc messages can be part of one strace message.
* LTPC delimiters (\x3c, \x3e) can be escaped by \x3f. 
* Modbus CRC / LTPC payload transmission is flawed. Modbus lookup table can be found at CRC16.cpp. CRC is calculated over ltp-message (length, type, payload). 



*/

//using namespace std;
class LTPCMessageParser;
class StraceMessageParser
{
private:
    std::string rawMessage;
    bool packetStart;
    bool escapeFound;
    bool packetEnd;
    int amountMessages;
    std::string ltpcString;
    LTPCMessageParser * ltpcMsgParser;
    enum state { init, start, escape, escaped, end };
    state curState;
    state nextState;

public:
    StraceMessageParser();
    ~StraceMessageParser();
    void addLine(std::string straceLine);
};
