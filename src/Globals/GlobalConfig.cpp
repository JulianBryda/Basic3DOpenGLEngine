#include "GlobalConfig.hpp"

namespace Config
{
	GlobalConfig::GlobalConfig()
	{
		g_settings = this;

		vsyncEnabled = true;
		debugMode = None;
		renderMode = Debug;
		screenWidth = 1280;
		screenHeight = 720;

	}

	GlobalConfig::~GlobalConfig()
	{
		g_settings = nullptr;
	}
}