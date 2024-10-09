#include "MaterialLib.hpp"

void MaterialLib::precompileMaterials()
{
	addMaterial(new Material("color", new Shader(".\\Vertex\\v_color.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_color.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("debug", new Shader(".\\Vertex\\v_debug.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_debug.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("render", new Shader(".\\Vertex\\v_render.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_render.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("skybox", new Shader(".\\Vertex\\v_skybox.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_skybox.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("depth", new Shader(".\\Vertex\\v_depth.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_depth.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("debugTriangle", new Shader(".\\Vertex\\v_debugTriangle.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_debugTriangle.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("debugWireframe", new Shader(".\\Vertex\\v_debugWireframe.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_debugWireframe.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("debugOverdraw", new Shader(".\\Vertex\\v_debugOverdraw.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_debugOverdraw.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("terrain", new Shader(".\\Vertex\\v_terrain.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_terrain.glsl", GL_FRAGMENT_SHADER)));
	addMaterial(new Material("visualizeNormals", new Shader(".\\Vertex\\v_visualizeNormals.glsl", GL_VERTEX_SHADER), new Shader(".\\Fragment\\f_visualizeNormals.glsl", GL_FRAGMENT_SHADER), new Shader(".\\Geometry\\g_visualizeNormals.glsl", GL_GEOMETRY_SHADER)));
}

void MaterialLib::addMaterial(Material* pMaterial)
{
	assert(pMaterial);

	g_materials.insert({ std::hash<std::string>()(pMaterial->getName()), pMaterial });
}

void MaterialLib::removeMaterial(Material* pMaterial)
{
	assert(pMaterial);

	g_materials.erase(std::hash<std::string>()(pMaterial->getName()));
}

void MaterialLib::deleteMaterial(Material* pMaterial)
{
	assert(pMaterial);

	removeMaterial(pMaterial);
	delete pMaterial;
}

Material* MaterialLib::get(std::string name)
{
	auto result = g_materials[std::hash<std::string>()(name)];
	assert(result);

	return result;
}