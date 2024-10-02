#include "PhysicEngine.hpp"

std::vector<GameObject*> PhysicEngine::m_physicalizedObjects;

float PhysicEngine::m_deltaTime, PhysicEngine::m_lastFrameTime;

GameObject* PhysicEngine::m_focusedGameObject;