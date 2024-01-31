#include "GameObjectCollisions.h"
#include "GameObject.h"

GameObjectCollisions::GameObjectCollisions(GameObject* gameObject, ColliderType colliderType)
{
	this->vao = 0;
	this->vbo = 0;
	this->ebo = 0;

	this->gameObject = gameObject;
	this->isDrawCollider = false;
	this->isCollisionEnabled = false;
	this->collider = new Collider(gameObject->getPositionPtr(), gameObject->getScale(), colliderType);
	
	std::string objName = (colliderType == ColliderType::BoundingBox) ? "Cube" : "Sphere";
	std::string path = std::format("\\Assets\\Objects\\{}.obj", objName);

	ObjectLoader::LoadObjFile(path.c_str(), &this->mesh);
}


GameObjectCollisions::~GameObjectCollisions()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	delete this->collider;
}


// Bounding Box Collision detection
bool GameObjectCollisions::checkBoundingBoxCollision(GameObject* object)
{
	//NOTE: maybe the z axis collision detection is broken
	return gameObject->getColliderPtr()->getAnchorPositionPtr()->x + gameObject->getColliderPtr()->getScale().x / 2 > object->getColliderPtr()->getAnchorPositionPtr()->x - object->getColliderPtr()->getScale().x / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->x - gameObject->getColliderPtr()->getScale().x / 2 < object->getColliderPtr()->getAnchorPositionPtr()->x + object->getColliderPtr()->getScale().x / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->y + gameObject->getColliderPtr()->getScale().y / 2 > object->getColliderPtr()->getAnchorPositionPtr()->y - object->getColliderPtr()->getScale().y / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->y - gameObject->getColliderPtr()->getScale().y / 2 < object->getColliderPtr()->getAnchorPositionPtr()->y + object->getColliderPtr()->getScale().y / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->z + gameObject->getColliderPtr()->getScale().z / 2 > object->getColliderPtr()->getAnchorPositionPtr()->z - object->getColliderPtr()->getScale().z / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->z - gameObject->getColliderPtr()->getScale().z / 2 < object->getColliderPtr()->getAnchorPositionPtr()->z + object->getColliderPtr()->getScale().z / 2;
}

// Bounding Box Collision detection
bool GameObjectCollisions::checkBoundingBoxCollision(glm::vec3 value)
{
	//NOTE: maybe the z axis collision detection is broken
	return gameObject->getColliderPtr()->getAnchorPositionPtr()->x + gameObject->getColliderPtr()->getScale().x / 2 > value.x &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->x - gameObject->getColliderPtr()->getScale().x / 2 < value.x &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->y + gameObject->getColliderPtr()->getScale().y / 2 > value.y &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->y - gameObject->getColliderPtr()->getScale().y / 2 < value.y &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->z + gameObject->getColliderPtr()->getScale().z / 2 > value.z &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->z - gameObject->getColliderPtr()->getScale().z / 2 < value.z;
}

bool GameObjectCollisions::checkBoundingBoxCollisionX(GameObject* object)
{
	return gameObject->getColliderPtr()->getAnchorPositionPtr()->x + gameObject->getColliderPtr()->getScale().x / 2 > object->getColliderPtr()->getAnchorPositionPtr()->x - object->getColliderPtr()->getScale().x / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->x - gameObject->getColliderPtr()->getScale().x / 2 < object->getColliderPtr()->getAnchorPositionPtr()->x + object->getColliderPtr()->getScale().x / 2;
}

bool GameObjectCollisions::checkBoundingBoxCollisionY(GameObject* object)
{
	return gameObject->getColliderPtr()->getAnchorPositionPtr()->y + gameObject->getColliderPtr()->getScale().y / 2 > object->getColliderPtr()->getAnchorPositionPtr()->y - object->getColliderPtr()->getScale().y / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->y - gameObject->getColliderPtr()->getScale().y / 2 < object->getColliderPtr()->getAnchorPositionPtr()->y + object->getColliderPtr()->getScale().y / 2;
}

bool GameObjectCollisions::checkBoundingBoxCollisionZ(GameObject* object)
{
	//NOTE: maybe the z axis collision detection is broken
	return gameObject->getColliderPtr()->getAnchorPositionPtr()->z + gameObject->getColliderPtr()->getScale().z / 2 > object->getColliderPtr()->getAnchorPositionPtr()->z - object->getColliderPtr()->getScale().z / 2 &&
		gameObject->getColliderPtr()->getAnchorPositionPtr()->z - gameObject->getColliderPtr()->getScale().z / 2 < object->getColliderPtr()->getAnchorPositionPtr()->z + object->getColliderPtr()->getScale().z / 2;
}


bool GameObjectCollisions::checkCircularCollision(GameObject* object)
{
	float distance = glm::length(*this->getColliderPtr()->getAnchorPositionPtr() - *object->getColliderPtr()->getAnchorPositionPtr());
	distance = abs(distance);

	return distance < this->getColliderPtr()->getScale().x + object->getColliderPtr()->getScale().x;
}

bool GameObjectCollisions::checkSATCollision(GameObject* object)
{
	return false;
}

void GameObjectCollisions::drawCollider()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, mesh.getIndicesPtr()->size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameObjectCollisions::snapColliderToObject()
{
	this->collider->setScale(this->gameObject->getScale());
}

// setter
void GameObjectCollisions::checkBuffers()
{
	if (this->isCollisionEnabled)
	{
		if (this->vao == 0)
		{
			this->createCollisionBoxBuffers();
			this->collider->setScale(this->gameObject->getScale());
		}
		else
			this->updateCollisionBoxBuffers();
	}
}


// getter
bool GameObjectCollisions::getIsDrawCollider() const { return this->isDrawCollider; }
bool* GameObjectCollisions::getIsDrawColliderPtr() { return &this->isDrawCollider; }
Collider* GameObjectCollisions::getColliderPtr() { return this->collider; }
bool GameObjectCollisions::getIsCollisionEnabled() const { return this->isCollisionEnabled; }
bool* GameObjectCollisions::getIsCollisionEnabledPtr() { return &this->isCollisionEnabled; }


void GameObjectCollisions::createCollisionBoxBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.getVerticesPtr()->size(), mesh.getVerticesPtr()->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.getIndicesPtr()->size(), mesh.getIndicesPtr()->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObjectCollisions::updateCollisionBoxBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.getVertices().size(), mesh.getVertices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.getIndices().size(), mesh.getIndices().data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}