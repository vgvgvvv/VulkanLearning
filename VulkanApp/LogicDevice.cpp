#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "VulkanDebug.h"
#include "vulkan/vulkan.h"


void FirstTriangleApplication::CreateLogicDevice()
{

	// 获取我们之前得到的物理设备
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	// 开始创建虚拟设备队列
	VkDeviceQueueCreateInfo queueCreateInfo;
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	// 指定队列族
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	// 队列优先级
	queueCreateInfo.pQueuePriorities = &queuePriority;

	// 指定功能集
	VkPhysicalDeviceFeatures deviceFeatures;

	// 创建设备
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;
	
	// 新的Vulkan已经忽略了设备层的校验层，但是我们已经对老版本Vulkan进行兼容
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}else
	{
		createInfo.enabledLayerCount = 0;
	}

	if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	// 参数是逻辑设备、队列族、队列索引和指向存储队列句柄的变量的指针。
	// 因为我们只从这个族创建一个队列，所以我们将简单地使用索引0。
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	
}
