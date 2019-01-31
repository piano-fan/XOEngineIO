#include <iostream>
#include "input.h"
#include "output.h"
#include "timer.h"


int main(int argc, char** argv){
    XO::InputHandler ih;
    auto core = XO::MakeCore();

    auto t = XO::Timer();
    core->Init();
    XO::SendMessage("Core initialized in " + std::to_string(t.Get()));

    ih.AttachCore(*core);

    std::string input;
    do{
        if( !std::cin.eof() ) {
            std::getline(std::cin, input);
            ih.OnInput(input);
        }
    } while( ih.Running() );

    return 0;
}
