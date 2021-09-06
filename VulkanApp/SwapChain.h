#pragma once

#include <vector>

#include "vulkan/vulkan.h"

struct SwapChainSupportDetails
{
	// �������湦�ܣ��������е���С/���ͼ������ͼ�����С/����Ⱥ͸߶ȣ�
	VkSurfaceCapabilitiesKHR capabilities;
	// �����ʽ�����ظ�ʽ��ɫ�ʿռ䣩
	std::vector<VkSurfaceFormatKHR> formats;
	// ���õ���ʾģʽ
	std::vector<VkPresentModeKHR> presentModes;
};