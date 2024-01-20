#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "InputHandler.hpp"
#include "RendererPipeline.hpp"
#include "ObjectRenderer.hpp"
#include "ImguiRenderer.hpp"
#include "PhysicEngine.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(1280, 720, "2D Renderer", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window!\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// enable vsync
	glfwSwapInterval(1);

	// enable anti aliasing
	glEnable(GL_MULTISAMPLE);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// register object renderer
	auto objectRenderer = ObjectRenderer(RendererType::Object);
	RendererPipeline::registerRenderer(&objectRenderer, RendererType::Object);

	// register imgui renderer to render user interface
	auto imguiRenderer = ImguiRenderer(RendererType::UserInterface, window);
	RendererPipeline::registerRenderer(&imguiRenderer, RendererType::UserInterface);


	// window lopp
	while (!glfwWindowShouldClose(window))
	{
		// handle input
		InputHandler::handleInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// do fancy physic stuff
		PhysicEngine::update();

		// render stuff
		RendererPipeline::renderPipeline();

		// other stuff here
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputHandler::HandleKeyInput(key, action);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	InputHandler::HandleMouseInput(button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputHandler::HandleScrollInput(yoffset);
}
