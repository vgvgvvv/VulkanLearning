#include <stdexcept>

#include "FirstTriangleApplication.h"


// �����ź���
void FirstTriangleApplication::CreateSyncObjects()
{
	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapChainImages.size(), VK_NULL_HANDLE);
	
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	// ���ź�״̬�²ų�ʼ����
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i ++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("filed to create semaphores");
		}
	}
	
}


// �ӽ�������ȡͼ��
// ��ͼ����Ϊ֡��������Attachmentִ���������
// ��ͼ�񷵻ص�������
void FirstTriangleApplication::DrawFrame()
{
	// �ȴ�����Χ��
	vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
	vkResetFences(device, 1, &inFlightFences[currentFrame]);
	
	uint32_t imageIndex;
	// �ӽ�������ȡͼ��
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

	// �����һ֡�Ƿ�ʹ�õ����ͼƬ
	if(imagesInFlight[imageIndex] != VK_NULL_HANDLE)
	{
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];

	// �ύframebuffer
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

	submitInfo.waitSemaphoreCount = 1;
	// ָ���ź���
	submitInfo.pWaitSemaphores = waitSemaphores;
	// ָ����Ҫ�ȴ��Ĺ��߽׶�
	submitInfo.pWaitDstStageMask = waitStages;

	// ָ��Ŀ��CommandBuffer
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	// һ��ִ��������ź���
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);
	
	if(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	// ������ύ�ؽ�����
	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	// �ȴ��ź�����Ҳ����֮ǰ���ƽ���ʹ�õ��ź���
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	// ��ʾ��ͼ��
	presentInfo.pImageIndices = &imageIndex;
	// �ж��swapchain�����ͨ��һ��ֵ����ʾ��ʾ����Ƿ�ɹ������ֻ��һ���Ļ���û�б�Ҫ
	presentInfo.pResults = nullptr;

	// ��ͼ���ύ��������
	vkQueuePresentKHR(presentQueue, &presentInfo);

	// �ȴ�ִ�����
	vkQueueWaitIdle(presentQueue);

	// ���㵱ǰ֡
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

