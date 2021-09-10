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
	// 来自glfwSetWindowUserPointer
	auto app = reinterpret_cast<FirstTriangleApplication*>(glfwGetWindowUserPointer(window));
	app->framebufferResized = true;
}

void FirstTriangleApplication::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vaulkan", nullptr, nullptr);
	// 把自己传回去
	glfwSetWindowUserPointer(window, this);
	// ReSize的回调
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void FirstTriangleApplication::InitVulkan()
{
	// 设备相关
	// ---------------------
	// 创建Vulkan实例
	CreateInstance();
	// 创建校验层
	SetupDebugCallback();
	// 创建窗口Surface
	CreateSurface();
	// 获取物理设备
	PickPhysicalDevice();
	// 创建逻辑设备
	CreateLogicDevice();

	// 交换链
	// ---------------------
	// 创建交换链
	CreateSwapChain();
	// 创建ImageView
	CreateImageViews();
	// 创建RenderPass
	CreateRenderPass();
	// 创建图形管线
	CreateGraphicsPipeline();
	// 创建FrameBuffer
	CreateFrameBuffers();

	// 绘制指令
	// ---------------------
	// 创建CommandPool
	CreateCommandPool();
	// 创建CommandBuffer
	CreateCommandBuffers();
	// 创建信号量用于同步交换链操作
	CreateSyncObjects();
}

void FirstTriangleApplication::MainLoop()
{
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		DrawFrame();
	}

	// 等待设备执行完毕
	vkDeviceWaitIdle(device);
}

void FirstTriangleApplication::CleanUp()
{
	CleanupSwapChain();

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i ++)
	{
		// 清除信号量
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
	
	// 销毁CommandPool
	vkDestroyCommandPool(device, commandPool, nullptr);

	// 销毁逻辑设备
	vkDestroyDevice(device, nullptr);

	// 物理设备无需销毁

	// 销毁验证层
	if(enableValidationLayers)
	{
		// 销毁Debug Messenger 
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}

	// 销毁Surface
	vkDestroySurfaceKHR(instance, surface, nullptr);
	
	// 销毁实例
	vkDestroyInstance(instance, nullptr);
	
	// 销毁窗口
	glfwDestroyWindow(window);
	// 销毁Glfw
	glfwTerminate();
}



