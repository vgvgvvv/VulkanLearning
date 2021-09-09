#include <stdexcept>

#include "FirstTriangleApplication.h"

// 创建信号量
void FirstTriangleApplication::CreateSemaphores()
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
	{
		throw std::runtime_error("filed to create semaphores");
	}
}


// 从交换链获取图像
// 将图像作为帧缓冲区的Attachment执行命令缓冲区
// 将图像返回到交换链
void FirstTriangleApplication::DrawFrame()
{
	uint32_t imageIndex;
	// 从交换链获取图像
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	// 提交framebuffer
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount = 1;
	// 指定信号量
	submitInfo.pWaitSemaphores = waitSemaphores;
	// 指定需要等待的管线阶段
	submitInfo.pWaitDstStageMask = waitStages;

	// 指定目标CommandBuffer
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	// 一旦执行完成则信号量
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	// 将结果提交回交换链
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	// 等待信号量，也就是之前绘制结束使用的信号量
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	// 显示的图像
	presentInfo.pImageIndices = &imageIndex;
	// 有多个swapchain则可以通过一组值来表示显示结果是否成功，如果只有一个的话就没有必要
	presentInfo.pResults = nullptr;

	// 将图像提交到交换链
	vkQueuePresentKHR(presentQueue, &presentInfo);
	
}

