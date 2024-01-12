#include "PhysicEngine.hpp"

std::vector<PhysicalizedGameObject*> PhysicEngine::m_physicalizedObjects;

float PhysicEngine::deltaTime, PhysicEngine::lastFrameTime;