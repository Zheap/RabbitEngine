#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Rabbit/Core/Log.h"
#include "Rabbit/Debug/Instrumentor.h"

#ifdef RB_PLATFORM_WINDOWS
    #include <Windows.h>
#endif