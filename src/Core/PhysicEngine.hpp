#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../Scene/Objects/GameObject.hpp"
#include "../Renderer/Renderer.hpp"
#include "../Renderer/UI/ObjectManager.hpp"

class PhysicEngine
{

public:

	PhysicEngine()
	{
		m_deltaTime = 0.f;
		m_lastFrameTime = 0.f;
	}

	~PhysicEngine()
	{

	}

	static PhysicEngine& getInstance()
	{
		static PhysicEngine instance;
		return instance;
	}

	void update()
	{
		updateDeltaTime();

		for (auto& object : Renderer::getInstance().getActiveScene()->getObjects())
		{
			bool isColliding = false;

			glm::vec3 velocity = object->getVelocity();
			glm::vec3 position = object->getPosition();

			if (object->getIsGravityEnabled())
			{
				velocity += glm::vec3(0.0f, -object->getGravity(), 0.0f) * m_deltaTime;
			}
			else if (object->getIsPullToObjectEnabled())
			{
				for (auto& secondObject : Renderer::getInstance().getActiveScene()->getObjects())
				{
					if (object == secondObject) continue;

					glm::vec3 direction = object->getPosition() - secondObject->getPosition();
					float distance = glm::length(direction);
					float forceMagnitude = GRAVCONST * object->getMass() * secondObject->getMass() / (distance * distance);
					glm::vec3 force = glm::normalize(direction) * forceMagnitude;

					velocity -= force / object->getMass();
				}
			}

			velocity -= object->getLinearDrag() * velocity * m_deltaTime;
			position += velocity * m_deltaTime;

			// collision detection
			if (object->getIsCollisionEnabled())
			{
				for (auto& secondObject : Renderer::getInstance().getActiveScene()->getObjects())
				{
					// check if collisions is enabled on object and object to check for are not the same
					if (!secondObject->getIsCollisionEnabled() || object == secondObject) continue;

					if (!object->checkCollision(*secondObject)) continue;

					glm::vec3 mtv = glm::vec3(0.f);
					glm::vec3 overlap = Collision::getOverlap(position, object->getScale(), secondObject->getPosition(), secondObject->getScale());
					glm::vec3 absOverlap = glm::abs(overlap);

					if (absOverlap.x < absOverlap.y && absOverlap.x < absOverlap.z)
					{
						mtv = glm::vec3(-overlap.x, 0.f, 0.f);
						velocity.x = 0.f;
					}
					else if (absOverlap.y < absOverlap.x && absOverlap.y < absOverlap.z)
					{
						mtv = glm::vec3(0.f, -overlap.y, 0.f);
						velocity.y = 0.f;
					}
					else
					{
						mtv = glm::vec3(0.f, 0.f, -overlap.z);
						velocity.z = 0.f;
					}

					position += mtv;
				}
			}

			object->setPosition(position);
			object->setVelocity(velocity);
		}
	}

private:

	void updateDeltaTime()
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		m_deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;
	}

	float m_deltaTime, m_lastFrameTime;

	const float GRAVCONST = 6.67430f;
};