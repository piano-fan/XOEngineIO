#ifndef XO_INPUT_H
#define XO_INPUT_H

#include <string>
#include "icore.h"


namespace XO{
    class InputHandler{
        ICore* m_engine = nullptr;
        bool m_loading = false;
        bool m_running = true;

        void SendBestMove();
    public:
        InputHandler();
        void AttachCore(ICore &e);

        void OnInput(std::string input);
        bool Running() const{
            return m_running;
        }
    };
}

#endif // XO_INPUT_H
