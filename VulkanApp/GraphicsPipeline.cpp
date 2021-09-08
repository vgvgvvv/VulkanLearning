#include <iostream>
#include <stdexcept>
#include <fstream>


#include "FirstTriangleApplication.h"

static std::vector<char> readFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!" + filename);
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void FirstTriangleApplication::CreateGraphicsPipeline()
{
    //------------------------------------------------------------------------
	// �����ɱ�̹��߽׶�
	
	// ��ȡshader
    auto vertexShader = readFile("../../src/demos/VulkanLearning/VulkanApp/Shader/vert.spv");
    auto fragmentShader = readFile("../../src/demos/VulkanLearning/VulkanApp/Shader/frag.spv");
 
	// ����Shader Module
    VkShaderModule vertexShaderModule = CreateShaderModule(vertexShader);
    VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShader);
 
	// ��������׶�
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    // ʹ�õ�module
	vertexShaderStageInfo.module = vertexShaderModule;
	// ���
    vertexShaderStageInfo.pName = "main";
 
    // ����ƬԪ�׶�
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";
 
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };
	
 
    //------------------------------------------------------------------------
	// �̶����߽׶�
 
	// ��������
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
 
	// Bindings: ����֮��ļ���Լ������ǰ����㻹�ǰ�ʵ��
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    // vertexInputInfo.pVertexBindingDescriptions = nullptr;
 
	// Attribute descriptions: ���ݸ�������ɫ�����������ͣ��󶨴��ĸ�ƫ������������
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    // vertexInputInfo.pVertexAttributeDescriptions = nullptr;
 
 
	// ���������Input Assembly��
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	/*
	 * VK_PRIMITIVE_TOPOLOGY_POINT_LIST: �Ӷ����
	 * VK_PRIMITIVE_TOPOLOGY_LINE_LIST��ÿ 2 ��������ߣ���������
	 * VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: ÿ�еĽ�������������һ�еĿ�ʼ����
	 * VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST��ÿ3������������Σ���������
	 * VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: ÿ�������εĵڶ����͵���������������һ�������ε�ǰ��������
	 */
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	// ͨ���������ǰ�˳�������Ӷ��㻺�������صģ����Ƕ���Ԫ�ػ��������������Լ�ָ��Ҫʹ�õ���������������ִ���Ż���
	// �������ö��㡣�����primitiveRestartEnable ��Ա���� ΪVK_TRUE�������_STRIPʹ��0xFFFF�����������������ģʽ��
	// ����ߺ�������0xFFFFFFFF��
    inputAssembly.primitiveRestartEnable = VK_FALSE;
 
	// �ӿ�Viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
	// Depth�����ڡ�0.0��1.0֮��
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
 
	// ��������
    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChainExtent;
 
	// ����ͨ��ViewportState���
	// һЩ�Կ��Ͽ���ʹ�ö���ӿںͼ������Σ����Կ��Դ�����
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
 
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
	
    // ��դ����
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	// ��Ȳü������С�ڽ�ƽ����ߴ���Զƽ����ü�
    rasterizer.depthClampEnable = VK_FALSE;
	// ���rasterizerDiscardEnableΪTrue�򼸺�����Զ��ͨ����դ���׶Σ��൱�ڽ�����֡���������κ������
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
	/*
	 * VK_POLYGON_MODE_FILL����Ƭ�������������
	 * VK_POLYGON_MODE_LINE: ����α߱�����Ϊ��
	 * VK_POLYGON_MODE_POINT: ����ζ������Ϊ��
	 */
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    // ������ϸ
	rasterizer.lineWidth = 1.0f;
	// �����޳�
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	// ����˳��˳ʱ�����ʱ��
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	// ��դ��������ͨ�����һ������ֵ�����Ƭ�ε�б��ƫ�����ֵ���ı����ֵ��
	// ����ʱ������Ӱ��ͼ�������ǲ���ʹ������ֻ������depthBiasEnableΪVK_FALSE.
    rasterizer.depthBiasEnable = VK_FALSE;
    // rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    // rasterizer.depthBiasClamp = 0.0f; // Optional
    // rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
 
	// TODO ���ز���
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;
 
	// TODO ��Ȼ��� 
 
	// TODO ��ɫ���
    // ���ַ���
	// 1. ��Ͼ�ֵ����ֵ�Բ���������ɫ
	// 2. ʹ�ð�λ������Ͼ�ֵ����ֵ
 
	// ÿ�����ӻ�����������
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = 
        VK_COLOR_COMPONENT_R_BIT | 
        VK_COLOR_COMPONENT_G_BIT | 
        VK_COLOR_COMPONENT_B_BIT | 
        VK_COLOR_COMPONENT_A_BIT;
 
    colorBlendAttachment.blendEnable = VK_FALSE;
 
	// ��ɫ���ѡ��
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
 
	// ��ɫ���ѡ��
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
 
    // colorBlendAttachment.blendEnable = VK_TRUE;
    // colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    // colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
 
	// ���������л����������飬�����������û�ϳ���
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
 
    // TODO ��̬״̬
	// ���贴���µĹ��߶���̬�޸Ĳ������ԣ������ӿڴ�С���߿���ϳ���
    // VkDynamicState dynamicStates[] = {
    //     VK_DYNAMIC_STATE_VIEWPORT,
    // 	VK_DYNAMIC_STATE_LINE_WIDTH
    // };
 
	// �����ϣ�����κεĶ�̬��̬����ֱ����Ϊnullptr
    // VkPipelineDynamicStateCreateInfo dynamicState{};
    // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicState.dynamicStateCount = 2;
    // dynamicState.pDynamicStates = dynamicStates;
 
	// PipelineLayout ���߲���
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    // pipelineLayoutInfo.pSetLayouts = nullptr;
	// ���ͳ��������Խ���ֵ̬������ɫ��
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    // pipelineLayoutInfo.pPushConstantRanges = nullptr;
 
	if(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
        throw std::runtime_error("failed to create pipeline layout");
	}
 
    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
 
	// ���ø����׶�
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = nullptr;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr;
 
    pipelineInfo.layout = pipelineLayout;
 
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
 
	// �������й��������µĹ���
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;
 
    if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
 
    // ����shader module
    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);

}
