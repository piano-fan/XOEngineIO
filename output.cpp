#include "output.h"
#include <iostream>
#include <sstream>


namespace XO{
    void SendCommand(std::string com){
        std::cout << com << std::endl;
    }

    void SendMessage(std::string msg){
        std::cout << "message " << msg << std::endl;
    }

    void SendMultilineMessage(std::string msg){
        std::stringstream ss(msg);
        std::string next;
        while(std::getline(ss, next, '\n')){
            SendMessage(next);
        }
    }
}
