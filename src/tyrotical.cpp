#include "tyrotical.hpp"

namespace Tyrotical {

    void TyroticalApp::run() {
        _window.setIcon("res/icon.png");
        while (!_window.closed()) {
            glfwPollEvents();
        }
    }
}