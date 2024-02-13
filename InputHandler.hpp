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

		Camera* activeCamera = RendererPipeline::getActiveScenePtr()->getActiveCameraPtr();
		glm::vec3 moveOff = glm::vec3(0.0f);
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		float horizontalAngle = activeCamera->getHorizontalAngle();
		float verticalAngle = activeCamera->getVerticalAngle();
		glm::vec3 anchor = activeCamera->getAnchor();

		if (mouse_middle)
		{
			horizontalAngle += mouseSpeed * deltaTime * float(last_xpos - xpos);
			verticalAngle += mouseSpeed * deltaTime * float(last_ypos - ypos);

			activeCamera->setHorizontalAngle(horizontalAngle);
			activeCamera->setVerticalAngle(verticalAngle);
		}
		else if (shift_mouse_middle)
		{
			float distance = glm::length(anchor - activeCamera->getPosition());

			glm::vec3 dir = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), 1.0f, cos(verticalAngle) * cos(horizontalAngle));
			moveOff = dir * glm::vec3(last_xpos - xpos, last_ypos - ypos, last_xpos - xpos) * deltaTime * (distance * 0.25f);
			moveOff = glm::vec3(-moveOff.z, moveOff.y, moveOff.x);

			// Update anchor position
			anchor -= moveOff;
		}

		activeCamera->setPosition(anchor + glm::vec3(cos(verticalAngle) * sin(horizontalAngle), -sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle)) * activeCamera->getDistance());
		activeCamera->setViewMatrix(glm::lookAt(activeCamera->getPosition(), anchor, activeCamera->getUp()));
		activeCamera->setAnchor(anchor);

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

	static void HandleScrollInput(double offset)
	{
		float off = static_cast<float>(offset) / 10.0f;
		RendererPipeline::getActiveScenePtr()->getActiveCameraPtr()->multiplyDistance(1.0f - off);
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

	static void moveObject()
	{

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

#pragma region World Coords Calculations
	static glm::vec3 calcWorldCoordinates()
	{
		int width, height;
		double mouseX, mouseY;

		glfwGetWindowSize(window, &width, &height);
		glfwGetCursorPos(window, &mouseX, &mouseY);

		double ndcX = (2.0 * mouseX) / width - 1.0;
		double ndcY = 1.0 - (2.0 * mouseY) / height;

		float depth;
		glReadPixels(static_cast<int>(mouseX), height - static_cast<int>(mouseY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		return calcWorldCoordinates(width, height, mouseX, mouseY, depth);
	}

	static glm::vec3 calcWorldCoordinates(double mouseX, double mouseY)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float depth;
		glReadPixels(static_cast<int>(mouseX), height - static_cast<int>(mouseY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		return calcWorldCoordinates(width, height, mouseX, mouseY, depth);
	}

	static glm::vec3 calcWorldCoordinates(int width, int height, double mouseX, double mouseY, float depth)
	{
		Camera* activeCamera = RendererPipeline::getActiveScenePtr()->getActiveCameraPtr();

		double ndcX = (2.0 * mouseX) / width - 1.0;
		double ndcY = 1.0 - (2.0 * mouseY) / height;

		glm::vec4 ndcPosition = glm::vec4(ndcX, ndcY, depth * 2.0 - 1.0, 1.0);
		glm::mat4 invProjection = glm::inverse(activeCamera->getProjectionMatrix());
		glm::vec4 eyePosition = invProjection * ndcPosition;

		glm::mat4 invModelView = glm::inverse(activeCamera->getViewMatrix());
		glm::vec4 worldPosition = invModelView * eyePosition;

		return glm::vec3(worldPosition) / worldPosition.w;
	}
#pragma endregion

	static void saveScreenShot();

private:

	static GLFWwindow* window;
	static bool mouse_middle, shift_mouse_middle;
	static double last_xpos, last_ypos;
	static float movementSpeed, deltaTime, lastFrameTime, mouseSpeed, speed;

	static void updateDeltaTime()
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
	}
};