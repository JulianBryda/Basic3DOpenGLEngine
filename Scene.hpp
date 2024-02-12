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
		this->directionalLight = new DirectionalLight("Sun", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));

		Camera* camera = new Camera("Camera", false);
		camera->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
		this->addCamera(camera);
	}

	~Scene()
	{
		delete directionalLight;
	}


	// getter
	inline std::vector<PointLight*>* getPointLightsPtr() { return &this->pointLights; }

	inline Camera* getActiveCameraPtr() const { return this->cameras[activeCameraIndex]; }

	inline std::vector<Camera*>* getCamerasPtr() { return &this->cameras; }

	inline int getActiveCameraIndex() const { return this->activeCameraIndex; }

	inline DirectionalLight* getDirectionalLightPtr() const { return this->directionalLight; }


	// setter
	inline void setActiveCamera(int index) { this->activeCameraIndex = index; }


	// modifier
	inline void addCamera(Camera* camera) { this->cameras.push_back(camera); }
	inline void deleteCamera(Camera* camera)
	{
		this->cameras.erase(std::remove(this->cameras.begin(), this->cameras.end(), camera), this->cameras.end());

		delete camera;
	}

	inline void addPointLight(PointLight* light) { this->pointLights.push_back(light); }
	void deletePointLight(PointLight* light)
	{
		pointLights.erase(std::remove(pointLights.begin(), pointLights.end(), light), pointLights.end());

		delete light;
	}

private:

	DirectionalLight* directionalLight;

	std::vector<PointLight*> pointLights;

	std::vector<Camera*> cameras;
	int activeCameraIndex;

	std::vector<GameObject*> objects;

};