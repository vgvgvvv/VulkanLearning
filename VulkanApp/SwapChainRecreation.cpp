#include "FirstTriangleApplication.h"
#include "GLFW/glfw3.h"

void FirstTriangleApplication::CleanupSwapChain()
{

	// 销毁FrameBuffer
	for (auto framebuffer : swapChainFrameBuffers)
	{
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}

	// 重新创建时我们需要清理Command buffers
	vkFreeCommandBuffers(device, commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
	
	// 清理图形管线
	vkDestroyPipeline(device, graphicsPipeline, nullptr);
	// 清理管线布局
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	// 清理RenderPass
	vkDestroyRenderPass(device, renderPass, nullptr);

	// 清理Image View
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}

	// 销毁SwapChain
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

