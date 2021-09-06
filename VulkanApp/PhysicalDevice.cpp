#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "QueueFamily.h"
#include "vulkan/vulkan.h"


// ��ȡ�����豸
void FirstTriangleApplication::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// δ�ҵ��κ��豸
	if(deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	// �ж��Ƿ����
	for (const auto& device : devices)
	{
		// ����ͨ��������ɸѡӲ����Ҳ����ͨ��������ѡ��
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
	// ͨ�����·�ʽ����ȡ��Ӧ���豸������Ŀǰ����������Ҫ��ȡ��������Ӧ�豸���е��豸
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

// ��ȡ���������豸�Ķ���
QueueFamilyIndices FirstTriangleApplication::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	// ��ȡQueue Family����
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for(const auto& queueFamily : queueFamilies)
	{
		// ��Ҫ֧��ͼ��
		if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		// ��Ҫ����֧��
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if(presentSupport)
		{
			indices.presentFamily = i;
		}

		// ���϶�֧�ֵ�������˳�
		if(indices.IsComplete())
		{
			break;
		}
		
		i++;
	}
	
	return indices;
}