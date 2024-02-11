#pragma once
#include <iostream>
#include <vector>

#include "Light.hpp"
#include "Camera.hpp"


class Scene
{

public:

	Scene()
	{
		Camera* camera = new Camera("Camera", false);
		camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		this->addCamera(camera);
	}

	~Scene()
	{

	}


	// getter
	inline std::vector<Light*>* getLightsPtr() { return &this->lights; }

	inline Camera* getActiveCameraPtr() const { return this->cameras[activeCameraIndex]; }

	inline std::vector<Camera*>* getCamerasPtr() { return &this->cameras; }

	inline int getActiveCameraIndex() const { return this->activeCameraIndex; }


	// setter
	inline void setActiveCamera(int index) { this->activeCameraIndex = index; }


	// modifier
	inline void addCamera(Camera* camera) { this->cameras.push_back(camera); }
	inline void deleteCamera(Camera* camera)
	{
		this->cameras.erase(std::remove(this->cameras.begin(), this->cameras.end(), camera), this->cameras.end());

		delete camera;
	}

	inline void addLight(Light* light) { this->lights.push_back(light); }

	void deleteLight(Light* light)
	{
		lights.erase(std::remove(lights.begin(), lights.end(), light), lights.end());

		delete light;
	}

private:

	std::vector<Light*> lights;

	std::vector<Camera*> cameras;
	int activeCameraIndex;

	std::vector<GameObject*> objects;

};