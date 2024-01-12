#pragma once
#include <glm/glm.hpp>
#include "GameObject.hpp"

class PhysicalizedObject
{

public:

	PhysicalizedObject()
	{
		this->gravity = 9.81f;
		this->velocity = glm::vec3(0.0f);
		this->mass = 0.0f;
		this->isGravityEnabled = false;
		this->linearDrag = 0.05f;
	}

	~PhysicalizedObject() {}

	// setter
	void setVelocity(glm::vec3 velocity) { this->velocity = velocity; }
	void setMass(float mass) { this->mass = mass; }
	void setGravity(float gravity) { this->gravity = gravity; }
	void setIsGravityEnabled(bool isGravityEnabled) { this->isGravityEnabled = isGravityEnabled; }
	void setLinearDrag(float linearDrag) { this->linearDrag = linearDrag; }
	void setIsInteractEnabled(bool isInteractEnabled) { this->isInteractEnabled = isInteractEnabled; }

	// getter
	glm::vec3 getVelocity() const { return this->velocity; }
	glm::vec3* getVelocityPtr() { return &this->velocity; }
	float getMass() const { return this->mass; }
	float getGravity() const { return this->gravity; }
	bool getIsGravityEnabled() const { return this->isGravityEnabled; }
	bool* getIsGravityEnabledPtr() { return &this->isGravityEnabled; }
	float getLinearDrag() const { return this->linearDrag; }
	float* getLinearDragPtr() { return &this->linearDrag; }
	bool getIsInteractEnabled() { return this->isInteractEnabled; }
	bool* getIsInteractEnabledPtr() { return &this->isInteractEnabled; }


	// math functions
	float getSpeed() const { return glm::length(this->velocity); }
	float getKineticEnergy() const { return 0.5f * this->mass * glm::pow(this->getSpeed(), 2); }
	float getPotentialEnergy() const { return this->mass * this->gravity; }


protected:

	glm::vec3 velocity;
	float mass, gravity, linearDrag;
	bool isGravityEnabled;
	bool isInteractEnabled;

};