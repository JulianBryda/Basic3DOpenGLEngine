#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <imgui_internal.h>

#include "../Renderer/UI/UserInterface.hpp"
#include "../Window/InputHandler.hpp"

class ActionHandler
{
private:

	enum ManipulationState
	{
		Nothing,
		Scaling,
		Moving,
		Rotating
	};


public:

	ActionHandler()
	{
		m_window = AppWindow::window;

		m_lastXManipulation = 0;
		m_lastYManipulation = 0;

		glfwGetCursorPos(m_window, &xPos, &yPos);

		// register event handler
		InputHandler::getInstance().subscribeMouse(std::bind(&ActionHandler::handleSelection, this), GLFW_MOUSE_BUTTON_LEFT);

		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::scaleObject, this), GLFW_KEY_S);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::duplicateObject, this), GLFW_KEY_D, GLFW_KEY_LEFT_SHIFT);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::recenterCamera, this), GLFW_KEY_C, GLFW_KEY_LEFT_SHIFT);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::selectAllObjects, this), GLFW_KEY_A);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::setManipulationFactor, this, glm::vec3(1.f, 0.f, 0.f)), GLFW_KEY_X);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::setManipulationFactor, this, glm::vec3(0.f, 1.f, 0.f)), GLFW_KEY_Y);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::setManipulationFactor, this, glm::vec3(0.f, 0.f, 1.f)), GLFW_KEY_Z);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::moveObject, this), GLFW_KEY_G);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::rotateObject, this), GLFW_KEY_R);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::undoManipulation, this), GLFW_KEY_ESCAPE);
		InputHandler::getInstance().subscribeKey(std::bind(&ActionHandler::deleteObject, this), GLFW_KEY_DELETE);

		InputHandler::getInstance().subscribeScroll(std::bind(&ActionHandler::handleCameraZoom, this, std::placeholders::_1));
	}

	~ActionHandler()
	{

	}

	static ActionHandler& getInstance()
	{
		static ActionHandler instance;
		return instance;
	}

	void handleActions()
	{
		camera = Renderer::getInstance().getActiveScene()->getActiveCamera();
		object = UserInterface::getInstance().getObjectManager()->getSelectedObject();
		objects = UserInterface::getInstance().getObjectManager()->getSelectedObjects();
		m_lastXClick = InputHandler::getInstance().lastXClick;
		m_lastYClick = InputHandler::getInstance().lastYClick;

		glfwGetCursorPos(m_window, &xPos, &yPos);


		handleCameraTransform(objects, camera, xPos, yPos);

		handleManipulation(objects, camera, xPos, yPos);
	}

