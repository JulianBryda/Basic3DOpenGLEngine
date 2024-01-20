#include "GameObjectCollisions.h"
#include "GameObject.h"

GameObjectCollisions::GameObjectCollisions(GameObject* gameObject)
{
	this->vao = 0;
	this->vbo = 0;
	this->ebo = 0;

	this->gameObject = gameObject;
	this->isDrawCollider = false;
	this->isCollisionEnabled = false;
	this->collider = new Collider(gameObject->getPositionPtr(), gameObject->getScale());
}


GameObjectCollisions::~GameObjectCollisions()
{
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


bool GameObjectCollisions::checkCirlceCollision(GameObject* object)
{
	return false;
}

bool GameObjectCollisions::checkSATCollision(GameObject* object)
{
	return false;
}

void GameObjectCollisions::drawCollider()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, gameObject->getIndices().size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GameObjectCollisions::snapColliderToObject()
{
	this->collider->setScale(this->gameObject->getScale());
}

// setter
void GameObjectCollisions::setIsCollisionEnabled(bool isCollisionEnabled)
{
	if (isCollisionEnabled)
	{
		if (this->vao == 0)
		{
			this->createCollisionBoxBuffers();
			this->collider->setScale(this->gameObject->getScale());
		}
		else
			this->updateCollisionBoxBuffers();
	}

	this->isCollisionEnabled = isCollisionEnabled;
}


// getter
bool GameObjectCollisions::getIsDrawCollider() const { return this->isDrawCollider; }
bool* GameObjectCollisions::getIsDrawColliderPtr() { return &this->isDrawCollider; }
Collider* GameObjectCollisions::getColliderPtr() { return this->collider; }
bool GameObjectCollisions::getIsCollisionEnabled() const { return this->isCollisionEnabled; }


void GameObjectCollisions::createCollisionBoxBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * gameObject->getVertices().size(), gameObject->getVertices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * gameObject->getIndices().size(), gameObject->getIndices().data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObjectCollisions::updateCollisionBoxBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * gameObject->getVertices().size(), gameObject->getVertices().data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * gameObject->getIndices().size(), gameObject->getIndices().data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObjectCollisions::deleteCollisionBoxBuffers() const
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
}