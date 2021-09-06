#include <set>
#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "QueueFamily.h"
#include "VulkanDebug.h"
#include "vulkan/vulkan.h"
#include "DeviceExtension.h"


void FirstTriangleApplication::CreateLogicDevice()
{

	// ��ȡ����֮ǰ�õ��������豸
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	//��ȡ������Ҫ�Ķ���
	std::set<uint32_t> uniqueQueueFamilies =
	{
		indices.graphicsFamily.value(),
		indices.presentFamily.value()
	};

	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfo.queueCount = 1;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	
	// ָ�����ܼ�
	VkPhysicalDeviceFeatures deviceFeatures{};

	// �����豸
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

	createInfo.pEnabledFeatures = &deviceFeatures;
	
	// ֧�ֵ���չ����
	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();
	
	// �µ�Vulkan�Ѿ��������豸���У��㣬���������Ѿ����ϰ汾Vulkan���м���
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

	// �������߼��豸�������塢����������ָ��洢���о���ı�����ָ�롣
	// ��Ϊ����ֻ������崴��һ�����У��������ǽ��򵥵�ʹ������0��
	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);

	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
	
}
