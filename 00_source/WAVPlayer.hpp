#pragma once
#include "Helper.hpp"
#include <map>
#include <chrono>

/*
The Wav player is responsible for playing audio. It is called on bump- and wheel-floating-events.
*Each tick should me 10ms.
*
*/

class WavPlayer
{
private:
    unsigned int curTick;
    unsigned int lastPlayedTick;
    unsigned int pauseTicksFloating;    
    unsigned int pauseTicksCollision; 
    unsigned int amountTicksLastPause;
    unsigned int debounce;
    //std::string soundfilePathCollision;
    //std::string soundfilePathFloating;
    std::map<std::string, unsigned int> pathTicksCollision;
    std::map<std::string, unsigned int> pathTicksFloating;
    std::chrono::time_point<std::chrono::steady_clock> lastTimeSoundPlayed;
    //std::chrono::_
    //enum soundfile_category{ collision, floating };

    unsigned int calculateSoundfileDuration(std::string soundfilePath);
public:
    WavPlayer(std::string configPath);
    ~WavPlayer();
    void tick();
    void playSoundfileByCategory(short sf_cat);
    //void playSoundFileFloating();
};
