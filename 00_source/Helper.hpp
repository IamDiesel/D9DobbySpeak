#pragma once
#include <iostream>
#include <string>
#include "StraceMessageParser.hpp"
#define _COLLISION 0
#define _FLOATING 1

/*
Helper functions for string conversion and shell I/O. 
*/
//enum soundfile_category{ collision, floating };
std::int16_t convertStringToInt16(std::string str);
int convertStringToInt10(std::string str);
std::string runShellCmdAndGetOutput(std::string cmd);
void traceMCUAndCheckForBumps(std::string mcuPID);
bool doesFileExist(std::string filepath);
bool isFileExecutable(std::string filepath);

