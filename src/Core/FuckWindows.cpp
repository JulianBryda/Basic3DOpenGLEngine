#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Globals/GlobalConfig.hpp"
#include "../Globals/GlobalTextures.hpp"

#include "../Utils/MaterialLib.hpp"

#include "../Core/ActionHandler.hpp"

#include "../Renderer/Renderer.hpp"
#include "PhysicEngine.hpp"

#include "../Scene/Objects/Skybox.hpp"

#include "../Utils/StatTracker.hpp"
#include "../Utils/AssetManager.hpp"

#include "../Window/AppWindow.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void drop_callback(GLFWwindow* window, int count, const char** paths);


int main()
{
	auto settings = Config::GlobalConfig();
	GLFWwindow* window = AppWindow::createAppWindow();

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetDropCallback(window, drop_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// enable vsync
	glfwSwapInterval(Config::g_settings->vsyncEnabled);

	// enable anti aliasing
	glEnable(GL_MULTISAMPLE);

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// enable face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// enable alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set line width for better visualization
	glLineWidth(2.f);

	// set point size for better visualization
	glPointSize(10.f);

	// load shader
	MaterialLib::precompileMaterials();

	// add skybox
	std::vector<const char*> faces =
	{
		".\\Assets\\Textures\\Skybox\\Skybox_Left.png",
		".\\Assets\\Textures\\Skybox\\Skybox_Right.png",
		".\\Assets\\Textures\\Skybox\\Skybox_Top.png",
		".\\Assets\\Textures\\Skybox\\Skybox_Bottom.png",
		".\\Assets\\Textures\\Skybox\\Skybox_Back.png",
		".\\Assets\\Textures\\Skybox\\Skybox_Front.png"
	};

	Skybox* skybox = new Skybox("Skybox");
	GLuint cubeMap;
	Textures::loadCubeMap(faces, cubeMap);
	Textures::addTexture("skybox", cubeMap);

	Renderer::getInstance().getActiveScene()->addEnvObject(skybox);

	GLuint overdraw;
	Textures::updateImageTexture(overdraw, Config::g_settings->screenWidth, Config::g_settings->screenHeight);
	Textures::addTexture("overdraw", overdraw);

	auto statTracker = StatTracker();

	// window lopp
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// measure time
		statTracker.measureTime([]()
			{
				// do fancy physic stuff
				PhysicEngine::getInstance().update();

				// render stuff
				Renderer::getInstance().render();

				// handle input
				ActionHandler::getInstance().handleActions();
			});

		statTracker.countFrame();

		// other stuff here
		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	Textures::updateImageTexture(Textures::get("overdraw"), width, height);
	Config::g_settings->updateScreenSize(width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputHandler::getInstance().HandleKeyInput(key, action);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	InputHandler::getInstance().HandleMouseInput(button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	InputHandler::getInstance().HandleScrollInput(yoffset);
}

void drop_callback(GLFWwindow* window, int count, const char** paths)
{
	AssetManager::getInstance().handleFileDrop(paths, count);
}