#pragma once

namespace Config
{

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

		GlobalConfig();
		~GlobalConfig();

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

	inline GlobalConfig* g_settings;
}