#pragma once
#include <iostream>
#include "RendererPipeline.hpp"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"


class InputHandler
{

public:

	static void handleInput(GLFWwindow* window)
	{
		updateDeltaTime();

		glm::vec3 moveOff = glm::vec3(0.0f);
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float* horizontalAngle = RendererPipeline::getActiveCamera()->getHorizontalAnglePtr();
		float* verticalAngle = RendererPipeline::getActiveCamera()->getVerticalAnglePtr();

		if (mouse_middle)
		{
			*horizontalAngle += mouseSpeed * deltaTime * float(last_xpos - xpos);
			*verticalAngle += mouseSpeed * deltaTime * float(last_ypos - ypos);
		}
		else if (shift_mouse_middle)
		{
			glm::vec3 dir = glm::vec3(cos(*verticalAngle) * sin(*horizontalAngle), 1.0f, cos(*verticalAngle) * cos(*horizontalAngle));
			moveOff = dir * glm::vec3(last_xpos - xpos, last_ypos - ypos, last_xpos - xpos) * deltaTime * (speed / 3);
			moveOff = glm::vec3(-moveOff.z, moveOff.y, moveOff.x);
			*RendererPipeline::getActiveCamera()->getAnchorPtr() -= moveOff;
		}

		*RendererPipeline::getActiveCamera()->getPositionPtr() = *RendererPipeline::getActiveCamera()->getAnchorPtr() + glm::vec3(cos(*verticalAngle) * sin(*horizontalAngle), -sin(*verticalAngle), cos(*verticalAngle) * cos(*horizontalAngle)) * RendererPipeline::getActiveCamera()->getDistance();

		*RendererPipeline::getActiveCamera()->getViewMatrixPtr() = glm::lookAt(RendererPipeline::getActiveCamera()->getPosition(), RendererPipeline::getActiveCamera()->getAnchor(), glm::vec3(0.0f, 1.0f, 0.0f));

		last_xpos = xpos;
		last_ypos = ypos;
	}

	static void HandleMouseInput(int key, int action, int mods)
	{
		if (action == 1)
		{
			switch (key)
			{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				if (mods == GLFW_MOD_SHIFT)
					shift_mouse_middle = true;
				else
					mouse_middle = true;
				break;
			default:
				break;
			}
		}
		else if (action == 0)
		{
			switch (key)
			{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				shift_mouse_middle = false;
				mouse_middle = false;
				break;
			default:
				break;
			}
		}
	}

	static void HandleScrollInput(int offset)
	{
		float off = offset / 10.0f;
		*RendererPipeline::getActiveCamera()->getDistancePtr() *= 1.0f - off;
	}

	static void HandleKeyInput(int key, int action)
	{
		if (action == 1)
		{
			// pressed
			switch (key)
			{
			case GLFW_KEY_H:

				break;
			case GLFW_KEY_W:

				break;
			case GLFW_KEY_S:

				break;
			case GLFW_KEY_D:

				break;
			case GLFW_KEY_A:

				break;
			case GLFW_KEY_SPACE:

				break;
			default:
				break;
			}
		}
		else if (action == 0)
		{
			// released
			switch (key)
			{
			case GLFW_KEY_W:

				break;
			case GLFW_KEY_S:

				break;
			case GLFW_KEY_D:

				break;
			case GLFW_KEY_A:

				break;
			case GLFW_KEY_SPACE:

				break;
			case GLFW_KEY_LEFT_SHIFT:

				break;
			default:
				break;
			}
		}
	}


private:

	static bool mouse_middle, shift_mouse_middle;
	static double last_xpos, last_ypos;
	static float movementSpeed, deltaTime, lastFrameTime, mouseSpeed, speed;

	static void updateDeltaTime()
	{
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
	}
};