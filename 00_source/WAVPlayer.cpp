#include "WAVPlayer.hpp"
#include "Helper.hpp"
#include "Configuration.hpp"
#include <random>
#include <string>


using namespace std;

WavPlayer::WavPlayer(string configPath)
{
    this->curTick = 0;
    this->lastPlayedTick = 0;
    this->lastTimeSoundPlayed = std::chrono::steady_clock::now();

    //Iterate over Configuration and add soundfilepath and the duration in ticks to the Collision and Floating maps:
    Configuration* config = new Configuration(configPath);
    map<string,string> configMap = config->getConfigMap();
    string folder;
    string keySoundfilePath;
    unsigned int amountTicksSoundfile;
    folder = configMap["FolderSoundfiles"];
    configMap.erase("FolderSoundfiles");
    for (const auto& el : configMap)
    {
        keySoundfilePath = folder + el.second;
        amountTicksSoundfile = calculateSoundfileDuration(keySoundfilePath) / 1 +5; //Assuming each tick is 10ms-->1ms. also adding 50ms as extra pause.

        if (el.first[0] == 'C' && doesFileExist(keySoundfilePath))
        {
#ifdef _WIN32
            this->pathTicksCollision.insert_or_assign(keySoundfilePath, amountTicksSoundfile);
#else
            this->pathTicksCollision.emplace(keySoundfilePath, amountTicksSoundfile);
#endif
        }
        else if(el.first[0] == 'F' && doesFileExist(keySoundfilePath))
        {
#ifdef _WIN32
            this->pathTicksFloating.insert_or_assign(keySoundfilePath, amountTicksSoundfile);
#else
            this->pathTicksFloating.emplace(keySoundfilePath, amountTicksSoundfile);
#endif
        }
        //cout << el.first << " " << el.second << endl;
    }
    //print
    cout << "Collision:" << endl;
    for (const auto& el : this->pathTicksCollision)
        cout << el.first << " " << el.second << endl;
    cout << "Floating:" << endl;
    for (const auto& el : this->pathTicksFloating)
        cout << el.first << " " << el.second << endl;


    this->amountTicksLastPause = 0;
    this->debounce = 0;

}
WavPlayer::~WavPlayer()
{

}
//tick function is obsolete
void WavPlayer::tick()
{
    this->curTick += 1;
    //handle overflow on active pause --> overflow occurs after ~497 days without bump or wheel floating event --> edge case :)
    if (this->curTick == 0 && sizeof(unsigned int) - this->lastPlayedTick < this->amountTicksLastPause) //if overflow occured and pause is active
    {
        //calculate remaining ticks of the active pause and reset lastplayedtick.
        this->amountTicksLastPause = this->amountTicksLastPause - (sizeof(unsigned int) - this->lastPlayedTick);
        this->lastPlayedTick = 0;
    }
}
void WavPlayer::playSoundfileByCategory(short sf_cat)
{
    this->debounce += 1;
    size_t mapSize;
    auto it = this->pathTicksCollision.begin();
    bool initSuccess = true;
    //this->pauseTicksCollision;
    //unsigned int pauseTicks;
    //chrono::high_resolution_clock::now();
    //chrono::steady_clock::time_point tp;
    

    switch (sf_cat)
    {
    case _COLLISION:
        it = this->pathTicksCollision.begin();
        mapSize = this->pathTicksCollision.size();
        //pauseTicks = & this->pauseTicksCollision;
        break;
    case _FLOATING:
        it = this->pathTicksFloating.begin();
        mapSize = this->pathTicksFloating.size();
        //pauseTicks = &this->pauseTicksFloating;
        break;
    default:
        cout << "Error at playSoundfileByCategroy(), unknown soundfile_category" << endl;
        initSuccess = false;
        break;
    }
    //(chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - this->lastTimeSoundPlayed).count()
    if (initSuccess && (this->debounce % 1 == 0))
    {
        //int msTimePassed = (int) (chrono::duration<double, std::milli>(chrono::steady_clock::now() - this->lastTimeSoundPlayed).count());
        int msTimePassed = (int) (chrono::duration<double, std::milli>(chrono::steady_clock::now() - this->lastTimeSoundPlayed).count());
        //cout << "timePassed: " << (chrono::duration<double, std::milli>(chrono::steady_clock::now() - this->lastTimeSoundPlayed).count()) << endl;
        //cout << "timePassed: " << (chrono::duration<double, std::milli>(chrono::steady_clock::now() - this->lastTimeSoundPlayed).count()) << endl;
        //cout << "amountTicksLastPause: " << this->amountTicksLastPause << endl;

        if (msTimePassed > this->amountTicksLastPause)
        {
            advance(it, rand() % mapSize);
            const auto now = std::chrono::system_clock::now();
            const std::time_t t_c = std::chrono::system_clock::to_time_t(now);

            string cmd = "aplay  " + it->first + " &";
            cout << std::ctime(&t_c) << "Playing soundfile: [" << cmd << "]" << endl;
            std::chrono::time_point<std::chrono::steady_clock> timeBeforeCMD;
            timeBeforeCMD = chrono::steady_clock::now();
            cout << runShellCmdAndGetOutput(cmd) << endl;
            this->lastTimeSoundPlayed = chrono::steady_clock::now();
            int timeSpentOnSoundfileCMD = (int)(chrono::duration<double, std::milli>(chrono::steady_clock::now() - timeBeforeCMD).count());
            this->amountTicksLastPause = (it->second+100) - timeSpentOnSoundfileCMD;
            if (timeSpentOnSoundfileCMD > it->second) //100ms delay
                this->amountTicksLastPause = 100;
            else
                this->amountTicksLastPause = it->second - timeSpentOnSoundfileCMD;
            //cout << "TimeSpentOnSF: " << timeSpentOnSoundfileCMD << endl;
        }

    }



}
//TODO: Check weather device is busy. When device is busy the following error message is prompted: "aplay: main:828: audio open error: Device or resource busy"
//Non-Error case: Playing WAVE '/data/badDobby.wav' : Signed 16 bit Little Endian, Rate 44100 Hz, Mono
/*void WavPlayer::playSoundfileCollision()
{
    this->debounce += 1;
    //select random soundfile from map
    auto it = this->pathTicksCollision.begin();
    advance(it, rand() % this->pathTicksCollision.size()); //it->fist stores the random soundfilepath, it->second the duration of soundfile in ticks
    
    if ((this->lastPlayedTick + this->amountTicksLastPause) < this->curTick && this->debounce % 1 == 0)
    {
        string cmd = "aplay  " + it->first + " &";
        cout << "Playing soundfile: [" << cmd << "]" << endl;
        //string cmd = "aplay  /tmp/badDobby.wav &";
        cout << runShellCmdAndGetOutput(cmd) << endl;
        this->lastPlayedTick = 0;
        this->curTick = 0;
        this->pauseTicksCollision = it->second;
        this->amountTicksLastPause = this->pauseTicksCollision;
    }
}
void WavPlayer::playSoundFileFloating()
{
    this->debounce += 1;
    //select random soundfile from map
    auto it = this->pathTicksFloating.begin();
    advance(it, rand() % this->pathTicksFloating.size()); //it->fist stores the random soundfilepath
    if ((this->lastPlayedTick + this->amountTicksLastPause) < this->curTick && this->debounce % 1 == 0)
    {
        string cmd = "aplay  " + it->first + " &";
        cout << "Playing soundfile: [" << cmd << "]" << endl;
        //string cmd = "aplay  /tmp/badDobby.wav &";
        cout << runShellCmdAndGetOutput(cmd) << endl;
        this->lastPlayedTick = 0;
        this->curTick = 0;
        this->pauseTicksFloating = it->second;
        this->amountTicksLastPause = this->pauseTicksFloating;
    }
}*/

