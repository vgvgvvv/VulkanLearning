#include "FirstTriangleApplication.h"

#include "GLFW/glfw3.h"

#define WIDTH	800
#define HEIGHT	600

void FirstTriangleApplication::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vaulkan", nullptr, nullptr);
}

void FirstTriangleApplication::InitVulkan()
{
	CreateInstance();
}

void FirstTriangleApplication::MainLoop()
{
	while(!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
}

void FirstTriangleApplication::CleanUp()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void FirstTriangleApplication::CreateInstance()
{
	// Program Info
	// ���ڲ������������ض�������߳�����Ż�
	VkApplicationInfo appInfo = {};
	// ָ��Info����
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// Instance ������Ϣ
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	
}
