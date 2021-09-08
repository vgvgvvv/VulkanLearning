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

	// 检查扩展支持
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	// 查找SwapChain支持
	class SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	// 选择可用的Surface格式
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	// 选择可用显示模式
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	// 选择交换范围
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// 创建交换链
	void CreateSwapChain();
	// 交换链
	VkSwapchainKHR swapChain;
	// 交换链图像
	std::vector<VkImage> swapChainImages;
	// 交换链格式
	VkFormat swapChainImageFormat;
	// 交换链大小
	VkExtent2D swapChainExtent;

private:
	// 创建图像视图
	void CreateImageViews();

	std::vector<VkImageView> swapChainImageViews;

private:
	// 创建图形管线
	void CreateGraphicsPipeline();
	// 创建shader模块
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
};
