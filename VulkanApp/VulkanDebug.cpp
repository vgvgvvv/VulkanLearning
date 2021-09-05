#include <iostream>

#include "FirstTriangleApplication.h"
#include "VulkanDebug.h"

#include "GLFW/glfw3.h"


static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	// 第一个参数是严重性：
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT: 诊断信息
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT：信息性消息，如创建资源
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT：关于不一定是错误的行为的消息，但很可能是您的应用程序中的错误
	// VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT：有关无效且可能导致崩溃的行为的消息
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	// VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT: 发生了一些与规格或性能无关的事件
	// VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT: 发生了违反规范或表明可能存在错误的事情
	// VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT：Vulkan 的潜在非最佳使用
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	// 该pCallbackData参数指的是一个VkDebugUtilsMessengerCallbackDataEXT包含消息本身详细信息的结构体
	// pMessage: 作为以空字符结尾的字符串的调试消息
	// pObjects：与消息相关的 Vulkan 对象句柄数组
	// objectCount: 数组中的对象数
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	// 最后，该pUserData参数包含一个在回调设置期间指定的指针，并允许您将自己的数据传递给它。
	void* pUserData
)
{
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}


// 检查校验层是否有效
bool FirstTriangleApplication::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// 遍历我们需要的每个Layer查看是否存在
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

// 获取所有所需的VK扩展
std::vector<const char*> FirstTriangleApplication::GetRequiredExtensions()
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers)
	{
		// 添加Debug所需扩展
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void FirstTriangleApplication::SetupDebugCallback()
{
	// 更多Debug相关的设置
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
	// 严重性
	createInfo.messageSeverity =
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	// 类型
	createInfo.messageType =
		VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

// 创建Debug Messenger
VkResult FirstTriangleApplication::CreateDebugUtilsMessengerEXT(VkInstance instrance,
	const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator,
	VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	// 获取扩展函数地址
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		// 无法获取到扩展函数
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

