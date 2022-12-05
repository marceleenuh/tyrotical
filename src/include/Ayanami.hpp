#pragma once
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <iostream>
#include <set>
#include <unordered_set>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

namespace Tyrotical {
    static inline void hideConsole() {
        #ifdef _WIN32
        ShowWindow(GetConsoleWindow(), SW_HIDE);
        #else
        printf("[Error] Couldn't hide console window.");
        #endif
    }

    static inline void showConsole() {
        #ifdef _WIN32
        ShowWindow(GetConsoleWindow(), SW_SHOW);
        #else
        printf("[Error] Couldn't show console window.");
        #endif
    }
}