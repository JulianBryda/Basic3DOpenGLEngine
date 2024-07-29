#include "InputHandler.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "stb_image_write.h"

float InputHandler::m_movementSpeed = 2.0;
float InputHandler::m_deltaTime;
float InputHandler::m_lastFrameTime;

float InputHandler::m_speed = 20.0f;
float InputHandler::m_mouseSpeed = 0.5f;

bool InputHandler::m_mouseMiddle = false;
bool InputHandler::m_shiftMouseMiddle = false;

double InputHandler::m_lastXpos = 0.0;
double InputHandler::m_lastYpos = 0.0;

double InputHandler::m_lastXClick = 0.0;
double InputHandler::m_lastYClick = 0.0;

GLFWwindow* InputHandler::m_window;


void InputHandler::saveScreenShot()
{
	int width, height;
	glfwGetWindowSize(m_window, &width, &height);

	char* pixels = new char[3 * width * height];
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	stbi_flip_vertically_on_write(true);

	std::time_t currentTime = std::time(nullptr);
	std::tm* timeInfo = std::localtime(&currentTime);
	
	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%Y.%m.%d - %H.%M.%S", timeInfo);
	std::string path = std::format("C:\\Users\\Julian\\Desktop\\FuckWindows {}.png", buffer);

	auto test = stbi_write_png(path.c_str(), width, height, 3, pixels, width * 3);

	std::cout << "Saved Screenshot!\n";

	delete[] pixels;
}
