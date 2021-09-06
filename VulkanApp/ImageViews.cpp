#include <stdexcept>

#include "FirstTriangleApplication.h"

// ����ͼ����ͼ���ڷ���ͼ��
void FirstTriangleApplication::CreateImageViews()
{
	swapChainImageViews.resize(swapChainImages.size());

	for(size_t i = 0; i < swapChainImages.size(); i++)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;

		/*
		 * ��components�ֶ������������ɫͨ�������磬�����Խ�����ͨ��ӳ�䵽��ɫ��
		 * ��ĺ�ɫͨ������Ҳ����ӳ��ĳ���0��1һ��ͨ���������ǵ������У����ǽ���
		 * ��ʹ��Ĭ��ӳ�䡣
		 */
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		/*
		 * ��subresourceRange�ֶ�������ͼ�����;�Լ�Ӧ�÷���ͼ����ĸ����֡���
		 * �ǵ�ͼ������û���κ� mipmapping ����������ɫĿ�ꡣ
		 */
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		// ����3DӦ�ó�������Դ���һ����㽻����
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}
