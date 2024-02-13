#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.hpp"
#include "Vertex.hpp"

namespace ObjectLoader
{

	static void load_model_mesh_assimp(const char* path, Mesh* mesh)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}

		for (uint32_t i = 0; i < scene->mNumMeshes; i++) 
		{
			const aiMesh* aiMesh = scene->mMeshes[i];
			for (uint32_t j = 0; j < aiMesh->mNumVertices; j++)
			{
				glm::vec3 vertex(aiMesh->mVertices[j].x, aiMesh->mVertices[j].y, aiMesh->mVertices[j].z);
				glm::vec3 normal(aiMesh->mNormals[j].x, aiMesh->mNormals[j].y, aiMesh->mNormals[j].z);
				glm::vec2 uv(aiMesh->mTextureCoords[0][j].x, aiMesh->mTextureCoords[0][j].y);

				mesh->addVertex(Vertex(vertex, normal, uv));
			}
			
			for (uint32_t j = 0; j < aiMesh->mNumFaces; j++)
			{
				const aiFace& face = aiMesh->mFaces[j];
				for (uint32_t k = 0; k < face.mNumIndices; k++)
				{
					mesh->addIndex(face.mIndices[k]);
				}
			}
		}
	}
};