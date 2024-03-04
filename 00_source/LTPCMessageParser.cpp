#include "LTPCMessageParser.hpp"
using namespace std;


LTPCMessageParser::LTPCMessageParser()
{
    this->lastBumpTrigger = new TriggerMessage();
    this->length = -1;
    this->type = -1;
    this->crcCalculator = new CRC16();
}
void LTPCMessageParser::update(string ltpcMessage)
{
    //Message must be at least: LENGTH(2 digits) | Type(2 digits) | Payload (>2 digits) | CRC (4 digits) --> >10
    if (ltpcMessage.length() >= 10)
    {
        ltpcMessage.erase(remove(ltpcMessage.begin(), ltpcMessage.end(), '\\'), ltpcMessage.end());
        ltpcMessage.erase(remove(ltpcMessage.begin(), ltpcMessage.end(), 'x'), ltpcMessage.end());
        this->length = convertStringToInt16(ltpcMessage.substr(0, 2));
        this->type = convertStringToInt16(ltpcMessage.substr(2, 2));
        if (this->type == 0x00 && length == 0x07 && this->crcCalculator->compareCRC16(ltpcMessage.substr(0, ltpcMessage.length() - 4), ltpcMessage.substr(ltpcMessage.length() - 4, 4)))
        {
            string payload = ltpcMessage.substr(4, length * 2);
            this->lastBumpTrigger->update(payload);
#ifdef _WIN32
            cout << this->lastBumpTrigger->toString() << endl;

#else
            this->lastBumpTrigger->checkBumpAndPlaySound();
#endif
        }
        /*else
        {
            if (this->type == 0x02)
            {
                this->lastBumpTrigger->tickPlayer();
            }
#ifdef _WIN32
            cout << "Non Trigger Message. Type: " << type << " Msg: " << ltpcMessage << endl;
#endif
        }*/
    }

}
LTPCMessageParser::~LTPCMessageParser()
{
        delete this->lastBumpTrigger;
        delete this->crcCalculator;
}
