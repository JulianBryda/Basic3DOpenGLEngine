#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "PhysicalizedObject.hpp"
#include "SquareObject.hpp"


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
					glm::vec3 direction = glm::normalize(position - m_physicalizedObjects[j]->getPosition());
					{
						if (direction.x <= 0) direction.x += 1.0f;
						else direction.x -= 1.0f;

						if (direction.y <= 0) direction.y += 1.0f;
						else direction.y -= 1.0f;

						if (direction.z <= 0) direction.z += 1.0f;
						else direction.z -= 1.0f;
					}

					obj->setVelocity(velocity * direction);
					isColliding = true;
					break;
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

	static void addObject(PhysicalizedGameObject* obj)
	{
		m_physicalizedObjects.push_back(obj);
	}

	static PhysicalizedGameObject* getObject(GameObject* object)
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

	static std::vector<PhysicalizedGameObject*> m_physicalizedObjects;

	static float deltaTime, lastFrameTime;

};