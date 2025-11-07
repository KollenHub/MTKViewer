#pragma once
#include <chrono>
#include <string>
class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTime;
    std::string m_TimerName;
    bool m_isRunning;
    void PrintTime();
public:
    Timer(const std::string &name);
    void Stop();
    
    ~Timer();
};
