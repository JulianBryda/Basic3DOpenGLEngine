#pragma once
#include "../Renderer/Graphics/Shader.hpp"

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
		debugTriangleShader = new Shader(".\\v_debugTriangle.glsl", ".\\f_debugTriangle.glsl");
		debugWireframeShader = new Shader(".\\v_debugWireframe.glsl", ".\\f_debugWireframe.glsl");
		debugOverdrawShader = new Shader(".\\v_debugOverdraw.glsl", ".\\f_debugOverdraw.glsl");
	}

	static Shader* getColorShaderPtr() { return colorShader; }
	static Shader* getDebugShaderPtr() { return debugShader; }
	static Shader* getRenderShaderPtr() { return renderShader; }
	static Shader* getSkyboxShaderPtr() { return skyboxShader; }
	static Shader* getDepthShaderPtr() { return depthShader; }
	static Shader* getDebugTriangleShaderPtr() { return debugTriangleShader; }
	static Shader* getDebugWireframeShaderPtr() { return debugWireframeShader; }
	static Shader* getDebugOverdrawShaderPtr() { return debugOverdrawShader; }

private:

	static Shader* colorShader, * debugShader, * renderShader, * skyboxShader, * depthShader, * debugTriangleShader, * debugWireframeShader, * debugOverdrawShader;
};