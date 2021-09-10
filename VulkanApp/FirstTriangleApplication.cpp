#include "FirstTriangleApplication.h"
#include "VulkanDebug.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#include "GLFW/glfw3.h"


#define WIDTH	800
#define HEIGHT	600

void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	// ����glfwSetWindowUserPointer
	auto app = reinterpret_cast<FirstTriangleApplication*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void FirstTriangleApplication::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vaulkan", nullptr, nullptr);
	// ���Լ�����ȥ
	glfwSetWindowUserPointer(window, this);
	// ReSize�Ļص�
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void FirstTriangleApplication::InitVulkan()
{
	// �豸���
	// ---------------------
	// ����Vulkanʵ��
	CreateInstance();
	// ����У���
	SetupDebugCallback();
	// ��������Surface
	CreateSurface();
	// ��ȡ�����豸
	PickPhysicalDevice();
	// �����߼��豸
	CreateLogicDevice();

	// ������
	// ---------------------
	// ����������
	CreateSwapChain();
	// ����ImageView
	CreateImageViews();
	// ����RenderPass
	CreateRenderPass();
	// ����ͼ�ι���
	CreateGraphicsPipeline();
	// ����FrameBuffer
	CreateFrameBuffers();

	// ����ָ��
	// ---------------------
	// ����CommandPool
	CreateCommandPool();
	// ����CommandBuffer
	CreateCommandBuffers();
	// �����ź�������ͬ������������
	CreateSyncObjects();
}

void FirstTriangleApplication::MainLoop()
{
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		DrawFrame();
	}

	// �ȴ��豸ִ�����
	vkDeviceWaitIdle(device);
}

void FirstTriangleApplication::CleanUp()
{
	CleanupSwapChain();

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i ++)
	{
		// ����ź���
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
	
	// ����CommandPool
	vkDestroyCommandPool(device, commandPool, nullptr);

	// �����߼��豸
	vkDestroyDevice(device, nullptr);

	// �����豸��������

	// ������֤��
	if(enableValidationLayers)
	{
		// ����Debug Messenger 
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	// ����Surface
	vkDestroySurfaceKHR(instance, surface, nullptr);
	
	// ����ʵ��
	vkDestroyInstance(instance, nullptr);
	
	// ���ٴ���
	glfwDestroyWindow(window);
	// ����Glfw
	glfwTerminate();
}



