#include <stdexcept>

#include "FirstTriangleApplication.h"

void FirstTriangleApplication::CreateRenderPass()
{
	// һ����ɫ������
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

	/*
	 * VK_ATTACHMENT_LOAD_OP_LOAD: ������������������
	 * VK_ATTACHMENT_LOAD_OP_CLEAR���ڿ�ʼʱ��ֵ���Ϊ����
	 * VK_ATTACHMENT_LOAD_OP_DONT_CARE: ��������δ���壻���ǲ��ں�����
	 */
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	/*
	 * VK_ATTACHMENT_STORE_OP_STORE����Ⱦ�����ݽ��洢���ڴ��У��Ժ���Զ�ȡ
	 * VK_ATTACHMENT_STORE_OP_DONT_CARE: ��Ⱦ������֡�����������ݽ���δ�����
	 */
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;

	// stencil����
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	/*
	 * VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL��������ɫ������ͼ��
	 * VK_IMAGE_LAYOUT_PRESENT_SRC_KHR��Ҫ�ڽ������г��ֵ�ͼ��
	 * VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL: �����ڴ渴�Ʋ���Ŀ���ͼ��
	 */
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	// SubPass�Լ�Attachment����
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// SubPass
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	// ��ͨ��ʹ�õ�Attachment����
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	// ��ͨ����������һ���������͵ĸ���
	/*
	 * pInputAttachments������ɫ����ȡ�ĸ���
	 * pResolveAttachments�����ڶ��ز�����ɫ�����ĸ���
	 * pDepthStencilAttachment: ��Ⱥ�ģ�����ݵĸ���
	 * pPreserveAttachments������ͨ��δʹ�õ����뱣�������ݵĸ���
	 */


	 // ��ͨ������
	 // ��ͨ�����Զ�����ͼ�񲼾֣�ָ������ͨ��֮����ڴ��ִ��������ϵ
	 // ����������һ����pass����pass��֮ǰ֮��Ĳ���Ҳ������ʽ����pass
	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // ��ʾ֮ǰ����֮���pass����ʽ��ʾ����Ҫ����srcSubpass����dstSubpass
	dependency.dstSubpass = 0;	// ����֮ǰ����pass��index

	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;

	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	
	// Render Pass
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	// pass�е�Attachment
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	// ָ����pass
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create render pass!");
	}
}
