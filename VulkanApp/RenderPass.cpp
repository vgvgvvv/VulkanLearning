#include <stdexcept>

#include "FirstTriangleApplication.h"

void FirstTriangleApplication::CreateRenderPass()
{
	// 一个颜色缓冲区
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	/*
	 * VK_ATTACHMENT_LOAD_OP_LOAD: 保留附件的现有内容
	 * VK_ATTACHMENT_LOAD_OP_CLEAR：在开始时将值清除为常量
	 * VK_ATTACHMENT_LOAD_OP_DONT_CARE: 现有内容未定义；我们不在乎他们
	 */
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	/*
	 * VK_ATTACHMENT_STORE_OP_STORE：渲染的内容将存储在内存中，以后可以读取
	 * VK_ATTACHMENT_STORE_OP_DONT_CARE: 渲染操作后帧缓冲区的内容将是未定义的
	 */
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	// stencil操作
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	/*
	 * VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL：用作彩色附件的图像
	 * VK_IMAGE_LAYOUT_PRESENT_SRC_KHR：要在交换链中呈现的图像
	 * VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: 用作内存复制操作目标的图像
	 */
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// SubPass以及Attachment引用
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// SubPass
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	// 子通道使用的Attachment引用
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	// 子通道可以引用一下其他类型的附件
	/*
	 * pInputAttachments：从着色器读取的附件
	 * pResolveAttachments：用于多重采样颜色附件的附件
	 * pDepthStencilAttachment: 深度和模板数据的附件
	 * pPreserveAttachments：此子通道未使用但必须保留其数据的附件
	 */


	 // 子通道依赖
	 // 子通道会自动处理图像布局，指定了子通道之间的内存和执行依赖关系
	 // 我们现在有一个子pass，子pass的之前之后的操作也算是隐式的子pass
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // 表示之前或者之后的pass的隐式表示。主要是在srcSubpass还是dstSubpass
	dependency.dstSubpass = 0;	// 我们之前的子pass的index

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	
	// Render Pass
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	// pass中的Attachment
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	// 指定子pass
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}
}
