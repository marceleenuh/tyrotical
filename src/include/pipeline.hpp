#pragma once
#include "Ayanami.hpp"
#include "device.hpp"

namespace Tyrotical {

    struct PipelineConfigInfo {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
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
        void operator=(const Pipeline&) = delete;

        static PipelineConfigInfo defaultConfig(uint32_t width, uint32_t height);

    private:
        static char* fileContents(const char* filepath);

        void createGraphicsPipeline(const PipelineConfigInfo& config, const char* vertexShaderPath, const char* fragmentShaderPath);
        void createShaderModule(const char* code, VkShaderModule* shaderModule);
        
        Device& _device;
        VkPipeline _pipeline;
        VkShaderModule vertexShader;
        VkShaderModule fragmentShader;
    };
}