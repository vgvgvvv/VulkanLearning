#include "FirstTriangleApplication.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "GLFW/glfw3.h"

#define WIDTH	800
#define HEIGHT	600

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

// �Ƿ�����У���
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

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

void FirstTriangleApplication::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vaulkan", nullptr, nullptr);
}

void FirstTriangleApplication::InitVulkan()
{
	CreateInstance();
}

void FirstTriangleApplication::MainLoop()
{
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
}

void FirstTriangleApplication::CleanUp()
{
	if(enableValidationLayers)
	{
		// ����Debug Messenger 
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	
	// ����ʵ��
	vkDestroyInstance(instance, nullptr);
	
	// ���ٴ���
	glfwDestroyWindow(window);
	// ����Glfw
	glfwTerminate();
}

void FirstTriangleApplication::CreateInstance()
{
	if(enableValidationLayers && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("validation layers requested, but not available!");
	}
	
	// Program Info
	// ���ڲ������������ض�������߳�����Ż�
	VkApplicationInfo appInfo = {};
	// ָ��Info����
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Instance ������Ϣ
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// ������õ�Vulkan��չ
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount
		, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions: " << std::endl;
	for(const auto& extension : extensions)
	{
		std::cout << "\t" << extension.extensionName << std::endl;
	}

	// ��ȡ����ϵͳ��չ
	auto glfwExtensions = GetRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(glfwExtensions.size());
	createInfo.ppEnabledExtensionNames = glfwExtensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	//ϵͳУ�������
	if(enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
		
	}else
	{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}

	// ���մ���ʵ��
	// ������Ϣ	+ ���������� + ����Instance��ָ��
	if(vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create instance");
	}
	
}

// ���У����Ƿ���Ч
bool FirstTriangleApplication::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// ����������Ҫ��ÿ��Layer�鿴�Ƿ����
	for(const char* layerName : validationLayers)
	{
		bool layerFound = false;
		for(const auto& layerProperies : availableLayers)
		{
			if(strcmp(layerName, layerProperies.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if(!layerFound)
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

	if(enableValidationLayers)
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
	if(!enableValidationLayers)
	{
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	PopulateDebugMessengerCreateInfo(createInfo);

	if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
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

	if(func != nullptr)
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

	if(func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}
