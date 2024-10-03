#pragma once
#include "Scene/Objects/GameObject.h"

#include "ThirdParty/PerlinNoise.hpp"

class Landscape : public GameObject
{
public:

	Landscape(std::string name) : GameObject(name, Mesh(), ShaderLib::getColorShaderPtr(), BoundingBox)
	{
		resolutionX = 128;
		resolutionY = 128;
		seed = rand();

		frequency = 0.005f;
		octaves = 8;

		scale = glm::vec3(50.f);

		update();
	}

	~Landscape()
	{
		delete[] heightmap;
	}

	void update()
	{
		delete[] heightmap;

		heightmap = new float[resolutionX * resolutionY];
		fillHeightmap(heightmap, seed, resolutionX, resolutionY, octaves);

		mesh = generateMesh(heightmap, resolutionX, resolutionY);
		mesh.calculateNormals();

		updateBuffers();
	}

	void fillHeightmap(float* heightmap, const siv::PerlinNoise::seed_type seed, int resolutionX, int resolutionY, int octaves) const
	{
		assert(heightmap != nullptr);
		const siv::PerlinNoise perlin{ seed };

		for (int x = 0; x < resolutionX; x++)
		{
			for (int y = 0; y < resolutionY; y++)
			{
				const float noise = perlin.octave2D_01(x * frequency, y * frequency, octaves);
				heightmap[x * resolutionY + y] = noise;
			}
		}
	}

	Mesh generateMesh(float* heightmap, int resolutionX, int resolutionY) const
	{
		assert(heightmap != nullptr);
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		for (int x = 0; x < resolutionX; x++)
		{
			for (int y = 0; y < resolutionY; y++)
			{
				float xCord = x / static_cast<float>(resolutionX) * 2 - .5f;
				float yCord = heightmap[x * resolutionY + y] * 2 - 1.f;
				float zCord = y / static_cast<float>(resolutionY) * 2 - .5f;

				Vertex vertex = Vertex(glm::vec3(xCord, yCord, zCord), glm::vec3(0.f), glm::vec2(xCord, zCord));
				vertices.push_back(vertex);
			}
		}

		for (int x = 0; x < resolutionX / 2; x++)
		{
			for (int y = 0; y < resolutionY / 2; y++)
			{
				indices.push_back(x * resolutionY + (y + 1));
				indices.push_back((x + 1) * resolutionY + y);
				indices.push_back(x * resolutionY + y);

				indices.push_back((x + 1) * resolutionY + (y + 1));
				indices.push_back((x + 1) * resolutionY + y);
				indices.push_back(x * resolutionY + (y + 1));
			}
		}

		return Mesh(vertices, indices);
	}

	// getter
	int getResolutionX() const { return resolutionX; }
	int* getResolutionXPtr() { return &resolutionX; }
	int getResolutionY() const { return resolutionY; }
	int* getResolutionYPtr() { return &resolutionY; }
	int getSeed() const { return seed; }
	int* getSeedPtr() { return &seed; }

	float getFrequency() const { return frequency; }
	float* getFrequencyPtr() { return &frequency; }

	int getOctaves() const { return octaves; }
	int* getOctavesPtr() { return &octaves; }

	// setter
	void setResolution(int x, int y)
	{
		// resolution must be divisble by 2!
		assert(x % 2 == 0);
		assert(y % 2 == 0);

		resolutionX = x;
		resolutionY = y;
	}




private:

	int resolutionX, resolutionY, seed, octaves;
	float* heightmap;
	float frequency;

};