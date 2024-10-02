#pragma once
#include <filesystem>
#include <map>

#include "stb_image.h"

struct AssetStruct
{
	std::string name;
	std::string path;
	GLuint image;
};

class AssetManager
{

public:

	AssetManager()
	{
		// check if folder exists
		if (!std::filesystem::exists(".\\Assets"))
		{
			std::filesystem::create_directory(".\\Assets");
		}

		m_icons.insert(std::pair(".default", loadTexture(".\\Assets\\Icons\\default-icon.png")));
		m_icons.insert(std::pair(".obj", loadTexture(".\\Assets\\Icons\\obj-icon.png")));
		m_icons.insert(std::pair(".folder", loadTexture(".\\Assets\\Icons\\folder-icon.png")));
	}

	static AssetManager& getInstance()
	{
		static AssetManager instance;
		return instance;
	}

	void handleFileDrop(const char** paths, int count)
	{
		for (int i = 0; i < count; i++)
		{
			std::string path = paths[i];

			int nameIndex = static_cast<int>(path.find_last_of('\\')) + 1;
			int nameSuffix = static_cast<int>(path.find_last_of("."));
			int nameLength = static_cast<int>(path.length()) - nameIndex - (static_cast<int>(path.length()) - nameSuffix);

			m_files.push_back({ path.substr(nameIndex, nameLength), path, m_icons.at(path.substr(nameSuffix + 1)) });
		}
	}

	// getter
	std::vector<AssetStruct>& getFiles()
	{
		return m_files;
	}

	GLuint getIcon(std::string name)
	{
		if (m_icons.find(name) != m_icons.end())
		{
			return m_icons.at(name);
		}
		else
		{
			return m_icons.at(".default");
		}
	}

private:

	GLuint loadTexture(const char* path)
	{
		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture \"" << path << "\"!\n";
		}

		stbi_image_free(data);

		return texture;
	}

	std::vector<AssetStruct> m_files;
	std::map<std::string, GLuint> m_icons;

	AssetManager(const AssetManager&) = delete;
	AssetManager& operator=(const AssetManager&) = delete;
};