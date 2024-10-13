#pragma once
#include <iostream>
#include <format>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObjectPhysics.h"
#include "GameObjectCollisions.h"

#include "../../Collider.hpp"

#include "../../Utils/ObjectLoader.hpp"

#include "../../Renderer/Graphics/Material.hpp"

class GameObject : public GameObjectPhysics, public GameObjectCollisions
{

public:

	GameObject(std::string name, std::string path, Material* pMaterial, ColliderType colliderType);
	GameObject(std::string name, Mesh mesh, Material* pMaterial, ColliderType colliderType);

	GameObject(const GameObject& other);

	~GameObject();

	virtual void genBuffers();
	virtual void updateBuffers();

	virtual void initParameterAtlas()
	{
		parameterAtlas.insert({ std::hash<std::string>()("modelMatrix"), Material::Parameter<glm::mat4>("modelMatrix", 1, &modelMatrix) });
	}

	void draw();

	// getter
	std::string getName() const;

	bool getHidden() const;
	bool getIsOutline() const;

	glm::vec3 getPosition() const;
	glm::vec3* getPositionPtr();
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	glm::vec3* getScalePtr();

	virtual glm::mat4 getModelMatrix();

	Material* getMaterialPtr();

	Mesh& getMesh();
	Mesh* getMeshPtr();

	std::unordered_map<size_t, Material::UniformType>& getParameterAtlas() { return parameterAtlas; }

	// setter
	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setIsHidden(bool isHidden);
	void setIsOutline(bool isOutline);
	void setName(std::string name);
	void setMaterial(Material* pMaterial);

	// imgui pointer functions
	float** getRotationPtr();

protected:

	GLuint vao;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	Mesh mesh;

	glm::mat4 modelMatrix;

	std::unordered_map<size_t, Material::UniformType> parameterAtlas;

private:

	std::string m_name;

	GLuint m_vbo, m_ebo;

	Material* m_pMaterial;

	bool m_hidden, m_outline;
};