#pragma once
#include "Shader.hpp"

class ShaderLib
{

public:

	static void LoadShader()
	{
		colorShader = new Shader("color");
		debugShader = new Shader("debug");
		renderShader = new Shader("render");
	}

	static Shader* getColorShader() { return colorShader; }
	static Shader* getDebugShader() { return debugShader; }
	static Shader* getRenderShader() { return renderShader; }

private:

	static Shader* colorShader, * debugShader, * renderShader;
};