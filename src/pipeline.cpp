#include "pipeline.hpp"

namespace Tyrotical {
    
    Pipeline::Pipeline(const PipelineConfigInfo& config, Device& device, const char* vertexShaderPath, const char* fragmentShaderPath) : _device{device} {
        createGraphicsPipeline(config, vertexShaderPath, fragmentShaderPath);
    }

    Pipeline::~Pipeline() {
        vkDestroyShaderModule(_device.device(), vertexShader, nullptr);
        vkDestroyShaderModule(_device.device(), fragmentShader, nullptr);
        vkDestroyPipeline(_device.device(), _pipeline, nullptr);
    }

    std::vector<char> Pipeline::fileContents(const char* filepath) {
        std::ifstream file{filepath, std::ios::ate | std::ios::binary};

        if (!file.is_open()) {
            char* buf;
            sprintf(buf, "failed to open file: %s", filepath);
            throw std::runtime_error(buf);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void Pipeline::createGraphicsPipeline(const PipelineConfigInfo& config, const char* vertexShaderPath, const char* fragmentShaderPath) {
        assert(config.pipelineLayout != VK_NULL_HANDLE && "[Error] Failed to create graphics pipeline: missing pipelineLayout in configInfo");
        assert(config.renderPass != VK_NULL_HANDLE && "[Error] Failed to create graphics pipeline: missing renderPass in configInfo");
        
        auto vertexCode = fileContents(vertexShaderPath);
        auto fragmentCode = fileContents(fragmentShaderPath);

        createShaderModule(vertexCode, &vertexShader);
        createShaderModule(fragmentCode, &fragmentShader);

        VkPipelineShaderStageCreateInfo shaderStages[2];
        shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStages[0].module = vertexShader;
        shaderStages[0].pName = "main";
        shaderStages[0].flags = 0;
        shaderStages[0].pNext = nullptr;
        shaderStages[0].pSpecializationInfo = nullptr;

        shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStages[1].module = fragmentShader;
        shaderStages[1].pName = "main";
        shaderStages[1].flags = 0;
        shaderStages[1].pNext = nullptr;
        shaderStages[1].pSpecializationInfo = nullptr;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;

        VkPipelineViewportStateCreateInfo viewportInfo{};
        viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportInfo.viewportCount = 1;
        viewportInfo.pViewports = &config.viewport;
        viewportInfo.scissorCount = 1;
        viewportInfo.pScissors = &config.scissor;

        VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
            VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
    
        colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlendInfo.logicOpEnable = VK_FALSE;
        colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        colorBlendInfo.attachmentCount = 1;
        colorBlendInfo.pAttachments = &colorBlendAttachment;
        colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        colorBlendInfo.blendConstants[3] = 0.0f;  // Optional

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &config.inputAssemblyInfo;
        pipelineInfo.pViewportState = &viewportInfo;
        pipelineInfo.pRasterizationState = &config.rasterizationInfo;
        pipelineInfo.pMultisampleState = &config.multisampleInfo;
        pipelineInfo.pColorBlendState = &colorBlendInfo;
        pipelineInfo.pDepthStencilState = &config.depthStencilInfo;
        pipelineInfo.pDynamicState = nullptr;


        pipelineInfo.layout = config.pipelineLayout;
        pipelineInfo.renderPass = config.renderPass;
        pipelineInfo.subpass = config.subpass;

        pipelineInfo.basePipelineIndex = -1;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        
        if (vkCreateGraphicsPipelines(_device.device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS) {
            throw std::runtime_error("[Error] Failed to construct graphics pipeline");
        }
    }

    void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(_device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module");
        }
    }

    PipelineConfigInfo Pipeline::defaultConfig(uint32_t width, uint32_t height) {
        PipelineConfigInfo configInfo{};
        
        /* VALUES FROM BRENDAN GALEA !!! */
        /* https://pastebin.com/EmsJWHzb */
        configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;
        
        configInfo.viewport.x = 0.0f;
        configInfo.viewport.y = 0.0f;
        configInfo.viewport.width = static_cast<float>(width);
        configInfo.viewport.height = static_cast<float>(height);
        configInfo.viewport.minDepth = 0.0f;
        configInfo.viewport.maxDepth = 1.0f;
        
        configInfo.scissor.offset = {0, 0};
        configInfo.scissor.extent = {width, height};
        
        configInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        configInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
        configInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        configInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        configInfo.rasterizationInfo.lineWidth = 1.0f;
        configInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        configInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        configInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
        configInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        configInfo.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        configInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional
        
        configInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        configInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
        configInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        configInfo.multisampleInfo.minSampleShading = 1.0f;           // Optional
        configInfo.multisampleInfo.pSampleMask = nullptr;             // Optional
        configInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        configInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional
        
        configInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        configInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
        configInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        configInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        configInfo.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        configInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
        configInfo.depthStencilInfo.front = {};  // Optional
        configInfo.depthStencilInfo.back = {};   // Optional
        
        return configInfo;
    }
}