#include "tyrotical.hpp"

namespace Tyrotical {

    TyroticalApp::TyroticalApp() {
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    TyroticalApp::~TyroticalApp() {
        vkDestroyPipelineLayout(_device.device(), _pipelineLayout, nullptr);
    }

    void TyroticalApp::run() {
        _window.setIcon("res/icon.png");
        while (!_window.closed()) {
            glfwPollEvents();
        }
    }

    void TyroticalApp::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(_device.device(), &pipelineLayoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("[Error] Failed to construct graphics pipeline layout");
        }
    }

    void TyroticalApp::createPipeline() {
        PipelineConfigInfo pipelineConfig = Pipeline::defaultConfig(_swapChain.width(), _swapChain.height());
        pipelineConfig.renderPass = _swapChain.getRenderPass();
        pipelineConfig.pipelineLayout = _pipelineLayout;
        _pipeline = std::make_unique<Pipeline>(pipelineConfig, _device, "shaders/shader.vert.spv", "shaders/shader.frag.spv");
    }

    void TyroticalApp::createCommandBuffers() {}
    void TyroticalApp::draw() {}
}