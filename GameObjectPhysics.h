#pragma once
#include <glm/glm.hpp>

class GameObject;

class GameObjectPhysics
{

public:

	GameObjectPhysics(GameObject* gameObject);

	~GameObjectPhysics();

	// setter
	void setVelocity(glm::vec3 velocity);
	void setMass(float mass);
	void setGravity(float gravity);
	void setLinearDrag(float linearDrag);
	void setIsInteractEnabled(bool isInteractEnabled);
	void setIsPhysicsEnabled(bool isPhysicsEnabled);
	void setIsGravityEnabled(bool isGravityEnabled);


	// getter
	glm::vec3 getVelocity() const;
	glm::vec3* getVelocityPtr();
	float getMass() const;
	float getGravity() const;
	bool getIsPhysicsEnabled() const;
	float getLinearDrag() const;
	float* getLinearDragPtr();
	bool getIsInteractEnabled() const;
	bool* getIsInteractEnabledPtr();
	bool getIsGravityEnabled() const;
	bool* getIsGravityEnabledPtr();

	// math functions
	float getSpeed() const;
	float getKineticEnergy() const;
	float getPotentialEnergy() const;


protected:

	glm::vec3 velocity;
	float mass, gravity, linearDrag;
	bool isPhysicsEnabled, isInteractEnabled, isGravityEnabled;


private:

	GameObject* gameObject;

};