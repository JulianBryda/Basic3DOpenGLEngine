#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameObject.h"
#include "InputHandler.hpp"
#include "RendererManager.hpp"
#include "ObjectRenderer.hpp"
#include "ImguiRenderer.hpp"
#include "PhysicEngine.hpp"
#include "ShaderLib.hpp"
#include "Skybox.hpp"
#include "AssetManager.hpp"
#include "LightRenderer.hpp"
#include "EnvironmentRenderer.hpp"
#include "PostProcessingRenderer.hpp"
#include "GlobalConfig.hpp"
#include "GlobalTextures.hpp"
#include "StatTracker.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void drop_callback(GLFWwindow* window, int count, const char** paths);


int main()
{
	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	auto settings = Config::GlobalConfig();

	GLFWwindow* window = glfwCreateWindow(settings.screenWidth, settings.screenHeight, "3D Renderer", NULL, NULL);
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
	glfwSetDropCallback(window, drop_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	auto textures = Textures::GlobalTextures(settings.screenWidth, settings.screenHeight);

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
	glLineWidth(2.0f);

	// load shader
	ShaderLib::LoadShader();

	// init InputHandler
	InputHandler::getInstance().setWindow(window);

	// register object renderer
	ObjectRenderer* objectRenderer = new ObjectRenderer();
	RendererManager::getInstance().addRenderer(objectRenderer);

	// register imgui renderer to render user interface
	ImguiRenderer* imguiRenderer = new ImguiRenderer(window);
	RendererManager::getInstance().addRenderer(imguiRenderer);

	// register light renderer
	LightRenderer* lightRenderer = new LightRenderer();
	RendererManager::getInstance().addRenderer(lightRenderer);

	// register env renderer
	EnvironmentRenderer* envRenderer = new EnvironmentRenderer();
	RendererManager::getInstance().addRenderer(envRenderer);

	// register post process renderer
	PostProcessingRenderer* ppoRenderer = new PostProcessingRenderer();
	RendererManager::getInstance().addRenderer(ppoRenderer);

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
	skybox->loadCubeMap(faces);

	RendererManager::getInstance().getActiveScene()->addEnvObject(skybox);

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
				PhysicEngine::update();

				// render stuff
				RendererManager::getInstance().renderAll();

				// handle input
				InputHandler::getInstance().handleInput();
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
	Textures::g_textures->updateTextures(width, height);
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