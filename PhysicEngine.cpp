#include "PhysicEngine.hpp"

std::vector<GameObject*> PhysicEngine::m_physicalizedObjects;

float PhysicEngine::deltaTime, PhysicEngine::lastFrameTime;

GameObject* PhysicEngine::focusedGameObject;