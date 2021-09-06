#include <set>
#include <string>


#include "FirstTriangleApplication.h"

#include "vulkan/vulkan.h"

const std::vector<const char*> FirstTriangleApplication::deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

// ����Ƿ�֧���豸��չ
bool FirstTriangleApplication::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableDeviceExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableDeviceExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	// �鿴�Ƿ�������Ҫ����չ���ܹ��ҵ�
	for(const auto& extension : availableDeviceExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}