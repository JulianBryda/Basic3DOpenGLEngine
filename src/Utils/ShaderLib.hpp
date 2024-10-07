#pragma once
#include <unordered_map>

#include "../Renderer/Graphics/ShaderOLD.hpp"

namespace ShaderLib
{

	void precompileShaders();

	void addShader(ShaderOLD* shader);
	void removeShader(ShaderOLD* shader);
	void deleteShader(ShaderOLD* shader);
	ShaderOLD* get(std::string name);


	inline std::unordered_map<size_t, ShaderOLD*> g_shaders;

}