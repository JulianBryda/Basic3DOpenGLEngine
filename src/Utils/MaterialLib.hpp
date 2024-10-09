#pragma once
#include <unordered_map>

#include "../Renderer/Graphics/Material.hpp"

namespace MaterialLib
{

	void precompileMaterials();

	void addMaterial(Material* pMaterial);
	void removeMaterial(Material* pMaterial);
	void deleteMaterial(Material* pMaterial);
	Material* get(std::string name);


	inline std::unordered_map<size_t, Material*> g_materials;

}