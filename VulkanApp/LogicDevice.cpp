#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "VulkanDebug.h"
#include "vulkan/vulkan.h"


void FirstTriangleApplication::CreateLogicDevice()
{

	// ��ȡ����֮ǰ�õ��������豸
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);

	// ��ʼ���������豸����
	VkDeviceQueueCreateInfo queueCreateInfo;
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	// ָ��������
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	// �������ȼ�
	queueCreateInfo.pQueuePriorities = &queuePriority;

	// ָ�����ܼ�
	VkPhysicalDeviceFeatures deviceFeatures;

	// �����豸
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;

	createInfo.pEnabledFeatures = &deviceFeatures;
	
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
	
}
