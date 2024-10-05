#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "../Scene/Objects/Camera.hpp"
#include "../Scene/Scene.hpp"

#include "../Core/PhysicEngine.hpp"


class Renderer
{
public:

	Renderer()
	{
		m_pDebugShader = ShaderLib::get("debug.glsl");
		m_activeScene = new Scene();
		addScene(m_activeScene);
	}

	static Renderer& getInstance()
	{
		static Renderer instance;
		return instance;
	}	
	
	void render();

	// getter

	Scene* getActiveScene()
	{
		return m_activeScene;
	}

	Shader* getDebugShader() const
	{
		return m_pDebugShader;
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

	std::vector<Scene*> m_scenes;
	Scene* m_activeScene;

	Shader* m_pDebugShader;
};
