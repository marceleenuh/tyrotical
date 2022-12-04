#pragma once
#include "Ayanami.hpp"
#include "device.hpp"

namespace Tyrotical {

    struct PipelineConfigInfo {};

    class Pipeline {
    public:
        Pipeline(const PipelineConfigInfo& config, Device& device, const char* vertexShaderPath, const char* fragmentShaderPath);
        ~Pipeline() {}

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