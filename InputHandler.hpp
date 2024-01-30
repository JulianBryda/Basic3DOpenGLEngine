#pragma once
#include <iostream>
#include "RendererPipeline.hpp"
#include <GLFW/glfw3.h>
#include <ctime>

#include "glm/glm.hpp"
#include "ImguiRenderer.hpp"


class InputHandler
{

public:

	static void init(GLFWwindow* window)
	{
		InputHandler::window = window;
	}

	static void handleInput()
	{
		updateDeltaTime();

		glm::vec3 moveOff = glm::vec3(0.0f);
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float* horizontalAngle = RendererPipeline::getActiveCameraPtr()->getHorizontalAnglePtr();
		float* verticalAngle = RendererPipeline::getActiveCameraPtr()->getVerticalAnglePtr();

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
			*RendererPipeline::getActiveCameraPtr()->getAnchorPtr() -= moveOff;
		}

		*RendererPipeline::getActiveCameraPtr()->getPositionPtr() = *RendererPipeline::getActiveCameraPtr()->getAnchorPtr() + glm::vec3(cos(*verticalAngle) * sin(*horizontalAngle), -sin(*verticalAngle), cos(*verticalAngle) * cos(*horizontalAngle)) * RendererPipeline::getActiveCameraPtr()->getDistance();

		*RendererPipeline::getActiveCameraPtr()->getViewMatrixPtr() = glm::lookAt(RendererPipeline::getActiveCameraPtr()->getPosition(), RendererPipeline::getActiveCameraPtr()->getAnchor(), glm::vec3(0.0f, 1.0f, 0.0f));

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
			case GLFW_MOUSE_BUTTON_LEFT:
				selectObject();
				break;
			default:
				break;
			}
		}
	}

	static void HandleScrollInput(int offset)
	{
		float off = offset / 10.0f;
		*RendererPipeline::getActiveCameraPtr()->getDistancePtr() *= 1.0f - off;
	}

	static void HandleKeyInput(int key, int action)
	{
		if (action == 1)
		{
			// pressed
			switch (key)
			{
			case GLFW_KEY_F2:
				// screenshot
				saveScreenShot();
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

	static void selectObject()
	{
		auto worldCoords = calcWorldCoordinates();
		auto value = RendererPipeline::getRendererPtr(RendererType::Object);
		if (value == nullptr) return;

		auto renderer = static_cast<ObjectRenderer*>(value);

		for (auto object : renderer->getObjects())
		{
			glm::vec3 distance = glm::abs(object->getPosition() - worldCoords);
			glm::vec3 objectScale = object->getScale();

			if (distance.x < objectScale.x && distance.y < objectScale.y && distance.z < objectScale.z)
			{
				ImguiRenderer::setSelectedObject(object);
				break;
			}
		}

	}

	static glm::vec3 calcWorldCoordinates()
	{
		int width, height;
		double mouseX, mouseY;

		glfwGetWindowSize(window, &width, &height);
		glfwGetCursorPos(window, &mouseX, &mouseY);


		float ndcX = (2.0 * mouseX) / width - 1.0;
		float ndcY = 1.0 - (2.0 * mouseY) / height;

		float depth;
		glReadPixels(mouseX, height - mouseY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		glm::vec4 ndcPosition = glm::vec4(ndcX, ndcY, depth * 2.0 - 1.0, 1.0);
		glm::mat4 invProjection = glm::inverse(RendererPipeline::getActiveCameraPtr()->getProjectionMatrix());
		glm::vec4 eyePosition = invProjection * ndcPosition;

		glm::mat4 invModelView = glm::inverse(RendererPipeline::getActiveCameraPtr()->getViewMatrix());
		glm::vec4 worldPosition = invModelView * eyePosition;

		return glm::vec3(worldPosition) / worldPosition.w;
	}

	static void saveScreenShot();

private:

	static GLFWwindow* window;
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