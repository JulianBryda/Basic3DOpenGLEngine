#pragma once
#include <iostream>
#include <random>
#include <thread>
#include <future>

#include "Scene/Objects/GameObject.hpp"

#include "ThirdParty/PerlinNoise.hpp"

class Landscape : public GameObject
{
public:

	Landscape(std::string name) : GameObject(name, Mesh(), MaterialLib::get("terrain"), AABB)
	{
		resolutionX = 128;
		resolutionY = 128;
		seed = rand();

		frequency = 0.005f;
		height = 1.f;
		octaves = 8;

		slopeStart = 0.85f;
		slopeEnd = 0.95f;

		flatColor = glm::vec3(121.f / 255.f, 157.f / 255.f, 65.f / 255.f);
		slopeColor = glm::vec3(115.f / 255.f, 97.f / 255.f, 54.f / 255.f);

		scale = glm::vec3(100.f);

		update();
	}

	~Landscape()
	{
		delete[] heightmap;
	}

	void update()
	{
		if (heightmap != nullptr)
			delete[] heightmap;

		heightmap = new float[resolutionX * resolutionY];
		fillHeightmap(heightmap, seed, resolutionX, resolutionY, frequency, octaves);

		mesh = generateMesh(heightmap, resolutionX, resolutionY);
		mesh.calculateNormals();

		updateBuffers();
	}

	void smoothenHeightmap(float factor)
	{
		assert(factor >= 0 && factor <= 1);

		for (int x = 1; x < resolutionX - 1; x++)
		{
			for (int y = 1; y < resolutionY - 1; y++)
			{
				float value = heightmap[x * resolutionY + y];
				float valueN = heightmap[x * resolutionY + (y - 1)];
				float valueO = heightmap[(x + 1) * resolutionY + y];
				float valueS = heightmap[x * resolutionY + (y + 1)];
				float valueW = heightmap[(x - 1) * resolutionY + y];

				float average = (value + valueN + valueO + valueS + valueW) / 5.f;
				float diff = average - heightmap[x * resolutionY + y];

				heightmap[x * resolutionY + y] += diff * factor;
			}
		}

		mesh = generateMesh(heightmap, resolutionX, resolutionY);
		mesh.calculateNormals();

		updateBuffers();
	}

