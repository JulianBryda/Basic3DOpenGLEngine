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
		this->m_lights.push_back(new DirectionalLight("Sun", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f)));

		Camera* camera = new Camera("Camera", false);
		camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		this->addCamera(camera);

		this->m_pActiveCamera = camera;
	}

	~Scene()
	{

	}


	// getter
	inline std::vector<Light*>& getLights()
	{
		return this->m_lights;
	}

	inline Camera* getActiveCamera()
	{
		return this->m_pActiveCamera;
	}

	inline std::vector<Camera*>& getCameras()
	{
		return this->m_cameras;
	}

	inline std::vector<GameObject*>& getObjects()
	{
		return this->m_objects;
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

	void addObject(GameObject* object)
	{
		this->m_objects.push_back(object);
	}

	void deleteObject(GameObject* object)
	{
		for (size_t i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[i] == object)
			{
				m_objects.erase(m_objects.begin() + i);
				break;
			}
		}

		delete object;
	}

	inline void addLight(Light* light)
	{
		this->m_lights.push_back(light);
	}

	void deleteLight(Light* light)
	{
		for (size_t i = 0; i < m_lights.size(); i++)
		{
			if (m_lights[i] == light)
			{
				m_lights.erase(m_lights.begin() + i);
				break;
			}
		}

		delete light;
	}

private:

	std::vector<Light*> m_lights;

	std::vector<Camera*> m_cameras;
	Camera* m_pActiveCamera;

	std::vector<GameObject*> m_objects;

};