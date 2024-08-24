#pragma once
#include <map>

#include "RendererBase.hpp"
#include "Scene.hpp"
#include "PhysicEngine.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class RendererManager
{

public:

	RendererManager()
	{
		m_pDebugShader = ShaderLib::getDebugShaderPtr();
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
		for (auto& renderer : m_renderers)
		{
			renderer->render(getActiveScene());
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

	Shader* getDebugShader() const
	{
		return m_pDebugShader;
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

	void setDebugShader(Shader* shader)
	{
		m_pDebugShader = shader;
	}

	void setActiveScene(Scene* scene)
	{
		m_activeScene = scene;
	}

	// modifier
	void addRenderer(RendererBase* renderer)
	{
		m_renderers.push_back(renderer);

		for (int i = 0; i < m_renderers.size() - 1; ++i)
		{
			bool swapped = false;

			for (int j = 0; j < m_renderers.size() - i - 1; ++j)
			{
				if (m_renderers[j]->getType() > m_renderers[j + 1]->getType())
				{
					std::swap(m_renderers[j], m_renderers[j + 1]);
					swapped = true;
				}
			}

			if (!swapped)
			{
				break;
			}
		}
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

	Shader* m_pDebugShader;


	RendererManager(const RendererManager&) = delete;
	RendererManager& operator=(const RendererManager&) = delete;
};