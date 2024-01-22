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

class GameObject : public GameObjectPhysics, public GameObjectCollisions
{

public:

	GameObject(std::string name, std::string path, std::string shaderName, ColliderType colliderType);
	GameObject(std::string name, std::vector<glm::vec3> vertices, std::vector<GLuint> indices, std::string shaderName, ColliderType colliderType);

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

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setIsHidden(bool isHidden);

	std::vector<glm::vec3> getVertices() const;
	std::vector<GLuint> getIndices() const;
	std::string getName() const;


protected:

	GLuint vao;

	std::vector<GLuint> indices;

	glm::vec3 position;

private:

	std::string name;

	GLuint vbo, ebo, texture;

	std::vector<glm::vec3> vertices;

	glm::vec3 rotation, scale;

	Shader* m_shader;

	Material* material;

	bool isHidden;

};