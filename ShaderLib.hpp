#pragma once
#include "Shader.hpp"

class ShaderLib
{

public:

	static void LoadShader()
	{
		colorShader = new Shader("color");
	}

	static Shader* getColorShader() { return colorShader; }

private:

	static Shader* colorShader;
};