#include "TriggerMessage.hpp"
#include "Helper.hpp"

using namespace std;
//enum soundfile_category{ collision, floating };
TriggerMessage::TriggerMessage()
{
    this->player = new WavPlayer("/data/DobbySounds/DobbySpeakConfig.json");
    this->msgIDTrigger = 0x00;
    this->bitsetPayload = NULL;
    this->key1 = false;
    this->key2 = false;
    this->key3 = false;
    this->key4 = false;
    this->leftBumper = false;
    this->rightBumper = false;
    this->leftWheelFloating = false;
    this->rightWheelFloating = false;
    this->isRobotFloating = false;
    this->wasRobotFloating = false;
    this->isDocked = false;
}
void TriggerMessage::update(string rawPayload)
{
    this->rawPayload = rawPayload; //Must be 7Bytes long
    string byte0 = rawPayload.substr(0, 2);    
    this->bitsetPayload = new bitset<8>(convertStringToInt16(byte0));
    this->key1 = bitsetPayload->test(0);
    this->key2 = bitsetPayload->test(1);
    this->key3 = bitsetPayload->test(2);
    this->key4 = bitsetPayload->test(3);
    this->leftBumper = bitsetPayload->test(4);
    this->rightBumper = bitsetPayload->test(5);
    this->leftWheelFloating = bitsetPayload->test(6);
    this->rightWheelFloating = bitsetPayload->test(7);
    this->wasRobotFloating = this->isRobotFloating;
    this->isRobotFloating = this->leftWheelFloating && this->rightWheelFloating;

    string byte4 = rawPayload.substr(8, 2);
    delete this->bitsetPayload,
    this->bitsetPayload = new bitset<8>(convertStringToInt16(byte4));
    this->isDocked = bitsetPayload->test(0);
    //cout << "Doked: " << this->isDocked << endl;
}
TriggerMessage::~TriggerMessage()
{
    delete this->bitsetPayload;
    delete this->player;
}
//tickPlayer is obsolete
void TriggerMessage::tickPlayer()
{
    this->player->tick();
}
string TriggerMessage::toString()
{
    streambuf* sCoutBuff = cout.rdbuf();
    ostringstream myOstream;
    cout.rdbuf(myOstream.rdbuf());
    cout << "Key1:" << "\t\t" << "Key2:" << "\t\t" << "Key3:" << "\t\t" << "Key4:" << "\t\t" << "lBump:" << "\t\t" << "rBump:" << "\t\t" << "lWlFlo:" << "\t\t" << "rWlFlo:" << endl;
    cout << this->key1 << "\t\t" << this->key2 << "\t\t" << this->key3 << "\t\t" << this->key4 << "\t\t" << this->leftBumper << "\t\t" << this->rightBumper << "\t\t" << this->leftWheelFloating << "\t\t" << this->rightWheelFloating << endl;
    cout.rdbuf(sCoutBuff);
    return myOstream.str();
}
void TriggerMessage::checkBumpAndPlaySound()
{
    if (this->isDocked == false && (this->leftBumper == true || this->rightBumper == true))
    {
        this->player->playSoundfileByCategory(_COLLISION);
    }
    if (this->isRobotFloating && !wasRobotFloating)
    {
        this->player->playSoundfileByCategory(_FLOATING);
    }
}