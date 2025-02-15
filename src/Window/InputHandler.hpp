#pragma once
#include <iostream>
#include <ctime>
#include <functional>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "../Window/AppWindow.hpp"

class InputHandler
{


public:

	InputHandler()
	{
		m_window = AppWindow::window;

		mouseMiddleDown = false;
		shiftMouseMiddleDown = false;

		mouseLeftDown = false;
		shiftMouseLeftDown = false;

		mouseRightDown = false;
		shiftMouseRightDown = false;

		lastXClick = 0;
		lastYClick = 0;
	}

	static InputHandler& getInstance()
	{
		static InputHandler instance;
		return instance;
	}

	void subscribeKey(std::function<void()> listener, int keyCode, int modifier = 0)
	{
		keyListeners.push_back({ keyCode, modifier , listener });
	}

	void subscribeMouse(std::function<void()> listener, int keyCode, int modifier = 0)
	{
		mouseListeners.push_back({ keyCode, modifier , listener });
	}

	void subscribeScroll(std::function<void(double offset)> listener)
	{
		scrollListeners.push_back(listener);
	}

	void HandleMouseInput(int key, int action, int mods)
	{
		if (action == 1)
		{
			NotifyMouseListeners(key);

			// pressed
			switch (key)
			{
			case GLFW_MOUSE_BUTTON_MIDDLE:
			{
				if (mods == GLFW_MOD_SHIFT)
					shiftMouseMiddleDown = true;
				else
					mouseMiddleDown = true;
				break;
			}
			case GLFW_MOUSE_BUTTON_LEFT:
			{
				glfwGetCursorPos(m_window, &lastXClick, &lastYClick);
				if (mods == GLFW_MOD_SHIFT)
					shiftMouseLeftDown = true;
				else
					mouseLeftDown = true;
				break;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				if (mods == GLFW_MOD_SHIFT)
					shiftMouseRightDown = true;
				else
					mouseRightDown = true;
				break;
			}
			default:
				break;
			}
		}
		else if (action == 0)
		{
			// released
			switch (key)
			{
			case GLFW_MOUSE_BUTTON_MIDDLE:
			{
				shiftMouseMiddleDown = false;
				mouseMiddleDown = false;
				break;
			}
			case GLFW_MOUSE_BUTTON_LEFT:
			{
				shiftMouseLeftDown = false;
				mouseLeftDown = false;
				break;
			}
			case GLFW_MOUSE_BUTTON_RIGHT:
			{
				shiftMouseRightDown = false;
				mouseRightDown = false;
				break;
			}
			default:
				break;
			}
		}
	}

	void HandleScrollInput(double offset)
	{
		NotifyScrollListeners(offset);
	}

	void HandleKeyInput(int key, int action)
	{
		// pressed
		if (action == 1)
		{
			NotifyKeyListeners(key);

			switch (key)
			{

			default:
				break;
			}
		}
	}

	//void saveScreenShot()
	//{
	//	int width, height;
	//	glfwGetWindowSize(m_window, &width, &height);

	//	char* pixels = new char[3 * width * height];
	//	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	//	stbi_flip_vertically_on_write(true);

	//	std::time_t currentTime = std::time(nullptr);
	//	std::tm* timeInfo = std::localtime(&currentTime);

	//	char buffer[80];
	//	std::strftime(buffer, sizeof(buffer), "%Y.%m.%d - %H.%M.%S", timeInfo);
	//	std::string path = std::format("C:\\Users\\Julian\\Desktop\\FuckWindows {}.png", buffer);

	//	auto test = stbi_write_png(path.c_str(), width, height, 3, pixels, width * 3);

	//	std::cout << "Saved Screenshot!\n";

	//	delete[] pixels;
	//}

	void saveShadowMapAsImage(GLuint id, int width, int height, const char* filePath)
	{
		float* depthData = new float[width * height]; // Assuming RGBA format

		// Retrieve the data
		glBindFramebuffer(GL_FRAMEBUFFER, id);
		glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, depthData);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		unsigned char* imageData = new unsigned char[width * height];
		for (int i = 0; i < width * height; ++i)
		{
			// Map depth value from [0, 1] to [0, 255]
			imageData[i] = static_cast<unsigned char>(depthData[i] * 255.f);
		}

		// Save the texture data as an image file
		if (stbi_write_png(filePath, width, height, 1, imageData, width))
		{
			std::cout << "Saved shadowMap to " << filePath << std::endl;
		}
		else
		{
			std::cerr << "Failed to save shadowMap to " << filePath << std::endl;
		}

		// Clean up
		delete[] depthData;
		delete[] imageData;
	}



	bool mouseMiddleDown, shiftMouseMiddleDown, mouseLeftDown, shiftMouseLeftDown, mouseRightDown, shiftMouseRightDown;
	double lastXClick, lastYClick;

private:

	struct Event
	{
		int keyCode;
		int modifier;
		std::function<void()> function;
	};

	void NotifyKeyListeners(int keyCode)
	{
		for (auto& listener : keyListeners)
		{
			if (listener.keyCode == keyCode && (listener.modifier == 0 || glfwGetKey(m_window, listener.modifier) == GLFW_PRESS))
				listener.function();
		}
	}

	void NotifyMouseListeners(int keyCode)
	{
		for (auto& listener : mouseListeners)
		{
			if (listener.keyCode == keyCode && (listener.modifier == 0 || glfwGetKey(m_window, listener.modifier) == GLFW_PRESS))
				listener.function();
		}
	}

	void NotifyScrollListeners(double offset)
	{
		for (auto& listener : scrollListeners)
		{
			listener(offset);
		}
	}

	GLFWwindow* m_window;

	std::vector<Event> keyListeners;
	std::vector<Event> mouseListeners;
	std::vector<std::function<void(double offset)>> scrollListeners;

	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;
};