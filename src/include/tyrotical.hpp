#pragma once 
#include "Ayanami.hpp"
#include "device.hpp"
#include "pipeline.hpp"
#include "swapchain.hpp"
#include "window.hpp"

namespace Tyrotical {

    class TyroticalApp {
    public:
        const int width = 800;
        const int height = 600;

        TyroticalApp();
        ~TyroticalApp();

        TyroticalApp(const TyroticalApp&) = delete;
        TyroticalApp &operator=(const TyroticalApp) = delete;

        void run();
    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void draw();

        Window _window{width, height, "Tyrotical"};
        Device _device{_window};
        SwapChain _swapChain{_device, _window.getExtent()};
        std::unique_ptr<Pipeline> _pipeline;
        VkPipelineLayout _pipelineLayout;
        std::vector<VkCommandBuffer> _commandBuffers;
    };
}