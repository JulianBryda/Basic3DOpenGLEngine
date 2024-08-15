#pragma once
#include <iostream>
#include <vector>

#include "DirectionalLight.hpp"
#include "Camera.hpp"


class Scene
{

public:

	Scene()
	{
		this->m_lights.push_back(new DirectionalLight("Sun", glm::vec3(0.1f), glm::vec3(1.0f), glm::vec3(1.0f)));

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

	inline std::vector<GameObject*>& getEnvObjects()
	{
		return this->m_envObjects;
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

	void addEnvObject(GameObject* object)
	{
		this->m_envObjects.push_back(object);
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

	glm::vec3 getWorldBBMin()
	{
		glm::vec3 min = glm::vec3(0.f);

		for (auto& object : m_objects)
		{
			glm::vec3 pos = object->getPosition();
			glm::vec3 scale = object->getScale();

			if (pos.x - scale.x < min.x) min.x = pos.x - scale.x;
			if (pos.y - scale.y < min.y) min.y = pos.y - scale.y;
			if (pos.z - scale.z < min.z) min.z = pos.z - scale.z;
		}

		return min;
	}

	glm::vec3 getWorldBBMax()
	{
		glm::vec3 max = glm::vec3(0.f);

		for (auto& object : m_objects)
		{
			glm::vec3 pos = object->getPosition();
			glm::vec3 scale = object->getScale();

			if (pos.x + scale.x > max.x) max.x = pos.x + scale.x;
			if (pos.y + scale.y > max.y) max.y = pos.y + scale.y;
			if (pos.z + scale.z > max.z) max.z = pos.z + scale.z;
		}

		return max;
	}

private:

	std::vector<Light*> m_lights;

	std::vector<Camera*> m_cameras;
	Camera* m_pActiveCamera;

	std::vector<GameObject*> m_objects;
	std::vector<GameObject*> m_envObjects;

};