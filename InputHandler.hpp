#pragma once
#include <iostream>
#include <GLFW/glfw3.h>


class InputHandler
{
	
public:

	static void handleInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			std::cout << "Escape key pressed!\n";
		}
	}


private:

};