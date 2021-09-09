



set(targetName VulkanTutorial_FrameBuffer)
ReMake_AddTarget(
        TARGET_NAME ${targetName}
        MODE EXE
)

ReMake_StartUse()
UseGml(${targetName})
UseGlfw(${targetName})
UseVulkan(${targetName})
ReMake_EndUse()