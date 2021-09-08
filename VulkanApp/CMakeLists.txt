



set(targetName VulkanApp)
ReMake_AddTarget(
        TARGET_NAME ${targetName}
        MODE EXE
        DEFINE VK_VERSION_1_0 GLFW_INCLUDE_VULKAN
)

ReMake_StartUse()
UseGml(${targetName})
UseGlfw(${targetName})
UseVulkan(${targetName})
UseShaderc(${targetName})
ReMake_EndUse()