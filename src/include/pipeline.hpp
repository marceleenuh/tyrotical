#pragma once
#include "Ayanami.hpp"
#include "device.hpp"

namespace Tyrotical {

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class Pipeline {
    public:
        Pipeline(const PipelineConfigInfo& config, Device& device, const char* vertexShaderPath, const char* fragmentShaderPath);
        ~Pipeline();

        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;

        static PipelineConfigInfo defaultConfig(uint32_t width, uint32_t height);
        void bind(VkCommandBuffer commandBuffer);

    private:
        static std::vector<char> fileContents(const char* filepath);

        void createGraphicsPipeline(const PipelineConfigInfo& config, const char* vertexShaderPath, const char* fragmentShaderPath);
        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
        
        Device& _device;
        VkPipeline _pipeline;
        VkShaderModule vertexShader;
        VkShaderModule fragmentShader;
    };
}