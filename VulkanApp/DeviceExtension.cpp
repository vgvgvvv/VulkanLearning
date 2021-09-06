#include <set>
#include <string>


#include "FirstTriangleApplication.h"
#include "DeviceExtension.h"
#include "vulkan/vulkan.h"


// 检查是否支持设备扩展
bool FirstTriangleApplication::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableDeviceExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableDeviceExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	// 查看是否所有需要的扩展都能够找到
	for(const auto& extension : availableDeviceExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
