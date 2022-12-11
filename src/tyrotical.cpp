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
            draw();
        }

        vkDeviceWaitIdle(_device.device());
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
        _pipeline = std::make_unique<Pipeline>(
            pipelineConfig, _device, "shaders/shader.vert.spv", "shaders/shader.frag.spv"
            );
    }

    void TyroticalApp::createCommandBuffers() {
        _commandBuffers.resize(_swapChain.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = _device.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(_commandBuffers.size());

        if (vkAllocateCommandBuffers(_device.device(), &allocInfo, _commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("[Error] Failed to allocate command buffers");
        }

        for (int i = 0; i < _commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("[Error] Failed to record command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = _swapChain.getRenderPass();
            renderPassInfo.framebuffer = _swapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = _swapChain.getSwapChainExtent();

            VkClearValue clearValues[2];
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {0, 0};
            renderPassInfo.clearValueCount = 2U;
            renderPassInfo.pClearValues = clearValues;

            vkCmdBeginRenderPass(_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            _pipeline->bind(_commandBuffers[i]);
            vkCmdDraw(_commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(_commandBuffers[i]);
            if (vkEndCommandBuffer(_commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("[Error] Failed to record command buffer");
            }
        }
    }

    void TyroticalApp::draw() {
        uint32_t imageIndex;
        VkResult result = _swapChain.acquireNextImage(&imageIndex);

        /* TODO: Handle error when resizing window */
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("[Error] Failed to acquire new image");
        }

        result = _swapChain.submitCommandBuffers(&_commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS) {
            throw std::runtime_error("[Error] Failed to submit command buffers");
        }
    }
}