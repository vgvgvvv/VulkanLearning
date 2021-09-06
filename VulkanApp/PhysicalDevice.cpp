#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "QueueFamily.h"
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
	// 通过以下方式来获取相应的设备，但是目前我们真正需要获取的是有相应设备队列的设备
	// VkPhysicalDeviceProperties deviceProperties;
	// vkGetPhysicalDeviceProperties(device, &deviceProperties);
	// VkPhysicalDeviceFeatures deviceFeatures;
	// vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	//
	// return deviceProperties.deviceType == 
	// 	VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
	// 	deviceFeatures.geometryShader;

	auto family = FindQueueFamilies(device);

	bool extensionSupport = CheckDeviceExtensionSupport(device);
	
	return family.IsComplete() && extensionSupport;
}

// 获取用于物理设备的队列
QueueFamilyIndices FirstTriangleApplication::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	// 获取Queue Family属性
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for(const auto& queueFamily : queueFamilies)
	{
		// 需要支持图形
		if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		// 需要窗口支持
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if(presentSupport)
		{
			indices.presentFamily = i;
		}

		// 以上都支持的情况下退出
		if(indices.IsComplete())
		{
			break;
		}
		
		i++;
	}
	
	return indices;
}