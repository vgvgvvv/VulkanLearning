#include <iostream>
#include <stdexcept>
#include <fstream>


#include "FirstTriangleApplication.h"

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }
}

void FirstTriangleApplication::CreateGraphicsPipeline()
{

	// 创建可编程管线阶段
	
	// 读取shader
    auto vertexShader = readFile("../../src/demos/VulkanApp/Shader/vert.spv");
    auto fragmentShader = readFile("../../src/demos/VulkanApp/Shader/frag.spv");

	// 创建Shader Module
    auto vertexShaderModule = CreateShaderModule(vertexShader);
    auto fragmentShaderModule = CreateShaderModule(fragmentShader);

	// 创建顶点阶段
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    // 使用的module
	vertexShaderStageInfo.module = vertexShaderModule;
	// 入口
    vertexShaderStageInfo.pName = "main";

    // 创建片元阶段
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };
	
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);
    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);

	// 固定管线阶段
	
}
