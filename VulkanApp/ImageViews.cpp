#include <stdexcept>

#include "FirstTriangleApplication.h"

// 创建图像视图用于访问图像
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
		 * 该components字段允许您混合颜色通道。例如，您可以将所有通道映射到单色纹
		 * 理的红色通道。您也可以映射的常量0和1一个通道。在我们的例子中，我们将坚
		 * 持使用默认映射。
		 */
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		/*
		 * 该subresourceRange字段描述了图像的用途以及应该访问图像的哪个部分。我
		 * 们的图像将用作没有任何 mipmapping 级别或多层的颜色目标。
		 */
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		// 处理3D应用程序你可以创建一个多层交换链
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}