unsigned int WavPlayer::calculateSoundfileDuration(string soundfilePath)
{
    #ifdef _WIN32
    string fileA = "badDobby.wav";
    string fileB = "dobbyBeat.wav";
    string fileC = "dobbyWee.wav";
    if(soundfilePath.find(fileA) != string::npos)
        return 1370;
    else if(soundfilePath.find(fileB) != string::npos)
        return 700;
    else if(soundfilePath.find(fileC) != string::npos)
        return 2000;
    
    #else
    //get size on disk
    //cout << "get size on disk" << endl;
    string cmd = "du -k " + soundfilePath;
    string res = runShellCmdAndGetOutput(cmd);
    //cout << res << endl;
    int durationMS = 100;
    if(res.find("du:") == string::npos) //if no error
    {
        std::string::size_type i = res.find(soundfilePath);
        if (i != std::string::npos)
        {
            res.erase(i, soundfilePath.length());
            //cout << "res after erase: " << res << endl;
        }
        else
            cout << "Soundfilepath could not be erased. Content of res: " << res << endl;
        //calculate duration
        int filesizeKB = convertStringToInt10(res);
        //cout << "FilesizeKB [" << filesizeKB << "]" << endl;
        //durationMS = (filesizeKB * 800000) / (44100*1*16) *10; //durationMS = filesize[B] / (sampleRate*NumChannels*BitsPerSample/8) * 1000
        durationMS = (int) filesizeKB * 11.33787;
        //cout << "Duration: " << durationMS << " " << soundfilePath << endl;
    }
    else
    {
        cout << "Error in calculateSoundfileDuration: du -k Error" << endl;
    }

    return durationMS;
    #endif
}

int main_test_configuration()
{
    cout << "alive" << endl;
    //Configuration* myConfig = new Configuration("C:\\SysGCC\\raspberry\\files\\DobbySpeakConfig_WIN32.json");
    WavPlayer* myWavPlayer = new WavPlayer("C:\\SysGCC\\raspberry\\files\\DobbySpeakConfig_WIN32.json");
    return 0;
}
