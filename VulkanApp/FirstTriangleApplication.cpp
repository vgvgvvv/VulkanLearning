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
	// ����CommandPool
	vkDestroyCommandPool(device, commandPool, nullptr);

	// ����FrameBuffer
	for(auto framebuffer : swapChainFrameBuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
	
	// ����ͼ�ι���
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
;	// ������߲���
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	// ����RenderPass
	vkDestroyRenderPass(device, renderPass, nullptr);
	
	// ����Image View
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	// ����SwapChain
	vkDestroySwapchainKHR(device, swapChain, nullptr);

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



