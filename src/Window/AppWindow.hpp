#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

#include "../Globals/GlobalConfig.hpp"

class AppWindow
{

public:

	static GLFWwindow* createAppWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		window = glfwCreateWindow(Config::g_settings->screenWidth, Config::g_settings->screenHeight, "3D Renderer", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window!\n";
			glfwTerminate();

			exit(-1);
		}

		return window;
	}

	static GLFWwindow* window;
};