private:

	void handleSelection()
	{
		if (m_manipulationStruct.state != Nothing)
		{
			setManipulationState(Nothing);
		}
		else
		{
			selectObject();
		}
	}

	void handleCameraZoom(double offset)
	{
		if (checkForUi()) return;

		float off = static_cast<float>(offset) / 10.0f;
		Renderer::getInstance().getActiveScene()->getActiveCamera()->multiplyDistance(1.0f - off);
	}

	void handleCameraTransform(std::vector<GameObject*>& objects, Camera* camera, double xPos, double yPos)
	{
		updateDeltaTime();

		Camera* activeCamera = Renderer::getInstance().getActiveScene()->getActiveCamera();
		if (activeCamera->isImmutable()) return;

		glm::vec3 moveOff = glm::vec3(0.0f);
		double xpos, ypos;
		glfwGetCursorPos(m_window, &xpos, &ypos);

		float horizontalAngle = activeCamera->getHorizontalAngle();
		float verticalAngle = activeCamera->getVerticalAngle();
		glm::vec3 anchor = activeCamera->getAnchor();

		if (InputHandler::getInstance().mouseMiddleDown)
		{
			horizontalAngle += m_mouseSpeed * m_deltaTime * float(m_lastXpos - xpos);
			verticalAngle += m_mouseSpeed * m_deltaTime * float(m_lastYpos - ypos);

			activeCamera->setHorizontalAngle(horizontalAngle);
			activeCamera->setVerticalAngle(verticalAngle);
		}
		else if (InputHandler::getInstance().shiftMouseMiddleDown)
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

	void handleManipulation(std::vector<GameObject*>& objects, Camera* camera, double xPos, double yPos)
	{
		if (m_manipulationStruct.state != Nothing)
		{
			for (int i = 0; i < objects.size(); i++)
			{
				auto object = objects[i];
				if (object == nullptr) continue;

				glm::vec2 cursorPosition = glm::vec2(xPos, yPos);
				glm::vec2 startPosition = glm::vec2(m_lastXManipulation, m_lastYManipulation);
				glm::vec2 mainObjectPosition = worldToScreen(UserInterface::getInstance().getObjectManager()->getSelectedObject()->getPosition(), camera->getViewMatrix(), camera->getProjectionMatrix());

				if (m_manipulationStruct.state == Scaling && m_manipulationStruct.inititalScale.size() == objects.size())
				{
					float distanceOffset = glm::length(mainObjectPosition - startPosition);
					float distance = glm::length(cursorPosition - mainObjectPosition) - distanceOffset;

					object->setScale(m_manipulationStruct.inititalScale[i] + glm::vec3(distance / 20.f) * m_manipulationStruct.manipulationFactor);
				}
				else if (m_manipulationStruct.state == Moving && m_manipulationStruct.initialPosition.size() == objects.size())
				{
					glm::vec2 objectPosition = worldToScreen(object->getPosition(), camera->getViewMatrix(), camera->getProjectionMatrix());

					int width, height;
					glfwGetWindowSize(m_window, &width, &height);

					float tempDepth;
					static float depth;
					glReadPixels(static_cast<int>(objectPosition.x), height - static_cast<int>(objectPosition.y), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &tempDepth);
					if (tempDepth > 0.f)
					{
						depth = tempDepth;
					}

					glm::vec3 cursorWorldPosition = screenToWorld(width, height, cursorPosition.x, cursorPosition.y, depth);
					glm::vec3 startWorldPosition = screenToWorld(width, height, startPosition.x, startPosition.y, depth);

					glm::vec3 direction = cursorWorldPosition - startWorldPosition;

					object->setPosition(m_manipulationStruct.initialPosition[i] + direction * m_manipulationStruct.manipulationFactor);
				}
				else if (m_manipulationStruct.state == Rotating && m_manipulationStruct.inititalRotation.size() == objects.size())
				{
					glm::vec3 rotationAxis = camera->getPosition() - object->getPosition();
					rotationAxis.x = rotationAxis.x < 0 ? -1.f : 1.f;
					rotationAxis.y = rotationAxis.y < 0 ? -1.f : 1.f;
					rotationAxis.z = rotationAxis.z < 0 ? -1.f : 1.f;

					glm::vec2 startDirection = startPosition - mainObjectPosition;
					glm::vec2 currentDirection = cursorPosition - mainObjectPosition;

					float startRadians = atan2(startDirection.y, startDirection.x);
					float currentRadians = atan2(currentDirection.y, currentDirection.x);

					float degrees = glm::degrees(startRadians - currentRadians);

					float value = degrees < 0 ? 360.f - abs(degrees) : degrees;

					object->setRotation(m_manipulationStruct.inititalRotation[i] + value * (m_manipulationStruct.manipulationFactor * rotationAxis));
				}
			}

		}
	}

	void selectAllObjects()
	{
		// set manipulation state to nothing to prevent undefined behavior
		setManipulationState(Nothing);

		// send nullptr to clear objects and avoid duplicates
		UserInterface::getInstance().getObjectManager()->setSelectedObject(nullptr);

		for (auto& object : Renderer::getInstance().getActiveScene()->getObjects())
		{
			UserInterface::getInstance().getObjectManager()->addSelectedObject(object);
		}
	}

	void undoManipulation()
	{
		for (int i = 0; i < objects.size(); i++)
		{
			auto object = objects[i];
			if (object == nullptr) continue;

			switch (m_manipulationStruct.state)
			{
			case Moving:
				object->setPosition(m_manipulationStruct.initialPosition[i]);
				break;
			case Rotating:
				object->setRotation(m_manipulationStruct.inititalRotation[i]);
				break;
			case Scaling:
				object->setScale(m_manipulationStruct.inititalScale[i]);
				break;
			default:
				break;
			}
		}

		setManipulationState(Nothing);
	}

	void setManipulationState(ManipulationState state)
	{
		m_manipulationStruct.state = state;
		m_manipulationStruct.initialPosition.clear();
		m_manipulationStruct.inititalRotation.clear();
		m_manipulationStruct.inititalScale.clear();
	}

	void recenterCamera()
	{
		if (object == nullptr) return;

		camera->setAnchor(object->getPosition());
		camera->setDistance(glm::length(object->getScale()) * 2);
	}

	void duplicateObject()
	{
		GameObject* object = UserInterface::getInstance().getObjectManager()->getSelectedObject();
		if (object == nullptr) return;

		GameObject* obj = new GameObject(*object);
		obj->setIsPhysicsEnabled(true);

		Renderer::getInstance().addObject(obj);
		UserInterface::getInstance().getObjectManager()->setSelectedObject(obj);

		moveObject();
	}

	void deleteObject()
	{
		auto& objects = UserInterface::getInstance().getObjectManager()->getSelectedObjects();

		for (int i = 0; i < objects.size(); i++)
		{
			auto object = objects[i];
			if (object == nullptr) continue;

			Renderer::getInstance().deleteObject(*object);
		}

		UserInterface::getInstance().getObjectManager()->setSelectedObject(nullptr);
	}

	void scaleObject()
	{
		m_lastXManipulation = xPos;
		m_lastYManipulation = yPos;
		setManipulationState(m_manipulationStruct.state == Scaling ? Nothing : Scaling);

		auto& objects = UserInterface::getInstance().getObjectManager()->getSelectedObjects();

		for (int i = 0; i < objects.size(); i++)
		{
			auto object = objects[i];
			if (object == nullptr) continue;

			m_manipulationStruct.inititalScale.push_back(object->getScale());
		}

		m_manipulationStruct.manipulationFactor = glm::vec3(1.f);
	}

	void moveObject()
	{
		m_lastXManipulation = xPos;
		m_lastYManipulation = yPos;
		setManipulationState(m_manipulationStruct.state == Moving ? Nothing : Moving);

		auto& objects = UserInterface::getInstance().getObjectManager()->getSelectedObjects();

		for (int i = 0; i < objects.size(); i++)
		{
			auto object = objects[i];
			if (object == nullptr) continue;

			m_manipulationStruct.initialPosition.push_back(object->getPosition());
		}

		m_manipulationStruct.manipulationFactor = glm::vec3(1.f);
	}

	void rotateObject()
	{
		m_lastXManipulation = xPos;
		m_lastYManipulation = yPos;
		setManipulationState(m_manipulationStruct.state == Rotating ? Nothing : Rotating);

		auto& objects = UserInterface::getInstance().getObjectManager()->getSelectedObjects();

		for (int i = 0; i < objects.size(); i++)
		{
			auto object = objects[i];
			if (object == nullptr) continue;

			m_manipulationStruct.inititalRotation.push_back(object->getRotation());
		}

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
		if (checkForUi()) return;

		double distance = glm::length(glm::abs(glm::vec2(xPos, yPos) - glm::vec2(m_lastXClick, m_lastYClick)));
		if (distance > 2) return;

		glm::vec3 worldCoords = screenToWorld(xPos, yPos);

		// check for object
		for (auto& object : Renderer::getInstance().getActiveScene()->getObjects())
		{
			glm::vec3 correctedWorldCoods = worldCoords * 0.9f;
			if (checkHitAABB(object, correctedWorldCoods))
			{
				UserInterface::getInstance().getObjectManager()->setSelectedObject(object);
				return;
			}
		}

		// no object clicked, send nullptr
		UserInterface::getInstance().getObjectManager()->setSelectedObject(nullptr);

	}


	bool checkForUi() const
	{
		if (!GImGui) return false;

		ImGuiContext& g = *GImGui;

		// check if imgui is on top 
		for (int i = 2; i < g.Windows.Size; i++)
		{
			auto& window = g.Windows[i];
			if (window->Active && xPos > window->Pos.x && xPos < window->Pos.x + window->Size.x && yPos > window->Pos.y && yPos < window->Pos.y + window->Size.y)
			{
				return true;
			}
		}

		return false;
	}

	bool checkHitAABB(GameObject* gameObject, glm::vec3 value)
	{
		return gameObject->getPosition().x + gameObject->getScale().x / 2 > value.x &&
			gameObject->getPosition().x - gameObject->getScale().x / 2 < value.x &&
			gameObject->getPosition().y + gameObject->getScale().y / 2 > value.y &&
			gameObject->getPosition().y - gameObject->getScale().y / 2 < value.y &&
			gameObject->getPosition().z + gameObject->getScale().z / 2 > value.z &&
			gameObject->getPosition().z - gameObject->getScale().z / 2 < value.z;
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
		Camera* activeCamera = Renderer::getInstance().getActiveScene()->getActiveCamera();

		double ndcX = (2.0 * mouseX) / width - 1.0;
		double ndcY = 1.0 - (2.0 * mouseY) / height;

		glm::vec4 ndcPosition = glm::vec4(ndcX, ndcY, depth * 2.0 - 1.0, 1.0);
		glm::mat4 invProjection = glm::inverse(activeCamera->getProjectionMatrix());
		glm::vec4 eyePosition = invProjection * ndcPosition;

		glm::mat4 invModelView = glm::inverse(activeCamera->getViewMatrix());
		glm::vec4 worldPosition = invModelView * eyePosition;

		return glm::vec3(worldPosition) / worldPosition.w;
	}

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
#pragma endregion

	void updateDeltaTime()
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;
	}


	struct ManipulationStruct
	{
		ManipulationState state;

		std::vector<glm::vec3> inititalScale;
		std::vector<glm::vec3> initialPosition;
		std::vector<glm::vec3> inititalRotation;

		glm::vec3 manipulationFactor;
	};

	// object scaling
	ManipulationStruct m_manipulationStruct;

	GLFWwindow* m_window;

	double m_lastXpos = 0,
		m_lastYpos = 0,
		m_lastXClick = 0,
		m_lastYClick = 0,
		m_lastXManipulation,
		m_lastYManipulation;

	float m_movementSpeed = 2.f,
		m_mouseSpeed = .5f,
		m_speed = 20.f,
		m_deltaTime = 0.f,
		m_lastFrameTime = 0.f;

	Camera* camera = Renderer::getInstance().getActiveScene()->getActiveCamera();
	GameObject* object = UserInterface::getInstance().getObjectManager()->getSelectedObject();
	std::vector<GameObject*>& objects = UserInterface::getInstance().getObjectManager()->getSelectedObjects();

	double xPos, yPos;
};