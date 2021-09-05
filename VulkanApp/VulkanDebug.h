#pragma once
#include <vector>

const std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

// 是否启用校验层
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif
