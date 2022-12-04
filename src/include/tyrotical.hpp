#pragma once 
#include "window.hpp"

namespace Tyrotical {

    class TyroticalApp {
    public:
        int width = 800;
        int height = 600;

        void run();
    private:
        Window _window{width, height, "Tyrotical"};
    };
}