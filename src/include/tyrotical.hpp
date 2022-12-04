#pragma once 
#include "pipeline.hpp"
#include "window.hpp"
#include "device.hpp"

namespace Tyrotical {

    class TyroticalApp {
    public:
        const int width = 800;
        const int height = 600;

        void run();
    private:
        Window _window{width, height, "Tyrotical"};
        Device _device{_window};
        Pipeline _pipeline{Pipeline::defaultConfig(width, height), _device, "shaders/shader.vert.spv", "shaders/shader.frag.spv"};
    };
}