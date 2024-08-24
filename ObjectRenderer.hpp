#pragma once
#include <iostream>
#include <vector>

#include "RendererManager.hpp"
#include "GameObject.h"
#include "Shader.hpp"
#include "ShaderLib.hpp"
#include "GlobalConfig.hpp"
#include "GlobalTextures.hpp"

class ObjectRenderer : public RendererBase
{

public:

	ObjectRenderer() : RendererBase(RendererType::Object)
	{

	}

	void render(Scene* activeScene) override
	{
		if (Config::g_settings->renderMode == Config::RenderMode::Debug)
		{
			GLuint clearValue = 0;
			glClearTexImage(Textures::g_textures->overdrawTexture, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &clearValue);

			for (auto& object : activeScene->getObjects())
			{
				if (object->getHidden()) continue;

				renderDebug(object, activeScene);
			}
		}
		else
		{
			for (auto& object : activeScene->getObjects())
			{
				if (object->getHidden()) continue;

				renderNormal(object, activeScene);
			}
		}
	}

	void renderDebug(GameObject* object, Scene* activeScene)
	{
		Shader* shader = RendererManager::getInstance().getDebugShader();
		shader->use();

		shader->setMat4("projection", activeScene->getActiveCamera()->getProjectionMatrix());
		shader->setMat4("view", activeScene->getActiveCamera()->getViewMatrix());
		shader->setMat4("model", object->getModelMatrix());

		shader->setFloat3("viewPos", activeScene->getActiveCamera()->getPosition());
		shader->setMaterial(object->getMaterialPtr());

		shader->setTexture(GL_TEXTURE_2D, Textures::g_textures->overdrawTexture, GL_TEXTURE0);
		shader->setUInt("atomicImage", 0);

		object->draw();

		// prevents flickering when visualizing overdraw
		if (Config::g_settings->debugMode == Config::DebugMode::Overdraw)
		{
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}

		// cleanup texture to prevent wrong usage on other objects
		shader->setTexture(GL_TEXTURE_2D, 0, GL_TEXTURE0);
	}

	void renderNormal(GameObject* object, Scene* activeScene)
	{
		Shader* shader = object->getShaderPtr();
		shader->use();

		shader->setMat4("projection", activeScene->getActiveCamera()->getProjectionMatrix());
		shader->setMat4("view", activeScene->getActiveCamera()->getViewMatrix());
		shader->setMat4("model", object->getModelMatrix());

		shader->setFloat3("viewPos", activeScene->getActiveCamera()->getPosition());
		shader->setMaterial(object->getMaterialPtr());

		shader->setTexture(object->getTextureType(), object->getTexture(), GL_TEXTURE0);

		shader->setLightCount(activeScene->getLights().size());
		for (int i = 0; i < activeScene->getLights().size(); i++)
		{
			shader->setLight(activeScene->getLights()[i], i, GL_TEXTURE1 + i);
		}

		object->draw();

		// cleanup texture to prevent wrong usage on other objects
		shader->setTexture(object->getTextureType(), 0, GL_TEXTURE0);

		shader = ShaderLib::getColorShaderPtr();
		shader->use();

		shader->setMat4("projection", activeScene->getActiveCamera()->getProjectionMatrix());
		shader->setMat4("view", activeScene->getActiveCamera()->getViewMatrix());

		// draw hitbox?
		if (object->getIsDrawCollider())
		{
			shader->setMat4("model", object->getColliderPtr()->getModelMatrix());

			shader->setFloat4("color", glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));

			object->drawCollider();
		}
	}

private:

	int x = 0;

};