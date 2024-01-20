#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Collider.hpp"

class GameObject;

class GameObjectCollisions
{

public:

	GameObjectCollisions(GameObject* gameObject);

	~GameObjectCollisions();

	void drawCollider();
	void snapColliderToObject();

	// collision detection
	bool checkBoundingBoxCollision(GameObject* object);
	bool checkBoundingBoxCollisionX(GameObject* object);
	bool checkBoundingBoxCollisionY(GameObject* object);
	bool checkBoundingBoxCollisionZ(GameObject* object);

	bool checkCirlceCollision(GameObject* object);
	bool checkSATCollision(GameObject* object);


	// setter
	void setIsCollisionEnabled(bool isCollisionEnabled);

	// getter
	bool getIsDrawCollider() const;
	bool* getIsDrawColliderPtr();
	bool getIsCollisionEnabled() const;
	Collider* getColliderPtr();


protected:

	bool isDrawCollider, isCollisionEnabled;


private:

	GLuint vao, vbo, ebo;
	GameObject* gameObject;
	Collider* collider;

	void createCollisionBoxBuffers();
	void updateCollisionBoxBuffers();
	void deleteCollisionBoxBuffers() const;
};