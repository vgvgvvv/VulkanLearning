#include <stdexcept>

#include "FirstTriangleApplication.h"
#include "GLFW/glfw3.h"

void FirstTriangleApplication::CreateSurface()
{
	if(glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
	
}
