#include "StraceMessageParser.hpp"
using namespace std;


StraceMessageParser::StraceMessageParser()
{
    this->packetStart = false;
    this->escapeFound = false;
    this->packetEnd = false;
    this->amountMessages = 0;
    this->ltpcString = "";
    this->curState = init;
    this->nextState = init;
    this->ltpcMsgParser = NULL;
    this->ltpcMsgParser = new LTPCMessageParser();
}
StraceMessageParser::~StraceMessageParser()
{
    delete this->ltpcMsgParser;
}

void StraceMessageParser::addLine(string straceLine)
{
    //cout << "addLine called" << endl;
    //Line format: read(19, "\x3c\x12\x02\x3a\xe8\xed\x7a\xf6\xff\x0c\x00\x02\x00\x0d\x02\x71\x00\x59\x3f\x3f\x00\x00\xf7\xaa\x3e\x3c\x07\x00\x00\x00\x97\x09\x01\x00\x00\x41\x0b\x3e", 1024) = 38
    if (straceLine[0] == 'r')
    {
        size_t idxFirst = straceLine.find('"');
        size_t idxLast = straceLine.find_last_of('"');
        string stracePayload = straceLine.substr(idxFirst + 1, idxLast - idxFirst - 1);
        //line format: \x3c\x12\x02\x3a\xe8\xed\x7a\xf6\xff\x0c\x00\x02\x00\x0d\x02\x71\x00\x59\x3f\x3f\x00\x00\xf7\xaa\x3e\x3c\x07\x00\x00\x00\x97\x09\x01\x00\x00\x41\x0b\x3e
        stracePayload.erase(remove(stracePayload.begin(), stracePayload.end(), '\\'), stracePayload.end());
        stracePayload.erase(remove(stracePayload.begin(), stracePayload.end(), 'x'), stracePayload.end());
        string ltpcString = "";
        //line format: 3c12023ae8ed7af6ff0c0002000d027100593f3f0000f7aa3e3c070000009709010000410b3e
        for (int idx = 0; idx < stracePayload.length(); idx += 2)
        {
            //***************************************
            //read input
            //***************************************
            string byteString = stracePayload.substr(idx, 2);
            int curSymbol = convertStringToInt16(byteString);

            //***************************************
            //evaluate state transition
            //***************************************
            switch (this->curState)
            {
            case init:
                if (curSymbol == 60) //character '<' found --> start of sequence; Transition to Start
                {
                    this->nextState = start;
                }
                break;
            case start:
                if (curSymbol == 62) //character '>' found --> end of sequence;
                {
                    this->nextState = end;
                }
                else if (curSymbol == 63) //character '?' found --> next '<', '>', '?' must be escaped; 
                {
                    this->nextState = escape;
                }
                break;
            case escape:
                this->nextState = escaped;
                break;
            case escaped:              //character '?' found --> next '<', '>', '?' must be escaped; 
                if (curSymbol == 63)
                {
                    this->nextState = escape;
                }
                else
                {
                    this->nextState = start;
                }
                break;
            case end:
                if (curSymbol == 60)
                {
                    this->nextState = start;
                }
                else
                {
                    this->nextState = init;
                }
            }
            //***************************************
            //execute state actions
            //***************************************
            switch (this->curState)
            {
            case init:
                break;
            case start:
                if (this->nextState != escape && this->nextState != end) //add symbol if it is not escaped or end of message is reached
                    this->ltpcString += byteString;
                break;
            case escape:
                break;
            case escaped:
                this->ltpcString += byteString;
                break;
            case end:
                this->ltpcMsgParser->update(this->ltpcString);
                this->amountMessages += 1;
#ifdef _WIN32
                cout << "#MSG: " << this->amountMessages << "LEN: " << this->ltpcString.length() << endl;
#endif
                this->ltpcString = "";
            }
            //***************************************
            //execute transition
            //***************************************
            this->curState = this->nextState;
        }
    }

}

