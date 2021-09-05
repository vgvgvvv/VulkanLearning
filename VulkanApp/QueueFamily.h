#pragma once
#include <optional>

// ������
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool IsCompelete() const
	{
		return graphicsFamily.has_value();
	}
};