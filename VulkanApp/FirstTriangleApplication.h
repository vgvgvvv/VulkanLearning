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
	// ����VK Instance
	void CreateInstance();
	VkInstance instance = VK_NULL_HANDLE; // Vkʵ��

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

	VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE; // Debug ����

private:

	// ��ȡ�����豸
	void PickPhysicalDevice();
	// �����豸�Ƿ����
	bool IsDeviceSuitable(VkPhysicalDevice device);
	// �ҵ�������
	class QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE; // �����豸

private:
	// �����߼��豸
	void CreateLogicDevice();

	VkDevice device = VK_NULL_HANDLE; // �߼��豸
	VkQueue graphicsQueue; // �߼�ͼ���豸����

private:
	// ��������Surface
	void CreateSurface();

	VkSurfaceKHR surface; // ����Surface
	VkQueue presentQueue; // ��ʾ����

private:

	// �����չ֧��
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	// ����SwapChain֧��
	class SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	// ѡ����õ�Surface��ʽ
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	// ѡ�������ʾģʽ
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	// ѡ�񽻻���Χ
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	// ����������
	void CreateSwapChain();
	// ������
	VkSwapchainKHR swapChain;
	// ������ͼ��
	std::vector<VkImage> swapChainImages;
	// ��������ʽ
	VkFormat swapChainImageFormat;
	// ��������С
	VkExtent2D swapChainExtent;

private:
	// ����ͼ����ͼ
	void CreateImageViews();

	std::vector<VkImageView> swapChainImageViews;

private:
	// ����ͼ�ι���
	void CreateGraphicsPipeline();
	// ����shaderģ��
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
};
