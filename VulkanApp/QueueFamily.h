#pragma once
#include <optional>

// ¶ÓÁÐ×å
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool IsCompelete() const
	{
		return graphicsFamily.has_value();
	}
};