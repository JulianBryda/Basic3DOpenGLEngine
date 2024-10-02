#pragma once
#include "GameObject.h"

class Landscape : public GameObject
{
public:

	Landscape(std::string name) : GameObject(name, Mesh(), ShaderLib::getColorShaderPtr(), BoundingBox)
	{

	}


private:


};