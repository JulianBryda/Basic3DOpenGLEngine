#pragma once
#include <iostream>
#include "RendererManager.hpp"
#include <GLFW/glfw3.h>
#include <ctime>

#include "glm/glm.hpp"
#include "ImguiRenderer.hpp"
#include "imgui_internal.h"


class InputHandler
{

public:

	static void init(GLFWwindow* window)
	{
		InputHandler::m_window = window;
	}

	static void handleInput()
	{
		updateDeltaTime();

		Camera* activeCamera = RendererManager::getInstance().getActiveScene()->getActiveCamera();
		glm::vec3 moveOff = glm::vec3(0.0f);
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);

		float horizontalAngle = activeCamera->getHorizontalAngle();
		float verticalAngle = activeCamera->getVerticalAngle();
		glm::vec3 anchor = activeCamera->getAnchor();

		if (m_mouseMiddle)
		{
			horizontalAngle += m_mouseSpeed * m_deltaTime * float(m_lastXpos - xpos);
			verticalAngle += m_mouseSpeed * m_deltaTime * float(m_lastYpos - ypos);

			activeCamera->setHorizontalAngle(horizontalAngle);
			activeCamera->setVerticalAngle(verticalAngle);
		}
		else if (m_shiftMouseMiddle)
		{
			float distance = glm::length(anchor - activeCamera->getPosition());

			glm::vec3 dir = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), 1.0f, cos(verticalAngle) * cos(horizontalAngle));
			moveOff = dir * glm::vec3(m_lastXpos - xpos, m_lastYpos - ypos, m_lastXpos - xpos) * m_deltaTime * (distance * 0.25f);
			moveOff = glm::vec3(-moveOff.z, moveOff.y, moveOff.x);

			// Update anchor position
			anchor -= moveOff;
		}

		activeCamera->setPosition(anchor + glm::vec3(cos(verticalAngle) * sin(horizontalAngle), -sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle)) * activeCamera->getDistance());
		activeCamera->setViewMatrix(glm::lookAt(activeCamera->getPosition(), anchor, activeCamera->getUp()));
		activeCamera->setAnchor(anchor);

		m_lastXpos = xpos;
		m_lastYpos = ypos;
	}

	static void HandleMouseInput(int key, int action, int mods)
	{
		if (action == 1)
		{
			switch (key)
			{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				if (mods == GLFW_MOD_SHIFT)
					m_shiftMouseMiddle = true;
				else
					m_mouseMiddle = true;
				break;
			case GLFW_MOUSE_BUTTON_LEFT:
				glfwGetCursorPos(m_window, &m_lastXClick, &m_lastYClick);
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
				m_shiftMouseMiddle = false;
				m_mouseMiddle = false;
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
		RendererManager::getInstance().getActiveScene()->getActiveCamera()->multiplyDistance(1.0f - off);
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
		double mouseX, mouseY;
		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		double distance = glm::length(glm::abs(glm::vec2(mouseX, mouseY) - glm::vec2(m_lastXClick, m_lastYClick)));
		if (distance > 2) return;

		auto worldCoords = calcWorldCoordinates(mouseX, mouseY);
		RendererBase* value = RendererManager::getInstance().getRenderer(RendererType::Object);
		if (value == nullptr) return;

		auto renderer = static_cast<ObjectRenderer*>(value);
		ImGuiContext& g = *GImGui;

		// check if imgui is on top 
		for (size_t i = 2; i < g.Windows.Size; i++)
		{
			auto& window = g.Windows[i];
			if (window->Active && mouseX > window->Pos.x && mouseX < window->Pos.x + window->Size.x && mouseY > window->Pos.y && mouseY < window->Pos.y + window->Size.y)
			{
				return;
			}
		}

		// check for object
		for (auto& object : renderer->getObjects())
		{
			glm::vec3 distance = glm::abs(object->getPosition() - worldCoords);
			glm::vec3 objectScale = object->getScale();

			if (distance.x < objectScale.x && distance.y < objectScale.y && distance.z < objectScale.z)
			{
				ImguiRenderer::setSelectedObject(object);
				return;
			}
		}

		// no object clicked, send nullptr
		ImguiRenderer::setSelectedObject(nullptr);

	}

#pragma region World Coords Calculations
	static glm::vec3 calcWorldCoordinates(double mouseX, double mouseY)
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);

		float depth;
		glReadPixels(static_cast<int>(mouseX), height - static_cast<int>(mouseY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		return calcWorldCoordinates(width, height, mouseX, mouseY, depth);
	}

	static glm::vec3 calcWorldCoordinates(int width, int height, double mouseX, double mouseY, float depth)
	{
		Camera* activeCamera = RendererManager::getInstance().getActiveScene()->getActiveCamera();

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

	static GLFWwindow* m_window;
	static bool m_mouseMiddle, m_shiftMouseMiddle;
	static double m_lastXpos, m_lastYpos, m_lastXClick, m_lastYClick;
	static float m_movementSpeed, m_deltaTime, m_lastFrameTime, m_mouseSpeed, m_speed;

	static void updateDeltaTime()
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;
	}
};