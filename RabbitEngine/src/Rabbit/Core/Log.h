#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Rabbit {

    class Log
    {
    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;
    };

}

// Core log macros
#define RB_CORE_TRACE(...)      ::Rabbit::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RB_CORE_INFO(...)       ::Rabbit::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RB_CORE_WARN(...)       ::Rabbit::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RB_CORE_ERROR(...)      ::Rabbit::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RB_CORE_CRITICAL(...)   ::Rabbit::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define RB_TRACE(...)           ::Rabbit::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RB_INFO(...)            ::Rabbit::Log::GetClientLogger()->info(__VA_ARGS__)
#define RB_WARN(...)            ::Rabbit::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RB_ERROR(...)           ::Rabbit::Log::GetClientLogger()->error(__VA_ARGS__)
#define RB_CRITICAL(...)        ::Rabbit::Log::GetClientLogger()->critical(__VA_ARGS__)





