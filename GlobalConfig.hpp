#pragma once


class GlobalConfig
{

public:

	GlobalConfig()
	{
		g_settings = this;

		vsyncEnabled = true;
		drawMode = GL_FILL;
	}

	~GlobalConfig()
	{
		delete g_settings;
	}



	bool vsyncEnabled;

	GLenum drawMode;

};

inline GlobalConfig* g_settings;