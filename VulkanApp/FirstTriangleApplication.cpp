#include "FirstTriangleApplication.h"
#include "VulkanDebug.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "GLFW/glfw3.h"

#define WIDTH	800
#define HEIGHT	600




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
	SetupDebugCallback();
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

