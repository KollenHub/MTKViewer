#include "Timer.h"
#include "Logger.h"

Timer::Timer(const std::string &name) : m_TimerName(name), m_StartTime(std::chrono::high_resolution_clock::now())
{
}

void Timer::PrintTime()
{
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = endTime - m_StartTime;
    Logger::info("{} 耗时:{}s", m_TimerName, duration.count());
}
void Timer::Stop()
{
    m_isRunning = false;
    PrintTime();
}

Timer::~Timer()
{
    if (m_isRunning)
    {
        PrintTime();
    }
}
