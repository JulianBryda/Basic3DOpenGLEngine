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

class GameObject : public GameObjectPhysics, public GameObjectCollisions
{

public:

	GameObject(std::string name, std::vector<glm::vec3> vertices, std::vector<GLuint> indices, std::string shader_name);

	virtual void genBuffers();
	virtual void updateBuffers();

	virtual void draw();

	void loadTexture(std::string textureName);
	static void loadTexture(std::string textureName, GLuint* texture);

	glm::vec3 getPosition() const;
	glm::vec3* getPositionPtr();
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	glm::vec3* getScalePtr();

	glm::mat4 getModelMatrix() const;
	glm::mat4 getScaleMatrix() const;

	GLuint getTexture() const;

	Shader* getShaderPtr() const;

	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	std::vector<glm::vec3> getVertices() const;
	std::vector<GLuint> getIndices() const;
	std::string getName() const;


protected:

	GLuint vao;
	std::vector<GLuint> indices;

private:

	std::string name;

	GLuint vbo, ebo, texture;

	std::vector<glm::vec3> vertices;

	glm::vec3 position, rotation, scale;

	Shader* m_shader;

};