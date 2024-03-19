# D9DobbySpeak
Play individual soundfiles when cleaning robot hits obstacle or is lifted from the ground. Compatible with Dreame D9.
![image](https://github.com/IamDiesel/D9DobbySpeak/assets/12062956/be1a5115-e55b-4807-a842-c9a36ef5abc2)


# Installation
## Preparation
Dreame D9 must be rooted. See https://valetudo.cloud/ for details. Credits to the valetudo team and their amazing work.

## A) Build binaries on windows (optional)
1) Download and install raspberry-gcc6.3.0-r5.exe (448 MB) from https://gnutoolchains.com/raspberry/
2) Install Visual Studio Code
3) Install the C/C++ extension for VS Code https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
4) Open VS Code
5) Click "Run" -> "Add Configuration" --> "C++ Windows"
6) Copy "tasks.json" from "00_source" to folder ".vscode" within the current VS Code Folder. Content must be:
    ```
    {
        "version": "2.0.0",
        "tasks": [
            {
                "type": "cppbuild",
                "label": "C/C++: arm-linux-gnueabihf-g++.exe build active file",
                "command": "c:\\SysGCC\\raspberry\\bin\\arm-linux-gnueabihf-g++.exe",
                "args": [
                    "-fdiagnostics-color=always",
                    "-g",
                    "${fileDirname}\\DobbySpeak.cpp",
                    "${fileDirname}\\CRC16.cpp",
                    "${fileDirname}\\Helper.cpp",
                    "${fileDirname}\\LTPCMessageParser.cpp",
                    "${fileDirname}\\StraceMessageParser.cpp",
                    "${fileDirname}\\TriggerMessage.cpp",
                    "${fileDirname}\\WAVPlayer.cpp",
                     "${fileDirname}\\Configuration.cpp",
                    "-o",
                    "${fileDirname}\\..\\01_bin\\data\\${fileBasenameNoExtension}"
                ],
                "options": {
                    "cwd": "c:\\SysGCC\\raspberry\\bin"
                },
                "problemMatcher": [
                    "$gcc"
                ],
                "group": {
                    "kind": "build",
                    "isDefault": true
                },
                "detail": "compiler: c:\\SysGCC\\raspberry\\bin\\arm-linux-gnueabihf-g++.exe"
            }
        ]
    }
   ```
7) Click on "Terminal" -> "Configure Default Build Task" --> Select "C/C++: arm-linux-gnueabihf-g++.exe..."
8) In VS Code open DobbySpeak.cpp and select "Run build task". The binary will generated under "01_bin/data/DobbySpeak" (without a trailing version number).
9) Additionally strace for arm is needed which is not part of this source code. E.g. strace arm can be found here: https://archlinuxarm.org/packages/aarch64/strace

## B) Transfer binaries & run DobbySpeak
* Transfer files (e.g. via WinScp) to robot
   * Folder DobbySounds to /data/DobbySounds
   * DobbySpeak_v010 to /data/DobbySpeak_v010
   * strace to /data/strace
* Manually start DobbySpeak 
  * Connect to robot via ssh
  * Make DobbySpeak executable: "chmod +x /data/DobbySpeak_v010"
  * Run DobbySpeak and pass output to logfile: /data/DobbySpeak_v010 &> /tmp/dobbyspeak.log &
  
* Hook DobbySpeak to firmware autostart: It is recommended to test DobbySpeak before permanently running it on startup.
  * rename /data/_root_postboot.sh to /data/_root_postboot_bu.sh
  * copy _root_postboot.sh to /data/_root_postboot.sh
  * check weather _root_postboot.sh is executable (if not run chmod +x _root_postboot.sh)
  * Restart robot, DobbySpeak should now run on startup
 
## C) Customize
Edit /data/DobbySounds/DobbySpeakConfig.json
* Folder Soundfiles: Path where the soundfiles are stored
* CN (where N is a number from 0 to 9): soundfiles that are randomly played when one or both of the tactile sensors are pressed (e.g. when robot crashes against an obstacle)
* FN (where N is a number from 0 to 9): soundfiles that are randomly played when both robot wheels are suspended from the ground (e.g. when the robot is lifted up in the air)

 ```yaml
{
  "FolderSoundfiles":  "/data/DobbySounds/",
  "C0": "badDobby.wav",
  "C1": "dobbyBeat.wav",
  "C2": "dobbyFart1.wav",
  "C3": "glass.wav",
  "C4": "shipHorn.wav",
  "F0": "dobbyWee.wav",
  "F1": "dobbyCry.wav",
  "F2": "dobbyJumpingOnBed.wav"
}


   
