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
	// 销毁CommandPool
	vkDestroyCommandPool(device, commandPool, nullptr);

	// 销毁FrameBuffer
	for(auto framebuffer : swapChainFrameBuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
	
	// 清理图形管线
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
;	// 清理管线布局
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	// 清理RenderPass
	vkDestroyRenderPass(device, renderPass, nullptr);
	
	// 清理Image View
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	// 销毁SwapChain
	vkDestroySwapchainKHR(device, swapChain, nullptr);

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



