#pragma once
#include <glm/glm.hpp>

#include "../../Collider.hpp"
#include "../../Utils/MaterialLib.hpp"
#include "../../Renderer/Data/Mesh.hpp"

class GameObject;

class GameObjectCollisions
{

public:

	GameObjectCollisions(GameObject* gameObject, ColliderType colliderType);

	~GameObjectCollisions();

	void drawCollider();
	void snapColliderToObject();

	// collision detection
	bool checkCollision(GameObject& object);

	bool checkCollision(glm::vec3 position, glm::vec3 scale, ColliderType colliderType)
	{
		switch (collider.getColliderType())
		{
		case AABB:
		{
			switch (colliderType)
			{
			case AABB:
				return Collision::checkAABB(collider.getAnchor(), collider.getScale(), position, scale);
			case Sphere:
				return Collision::checkAABBToSphere(collider.getAnchor(), collider.getScale(), position, scale);
			default:
				return false;
			}
		}
		case OBB:
			break;
		case Sphere:
		{
			switch (colliderType)
			{
			case AABB:
				return Collision::checkAABBToSphere(position, scale, collider.getAnchor(), collider.getScale());
			case Sphere:
				return Collision::checkSphere(position, scale, collider.getAnchor(), collider.getScale());
			default:
				return false;
			}
		}
		case Capsule:
			return false;
		case Convex:
			return false;
		case NONE:
			return false;
		default:
			return false;
		}
	}

	// setter
	void checkBuffers();

	// getter
	bool getIsDrawCollider() const;
	bool* getIsDrawColliderPtr();
	bool getIsCollisionEnabled() const;
	bool* getIsCollisionEnabledPtr();
	Collider* getColliderPtr();
	Collider& getCollider();


protected:

	bool isDrawCollider, isCollisionEnabled;

	Collider collider;

private:

	GLuint vao, vbo, ebo;
	GameObject* gameObject;
	Mesh mesh;

	void createCollisionBoxBuffers();
	void updateCollisionBoxBuffers();

};