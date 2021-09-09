#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "QueueFamily.h"

/*
 * Vulkan 中的命令，如绘图操作和内存传输，不直接使用函数调用执行。您必须在命令缓冲区对象中记录要执行的所有操作。
 * 这样做的好处是，所有设置绘图命令的繁重工作都可以提前在多线程中完成。之后，您只需告诉 Vulkan 执行主循环中的命令。
 */

void FirstTriangleApplication::CreateCommandPool()
{
	// CommandBuffer是用来提交到指定的设备队列的
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
	/*
	 * VK_COMMAND_POOL_CREATE_TRANSIENT_BIT：提示命令缓冲区经常用新命令重新记录（可能会改变内存分配行为）
	 * VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: 允许单独重新记录命令缓冲区，如果没有这个标志，它们都必须一起重置
	 */
	poolInfo.flags = 0;

	if(vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

// CommandBuffer 记录绘图指令
void FirstTriangleApplication::CreateCommandBuffer()
{
	commandBuffers.resize(swapChainFrameBuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	/*
	 * VK_COMMAND_BUFFER_LEVEL_PRIMARY: 可以提交到队列执行，但不能从其他CommandBuffer调用。
	 * VK_COMMAND_BUFFER_LEVEL_SECONDARY: 不能直接提交，但可以从主CommandBuffer调用。(一般用于复用CommandBuffer)
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
		 * VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: 命令缓冲区将在执行一次后立即重新记录。
		 * VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT：这是一个辅助命令缓冲区，将完全在单个渲染通道中。
		 * VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT: 命令缓冲区可以在它也已经挂起执行时重新提交。
		 */
		beginInfo.flags = 0;
		// 继承
		beginInfo.pInheritanceInfo = nullptr;

		// begin command buffer将会重置commandbuffer
		if(vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to begin recording command buffer!");
		}

		// 开始RenderPass
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = swapChainFrameBuffers[i];

		// 指定渲染区域
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = swapChainExtent;

		VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		/*
		 * VK_SUBPASS_CONTENTS_INLINE：渲染通道命令将嵌入主命令缓冲区本身，不会执行辅助命令缓冲区。
		 * VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS：渲染通道命令将从辅助命令缓冲区执行。
		 */
		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		// 绑定图形管线
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		/*
		 * vertexCount：即使我们没有顶点缓冲区，从技术上讲，我们仍然有 3 个顶点要绘制。
		 * instanceCount：用于实例化渲染，1如果您不这样做，请使用。
		 * firstVertex：用作顶点缓冲区的偏移量，定义 的最小值gl_VertexIndex。
		 * firstInstance：用作实例渲染的偏移量，定义 的最低值gl_InstanceIndex。
		 */
		vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

		// 结束Pass
		vkCmdEndRenderPass(commandBuffers[i]);

		if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}
