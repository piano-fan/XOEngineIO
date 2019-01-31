#ifndef XO_TIMER_H
#define XO_TIMER_H

#include <ctime>
#include <string>


namespace XO{
    class Timer{
        clock_t m_start;
    public:
        Timer(){
            Reset();
        }
        void Reset(){
            m_start = clock();
        }
        clock_t Get() const{
            return (clock() - m_start) / (double)CLOCKS_PER_SEC * 1e6;
        }
    };
}

#endif // XO_TIMER_H
