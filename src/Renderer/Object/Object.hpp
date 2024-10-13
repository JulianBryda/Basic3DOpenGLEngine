#pragma once
#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../../Scene/Scene.hpp"
#include "../Graphics/Material.hpp"
#include "../../Globals/GlobalTextures.hpp"

#include "../../Landscape.hpp"

namespace Object
{
	void renderDebug(GameObject* object, Scene* activeScene)
	{
		Material* material = Renderer::getInstance().getDebugMaterial();
		material->use();

		auto& objectAtlas = object->getParameterAtlas();
		auto finalAtlas = activeScene->getActiveCamera()->getParameterAtlas(); // do not use reference on this one!

		//merge maps
		for (const auto& pair : objectAtlas)
		{
			finalAtlas.insert_or_assign(pair.first, pair.second);
		}

		for (auto& name : material->getParameterAtlas())
		{
			if (finalAtlas.contains(name))
			{
				Material::UniformType uniformType = finalAtlas[name];

				material->setUniform(uniformType);
			}
		}

		material->setFloat("time", glfwGetTime());
		material->setTexture(GL_TEXTURE_2D, Textures::get("overdraw"), GL_TEXTURE0);
		material->setUInt("atomicImage", 0);

		object->draw();

		// prevents flickering when visualizing overdraw
		if (Config::g_settings->debugMode == Config::DebugMode::Overdraw)
		{
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
		}

		// cleanup texture to prevent wrong usage on other objects
		material->setTexture(GL_TEXTURE_2D, 0, GL_TEXTURE0);
	}

	void renderNormal(GameObject* object, Scene* activeScene)
	{
		Material* material = object->getMaterialPtr();
		material->use();

		auto& objectAtlas = object->getParameterAtlas();
		auto finalAtlas = activeScene->getActiveCamera()->getParameterAtlas(); // do not use refernce on this one!

		//merge maps
		for (const auto& pair : objectAtlas)
		{
			finalAtlas.insert_or_assign(pair.first, pair.second);
		}

		for (auto& name : material->getParameterAtlas())
		{
			if (finalAtlas.contains(name))
			{
				Material::UniformType uniformType = finalAtlas[name];

				material->setUniform(uniformType);
			}
		}

		Landscape* landscape = dynamic_cast<Landscape*>(object);
		if (landscape)
		{
			material->setFloat("slopeStart", landscape->getSlopeStart());
			material->setFloat("slopeEnd", landscape->getSlopeEnd());

			material->setFloat3("flatColor", landscape->getFlatColor());
			material->setFloat3("slopeColor", landscape->getSlopeColor());
		}
		else
		{
			material->setFloat("time", glfwGetTime());

			//material->setTexture(object->getTextureType(), object->getTexture(), GL_TEXTURE0);

			material->setLightCount(activeScene->getLights().size());
			for (int i = 0; i < activeScene->getLights().size(); i++)
			{
				material->setLight(activeScene->getLights()[i], i, GL_TEXTURE1 + i);
			}
		}

		object->draw();

		/*
		// cleanup texture to prevent wrong usage on other objects
		shader->setTexture(object->getTextureType(), 0, GL_TEXTURE0);

		shader = ShaderLib::get("color.glsl");
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
		*/
	}

	void render(Scene* activeScene)
	{
		if (Config::g_settings->renderMode == Config::RenderMode::Debug)
		{
			GLuint clearValue = 0;
			glClearTexImage(Textures::get("overdraw"), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, &clearValue);

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

};