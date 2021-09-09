#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "QueueFamily.h"

/*
 * Vulkan �е�������ͼ�������ڴ洫�䣬��ֱ��ʹ�ú�������ִ�С���������������������м�¼Ҫִ�е����в�����
 * �������ĺô��ǣ��������û�ͼ����ķ��ع�����������ǰ�ڶ��߳�����ɡ�֮����ֻ����� Vulkan ִ����ѭ���е����
 */

void FirstTriangleApplication::CreateCommandPool()
{
	// CommandBuffer�������ύ��ָ�����豸���е�
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	/*
	 * VK_COMMAND_POOL_CREATE_TRANSIENT_BIT����ʾ����������������������¼�¼�����ܻ�ı��ڴ������Ϊ��
	 * VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: ���������¼�¼������������û�������־�����Ƕ�����һ������
	 */
	poolInfo.flags = 0;

	if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

// CommandBuffer ��¼��ͼָ��
void FirstTriangleApplication::CreateCommandBuffer()
{
	commandBuffers.resize(swapChainFrameBuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	/*
	 * VK_COMMAND_BUFFER_LEVEL_PRIMARY: �����ύ������ִ�У������ܴ�����CommandBuffer���á�
	 * VK_COMMAND_BUFFER_LEVEL_SECONDARY: ����ֱ���ύ�������Դ���CommandBuffer���á�(һ�����ڸ���CommandBuffer)
	 */
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if(vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate command buffer!");
	}

	for(size_t i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		/*
		 * VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: �����������ִ��һ�κ��������¼�¼��
		 * VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT������һ�������������������ȫ�ڵ�����Ⱦͨ���С�
		 * VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: ���������������Ҳ�Ѿ�����ִ��ʱ�����ύ��
		 */
		beginInfo.flags = 0;
		// �̳�
		beginInfo.pInheritanceInfo = nullptr;

		// begin command buffer��������commandbuffer
		if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		// ��ʼRenderPass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFrameBuffers[i];

		// ָ����Ⱦ����
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		/*
		 * VK_SUBPASS_CONTENTS_INLINE����Ⱦͨ�����Ƕ�������������������ִ�и������������
		 * VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS����Ⱦͨ������Ӹ����������ִ�С�
		 */
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// ��ͼ�ι���
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		/*
		 * vertexCount����ʹ����û�ж��㻺�������Ӽ����Ͻ���������Ȼ�� 3 ������Ҫ���ơ�
		 * instanceCount������ʵ������Ⱦ��1�����������������ʹ�á�
		 * firstVertex���������㻺������ƫ���������� ����Сֵgl_VertexIndex��
		 * firstInstance������ʵ����Ⱦ��ƫ���������� �����ֵgl_InstanceIndex��
		 */
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		// ����Pass
		vkCmdEndRenderPass(commandBuffers[i]);

		if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}
