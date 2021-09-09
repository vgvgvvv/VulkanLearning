#include <stdexcept>

#include "FirstTriangleApplication.h"

// �����ź���
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


// �ӽ�������ȡͼ��
// ��ͼ����Ϊ֡��������Attachmentִ���������
// ��ͼ�񷵻ص�������
void FirstTriangleApplication::DrawFrame()
{
	uint32_t imageIndex;
	// �ӽ�������ȡͼ��
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	// �ύframebuffer
	VkSubmitInfo submitInfo;
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
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
	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS)
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
	
}

