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

			if (obj->getIsGravityEnabled())
			{
				velocity += glm::vec3(0.0f, -obj->getGravity(), 0.0f) * m_deltaTime;
			}
			else if (obj->getIsPullToObjectEnabled())
			{
				for (size_t j = 0; j < m_physicalizedObjects.size(); j++)
				{
					if (obj == m_physicalizedObjects[j]) continue;

					glm::vec3 direction = obj->getPosition() - m_physicalizedObjects[j]->getPosition();
					float distance = glm::length(direction);
					float forceMagnitude = GRAVCONST * obj->getMass() * m_physicalizedObjects[j]->getMass() / (distance * distance);
					glm::vec3 force = glm::normalize(direction) * forceMagnitude;

					velocity -= force / obj->getMass();
				}
			}

			velocity -= obj->getLinearDrag() * velocity * m_deltaTime;
			position += velocity * m_deltaTime;

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
					if (m_focusedGameObject != nullptr && m_focusedGameObject != m_physicalizedObjects[j])
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
					if (m_focusedGameObject != nullptr && m_focusedGameObject != m_physicalizedObjects[j])
					{
						m_physicalizedObjects[j]->setIsHidden(false);
					}
				}

				// check which collision algorithm to use
				switch (obj->getColliderPtr()->getColliderType())
				{
				case ColliderType::BoundingBox:
				{
					if (Collision::checkBoundingBoxCollisionX(position, obj->getScale(), m_physicalizedObjects[j]->getPosition(), m_physicalizedObjects[j]->getScale()))
					{
						velocity *= glm::vec3(1.f, 1.f, .1f);
						position.x = obj->getPositionPtr()->x;
					}

					if (Collision::checkBoundingBoxCollisionY(position, obj->getScale(), m_physicalizedObjects[j]->getPosition(), m_physicalizedObjects[j]->getScale()))
					{
						velocity *= glm::vec3(.1f, 1.f, 1.f);
						position.x = obj->getPositionPtr()->y;
					}

					if (Collision::checkBoundingBoxCollisionZ(position, obj->getScale(), m_physicalizedObjects[j]->getPosition(), m_physicalizedObjects[j]->getScale()))
					{
						velocity *= glm::vec3(1.f, .1f, 1.f);
						position.y = obj->getPositionPtr()->z;
					}

					break;
				}
				case ColliderType::Circular:
				{
					if (obj->checkCircularCollision(m_physicalizedObjects[j]))
					{
						obj->setVelocity(glm::vec3(0.0f));
					}
					break;
				}
				default:
				{
					break;
				}
				}
			}

			obj->setPosition(position);
			obj->setVelocity(velocity);
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

	static GameObject* getFocusedGameObject() { return m_focusedGameObject; }
	static void setFocusedGameObject(GameObject* object) { m_focusedGameObject = object; }

private:

	static void updateDeltaTime()
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;
	}

	static std::vector<GameObject*> m_physicalizedObjects;

	static float m_deltaTime, m_lastFrameTime;

	static GameObject* m_focusedGameObject;

	static constexpr float GRAVCONST = 6.67430f;
};