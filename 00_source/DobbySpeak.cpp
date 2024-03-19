#include "Helper.hpp"
//#ifdef _WIN32
#include <fstream>
//#endif

#define __VERSION_DOB__ "0.1.0"
using namespace std;


#ifdef _WIN32
int main()
{

    string rawMessage = "\\x07\\x00\\x00\\x00\\x07\\x00\\x00\\x00\\x00\\x00\\x99";
    string rawMessage2 = "\\x07\\x00\\x00\\x00\\xa7\\x5c\\x01\\x00\\x00\\x89\\x5a";
    string rawMessage3 = "\\x07\\x00\\x10\\x00\\x07\\x00\\x00\\x00\\x00\\xc1\\x88"; //bumpOccured
    //LTPCMessageParser* parser = new LTPCMessageParser(rawMessage3);
    StraceMessageParser* sParser = new StraceMessageParser();
    //sParser->addLine("read(19, \"\\x3c\\x07\\x00\\x00\\x00\\xa7\\x0c\\x01\\x00\\x00\\x89\\x4b\\x3e\", 1024) = 22");
    //sParser->addLine("read(19, \"\\x3c\\x07\\x00\\x00\\x00\\xa7\\x0c\\x01\\x00\\x00\\x89\\x4b\\x3e\", 1024) = 22");
    //sParser->addLine("read(19, \"\\x3c\\x07\\x00\\x00\\x00\\xa7\\x0c\\x01\\x00\\x00\\x89\\x4b\\x3e\", 1024) = 22");

    FILE* fp;
    cout << fopen_s(&fp, "D:\\DobbySpeakGIT\\D9DobbySpeak\\02_doc\\straceOutputStrSize128", "r") << endl;
    ifstream file(fp);

    if (file.is_open())
    {
        string straceLine;
        while (getline(file, straceLine))
        {
            //cout << "StraceLine: " << straceLine << endl;
            sParser->addLine(straceLine);
        }
    }
    return 0;
}
#else

int main() {
    string cmd = "lsof | grep /dev/ttyS4";
    string res = runShellCmdAndGetOutput(cmd);
    cout << "Res of: " << cmd << ":[" << res << "]" << endl;
    if (res.find("usr/bin/ava") != string::npos)
    {
        string mcuPID = res.substr(0, 4);
        cout << "Found ava mcu process ID: " << mcuPID << endl;
        //cmd = "test -f /data/strace && echo \"OK\"";
        //res = runShellCmdAndGetOutput(cmd);
        if (doesFileExist("/data/strace"))
        {
            cout << "strace found" << endl;
            //cmd = "test -x /data/strace && echo \"OK\"";
            //res = runShellCmdAndGetOutput(cmd);
                if (isFileExecutable("/data/strace"))
                {
                    cout << "strace is executable" << endl;
                    cmd = "/data/strace -s 128 -xx -p " + mcuPID + " -P /dev/ttyS4";
                    cout << "Version: " << __VERSION_DOB__ << endl;
                    traceMCUAndCheckForBumps(mcuPID);
                }
                else
                    cout << "Strace is not executable" << endl;
        }
        else
            cout << "Strace not found" << endl;


    }
    return 0;
}
#endif