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
	struct ObjectData
	{
		char name[255];
		Mesh mesh;
		glm::vec3 bbMin;
		glm::vec3 bbMax;

	};

	static void computeBoundingBox(const aiScene* scene, aiVector3D* min, aiVector3D* max) {
		min->x = min->y = min->z = std::numeric_limits<float>::max();
		max->x = max->y = max->z = std::numeric_limits<float>::lowest();

		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
				const aiVector3D* position = &mesh->mVertices[j];

				min->x = std::min(min->x, position->x);
				min->y = std::min(min->y, position->y);
				min->z = std::min(min->z, position->z);

				max->x = std::max(max->x, position->x);
				max->y = std::max(max->y, position->y);
				max->z = std::max(max->z, position->z);
			}
		}
	}

	static float calculateScalingFactor(const aiVector3D& min, const aiVector3D& max, float targetSize) {
		aiVector3D size = max - min;
		float maxDimension = std::max(size.x, std::max(size.y, size.z));

		// Calculate scaling factor to normalize the size to the target size
		float scalingFactor = targetSize / maxDimension;
		return scalingFactor;
	}

	static void scaleModel(aiScene* scene, float scalingFactor) {
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[i];
			for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
				mesh->mVertices[j] *= scalingFactor;
			}
		}
	}

	static void normalizeModelSize(const aiScene* scene, float targetSize) {
		aiVector3D min, max;
		computeBoundingBox(scene, &min, &max);

		float scalingFactor = calculateScalingFactor(min, max, targetSize);

		scaleModel(const_cast<aiScene*>(scene), scalingFactor);
	}

	static ObjectData* load_model_mesh_assimp(const char* path, int& meshCount)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals | aiProcess_GenUVCoords);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return nullptr;
		}

		float targetSize = 1.f;
		normalizeModelSize(scene, targetSize);

		std::vector<ObjectData>* objects = new std::vector<ObjectData>();

		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* aiMesh = scene->mMeshes[i];

			ObjectData data = ObjectData();
			strcpy(data.name, aiMesh->mName.data);
			data.bbMin = glm::vec3(aiMesh->mAABB.mMin.x, aiMesh->mAABB.mMin.y, aiMesh->mAABB.mMin.z);
			data.bbMax = glm::vec3(aiMesh->mAABB.mMax.x, aiMesh->mAABB.mMax.y, aiMesh->mAABB.mMax.z);

			for (uint32_t j = 0; j < aiMesh->mNumVertices; j++)
			{
				glm::vec3 vertex(aiMesh->mVertices[j].x, aiMesh->mVertices[j].y, aiMesh->mVertices[j].z);
				glm::vec3 normal(aiMesh->mNormals[j].x, aiMesh->mNormals[j].y, aiMesh->mNormals[j].z);
				glm::vec2 uv(aiMesh->mTextureCoords[0][j].x, aiMesh->mTextureCoords[0][j].y);

				data.mesh.addVertex(Vertex(vertex, normal, uv));
			}

			for (uint32_t j = 0; j < aiMesh->mNumFaces; j++)
			{
				const aiFace& face = aiMesh->mFaces[j];
				for (uint32_t k = 0; k < face.mNumIndices; k++)
				{
					data.mesh.addIndex(face.mIndices[k]);
				}
			}

			objects->push_back(data);
		}

		meshCount = objects->size();

		ObjectData* data = new ObjectData[objects->size()];
		std::copy(objects->begin(), objects->end(), data);

		delete objects;

		return data;
	}

	static void load_model_mesh_assimp(const char* path, Mesh& mesh)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			return;
		}

		float targetSize = 1.f;
		normalizeModelSize(scene, targetSize);

		for (uint32_t i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh* aiMesh = scene->mMeshes[i];

			for (uint32_t j = 0; j < aiMesh->mNumVertices; j++)
			{
				glm::vec3 vertex(aiMesh->mVertices[j].x, aiMesh->mVertices[j].y, aiMesh->mVertices[j].z);
				glm::vec3 normal(aiMesh->mNormals[j].x, aiMesh->mNormals[j].y, aiMesh->mNormals[j].z);
				glm::vec2 uv(aiMesh->mTextureCoords[0][j].x, aiMesh->mTextureCoords[0][j].y);

				mesh.addVertex(Vertex(vertex, normal, uv));
			}

			for (uint32_t j = 0; j < aiMesh->mNumFaces; j++)
			{
				const aiFace& face = aiMesh->mFaces[j];
				for (uint32_t k = 0; k < face.mNumIndices; k++)
				{
					mesh.addIndex(face.mIndices[k]);
				}
			}
		}
	}
};