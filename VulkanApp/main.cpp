#include <iostream>

#include "FirstTriangleApplication.h"

int main()
{
	
	FirstTriangleApplication app;
	try
	{
		app.Run();
	} catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
