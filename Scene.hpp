#pragma once
#include <iostream>
#include <vector>

#include "Light.hpp"


class Scene
{

public:

	Scene()
	{

	}

	~Scene()
	{

	}


	// getter
	inline std::vector<Light>* getLightsPtr() { return &this->m_lights; }


	// modifier
	inline void addLight(Light light) { this->m_lights.push_back(light); }

	void deleteLight(Light* light)
	{
		m_lights.erase(std::remove(m_lights.begin(), m_lights.end(), light), m_lights.end());

		delete light;
	}

private:

	std::vector<Light> m_lights;


};