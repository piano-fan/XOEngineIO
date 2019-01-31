#include <cassert>
#include <sstream>
#include <tuple>
#include <list>
#include "input.h"
#include "output.h"
#include "timer.h"


namespace XO{
    std::tuple<int, int, int> PieceFromString(std::stringstream &ss){
        std::string x, y, p;
        std::getline(ss, x, ',');
        std::getline(ss, y, ',');
        std::getline(ss, p, ',');

        return {std::stoi(x), std::stoi(y), std::stoi(p)};
    }

    std::tuple<int, int> PointFromString(std::stringstream &ss){
        std::string x, y;
        std::getline(ss, x, ',');
        std::getline(ss, y, ',');

        return {std::stoi(x), std::stoi(y)};
    }

    void InputHandler::SendBestMove(){
        auto t = Timer();
        auto [x, y, pos_count] = m_engine->MakeBestMove(GomocupStoneID::OWN);
        auto time = t.Get();
        SendMessage("Move played in " + std::to_string(time));
        SendMessage(std::to_string(pos_count) + " positions ("
                + std::to_string(static_cast<double>(time) / pos_count) + " per position)");
        SendCommand(std::to_string(x) + ',' + std::to_string(y));
    }

    InputHandler::InputHandler(){}

    void InputHandler::AttachCore(ICore &e){
        m_engine = &e;
    }

    void InputHandler::OnInput(std::string input){
        assert(m_engine);

        std::stringstream words(input);
        std::string command;
        words >> command;

        static Timer board_timer;

        if(m_loading){
            if(command == "done"){
                m_loading = false;
                SendMessage("Board loaded in " + std::to_string(board_timer.Get()));
                SendBestMove();
            }
            else if(command == "load"){
                m_loading = false;
                SendMessage("Board loaded in " + std::to_string(board_timer.Get()));
                SendCommand("OK");
            }
            else{
                auto ss = std::stringstream(command);
                auto [x, y, p] = PieceFromString(ss);
                m_engine->Set(x, y, GomocupStoneID(p));
            }
        }
        else if(command == "begin"){
            SendBestMove();
        }
        else if(command == "turn"){
            auto [x, y] = PointFromString(words);
            m_engine->Set(x, y, GomocupStoneID::OPPONENT);
            SendBestMove();
        }
        else if(command == "end"){
            m_running = false;
        }
        else if(command == "about"){
            auto [name, version, author] = m_engine->About();
            SendCommand("name=\"" + name + "\","
                    + "version=\"" + version + "\","
                    + "author=\"" + author + "\"");
        }
        else if(command == "start"){
            std::string arg1, arg2;
            words >> arg1;
            words >> arg2;
            int w = std::stoi(arg1);
            int h = std::stoi(arg2);
            auto t = Timer();
            m_engine->Resize(w, h);
            SendMessage("New game started in " + std::to_string(t.Get()));
            SendCommand("OK");
        }
        else if(command == "restart"){
            auto t = Timer();
            m_engine->NewGame();
            SendMessage("Game restarted in " + std::to_string(t.Get()));
            SendCommand("OK");
        }
        else if(command == "board"){
            assert(!m_loading);
            m_loading = true;
            board_timer = Timer();
        }
        /*else if(command == "takeback"){   //TODO: not supported yet
            auto [x, y] = PointFromString(words);
            m_engine->TakeBack(x, y);
            SendCommand("OK");
        }*/
        else if(command == "single_play"){
            std::list<std::pair<unsigned int, unsigned int>> ml;
            long total_variations = 0;
            auto t = Timer();
            while(1){
                auto [x, y, pos_count] = m_engine->MakeBestMove(GomocupStoneID::OWN);
                total_variations += pos_count;
                ml.push_back({x, y});
                if(m_engine->IsOver(x, y, GomocupStoneID::OWN)){
                    break;
                }
                auto [x2, y2, pos_count2] = m_engine->MakeBestMove(GomocupStoneID::OPPONENT);
                total_variations += pos_count2;
                ml.push_back({x2, y2});
                if(m_engine->IsOver(x2, y2, GomocupStoneID::OPPONENT)){
                    break;
                }
            }
            auto time = t.Get();
            auto movecount = ml.size();
            std::string movestring;
            for(auto &move: ml){
                movestring += " " + std::to_string(move.first) + " " + std::to_string(move.second);
            }
            std::string outputmessage = std::to_string(movecount) + " moves played in "+ std::to_string(time)
                + " (" + std::to_string(static_cast<double>(time) / movecount) + " per move)";
            std::string outputmessage2 = std::to_string(total_variations) + " positions"
                + " (" + std::to_string(static_cast<double>(time) / total_variations) + " per position)";
            SendCommand(std::string("play") + movestring);
            SendMessage(outputmessage);
            SendMessage(outputmessage2);
        }
        else if(command == "squareinfo"){
            int x, y;
            words >> x;
            words >> y;
            SendMultilineMessage(m_engine->GetSquareCacheRepr(x, y));
            SendCommand("OK");
        }
        else{
            SendCommand("unknown command: " + input);
        }
    }
}
