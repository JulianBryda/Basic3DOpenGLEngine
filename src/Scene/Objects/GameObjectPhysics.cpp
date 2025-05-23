#include "GameObjectPhysics.h"
#include "GameObject.hpp"

GameObjectPhysics::GameObjectPhysics(GameObject* gameObject)
{
	this->gravity = 9.81f;
	this->velocity = glm::vec3(0.0f);
	this->mass = 50.0f;
	this->isPhysicsEnabled = false;
	this->linearDrag = 0.05f;
	this->isInteractEnabled = false;
	this->isGravityEnabled = false;
	this->isPullToObjectEnabled = false;

	this->gameObject = gameObject;
}

GameObjectPhysics::~GameObjectPhysics()
{

}


// setter
void GameObjectPhysics::setVelocity(glm::vec3 velocity) { this->velocity = velocity; }
void GameObjectPhysics::setMass(float mass) { this->mass = mass; }
void GameObjectPhysics::setGravity(float gravity) { this->gravity = gravity; }
void GameObjectPhysics::setLinearDrag(float linearDrag) { this->linearDrag = linearDrag; }
void GameObjectPhysics::setIsInteractEnabled(bool isInteractEnabled) { this->isInteractEnabled = isInteractEnabled; }
void GameObjectPhysics::setIsGravityEnabled(bool isGravityEnabled) { this->isGravityEnabled = isGravityEnabled; }
void GameObjectPhysics::setIsPullToObjectEnabled(bool isPullToObjectEnabled) { this->isPullToObjectEnabled = isPullToObjectEnabled; }

bool GameObjectPhysics::toggleIsGravityEnabled()
{
	this->isGravityEnabled = !this->isGravityEnabled;
	return this->isGravityEnabled;
};
bool GameObjectPhysics::toggleIsPullToObjectEnabled()
{
	this->isPullToObjectEnabled = !this->isPullToObjectEnabled;
	return this->isPullToObjectEnabled;
}

void GameObjectPhysics::setIsPhysicsEnabled(bool isPhysicsEnabled)
{
	this->isPhysicsEnabled = isPhysicsEnabled;
}


// getter
glm::vec3 GameObjectPhysics::getVelocity() const { return this->velocity; }
glm::vec3* GameObjectPhysics::getVelocityPtr() { return &this->velocity; }
float GameObjectPhysics::getMass() const { return this->mass; }
float* GameObjectPhysics::getMassPtr() { return &this->mass; }
float GameObjectPhysics::getGravity() const { return this->gravity; }
bool GameObjectPhysics::getIsPhysicsEnabled() const { return this->isPhysicsEnabled; }
float GameObjectPhysics::getLinearDrag() const { return this->linearDrag; }
float* GameObjectPhysics::getLinearDragPtr() { return &this->linearDrag; }
bool GameObjectPhysics::getIsInteractEnabled() const { return this->isInteractEnabled; }
bool* GameObjectPhysics::getIsInteractEnabledPtr() { return &this->isInteractEnabled; }
bool GameObjectPhysics::getIsGravityEnabled() const { return this->isGravityEnabled; }
bool* GameObjectPhysics::getIsGravityEnabledPtr() { return &this->isGravityEnabled; }
bool GameObjectPhysics::getIsPullToObjectEnabled() const { return this->isPullToObjectEnabled; }
bool* GameObjectPhysics::getIsPullToObjectEnabledPtr() { return &this->isPullToObjectEnabled; }

// math functions
float GameObjectPhysics::getSpeed() const { return glm::length(this->velocity); }
float GameObjectPhysics::getKineticEnergy() const { return 0.5f * this->mass * static_cast<float>(glm::pow(this->getSpeed(), 2)); }
float GameObjectPhysics::getPotentialEnergy() const { return this->mass * this->gravity; }