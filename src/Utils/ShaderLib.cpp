#include "ShaderLib.hpp"

void ShaderLib::precompileShaders()
{
	addShader(new Shader(".\\Vertex\\v_color.glsl", ".\\Fragment\\f_color.glsl"));
	addShader(new Shader(".\\Vertex\\v_debug.glsl", ".\\Fragment\\f_debug.glsl"));
	addShader(new Shader(".\\Vertex\\v_render.glsl", ".\\Fragment\\f_render.glsl"));
	addShader(new Shader(".\\Vertex\\v_skybox.glsl", ".\\Fragment\\f_skybox.glsl"));
	addShader(new Shader(".\\Vertex\\v_depth.glsl", ".\\Fragment\\f_depth.glsl"));
	addShader(new Shader(".\\Vertex\\v_debugTriangle.glsl", ".\\Fragment\\f_debugTriangle.glsl"));
	addShader(new Shader(".\\Vertex\\v_debugWireframe.glsl", ".\\Fragment\\f_debugWireframe.glsl"));
	addShader(new Shader(".\\Vertex\\v_debugOverdraw.glsl", ".\\Fragment\\f_debugOverdraw.glsl"));
	addShader(new Shader(".\\Vertex\\v_terrain.glsl", ".\\Fragment\\f_terrain.glsl"));
	addShader(new Shader(".\\Vertex\\v_visualizeNormals.glsl", ".\\Fragment\\f_visualizeNormals.glsl", ".\\Geometry\\g_visualizeNormals.glsl"));
}

void ShaderLib::addShader(Shader* shader)
{
	assert(shader);

	g_shaders.insert({ std::hash<std::string>()(shader->getName()), shader });
}

void ShaderLib::removeShader(Shader* shader)
{
	assert(shader);

	g_shaders.erase(std::hash<std::string>()(shader->getName()));
}

void ShaderLib::deleteShader(Shader* shader)
{
	assert(shader);

	removeShader(shader);
	delete shader;
}

Shader* ShaderLib::get(std::string name)
{
	auto result = g_shaders[std::hash<std::string>()(name)];
	assert(result);

	return result;
}