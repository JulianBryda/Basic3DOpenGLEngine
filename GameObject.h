#pragma once
#include <iostream>
#include <format>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "GameObjectPhysics.h"
#include "GameObjectCollisions.h"
#include "ObjectLoader.hpp"
#include "Material.hpp"
#include "Mesh.hpp"

class GameObject : public GameObjectPhysics, public GameObjectCollisions
{

public:

	GameObject(std::string name, std::string path, std::string shaderName, ColliderType colliderType);
	GameObject(std::string name, Mesh mesh, std::string shaderName, ColliderType colliderType);

	~GameObject();

	virtual void genBuffers();
	virtual void updateBuffers();

	virtual void draw();

	void loadTexture(std::string textureName);
	static void loadTexture(std::string textureName, GLuint* texture);

	bool getIsHidden() const;

	glm::vec3 getPosition() const;
	glm::vec3* getPositionPtr();
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	glm::vec3* getScalePtr();

	glm::mat4 getModelMatrix() const;
	glm::mat4 getScaleMatrix() const;

	GLuint getTexture() const;

	Shader* getShaderPtr() const;

	Material* getMaterialPtr() const;

	Mesh getMesh() const;
	Mesh* getMeshPtr();

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setIsHidden(bool isHidden);
	void setShader(Shader* shader);

	std::string getName() const;


protected:

	GLuint vao;

	glm::vec3 position;

	Mesh mesh;

private:

	std::string name;

	GLuint vbo, ibo, texture;

	glm::vec3 rotation, scale;

	Shader* m_shader;

	Material* material;

	bool isHidden;

};