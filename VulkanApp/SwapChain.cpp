#include "SwapChain.h"

#include <algorithm>
#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "QueueFamily.h"
#include "GLFW/glfw3.h"

// ���SwapChain����
SwapChainSupportDetails FirstTriangleApplication::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	// ���capabilities
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

	// ���format
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	if(formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	// ���present mode
	uint32_t presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	if(presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	
	return details;
}

VkSurfaceFormatKHR FirstTriangleApplication::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	/*
	 * ÿ��VkSurfaceFormatKHR��Ŀ����һ��format��һ��colorSpace��Ա���� format
	 * ��Աָ����ɫͨ�������͡����磬 VK_FORMAT_B8G8R8A8_SRGB��ζ�������� 8 λ��
	 * ����������˳��洢 B��G��R �� alpha ͨ����ÿ�������ܹ� 32 λ����
	 * colorSpace��Աʹ��VK_COLOR_SPACE_SRGB_NONLINEAR_KHR��־ָʾ�Ƿ�֧�� SRGB
	 * ��ɫ�ռ䡣��ע�⣬�ñ�־����VK_COLORSPACE_SRGB_NONLINEAR_KHR�ھɰ汾�Ĺ�
	 * ���б����á�
	 */
	
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR FirstTriangleApplication::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	/*
	 * VK_PRESENT_MODE_IMMEDIATE_KHR������Ӧ�ó����ύ��ͼ����������䵽��Ļ�ϣ�
	 * ����ܻᵼ��˺�ѡ�
	 * 
	 * VK_PRESENT_MODE_FIFO_KHR����������һ�����У�����ʾ��ˢ��ʱ����ʾ���Ӷ�
	 * ǰ���ȡͼ�񣬳����ڶ��к��������Ⱦͼ�����������������������ȴ���
	 * ���ִ���Ϸ�еĴ�ֱͬ����Ϊ���ơ���ʾˢ�µ���һ�̱���Ϊ����ֱ�հס���
	 * 
	 * VK_PRESENT_MODE_FIFO_RELAXED_KHR����ģʽ���������ӳ��Ҷ��������һ����
	 * �հ�ʱΪ��ʱ��ǰһ��ģʽ��ͬ��ͼ�����յ���ʱ�������䣬�����ǵȴ���һ����
	 * ֱ�հס�����ܻᵼ�¿ɼ���˺�ѡ�
	 * 
	 * VK_PRESENT_MODE_MAILBOX_KHR�����ǵڶ���ģʽ����һ�ֱ��塣����������ʱ��
	 * ��������Ӧ�ó��򣬶��ǽ��Ѿ��Ŷӵ�ͼ��򵥵��滻Ϊ���µ�ͼ�񡣴�ģʽ����
	 * ���ڱ���˺�ѵ�ͬʱ�����ܿ����Ⱦ֡���Ӷ����±ȱ�׼��ֱͬ�����ٵ��ӳ�����
	 * ��������׳Ƶġ����ػ��塱�����ܵ��������������岢��һ����ζ��֡���ѽ�����
	 */

	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D FirstTriangleApplication::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if(capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);

		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height,
			capabilities.maxImageExtent.height);
		
		return actualExtent;

	}

}

// ����������
void FirstTriangleApplication::CreateSwapChain()
{
	SwapChainSupportDetails support = QuerySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(support.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(support.presentModes);
	VkExtent2D extent = ChooseSwapExtent(support.capabilities);

	// Ȼ�����򵥵ؼ�������Сֵ��ζ��������ʱ���ܲ��ò��ȴ�������������ڲ���
	// ����Ȼ����ܻ�ȡ��һ��ͼ�������Ⱦ����ˣ������������ٱ���Сֵ��һ��ͼ��
	uint32_t imageCount = support.capabilities.minImageCount + 1;

	// ��֤�������
	if (support.capabilities.maxImageCount > 0 && imageCount > support.capabilities.maxImageCount) {
		imageCount = support.capabilities.maxImageCount;
	}

	// ����SwapChain
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	// imageArrayLayersָ�����ÿ��ͼ�����������
	// 1���������ڿ������� 3D Ӧ�ó��򣬷���ʼ�����
	createInfo.imageArrayLayers = 1;
	// 
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = {
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	/*
	 * VK_SHARING_MODE_EXCLUSIVE��һ��ͼ��һ����һ�����м���ӵ�У���������һ����
	 * �м�����ʹ����֮ǰ������ȷת������Ȩ����ѡ����ṩ������ܡ�
	 * VK_SHARING_MODE_CONCURRENT��ͼ����Կ�������ϵ��ʹ�ã�������ȷ������Ȩ
	 * ת�ơ�
	 */
	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// ֧�ֻ���ͼ��Ӧ��ĳ�ֱ任��������ת
	createInfo.preTransform = support.capabilities.currentTransform;
	// �Ƿ�ʹ��Alphaͨ���ʹ���ϵͳ���������ڻ��
	// VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR ����Alphaͨ��
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;
	// ���в��ɼ�Ԫ��
	createInfo.clipped = VK_TRUE;
	/*
	 * ʹ�� Vulkan ʱ�����Ľ��������ܻ�������Ӧ�ó�������ʱ�����Ч��δ�Ż�������
	 * ��Ϊ�����ѵ�����С������������£�������ʵ������Ҫ��ͷ��ʼ���´��������ұ�
	 * ���ڴ��ֶ���ָ���Ծ��������á�����һ�����ӵ����⣬���ǽ����Ժ���½�����ϸ
	 * �˽⡣�������Ǽ�������ֻ�ᴴ��һ����������
	 */
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if(vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());
	
	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}