	void simulateErosion(const int iterations, const float depositionRate, const float evapRate, const float friction)
	{
		system("cls");
		std::cout << "Hydraulic erosion simulation at 0%\n";

		std::vector<std::future<void>> futures;
		std::vector<int*> progress;

		const int processor_count = std::thread::hardware_concurrency();
		const int iterationsPerThread = iterations / processor_count;

		if (iterationsPerThread > 0)
		{
			for (int i = 0; i < processor_count; i++)
			{
				int prog = 0;
				futures.emplace_back(std::async(std::launch::async, [&] {
					erode(iterationsPerThread, depositionRate, evapRate, friction, prog);
					}));
				progress.emplace_back(&prog);
			}
		}
		else
		{
			erode(iterationsPerThread, depositionRate, evapRate, friction);
		}

		bool finished = false;

		while (!finished)
		{
			int totalProgress = 0;

			for (int i = 0; i < futures.size(); i++)
			{
				auto status = futures[i].wait_for(std::chrono::milliseconds(0));
				if (status == std::future_status::ready)
				{
					finished = true;
				}
				else
				{
					finished = false;
				}

				totalProgress += *progress[i];
			}

			totalProgress /= futures.size();

			setConsolePosition(32, 0);
			std::cout << totalProgress + 1 << "%\n";

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		mesh = generateMesh(heightmap, resolutionX, resolutionY);
		mesh.calculateNormals();

		updateBuffers();
	}

	void fillHeightmap(float* heightmap, const siv::PerlinNoise::seed_type seed, int resolutionX, int resolutionY, float frequency, int octaves) const
	{
		assert(heightmap != nullptr);
		const siv::PerlinNoise perlin{ seed };

		for (int x = 0; x < resolutionX; x++)
		{
			for (int y = 0; y < resolutionY; y++)
			{
				const float xCoord = x / (resolutionX / 128.f);
				const float yCoord = y / (resolutionY / 128.f);

				const float noise = perlin.octave2D_01(xCoord * frequency, yCoord * frequency, octaves);
				heightmap[x * resolutionY + y] = noise * height;
			}
		}
	}

	Mesh generateMesh(float* heightmap, int resolutionX, int resolutionY) const
	{
		assert(heightmap != nullptr);
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		float minHeight = std::numeric_limits<float>::max();
		float maxHeight = std::numeric_limits<float>::min();
		for (int i = 0; i < resolutionX * resolutionY; i++)
		{
			if (heightmap[i] < minHeight)
				minHeight = heightmap[i];
			if (heightmap[i] > maxHeight)
				maxHeight = heightmap[i];
		}

		for (int x = 0; x < resolutionX; x++)
		{
			for (int y = 0; y < resolutionY; y++)
			{
				float xCord = x / static_cast<float>(resolutionX) * 2 - .5f;
				float yCord = heightmap[x * resolutionY + y] - (maxHeight + minHeight) / 2.f;
				float zCord = y / static_cast<float>(resolutionY) * 2 - .5f;

				Vertex vertex = Vertex(glm::vec3(xCord, yCord, zCord), glm::vec3(0.f), glm::vec2(xCord, zCord), glm::vec3(), glm::vec3()); // TODO CALC TANGENT AND BITANGENT
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


	void setConsolePosition(int x, int y);

	// getter
	int getResolutionX() const { return resolutionX; }
	int* getResolutionXPtr() { return &resolutionX; }
	int getResolutionY() const { return resolutionY; }
	int* getResolutionYPtr() { return &resolutionY; }
	int getSeed() const { return seed; }
	int* getSeedPtr() { return &seed; }

	float getFrequency() const { return frequency; }
	float* getFrequencyPtr() { return &frequency; }
	float getHeight() const { return height; }
	float* getHeightPtr() { return &height; }

	float getSlopeStart() const { return slopeStart; }
	float* getSlopeStartPtr() { return &slopeStart; }
	float getSlopeEnd() const { return slopeEnd; }
	float* getSlopeEndPtr() { return &slopeEnd; }

	glm::vec3 getFlatColor() const { return flatColor; }
	glm::vec3* getFlatColorPtr() { return &flatColor; }
	glm::vec3 getSlopeColor() const { return slopeColor; }
	glm::vec3* getSlopeColorPtr() { return &slopeColor; }

	int getOctaves() const { return octaves; }
	int* getOctavesPtr() { return &octaves; }

	float* getHeightmap() const { return heightmap; }

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

	struct Particle
	{
		Particle(glm::vec2 _pos) { pos = _pos; }

		glm::vec2 pos;
		glm::vec2 speed = glm::vec2(0.f);

		float volume = 1.f;
		float sediment = 0.f;
	};

	void erode(const int iterations, const float depositionRate, const float evapRate, const float friction)
	{
		int progress = 0;
		erode(iterations, depositionRate, evapRate, friction, progress);
	}

	void erode(const int iterations, const float depositionRate, const float evapRate, const float friction, int& progress)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrX(1, resolutionX - 2);
		std::uniform_int_distribution<> distrY(1, resolutionY - 2);

		for (int i = 0; i < iterations; i++)
		{
			int x = distrX(gen);
			int y = distrY(gen);

			Particle particle(glm::vec2(x, y));

			while (particle.volume > 0.01f)
			{
				glm::ivec2 ipos = particle.pos;
				if (!glm::all(glm::greaterThan(ipos, glm::ivec2(0))) || !glm::all(glm::lessThan(ipos, glm::ivec2(resolutionX - 1, resolutionY - 1)))) break;

				glm::vec3 n = surfaceNormal(ipos.x, ipos.y);

				particle.speed += glm::vec2(n.x, n.z) / (particle.volume * 1.f);
				particle.pos += particle.speed;
				particle.speed *= 1.f - friction;

				if (!glm::all(glm::greaterThan(particle.pos, glm::vec2(0))) || !glm::all(glm::lessThan(particle.pos, glm::vec2(resolutionX, resolutionY)))) break;

				float maxsediment = particle.volume * glm::length(particle.speed) * (heightmap[ipos.x * resolutionY + ipos.y] - heightmap[static_cast<int>(particle.pos.x) * resolutionY + static_cast<int>(particle.pos.y)]);
				if (maxsediment < 0.0) maxsediment = 0.0;
				float sdiff = maxsediment - particle.sediment;

				particle.sediment += depositionRate * sdiff;
				heightmap[ipos.x * resolutionY + ipos.y] -= particle.volume * depositionRate * sdiff;

				particle.volume *= 1.0f - evapRate;
			}

			progress = static_cast<float>(i) / iterations * 100.f;
		}
	}

	glm::vec3 surfaceNormal(int x, int y)
	{
		float scale = 60.f;

		glm::vec3 n = glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (heightmap[x * resolutionY + y] - heightmap[(x + 1) * resolutionY + y]), 1.0, 0.0));  //Positive X
		n += glm::vec3(0.15) * glm::normalize(glm::vec3(scale * (heightmap[(x - 1) * resolutionY + y] - heightmap[x * resolutionY + y]), 1.0, 0.0));  //Negative X
		n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[x * resolutionY + y] - heightmap[x * resolutionY + (y + 1)])));    //Positive Y
		n += glm::vec3(0.15) * glm::normalize(glm::vec3(0.0, 1.0, scale * (heightmap[x * resolutionY + (y - 1)] - heightmap[x * resolutionY + y])));  //Negative Y

		//Diagonals! (This removes the last spatial artifacts)
		n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[x * resolutionY + y] - heightmap[(x + 1) * resolutionY + (y + 1)]) / sqrt(2.f), sqrt(2.f), scale * (heightmap[x * resolutionY + y] - heightmap[(x + 1) * resolutionY + (y + 1)]) / sqrt(2.f)));    //Positive Y
		n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[x * resolutionY + y] - heightmap[(x + 1) * resolutionY + (y - 1)]) / sqrt(2.f), sqrt(2.f), scale * (heightmap[x * resolutionY + y] - heightmap[(x + 1) * resolutionY + (y - 1)]) / sqrt(2.f)));    //Positive Y
		n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[x * resolutionY + y] - heightmap[(x - 1) * resolutionY + (y + 1)]) / sqrt(2.f), sqrt(2.f), scale * (heightmap[x * resolutionY + y] - heightmap[(x - 1) * resolutionY + (y + 1)]) / sqrt(2.f)));    //Positive Y
		n += glm::vec3(0.1) * glm::normalize(glm::vec3(scale * (heightmap[x * resolutionY + y] - heightmap[(x - 1) * resolutionY + (y - 1)]) / sqrt(2.f), sqrt(2.f), scale * (heightmap[x * resolutionY + y] - heightmap[(x - 1) * resolutionY + (y - 1)]) / sqrt(2.f)));    //Positive Y

		return n;
	}


	int resolutionX, resolutionY, seed, octaves;
	float* heightmap;
	float frequency, height;

	float slopeStart, slopeEnd;
	glm::vec3 flatColor, slopeColor;

};