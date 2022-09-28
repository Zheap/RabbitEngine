#pragma once

#include "Rabbit/Core/PlatformDetection.h"

#include <memory>

#ifdef RB_DEBUG
    #if defined(RB_PLATFORM_WINDOWS)
        #define RB_DEBUGBREAK() __debugbreak()
    #elif defined(RB_PLATFORM_LINUX)
        #include <signal.h>
        #define RB_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define RB_ENABLE_ASSERTS
#else
    #define RB_DEBUGBREAK()
#endif

#define RB_EXPAND_MACRO(x) x
#define RB_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define RB_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Rabbit {

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}

#include "Rabbit/Core/Log.h"
#include "Rabbit/Core/Assert.h"
