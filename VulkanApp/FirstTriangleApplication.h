#pragma once
#include <vector>

#include "QueueFamily.h"
#include "GLFW/glfw3.h"
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
	GLFWwindow* window;

private:
	// ����VK Instance
	void CreateInstance();
	bool CheckValidationLayerSupport();

	// ��ȡ�����������չ
	std::vector<const char*> GetRequiredExtensions();

	// Debug Callback
	void SetupDebugCallback();
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	// ����Debug Messenger����
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instrance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	// ����Debug Messenger
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

private:

	// ��ȡ�����豸
	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

private:
	VkInstance instance; // Vkʵ��
	VkDebugUtilsMessengerEXT debugMessenger; // Debug ����
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // �����豸
};
