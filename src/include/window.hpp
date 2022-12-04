#pragma once
#include "Ayanami.hpp"

namespace Tyrotical {

    struct Image {
        int width, height, channels;
        uint8_t* pixels;
        GLFWimage image[1];
    };

    class Window {
    public:
        Window(int width, int height, const char* windowTitle);
        ~Window();

        Window(const Window&) = delete;
        Window &operator=(const Window) = delete;

        bool closed() { return glfwWindowShouldClose(_window); }
        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        void setIcon(const char* filepath);
        void setIcon(Image& img);
    private:
        void createWindow();

        const int _width, _height;
        const char* _windowTitle;

        GLFWwindow* _window;
        Image _icon;
    };

}