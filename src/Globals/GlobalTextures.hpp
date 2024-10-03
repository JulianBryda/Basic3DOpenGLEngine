#pragma once
#include <glad/glad.h>

namespace Textures
{
	class GlobalTextures;
	inline GlobalTextures* g_textures = nullptr;

	class GlobalTextures
	{

	public:

		GlobalTextures(int screenWidth, int screenHeight)
		{
			g_textures = this;
			overdrawTexture = 0;

			glGenTextures(1, &overdrawTexture);
			glBindTexture(GL_TEXTURE_2D, overdrawTexture);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, screenWidth, screenHeight);

			glBindImageTexture(0, overdrawTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		~GlobalTextures()
		{
			g_textures = nullptr;
		}

		void updateTextures(int newScreenWidth, int newScreenHeight)
		{
			if (overdrawTexture != 0)
			{
				glDeleteTextures(1, &overdrawTexture);
			}

			glGenTextures(1, &overdrawTexture);
			glBindTexture(GL_TEXTURE_2D, overdrawTexture);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, newScreenWidth, newScreenHeight);

			glBindImageTexture(0, overdrawTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		GLuint overdrawTexture;
	};
}