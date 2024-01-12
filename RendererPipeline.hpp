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
				it->second->render();
			}
		}
	}


	static void registerRenderer(Renderer* renderer, RendererType type)
	{
		rendererMap.insert(std::pair<RendererType, Renderer*>(type, renderer));
	}

	static std::multimap<RendererType, Renderer*>& getRendererMap()
	{
		return rendererMap;
	}

private:

	static std::multimap<RendererType, Renderer*> rendererMap;

};
