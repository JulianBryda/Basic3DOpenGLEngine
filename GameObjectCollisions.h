#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Collider.hpp"
#include "ShaderLib.hpp"
#include "Mesh.hpp"

class GameObject;

class GameObjectCollisions
{

public:

	GameObjectCollisions(GameObject* gameObject, ColliderType colliderType);

	~GameObjectCollisions();

	void drawCollider();
	void snapColliderToObject();

	// collision detection
	bool checkBoundingBoxCollision(GameObject& object);
	bool checkBoundingBoxCollision(glm::vec3 value);

	bool checkBoundingBoxCollisionX(GameObject& object);
	bool checkBoundingBoxCollisionY(GameObject& object);
	bool checkBoundingBoxCollisionZ(GameObject& object);

	bool checkCircularCollision(GameObject& object);
	bool checkSATCollision(GameObject& object);


	// setter
	void checkBuffers();

	// getter
	bool getIsDrawCollider() const;
	bool* getIsDrawColliderPtr();
	bool getIsCollisionEnabled() const;
	bool* getIsCollisionEnabledPtr();
	Collider* getColliderPtr();


protected:

	bool isDrawCollider, isCollisionEnabled;


private:

	GLuint vao, vbo, ebo;
	GameObject* gameObject;
	Collider* collider;
	Mesh mesh;

	void createCollisionBoxBuffers();
	void updateCollisionBoxBuffers();
};