#pragma once
#include "../Scene/Objects/GameObject.h"
#include "ObjectLoader.hpp"

namespace GameObjectConstructor
{

	static std::vector<GameObject*> createGameObjects(const char* path)
	{
		int objectCount = 0;
		std::vector<GameObject*> objects;
		ObjectLoader::ObjectData* objectData = ObjectLoader::load_model_mesh_assimp(path, objectCount);
		if (objectData == nullptr) return objects;

		for (int i = 0; i < objectCount; i++)
		{
			GameObject* obj = new GameObject(objectData[i].name, objectData[i].mesh, ShaderLib::get("render.glsl"), BoundingBox);
			objects.push_back(obj);
		}

		delete[] objectData;

		return objects;
	}

};