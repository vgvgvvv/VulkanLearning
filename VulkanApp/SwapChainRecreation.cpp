#include "FirstTriangleApplication.h"
#include "GLFW/glfw3.h"

void FirstTriangleApplication::CleanupSwapChain()
{

	// ����FrameBuffer
	for (auto framebuffer : swapChainFrameBuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	// ���´���ʱ������Ҫ����Command buffers
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	
	// ����ͼ�ι���
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	// ������߲���
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	// ����RenderPass
	vkDestroyRenderPass(device, renderPass, nullptr);

	// ����Image View
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	// ����SwapChain
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}


void FirstTriangleApplication::ReCreateSwapChain()
{
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(window, &width, &height);
	while(width == 0 || height == 0)
	{
		glfwGetFramebufferSize(window, &width, &height);
		glfwWaitEvents();
	}
	
	vkDeviceWaitIdle(device);

	CleanupSwapChain();
	
	CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
	CreateFrameBuffers();
	CreateCommandBuffers();
}

