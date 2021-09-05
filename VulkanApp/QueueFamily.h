#pragma once
#include <optional>

// ¶ÓÁÐ×å
struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	
	bool IsComplete() const
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};