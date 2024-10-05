#pragma once
#include <unordered_map>

#include "../Renderer/Graphics/Shader.hpp"

namespace ShaderLib
{

	void precompileShaders();

	void addShader(Shader* shader);
	void removeShader(Shader* shader);
	void deleteShader(Shader* shader);
	Shader* get(std::string name);


	inline std::unordered_map<size_t, Shader*> g_shaders;

}