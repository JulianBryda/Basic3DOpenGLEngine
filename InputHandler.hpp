#pragma once
#include <iostream>
#include "RendererManager.hpp"
#include <GLFW/glfw3.h>
#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "glm/glm.hpp"
#include "ImguiRenderer.hpp"
#include "imgui_internal.h"

class InputHandler
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


	InputHandler()
	{
		m_window = nullptr;

		m_manipulationStruct = { ManipulationState::Nothing };
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
		if (activeCamera->isImmutable()) return;

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


		// handle manipulation
		if (m_manipulationStruct.state != Nothing)
		{
			auto& objects = ImguiRenderer::getSelectedObjects();

			for (int i = 0; i < objects.size(); i++)
			{
				auto object = objects[i];
				if (object == nullptr) continue;

				glm::vec2 cursorPosition = glm::vec2(xpos, ypos);
				glm::vec2 startPosition = glm::vec2(m_lastXManipulation, m_lastYManipulation);
				glm::vec2 mainObjectPosition = worldToScreen(ImguiRenderer::getSelectedObject()->getPosition(), activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());

				if (m_manipulationStruct.state == Scaling && m_manipulationStruct.inititalScale.size() == objects.size())
				{
					float distanceOffset = glm::length(mainObjectPosition - startPosition);
					float distance = glm::length(cursorPosition - mainObjectPosition) - distanceOffset;

					object->setScale(m_manipulationStruct.inititalScale[i] + glm::vec3(distance / 20.f) * m_manipulationStruct.manipulationFactor);
				}
				else if (m_manipulationStruct.state == Moving && m_manipulationStruct.initialPosition.size() == objects.size())
				{
					glm::vec2 objectPosition = worldToScreen(object->getPosition(), activeCamera->getViewMatrix(), activeCamera->getProjectionMatrix());

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

					object->setPosition(m_manipulationStruct.initialPosition[i] + direction * m_manipulationStruct.manipulationFactor);
				}
				else if (m_manipulationStruct.state == Rotating && m_manipulationStruct.inititalRotation.size() == objects.size())
				{
					glm::vec3 rotationAxis = activeCamera->getPosition() - object->getPosition();
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
					setManipulationState(Nothing);
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
			case GLFW_KEY_C:
				if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
				{
					recenterCamera();
				}
				break;
			case GLFW_KEY_A:
				selectAllObjects();
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
			case GLFW_KEY_F2:
				saveDepthMaps();
				break;
			case GLFW_KEY_F3:
				saveOverdrawMap();
				break;
			default:
				break;
			}
		}
	}

	void selectAllObjects()
	{
		// set manipulation state to nothing to prevent undefined behavior
		setManipulationState(Nothing);

		// send nullptr to clear objects and avoid duplicates
		ImguiRenderer::setSelectedObject(nullptr);

		for (auto& object : RendererManager::getInstance().getActiveScene()->getObjects())
		{
			ImguiRenderer::addSelectedObject(object);
		}
	}

	void undoManipulation()
	{
		auto& objects = ImguiRenderer::getSelectedObjects();

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
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		Camera* camera = RendererManager::getInstance().getActiveScene()->getActiveCamera();
		camera->setAnchor(object->getPosition());
		camera->setDistance(glm::length(object->getScale()) * 2);

	}

	void duplicateObject()
	{
		GameObject* object = ImguiRenderer::getSelectedObject();
		if (object == nullptr) return;

		GameObject* obj = new GameObject(*object);
		obj->setIsPhysicsEnabled(true);

		RendererManager::getInstance().addObject(obj);
		ImguiRenderer::setSelectedObject(obj);

		moveObject();
	}

	void deleteObject()
	{
		auto& objects = ImguiRenderer::getSelectedObjects();

		for (int i = 0; i < objects.size(); i++)
		{
			auto object = objects[i];
			if (object == nullptr) continue;

			RendererManager::getInstance().deleteObject(*object);
		}

		ImguiRenderer::setSelectedObject(nullptr);
	}

	void scaleObject()
	{
		glfwGetCursorPos(m_window, &m_lastXManipulation, &m_lastYManipulation);
		setManipulationState(m_manipulationStruct.state == Scaling ? Nothing : Scaling);

		auto& objects = ImguiRenderer::getSelectedObjects();

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
		glfwGetCursorPos(m_window, &m_lastXManipulation, &m_lastYManipulation);
		setManipulationState(m_manipulationStruct.state == Moving ? Nothing : Moving);

		auto& objects = ImguiRenderer::getSelectedObjects();

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
		glfwGetCursorPos(m_window, &m_lastXManipulation, &m_lastYManipulation);
		setManipulationState(m_manipulationStruct.state == Rotating ? Nothing : Rotating);

		auto& objects = ImguiRenderer::getSelectedObjects();

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
		double mouseX, mouseY;
		glfwGetCursorPos(m_window, &mouseX, &mouseY);

		double distance = glm::length(glm::abs(glm::vec2(mouseX, mouseY) - glm::vec2(m_lastXClick, m_lastYClick)));
		if (distance > 2) return;

		glm::vec3 worldCoords = screenToWorld(mouseX, mouseY);
		ImGuiContext& g = *GImGui;

		// check if imgui is on top 
		for (int i = 2; i < g.Windows.Size; i++)
		{
			auto& window = g.Windows[i];
			if (window->Active && mouseX > window->Pos.x && mouseX < window->Pos.x + window->Size.x && mouseY > window->Pos.y && mouseY < window->Pos.y + window->Size.y)
			{
				return;
			}
		}

		// check for object
		for (auto& object : RendererManager::getInstance().getActiveScene()->getObjects())
		{
			glm::vec3 correctedWorldCoods = worldCoords * 0.9f;
			if (checkHitAABB(object, correctedWorldCoods))
			{
				ImguiRenderer::setSelectedObject(object);
				return;
			}
		}

		// no object clicked, send nullptr
		ImguiRenderer::setSelectedObject(nullptr);

	}

	void saveOverdrawMap()
	{
		const char* filePath = ".\\Assets\\Debug\\Overdraw.png";
		int width = Config::g_settings->screenWidth, height = Config::g_settings->screenHeight;

		std::vector<GLuint> pixels(width * height);

		glBindTexture(GL_TEXTURE_2D, Textures::g_textures->overdrawTexture);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, pixels.data());

		std::vector<unsigned char> rgbaPixels(width * height * 4);

		for (int i = 0; i < width * height; i++) {
			unsigned char intensity = static_cast<unsigned char>(pixels[i] / 10);  // Scale the overdraw count to fit into 8 bits
			rgbaPixels[i * 4 + 0] = intensity; // R
			rgbaPixels[i * 4 + 1] = 0;         // G
			rgbaPixels[i * 4 + 2] = 0;         // B
			rgbaPixels[i * 4 + 3] = 255;       // A
		}

		if (stbi_write_png(filePath, width, height, 4, rgbaPixels.data(), width * 4))
		{
			std::cout << "Saved overdraw to " << filePath << std::endl;
		}
		else
		{
			std::cerr << "Failed to save overdraw to " << filePath << std::endl;
		}
	}

	void saveDepthMaps()
	{
		auto& lights = RendererManager::getInstance().getActiveScene()->getLights();

		for (int i = 0; i < lights.size(); i++)
		{
			saveShadowMapAsImage(lights[i]->getDepthMapFBO(), lights[i]->getShadowWidth(), lights[i]->getShadowHeight(), std::format(".\\Assets\\Debug\\{}.png", lights[i]->getName()).c_str());
		}
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

	void setWindow(GLFWwindow* window)
	{
		m_window = window;
	}

	GLFWwindow* getWindow() const
	{
		return m_window;
	}


private:

	struct ManipulationStruct
	{
		ManipulationState state;

		std::vector<glm::vec3> inititalScale;
		std::vector<glm::vec3> initialPosition;
		std::vector<glm::vec3> inititalRotation;

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