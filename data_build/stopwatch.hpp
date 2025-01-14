#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>
#include <cassert>

class stopwatch{
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> startTime, endTime;
        std::chrono::duration<double> totalDuration;
        bool running;
    
    public:
        inline stopwatch(): running(false), totalDuration(std::chrono::duration<double>::zero()) {}
    
        inline void start(){
            assert(!running);
            startTime = std::chrono::high_resolution_clock::now();
            running = true;
        }
    
        inline void stop(){
            assert(running);
            endTime = std::chrono::high_resolution_clock::now();
            totalDuration += endTime - startTime;
            running = false;
        }
    
        inline void reset(){
            running = false;
            totalDuration = std::chrono::duration<double>::zero();
        }
    
        inline double elapsed(){
            if(running){
                auto currentTime = std::chrono::high_resolution_clock::now();
                return totalDuration.count() + std::chrono::duration<double>(currentTime - startTime).count();
            }else return totalDuration.count();
        }
};

#endif
