#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/platform_sink.h>

class Logger
{
private:
    std::shared_ptr<spdlog::logger> m_Logger;

    Logger()
    {
        auto console_sink = spdlog::make_platform_sink<spdlog::sinks::stdout_color_sink_mt>();

        m_Logger = std::make_shared<spdlog::logger>("console", std::move(console_sink));
    }

public:
    static Logger &Instance()
    {
        static Logger instance;
        return instance;
    }

    // 删除拷贝构造函数和赋值运算符，确保单例
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    static void SetOutLevel(spdlog::level::level_enum level)
    {
        Instance().m_Logger->set_level(level);
    }

    // 提供日志接口
    template <typename... Args>
    static void trace(const char *fmt, const Args &...args)
    {
        Instance().m_Logger->trace(fmt, args...);
    }

    template <typename... Args>
    static void debug(const char *fmt, const Args &...args)
    {
        Instance().m_Logger->debug(fmt, args...);
    }

    template <typename... Args>
    static void info(const char *fmt, const Args &...args)
    {
        Instance().m_Logger->info(fmt, args...);
    }

    template <typename... Args>
    static void warn(const char *fmt, const Args &...args)
    {
        Instance().m_Logger->warn(fmt, args...);
    }

    template <typename... Args>
    static void error(const char *fmt, const Args &...args)
    {
        Instance().m_Logger->error(fmt, args...);
    }

    template <typename... Args>
    static void critical(const char *fmt, const Args &...args)
    {
        Instance().m_Logger->critical(fmt, args...);
    }
};
