#pragma once
#include <iostream>
#include <vector>

#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "SpotLight.hpp"
#include "Camera.hpp"


class Scene
{

public:

	Scene()
	{
		this->m_directionalLights.push_back(new DirectionalLight("Sun", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

		Camera* camera = new Camera("Camera", false);
		camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		this->addCamera(camera);

		this->m_pActiveCamera = camera;
	}

	~Scene()
	{

	}


	// getter
	inline std::vector<PointLight*>& getPointLights()
	{
		return this->m_pointLights;
	}

	inline Camera* getActiveCamera()
	{
		return this->m_pActiveCamera;
	}

	inline std::vector<Camera*>& getCameras()
	{
		return this->m_cameras;
	}

	inline std::vector<DirectionalLight*>& getDirectionalLights()
	{
		return this->m_directionalLights;
	}


	// setter
	inline void setActiveCamera(Camera* camera) { this->m_pActiveCamera = camera; }


	// modifier
	inline void addCamera(Camera* camera)
	{
		this->m_cameras.push_back(camera);
	}

	void deleteCamera(Camera* camera)
	{
		for (size_t i = 0; i < m_cameras.size(); i++)
		{
			if (m_cameras[i] == camera)
			{
				m_cameras.erase(m_cameras.begin() + i);
				break;
			}
		}

		delete camera;
	}

	inline void addPointLight(PointLight* light)
	{
		this->m_pointLights.push_back(light);
	}

	void deletePointLight(PointLight* light)
	{
		for (size_t i = 0; i < m_pointLights.size(); i++)
		{
			if (m_pointLights[i] == light)
			{
				m_pointLights.erase(m_pointLights.begin() + i);
				break;
			}
		}

		delete light;
	}

private:

	std::vector<DirectionalLight*> m_directionalLights;
	std::vector<PointLight*> m_pointLights;

	std::vector<Camera*> m_cameras;
	Camera* m_pActiveCamera;

	std::vector<GameObject*> m_objects;

};