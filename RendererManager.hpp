#pragma once
#include <map>

#include "RendererBase.hpp"
#include "Scene.hpp"
#include "PhysicEngine.hpp"

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"


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
		m_renderMode = RenderMode::Debug;
		m_activeScene = new Scene();
		addScene(m_activeScene);
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
					renderer->render(getActiveScene());
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
		return m_activeScene;
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

	void setActiveScene(Scene* scene)
	{
		m_activeScene = scene;
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
		m_scenes.push_back(scene);
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

	void addObject(GameObject* object)
	{
		m_activeScene->addObject(object);
	}

	void deleteObject(GameObject& object)
	{
		m_activeScene->deleteObject(&object);
		PhysicEngine::removeObject(&object);
	}

private:

	std::vector<RendererBase*> m_renderers;

	std::vector<Scene*> m_scenes;
	Scene* m_activeScene;

	RenderMode m_renderMode;


	RendererManager(const RendererManager&) = delete;
	RendererManager& operator=(const RendererManager&) = delete;
};