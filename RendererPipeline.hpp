#pragma once
#include <map>

#include "Renderer.hpp"

class RendererPipeline
{

public:

	/// <summary>
	/// calls the render function of every registered renderer
	/// </summary>
	static void renderPipeline()
	{
		// you have to add +1 to RendererType::MAX otherwise the for loop will stop before it reaches the max value
		for (int i = RendererType::MIN; i != RendererType::MAX + 1; i++)
		{
			RendererType type = static_cast<RendererType>(i);
			auto values = rendererMap.equal_range(type);

			for (auto& it = values.first; it != values.second; ++it)
			{
				it->second->render(cameras[activeCameraIndex]);
			}
		}
	}


	static void registerRenderer(Renderer* renderer, RendererType type)
	{
		if (rendererMap.contains(type))
		{
			std::cout << "Failed to add new renderer! Type: " << type << " already registered!\n";
			return;
		}

		rendererMap.insert(std::pair<RendererType, Renderer*>(type, renderer));
	}

	static std::multimap<RendererType, Renderer*>& getRendererMap()
	{
		return rendererMap;
	}

	static Renderer* getRendererPtr(RendererType rendererType)
	{
		return RendererPipeline::getRendererMap().find(rendererType)->second;
	}


	static void addCamera(Camera* camera)
	{
		cameras.push_back(camera);
	}

	static void deleteCamera(Camera* camera)
	{
		cameras.erase(std::remove(cameras.begin(), cameras.end(), camera), cameras.end());

		delete camera;
	}

	static void setActiveCamera(int index)
	{
		activeCameraIndex = index;
	}

	static Camera* getActiveCamera()
	{
		return cameras[activeCameraIndex];
	}

	static std::vector<Camera*> getCameras()
	{
		return cameras;
	}

	static int getActiveCameraIndex()
	{
		return activeCameraIndex;
	}


	static void addObjectToRenderer(GameObject* object, RendererType type);

private:

	static std::multimap<RendererType, Renderer*> rendererMap;
	static std::vector<Camera*> cameras;
	static int activeCameraIndex;

};
