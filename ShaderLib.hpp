#pragma once
#include "Shader.hpp"

class ShaderLib
{

public:

	static void LoadShader()
	{
		colorShader = new Shader(".\\v_color.glsl", ".\\f_color.glsl");
		debugShader = new Shader(".\\v_debug.glsl", ".\\f_debug.glsl");
		renderShader = new Shader(".\\v_render.glsl", ".\\f_render.glsl", ".\\LibraryShader\\structs.glsl");
		skyboxShader = new Shader(".\\v_skybox.glsl", ".\\f_skybox.glsl");
		depthShader = new Shader(".\\v_depth.glsl", ".\\f_depth.glsl");
	}

	static Shader* getColorShaderPtr() { return colorShader; }
	static Shader* getDebugShaderPtr() { return debugShader; }
	static Shader* getRenderShaderPtr() { return renderShader; }
	static Shader* getSkyboxShaderPtr() { return skyboxShader; }
	static Shader* getDepthShaderPtr() { return depthShader; }

private:

	static Shader* colorShader, * debugShader, * renderShader, * skyboxShader, * depthShader;
};