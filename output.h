#ifndef XO_OUTPUT_H
#define XO_OUTPUT_H

#include <string>


namespace XO{
    void SendCommand(std::string);
    void SendMessage(std::string);
    void SendMultilineMessage(std::string);
}


#endif // XO_OUTPUT_H
