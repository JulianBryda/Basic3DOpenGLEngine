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


	InputHandler()
	{
		m_window = nullptr;

		m_manipulationStruct = { ManipulationState::Nothing, glm::vec3(0.f), glm::vec3(1.f) };
		m_movementSpeed = 2.f;
		m_mouseSpeed = .5f;
		m_speed = 20.f;

		m_mouseMiddle = false;
		m_shiftMouseMiddle = false;
		m_deltaTime = 0.f;
		m_lastFrameTime = 0.f;
		m_lastXpos = 0;
		m_lastYpos = 0;
		m_lastXClick = 0;
		m_lastYClick = 0;
		m_lastXManipulation = 0;
		m_lastYManipulation = 0;
	}

	static InputHandler& getInstance()
	{
		static InputHandler instance;
		return instance;
	}


	void handleInput()
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


		// handle scaling
		if (m_manipulationStruct.state != Nothing)
		{
			GameObject* object = ImguiRenderer::getSelectedObject();
			if (object == nullptr)
			{
				m_manipulationStruct.state = Nothing;
				return;
			}

			glm::vec2 cursorPosition = glm::vec2(xpos, ypos);
			glm::vec2 startPosition = glm::vec2(m_lastXManipulation, m_lastYManipulation);
			glm::vec2 objectPosition = worldToScreen(object->getPosition(), activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());

			if (m_manipulationStruct.state == Scaling)
			{
				float distanceOffset = glm::length(objectPosition - startPosition);
				float distance = glm::length(cursorPosition - objectPosition) - distanceOffset;

				object->setScale(m_manipulationStruct.inititalScale + glm::vec3(distance / 20.f) * m_manipulationStruct.manipulationFactor);
			}
			else if (m_manipulationStruct.state == Moving)
			{
				int width, height;
				glfwGetWindowSize(m_window, &width, &height);

				float tempDepth;
				static float depth;
				glReadPixels(static_cast<int>(objectPosition.x), height - static_cast<int>(objectPosition.y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &tempDepth);
				if (tempDepth != 0)
				{
					depth = tempDepth;
				}

				glm::vec3 cursorWorldPosition = screenToWorld(width, height, cursorPosition.x, cursorPosition.y, depth);
				glm::vec3 startWorldPosition = screenToWorld(width, height, startPosition.x, startPosition.y, depth);

				glm::vec3 direction = cursorWorldPosition - startWorldPosition;

				object->setPosition(m_manipulationStruct.initialPosition + direction * m_manipulationStruct.manipulationFactor);
			}
			else if (m_manipulationStruct.state == Rotating)
			{
				glm::vec3 rotationAxis = activeCamera->getPosition() - object->getPosition();
				rotationAxis.x = rotationAxis.x < 0 ? -1.f : 1.f;
				rotationAxis.y = rotationAxis.y < 0 ? -1.f : 1.f;
				rotationAxis.z = rotationAxis.z < 0 ? -1.f : 1.f;

				glm::vec2 startDirection = startPosition - objectPosition;
				glm::vec2 currentDirection = cursorPosition - objectPosition;

				float startRadians = atan2(startDirection.y, startDirection.x);
				float currentRadians = atan2(currentDirection.y, currentDirection.x);

				float degrees = glm::degrees(startRadians - currentRadians);

				float value = degrees < 0 ? 360.f - abs(degrees) : degrees;

				object->setRotation(m_manipulationStruct.inititalRotation + value * (m_manipulationStruct.manipulationFactor * rotationAxis));
			}

		}
	}

	void HandleMouseInput(int key, int action, int mods)
	{
		if (action == 1)
		{
			// pressed
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
			// released
			switch (key)
			{
			case GLFW_MOUSE_BUTTON_MIDDLE:
				m_shiftMouseMiddle = false;
				m_mouseMiddle = false;
				break;
			case GLFW_MOUSE_BUTTON_LEFT:
				if (m_manipulationStruct.state != Nothing)
				{
					m_manipulationStruct.state = Nothing;
				}
				else
				{
					selectObject();
				}
				break;
			default:
				break;
			}
		}
	}

	void HandleScrollInput(double offset)
	{
		float off = static_cast<float>(offset) / 10.0f;
		RendererManager::getInstance().getActiveScene()->getActiveCamera()->multiplyDistance(1.0f - off);
	}

	void HandleKeyInput(int key, int action)
	{
		if (action == 1)
		{
			// pressed
			switch (key)
			{
			case GLFW_KEY_F2:
				// screenshot
				//saveScreenShot();
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
				scaleObject();
				break;
			case GLFW_KEY_D:
				if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				{
					duplicateObject();
				}
				break;
			case GLFW_KEY_A:

				break;
			case GLFW_KEY_X:
				setManipulationFactor(glm::vec3(1.f, 0.f, 0.f));
				break;
			case GLFW_KEY_Y:
				setManipulationFactor(glm::vec3(0.f, 1.f, 0.f));
				break;
			case GLFW_KEY_Z:
				setManipulationFactor(glm::vec3(0.f, 0.f, 1.f));
				break;
			case GLFW_KEY_G:
				moveObject();
				break;
			case GLFW_KEY_R:
				rotateObject();
				break;
			case GLFW_KEY_SPACE:

				break;
			case GLFW_KEY_LEFT_SHIFT:

				break;
			case GLFW_KEY_ESCAPE:
				undoManipulation();
				break;
			case GLFW_KEY_DELETE:
				deleteObject();
				break;
			default:
				break;
			}
		}
	}

	void undoManipulation()
	{
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		switch (m_manipulationStruct.state)
		{
		case Moving:
			object->setPosition(m_manipulationStruct.initialPosition);
			break;
		case Rotating:
			object->setRotation(m_manipulationStruct.inititalRotation);
			break;
		case Scaling:
			object->setScale(m_manipulationStruct.inititalScale);
			break;
		default:
			break;
		}

		m_manipulationStruct.state = Nothing;
	}

	void duplicateObject()
	{
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		GameObject* obj = new GameObject(*object);
		obj->setIsPhysicsEnabled(true);

		RendererManager::getInstance().addObject(obj, Object);
		ImguiRenderer::setSelectedObject(obj);

		moveObject();
	}

	void deleteObject()
	{
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		RendererManager::getInstance().deleteObject(*object, Object);
		ImguiRenderer::setSelectedObject(nullptr);
	}

	void scaleObject()
	{
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		m_manipulationStruct.state = m_manipulationStruct.state == Scaling ? Nothing : Scaling;
		glfwGetCursorPos(m_window, &m_lastXManipulation, &m_lastYManipulation);

		m_manipulationStruct.inititalScale = object->getScale();
		m_manipulationStruct.manipulationFactor = glm::vec3(1.f);
	}

	void moveObject()
	{
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		m_manipulationStruct.state = m_manipulationStruct.state == Moving ? Nothing : Moving;
		glfwGetCursorPos(m_window, &m_lastXManipulation, &m_lastYManipulation);

		m_manipulationStruct.initialPosition = object->getPosition();
		m_manipulationStruct.manipulationFactor = glm::vec3(1.f);
	}

	void rotateObject()
	{
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		m_manipulationStruct.state = m_manipulationStruct.state == Rotating ? Nothing : Rotating;
		glfwGetCursorPos(m_window, &m_lastXManipulation, &m_lastYManipulation);

		m_manipulationStruct.inititalRotation = object->getRotation();
		m_manipulationStruct.manipulationFactor = glm::vec3(1.f);
	}

	void setManipulationFactor(glm::vec3 factor)
	{
		if (m_manipulationStruct.state != Nothing)
		{
			m_manipulationStruct.manipulationFactor = factor;
		}
	}

	void selectObject()
	{
		double mouseX, mouseY;
		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		double distance = glm::length(glm::abs(glm::vec2(mouseX, mouseY) - glm::vec2(m_lastXClick, m_lastYClick)));
		if (distance > 2) return;

		glm::vec3 worldCoords = screenToWorld(mouseX, mouseY);
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
	glm::vec3 screenToWorld(double mouseX, double mouseY)
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);

		float depth;
		glReadPixels(static_cast<int>(mouseX), height - static_cast<int>(mouseY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

		return screenToWorld(width, height, mouseX, mouseY, depth);
	}

	glm::vec3 screenToWorld(int width, int height, double mouseX, double mouseY, float depth)
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

	glm::vec2 worldToScreen(glm::vec3 worldPos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		int width, height;
		glfwGetWindowSize(m_window, &width, &height);

		// 1. Convert world coordinates to view (camera) coordinates
		glm::vec4 viewPos = viewMatrix * glm::vec4(worldPos, 1.0f);

		// 2. Convert view coordinates to clip coordinates
		glm::vec4 clipPos = projectionMatrix * viewPos;

		// 3. Perform perspective divide to get normalized device coordinates (NDC)
		glm::vec3 ndcPos = glm::vec3(clipPos.x, clipPos.y, clipPos.z) / clipPos.w;

		// 4. Convert NDC to screen coordinates
		// NDC coordinates range from -1 to 1. Map these to screen coordinates.
		glm::vec2 screenPos;
		screenPos.x = (ndcPos.x * 0.5f + 0.5f) * width;
		screenPos.y = (1.0f - (ndcPos.y * 0.5f + 0.5f)) * height; // y is inverted in screen space

		return screenPos;
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

	void setWindow(GLFWwindow* window)
	{
		m_window = window;
	}

private:

	enum ManipulationState
	{
		Nothing,
		Scaling,
		Moving,
		Rotating
	};

	struct ManipulationStruct
	{
		ManipulationState state;

		glm::vec3 inititalScale;
		glm::vec3 initialPosition;
		glm::vec3 inititalRotation;

		glm::vec3 manipulationFactor;
	};


	GLFWwindow* m_window;
	bool m_mouseMiddle, m_shiftMouseMiddle;
	double m_lastXpos, m_lastYpos, m_lastXClick, m_lastYClick, m_lastXManipulation, m_lastYManipulation;
	float m_movementSpeed, m_deltaTime, m_lastFrameTime, m_mouseSpeed, m_speed;

	// object scaling
	ManipulationStruct m_manipulationStruct;

	void updateDeltaTime()
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;
	}


	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;
};