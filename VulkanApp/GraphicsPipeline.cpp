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
	// 创建可编程管线阶段
	
	// 读取shader
    auto vertexShader = readFile("../../src/demos/VulkanLearning/VulkanApp/Shader/vert.spv");
    auto fragmentShader = readFile("../../src/demos/VulkanLearning/VulkanApp/Shader/frag.spv");
 
	// 创建Shader Module
    VkShaderModule vertexShaderModule = CreateShaderModule(vertexShader);
    VkShaderModule fragmentShaderModule = CreateShaderModule(fragmentShader);
 
	// 创建顶点阶段
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    // 使用的module
	vertexShaderStageInfo.module = vertexShaderModule;
	// 入口
    vertexShaderStageInfo.pName = "main";
 
    // 创建片元阶段
    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";
 
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertexShaderStageInfo, fragmentShaderStageInfo };
	
 
    //------------------------------------------------------------------------
	// 固定管线阶段
 
	// 顶点输入
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
 
	// Bindings: 数据之间的间距以及数据是按顶点还是按实例
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    // vertexInputInfo.pVertexBindingDescriptions = nullptr;
 
	// Attribute descriptions: 传递给顶点着色器的属性类型，绑定从哪个偏移量加载它们
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    // vertexInputInfo.pVertexAttributeDescriptions = nullptr;
 
 
	// 输入组件（Input Assembly）
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	/*
	 * VK_PRIMITIVE_TOPOLOGY_POINT_LIST: 从顶点点
	 * VK_PRIMITIVE_TOPOLOGY_LINE_LIST：每 2 个顶点的线，无需重用
	 * VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: 每行的结束顶点用作下一行的开始顶点
	 * VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST：每3个顶点的三角形，无需重用
	 * VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: 每个三角形的第二个和第三个顶点用作下一个三角形的前两个顶点
	 */
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	// 通常，顶点是按顺序按索引从顶点缓冲区加载的，但是对于元素缓冲区，您可以自己指定要使用的索引。这允许您执行优化，
	// 例如重用顶点。如果将primitiveRestartEnable 成员设置 为VK_TRUE，则可以_STRIP使用0xFFFF或的特殊索引在拓扑模式中
	// 拆分线和三角形0xFFFFFFFF。
    inputAssembly.primitiveRestartEnable = VK_FALSE;
 
	// 视口Viewport
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) swapChainExtent.width;
    viewport.height = (float) swapChainExtent.height;
	// Depth必须在。0.0到1.0之间
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
 
	// 剪刀矩形
    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChainExtent;
 
	// 我们通过ViewportState结合
	// 一些显卡上可以使用多个视口和剪刀矩形，所以可以传数组
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
 
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;
	
    // 光栅化器
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	// 深度裁剪，如果小于近平面或者大于远平面则裁剪
    rasterizer.depthClampEnable = VK_FALSE;
	// 如果rasterizerDiscardEnable为True则几何体永远不通过光栅化阶段，相当于禁用了帧缓冲区的任何输出。
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
	/*
	 * VK_POLYGON_MODE_FILL：用片段填充多边形区域
	 * VK_POLYGON_MODE_LINE: 多边形边被绘制为线
	 * VK_POLYGON_MODE_POINT: 多边形顶点绘制为点
	 */
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    // 线条粗细
	rasterizer.lineWidth = 1.0f;
	// 背面剔除
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	// 顶点顺序，顺时针或逆时针
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	// 光栅化器可以通过添加一个常数值或根据片段的斜率偏置深度值来改变深度值。
	// 这有时用于阴影贴图，但我们不会使用它。只需设置depthBiasEnable为VK_FALSE.
    rasterizer.depthBiasEnable = VK_FALSE;
    // rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    // rasterizer.depthBiasClamp = 0.0f; // Optional
    // rasterizer.depthBiasSlopeFactor = 0.0f; // Optional
 
	// TODO 多重采样
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;
    multisampling.pSampleMask = nullptr;
    multisampling.alphaToCoverageEnable = VK_FALSE;
    multisampling.alphaToOneEnable = VK_FALSE;
 
	// TODO 深度缓冲 
 
	// TODO 颜色混合
    // 两种方法
	// 1. 混合旧值和新值以产生最终颜色
	// 2. 使用按位运算组合旧值和新值
 
	// 每个附加缓冲区的配置
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = 
        VK_COLOR_COMPONENT_R_BIT | 
        VK_COLOR_COMPONENT_G_BIT | 
        VK_COLOR_COMPONENT_B_BIT | 
        VK_COLOR_COMPONENT_A_BIT;
 
    colorBlendAttachment.blendEnable = VK_FALSE;
 
	// 颜色混合选项
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
 
	// 颜色混合选项
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
 
	// 引用了所有缓冲区的数组，并且允许设置混合常数
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
 
    // TODO 动态状态
	// 无需创建新的管线而动态修改部分属性，例如视口大小，线宽，混合常熟
    // VkDynamicState dynamicStates[] = {
    //     VK_DYNAMIC_STATE_VIEWPORT,
    // 	VK_DYNAMIC_STATE_LINE_WIDTH
    // };
 
	// 如果不希望有任何的动态动态可以直接置为nullptr
    // VkPipelineDynamicStateCreateInfo dynamicState{};
    // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    // dynamicState.dynamicStateCount = 2;
    // dynamicState.pDynamicStates = dynamicStates;
 
	// PipelineLayout 管线布局
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    // pipelineLayoutInfo.pSetLayouts = nullptr;
	// 推送常量，可以将动态值传给着色器
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
 
	// 设置各个阶段
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
 
	// 利用现有管线派生新的管线
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.basePipelineIndex = -1;
 
    if(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
 
    // 销毁shader module
    vkDestroyShaderModule(device, fragmentShaderModule, nullptr);
    vkDestroyShaderModule(device, vertexShaderModule, nullptr);

}
