#include "InputHandler.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "stb_image_write.h"

float InputHandler::movementSpeed = 2.0;
float InputHandler::deltaTime;
float InputHandler::lastFrameTime;

float InputHandler::speed = 20.0f;
float InputHandler::mouseSpeed = 0.5f;

bool InputHandler::mouse_middle = false;
bool InputHandler::shift_mouse_middle = false;

double InputHandler::last_xpos = 0.0;
double InputHandler::last_ypos = 0.0;

GLFWwindow* InputHandler::window;


void InputHandler::saveScreenShot()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

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
