#include "pipeline.hpp"

namespace Tyrotical {
    
    Pipeline::Pipeline(const PipelineConfigInfo& config, Device& device, const char* vertexShaderPath, const char* fragmentShaderPath) : _device{device} {
        createGraphicsPipeline(config, vertexShaderPath, fragmentShaderPath);
    }

    char* Pipeline::fileContents(const char* filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if (!file.is_open()) {
            char* err;
            sprintf(err, "[Error] failed to read file %s", filepath);
            throw std::runtime_error(err);
            free(err);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        char* buf = new char[fileSize];

        file.seekg(0);
        file.read(buf, fileSize);
        file.close();

        return buf;
    }

    void Pipeline::createGraphicsPipeline(const PipelineConfigInfo& config, const char* vertexShaderPath, const char* fragmentShaderPath) {
        char* vertexCode = fileContents(vertexShaderPath);
        char* fragmentCode = fileContents(fragmentShaderPath);
    }

    void Pipeline::createShaderModule(const char* code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        const std::vector<char> codeArray(code, code + sizeof(code) / sizeof(*code));
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = codeArray.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(codeArray.data());

        if (vkCreateShaderModule(_device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("[Error] Failed to create shader module");
        }
    }

    PipelineConfigInfo Pipeline::defaultConfig(uint32_t width, uint32_t height) {
        PipelineConfigInfo configInfo{};

        return configInfo;
    }
}