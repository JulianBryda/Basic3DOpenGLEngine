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

			// collision detection
			for (size_t j = 0; j < m_physicalizedObjects.size(); j++)
			{
				// check if collisions is enabled on object and object to check for are not the same
				if (!m_physicalizedObjects[j]->getIsCollisionEnabled() || obj == m_physicalizedObjects[j]) continue;

				// check if objects are even close enough to collide
				if (std::abs(glm::length(obj->getPosition() - m_physicalizedObjects[j]->getPosition())) > glm::length(obj->getScale() + m_physicalizedObjects[j]->getScale()))
				{
					// object is not close enough
					// every object not in collision range of selected object will get hidden
					if (focusedGameObject != nullptr && focusedGameObject != m_physicalizedObjects[j])
					{
						m_physicalizedObjects[j]->setIsHidden(true);
					}
					else
					{
						m_physicalizedObjects[j]->setIsHidden(false);
					}

					// object not close enough to collide so skip collisions detection 
					continue;
				}
				else
				{
					// object is close enough
					if (focusedGameObject != nullptr && focusedGameObject != m_physicalizedObjects[j])
					{
						m_physicalizedObjects[j]->setIsHidden(false);
					}
				}

				// check which collision algorithm to use
				switch (obj->getColliderPtr()->getColliderType())
				{
				case ColliderType::BoundingBox:
				{
					if (obj->checkBoundingBoxCollision(m_physicalizedObjects[j]))
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
					break;
				}
				case ColliderType::Circular:
				{
					if (obj->checkCircularCollision(m_physicalizedObjects[j]))
					{
						obj->setVelocity(glm::vec3(0.0f));
						isColliding = true;
					}
					break;
				}
				default:
				{
					break;
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

	static GameObject* getFocusedGameObject() { return focusedGameObject; }
	static void setFocusedGameObject(GameObject* object) { focusedGameObject = object; }

private:

	static void updateDeltaTime()
	{
		float currentFrameTime = glfwGetTime();
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
	}

	static std::vector<GameObject*> m_physicalizedObjects;

	static float deltaTime, lastFrameTime;

	static GameObject* focusedGameObject;

};