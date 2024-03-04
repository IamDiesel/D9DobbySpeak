#pragma once

#include "WAVPlayer.hpp"
#include <bitset>
#include <sstream>
/*
This file parses the first byte of trigger messages, since it contains  bump and wheel floating feedback.
The following 6 bytes are ignored. 
*/
class WavPlayer;
class TriggerMessage
{
private:
    std::string rawPayload;
    //byte 0
    bool key1;
    bool key2;
    bool key3;
    bool key4;
    bool leftBumper;
    bool rightBumper;
    bool leftWheelFloating;
    bool rightWheelFloating;
    bool isRobotFloating;
    bool wasRobotFloating;
    //byte 4
    bool isDocked;
    std::bitset<8> * bitsetPayload;
    std::uint16_t msgIDTrigger;
    WavPlayer * player;
public:
    TriggerMessage();
    void update(std::string rawPayload);
    ~TriggerMessage();
    void tickPlayer();
    std::string toString();
    void checkBumpAndPlaySound();
};