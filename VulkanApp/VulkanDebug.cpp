#include <iostream>

#include "FirstTriangleApplication.h"
#include "VulkanDebug.h"

#include "GLFW/glfw3.h"


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	// ��һ�������������ԣ�
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: �����Ϣ
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT����Ϣ����Ϣ���紴����Դ
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT�����ڲ�һ���Ǵ������Ϊ����Ϣ�����ܿ���������Ӧ�ó����еĴ���
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT���й���Ч�ҿ��ܵ��±�������Ϊ����Ϣ
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	// VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: ������һЩ����������޹ص��¼�
	// VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: ������Υ���淶��������ܴ��ڴ��������
	// VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT��Vulkan ��Ǳ�ڷ����ʹ��
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	// ��pCallbackData����ָ����һ��VkDebugUtilsMessengerCallbackDataEXT������Ϣ������ϸ��Ϣ�Ľṹ��
	// pMessage: ��Ϊ�Կ��ַ���β���ַ����ĵ�����Ϣ
	// pObjects������Ϣ��ص� Vulkan ����������
	// objectCount: �����еĶ�����
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	// ��󣬸�pUserData��������һ���ڻص������ڼ�ָ����ָ�룬�����������Լ������ݴ��ݸ�����
	void* pUserData
)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}


// ���У����Ƿ���Ч
bool FirstTriangleApplication::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// ����������Ҫ��ÿ��Layer�鿴�Ƿ����
	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;
		for (const auto& layerProperies : availableLayers)
		{
			if (strcmp(layerName, layerProperies.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}
	return true;
}

// ��ȡ���������VK��չ
std::vector<const char*> FirstTriangleApplication::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		// ���Debug������չ
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void FirstTriangleApplication::SetupDebugCallback()
{
	// ����Debug��ص�����
	// https://www.khronos.org/registry/vulkan/specs/1.1-extensions/html/vkspec.html#VK_EXT_debug_utils
	if (!enableValidationLayers)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set up debug messenger!");
	}
}

void FirstTriangleApplication::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	// ������
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	// ����
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

// ����Debug Messenger
VkResult FirstTriangleApplication::CreateDebugUtilsMessengerEXT(VkInstance instrance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	// ��ȡ��չ������ַ
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		// �޷���ȡ����չ����
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void FirstTriangleApplication::DestroyDebugUtilsMessengerEXT(VkInstance instance,
	VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

