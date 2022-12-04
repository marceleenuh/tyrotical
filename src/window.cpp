#include "window.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Tyrotical {

    Window::Window(int width, int height, const char* windowTitle) : _width{width}, _height{height}, _windowTitle{windowTitle} {
        createWindow();
        hideConsole();
    }

    Window::~Window() {
        showConsole();
        glfwDestroyWindow(_window);
        glfwTerminate();
    }

    void Window::createWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        _window = glfwCreateWindow(_width, _height, _windowTitle, nullptr, nullptr);
    }

    void Window::setIcon(const char* filepath) {
        _icon.pixels = stbi_load(filepath, &_icon.width, &_icon.height, &_icon.channels, 4);

        GLFWimage images[1];
        images[0].width = _icon.width;
        images[0].height = _icon.height;
        images[0].pixels = _icon.pixels;

        glfwSetWindowIcon(_window, 1, images);
    }

    void Window::setIcon(Image& img) {
        GLFWimage images[1];
        images[0].width = img.width;
        images[0].height = img.height;
        images[0].pixels = img.pixels;

        glfwSetWindowIcon(_window, 1, images);
        _icon = img;
    }
}