#pragma once

#include "Rabbit/Core/Core.h"
#include "Rabbit/Core/Log.h"
#include <filesystem>

#ifdef RB_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define RB_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { RB##type##ERROR(msg, __VA_ARGS__); RB_DEBUGBREAK(); } }
#define RB_INTERNAL_ASSERT_WITH_MSG(type, check, ...) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
#define RB_INTERNAL_ASSERT_NO_MSG(type, check) RB_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", RB_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define RB_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define RB_INTERNAL_ASSERT_GET_MACRO(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, RB_INTERNAL_ASSERT_WITH_MSG, RB_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define RB_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
#define RB_CORE_ASSERT(...) RB_EXPAND_MACRO( RB_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
#define RB_ASSERT(...)
#define RB_CORE_ASSERT(...)
#endif
