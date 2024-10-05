#pragma once
#include <iostream>
#include <vector>

#include "Lights/DirectionalLight.hpp"
#include "Objects/Camera.hpp"


class Scene
{

public:

	Scene()
	{
		this->m_lights.push_back(new DirectionalLight("Sun", glm::vec3(1.0f)));

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

	inline uint32_t countVertices()
	{
		size_t vertices = 0;

		for (auto& object : m_objects)
		{
			vertices += object->getMesh().getVertices().size();
		}

		return static_cast<uint32_t>(vertices);
	}

	inline uint32_t countIndices()
	{
		size_t indices = 0;

		for (auto& object : m_objects)
		{
			indices += object->getMesh().getIndices().size();
		}

		return static_cast<uint32_t>(indices);
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

	glm::vec3 getWorldBBMinMax(glm::vec3& max)
	{
		glm::vec3 min(std::numeric_limits<float>::max());
		max = glm::vec3(std::numeric_limits<float>::lowest());

		for (const auto& object : m_objects)
		{
			const glm::vec3& position = object->getPosition();
			const glm::vec3 halfScale = object->getScale() * 0.5f; // Calculate once

			const glm::vec3 posMin = position - halfScale;
			const glm::vec3 posMax = position + halfScale;

			min.x = std::min(min.x, posMin.x);
			min.y = std::min(min.y, posMin.y);
			min.z = std::min(min.z, posMin.z);

			max.x = std::max(max.x, posMax.x);
			max.y = std::max(max.y, posMax.y);
			max.z = std::max(max.z, posMax.z);
		}

		return min;
	}

private:

	std::vector<Light*> m_lights;

	std::vector<Camera*> m_cameras;
	Camera* m_pActiveCamera;

	std::vector<GameObject*> m_objects;
	std::vector<GameObject*> m_envObjects;
};