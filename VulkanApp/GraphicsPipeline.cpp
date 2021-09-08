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

	// �����ɱ�̹��߽׶�
	
	// ��ȡshader
    auto vertexShader = readFile("../../src/demos/VulkanApp/Shader/vert.spv");
    auto fragmentShader = readFile("../../src/demos/VulkanApp/Shader/frag.spv");

	// ����Shader Module
    auto vertexShaderModule = CreateShaderModule(vertexShader);
    auto fragmentShaderModule = CreateShaderModule(fragmentShader);

	// ��������׶�
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    // ʹ�õ�module
	vertexShaderStageInfo.module = vertexShaderModule;
	// ���
    vertexShaderStageInfo.pName = "main";

    // ����ƬԪ�׶�
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo;
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };
	
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);
    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);

	// �̶����߽׶�
	
}
