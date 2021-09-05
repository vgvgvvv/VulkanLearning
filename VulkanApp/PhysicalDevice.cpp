#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "vulkan/vulkan.h"

// 获取物理设备
void FirstTriangleApplication::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// 未找到任何设备
	if(deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// 判断是否可用
	for (const auto& device : devices)
	{
		// 除了通过可用性筛选硬件，也可以通过分数来选择
		if(IsDeviceSuitable(device))
		{
			physicalDevice = device;
			break;
		}
	}

	if(physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}


bool FirstTriangleApplication::IsDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	
	return deviceProperties.deviceType == 
		VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		deviceFeatures.geometryShader;
}
