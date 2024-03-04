#include "Helper.hpp"
#include <exception>
using namespace std;


int16_t convertStringToInt16(string str)
{
    try
    {
        return std::stoi(str, nullptr, 16);
    }
    catch (const invalid_argument& e)
    {
        cout << "Error at convertStringToInt16()" << endl;
        cout << e.what() << '\n' << "Input String: [" << str << "]" << endl;
        return 0x0000;
    }
    
}
int convertStringToInt10(string str)
{
    try
    {
        return std::stoi(str, nullptr, 10);
    }
    catch (const invalid_argument& e)
    {
        cout << "Error at convertStringToInt10()" << endl;
        cout << e.what() << '\n' << "Input String: [" << str << "]" << endl;
        return 0x00000000;
    }
    
}

string runShellCmdAndGetOutput(string cmd) {
    char buffer[256];
    string res = "";
#ifdef _WIN32
    FILE* shellPipe = _popen(cmd.c_str(), "r");
#else
    FILE* shellPipe = popen(cmd.c_str(), "r");
#endif
    if (!shellPipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(shellPipe)) {
            if (fgets(buffer, 256, shellPipe) != NULL)
                res += buffer;
        }
    }
    catch (...) {
#ifdef _WIN32
        _pclose(shellPipe);
#else
        pclose(shellPipe);
#endif
        throw;
    }
#ifdef _WIN32
    _pclose(shellPipe);
#else
    pclose(shellPipe);
#endif
    return res;
}
void traceMCUAndCheckForBumps(string mcuPID) {
    char buffer[256];
    StraceMessageParser* sParser = new StraceMessageParser();
    string cmd = "/data/strace -s 128 -xx -p " + mcuPID + " -P /dev/ttyS4 2>&1";

#ifdef _WIN32
    FILE* shellPipe = _popen(cmd.c_str(), "r");
#else
    FILE* shellPipe = popen(cmd.c_str(), "r");
#endif
    if (!shellPipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(shellPipe)) {
            if (fgets(buffer, 256, shellPipe) != NULL)
                //cout << buffer;
                //cout << "line" << endl;
                sParser->addLine(buffer);
            //res += buffer;
        }
    }
    catch (...) {
#ifdef _WIN32
        _pclose(shellPipe);
#else
        pclose(shellPipe);
#endif
        throw;
    }
#ifdef _WIN32
    _pclose(shellPipe);
#else
    pclose(shellPipe);
#endif
    //return res;
}

#ifdef _WIN32
bool doesFileExist(string filepath)
{
    return true;
}
#else
bool doesFileExist(string filepath)
{
    string cmd = "test -f " + filepath + " && echo \"OK\"";
    string res = runShellCmdAndGetOutput(cmd);
    return (res.find("OK") != string::npos);
}
#endif

bool isFileExecutable(string filepath)
{
    string cmd = "test -x " + filepath + " && echo \"OK\"";
    string res = runShellCmdAndGetOutput(cmd);
    return (res.find("OK") != string::npos);
}

