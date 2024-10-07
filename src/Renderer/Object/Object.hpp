#pragma once
#include <iostream>
#include <vector>

#include "../../Scene/Scene.hpp"
#include "../Graphics/ShaderOLD.hpp"
#include "../../Globals/GlobalTextures.hpp"

#include "../../Landscape.hpp"

namespace Object
{
	void renderDebug(GameObject* object, Scene* activeScene)
	{
		ShaderOLD* shader = Renderer::getInstance().getDebugShader();
		shader->use();

		auto& objectAtlas = object->getParameterAtlas();
		auto finalAtlas = activeScene->getActiveCamera()->getParameterAtlas(); // do not use refernce on this one!

		//merge maps
		for (const auto& pair : objectAtlas)
		{
			finalAtlas.insert_or_assign(pair.first, pair.second);
		}

		for (auto& name : shader->getParameterAtlas())
		{
			if (finalAtlas.contains(name))
			{
				ShaderOLD::UniformType uniformType = finalAtlas[name];

				shader->setUniform(uniformType);
			}
		}

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
		ShaderOLD* shader = object->getShaderPtr();
		shader->use();

		auto& objectAtlas = object->getParameterAtlas();
		auto finalAtlas = activeScene->getActiveCamera()->getParameterAtlas(); // do not use refernce on this one!

		//merge maps
		for (const auto& pair : objectAtlas)
		{
			finalAtlas.insert_or_assign(pair.first, pair.second);
		}

		for (auto& name : shader->getParameterAtlas())
		{
			if (finalAtlas.contains(name))
			{
				ShaderOLD::UniformType uniformType = finalAtlas[name];

				shader->setUniform(uniformType);
			}
		}

		Landscape* landscape = dynamic_cast<Landscape*>(object);
		if (landscape)
		{
			shader->setFloat("slopeStart", landscape->getSlopeStart());
			shader->setFloat("slopeEnd", landscape->getSlopeEnd());

			shader->setFloat3("flatColor", landscape->getFlatColor());
			shader->setFloat3("slopeColor", landscape->getSlopeColor());
		}
		else
		{
			shader->setFloat("time", glfwGetTime());

			shader->setTexture(object->getTextureType(), object->getTexture(), GL_TEXTURE0);

			shader->setLightCount(activeScene->getLights().size());
			for (int i = 0; i < activeScene->getLights().size(); i++)
			{
				shader->setLight(activeScene->getLights()[i], i, GL_TEXTURE1 + i);
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

};