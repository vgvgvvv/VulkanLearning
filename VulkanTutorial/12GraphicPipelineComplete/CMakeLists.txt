



set(targetName VulkanTutorial_GraphicsPipelineComplete)
ReMake_AddTarget(
        TARGET_NAME ${targetName}
        MODE EXE
)

ReMake_StartUse()
UseGml(${targetName})
UseGlfw(${targetName})
UseVulkan(${targetName})
ReMake_EndUse()