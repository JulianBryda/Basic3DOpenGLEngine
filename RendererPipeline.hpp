#pragma once
#include <map>

#include "Renderer.hpp"
#include "Scene.hpp"

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
				it->second->render(getActiveScenePtr()->getActiveCameraPtr());
			}
		}
	}


	// getter
	static std::multimap<RendererType, Renderer*>& getRendererMap() { return rendererMap; }

	static Renderer* getRendererPtr(RendererType rendererType) { return RendererPipeline::getRendererMap().find(rendererType)->second; }

	static Scene* getActiveScenePtr()
	{
		assert(scenes.size() != 0); // no scene!
		return scenes[activeSceneIndex];
	}


	// setter
	static void setGlobalRenderMode(RenderMode renderMode)
	{
		for (auto const& x : rendererMap)
		{
			x.second->setRenderMode(renderMode);
		}
	}


	// modifier
	static void registerRenderer(Renderer* renderer, RendererType type)
	{
		if (rendererMap.contains(type))
		{
			std::cout << "Failed to add new renderer! Type: " << type << " already registered!\n";
			return;
		}

		rendererMap.insert(std::pair<RendererType, Renderer*>(type, renderer));
	}

	static void addScene(Scene* scene) { scenes.push_back(scene); }
	static void deleteScene(Scene* scene)
	{
		scenes.erase(std::remove(scenes.begin(), scenes.end(), scene), scenes.end());

		delete scene;
	}

	static void addObjectToRenderer(GameObject* object, RendererType type);

private:

	static std::multimap<RendererType, Renderer*> rendererMap;

	static std::vector<Scene*> scenes;
	static int activeSceneIndex;

};
