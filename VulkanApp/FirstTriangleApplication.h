#pragma once
#include <vector>

#include "vulkan/vulkan.h"


class FirstTriangleApplication
{
public:
	void Run()
	{
		InitWindow();
		InitVulkan();
		MainLoop();
		CleanUp();
	}

private:

	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void CleanUp();

private:
	class GLFWwindow* window;

private:
	// 创建VK Instance
	void CreateInstance();
	VkInstance instance = VK_NULL_HANDLE; // Vk实例

	bool CheckValidationLayerSupport();

	// 获取我们所需的扩展
	std::vector<const char*> GetRequiredExtensions();

	// Debug Callback
	void SetupDebugCallback();
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	// 创建Debug Messenger对象
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instrance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	// 销毁Debug Messenger
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE; // Debug 代理

private:

	// 获取物理设备
	void PickPhysicalDevice();
	// 物理设备是否可用
	bool IsDeviceSuitable(VkPhysicalDevice device);
	// 找到队列族
	class QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // 物理设备

private:
	// 创建逻辑设备
	void CreateLogicDevice();

	VkDevice device = VK_NULL_HANDLE; // 逻辑设备
	VkQueue graphicsQueue; // 逻辑图形设备队列

private:
	// 创建窗口Surface
	void CreateSurface();

	VkSurfaceKHR surface; // 窗口Surface
	VkQueue presentQueue; // 显示队列

private:

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	static const std::vector<const char*> deviceExtensions;
};
