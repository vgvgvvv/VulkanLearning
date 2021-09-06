#pragma once

#include <vector>

#include "vulkan/vulkan.h"

struct SwapChainSupportDetails
{
	// 基本表面功能（交换链中的最小/最大图像数，图像的最小/最大宽度和高度）
	VkSurfaceCapabilitiesKHR capabilities;
	// 表面格式（像素格式、色彩空间）
	std::vector<VkSurfaceFormatKHR> formats;
	// 可用的演示模式
	std::vector<VkPresentModeKHR> presentModes;
};