#pragma once
#include <map>

#include "RendererBase.hpp"
#include "Scene.hpp"

enum RenderMode
{
	Debug,
	Render
};

class RendererManager
{

public:

	RendererManager()
	{
		this->m_renderMode = RenderMode::Debug;
		this->m_activeSceneIndex = 0;
	}

	static RendererManager& getInstance()
	{
		static RendererManager instance;
		return instance;
	}

	/// <summary>
	/// calls the render function of every registered renderer
	/// </summary>
	void renderAll()
	{
		RendererType type = RendererType::MIN;

		while (type != RendererType::MAX)
		{
			for (auto& renderer : m_renderers)
			{
				if (renderer->getType() == type)
				{
					renderer->render(getActiveScene()->getActiveCamera());
					type = static_cast<RendererType>(static_cast<int>(type) + 1);
					break;
				}
			}
		}
	}


	// getter
	std::vector<RendererBase*>& getRenderers()
	{
		return m_renderers;
	}

	Scene* getActiveScene()
	{
		assert(m_scenes.size() != 0); // no scene!
		return m_scenes[m_activeSceneIndex];
	}

	RenderMode getRenderMode() const
	{
		return m_renderMode;
	}

	RendererBase* getRenderer(RendererType type)
	{
		RendererBase* base = nullptr;

		for (RendererBase* renderer : m_renderers)
		{
			if (renderer->getType() == type)
			{
				base = renderer;
				break;
			}
		}

		return base;
	}

	// setter
	void setRenderMode(RenderMode renderMode)
	{
		RendererManager::m_renderMode = renderMode;
	}

	// modifier
	void addRenderer(RendererBase* renderer)
	{
		//if (m_renderers.contains(type))
		//{
		//	std::cout << "Failed to add new renderer! Type: " << type << " already registered!\n";
		//	return;
		//}

		m_renderers.push_back(renderer);
	}

	void addScene(Scene* scene)
	{
		this->m_scenes.push_back(scene);
	}

	void deleteScene(Scene* scene)
	{
		for (size_t i = 0; i < m_scenes.size(); i++)
		{
			if (m_scenes[i] == scene)
			{
				m_scenes.erase(m_scenes.begin() + i);
				break;
			}
		}

		delete scene;
	}

	void addObject(GameObject* object, RendererType type)
	{
		switch (type)
		{
		case Object:
			getRenderer(type)->addObject(object);
			break;
		case Environment:
			getRenderer(type)->addObject(object);
			break;
		default:
			throw std::exception("Type is not supported!");
			break;
		}
	}

private:

	std::vector<RendererBase*> m_renderers;

	std::vector<Scene*> m_scenes;
	int m_activeSceneIndex;

	RenderMode m_renderMode;


	RendererManager(const RendererManager&) = delete;
	RendererManager& operator=(const RendererManager&) = delete;
};
