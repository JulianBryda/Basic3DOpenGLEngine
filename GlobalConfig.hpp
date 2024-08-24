#pragma once

namespace Config
{
	// fucking c++ undeclared identifier :(
	class GlobalConfig;
	inline GlobalConfig* g_settings = nullptr;

	enum DebugMode
	{
		Triangles,
		Wireframe,
		Overdraw,
		None
	};

	enum RenderMode
	{
		Debug,
		Render
	};

	class GlobalConfig
	{

	public:

		GlobalConfig()
		{
			g_settings = this;

			vsyncEnabled = true;
			debugMode = None;
			renderMode = Debug;
			screenWidth = 1280;
			screenHeight = 720;
			
		}

		~GlobalConfig()
		{
			g_settings = nullptr;
		}

		void updateScreenSize(int newScreenWidth, int newScreenHeight)
		{
			screenWidth = newScreenWidth;
			screenHeight = newScreenHeight;
		}


		bool vsyncEnabled;
		DebugMode debugMode;
		RenderMode renderMode;

		int screenWidth, screenHeight;
	};
}