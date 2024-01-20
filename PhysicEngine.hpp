#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "GameObject.h"

class PhysicEngine
{

public:

	static void update()
	{
		updateDeltaTime();


		for (size_t i = 0; i < m_physicalizedObjects.size(); i++)
		{
			auto obj = m_physicalizedObjects[i];
			bool isColliding = false;

			glm::vec3 velocity = obj->getVelocity();
			glm::vec3 position = obj->getPosition();
			glm::vec3 oldPosition = obj->getPosition();

			if (obj->getIsGravityEnabled())
			{
				velocity = velocity + glm::vec3(0.0f, -obj->getGravity(), 0.0f) * deltaTime;
			}

			velocity -= obj->getLinearDrag() * velocity * deltaTime;
			position += velocity * deltaTime;

			obj->setPosition(position);

			for (size_t j = 0; j < m_physicalizedObjects.size(); j++)
			{
				if (obj != m_physicalizedObjects[j] && obj->checkBoundingBoxCollision(m_physicalizedObjects[j]))
				{
					if (obj->checkBoundingBoxCollisionX(m_physicalizedObjects[j]))
					{
						obj->setVelocity(velocity * glm::vec3(0.0f, 1.0f, 1.0f));
						isColliding = true;
					}

					if (obj->checkBoundingBoxCollisionY(m_physicalizedObjects[j]))
					{
						obj->setVelocity(velocity * glm::vec3(1.0f, 0.0f, 1.0f));
						isColliding = true;
					}
				}
			}

			if (isColliding)
			{
				obj->setPosition(oldPosition);
			}
			else
			{
				obj->setVelocity(velocity);
			}
		}
	}

	static void addObject(GameObject* obj)
	{
		m_physicalizedObjects.push_back(obj);
	}

	static void removeObject(GameObject* obj)
	{
		m_physicalizedObjects.erase(std::remove(m_physicalizedObjects.begin(), m_physicalizedObjects.end(), obj), m_physicalizedObjects.end());
	}

	static GameObject* getObject(GameObject* object)
	{
		for (size_t i = 0; i < m_physicalizedObjects.size(); i++)
		{
			if (object == m_physicalizedObjects[i])
			{
				return m_physicalizedObjects[i];
			}
		}

		return nullptr;
	}

private:

	static void updateDeltaTime()
	{
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
	}

	static std::vector<GameObject*> m_physicalizedObjects;

	static float deltaTime, lastFrameTime;

};