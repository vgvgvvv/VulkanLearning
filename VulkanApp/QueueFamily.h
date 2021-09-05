#pragma once
#include <optional>

// ¶ÓÁÐ×å
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool IsComplete() const
	{
		return graphicsFamily.has_value();
	}
};