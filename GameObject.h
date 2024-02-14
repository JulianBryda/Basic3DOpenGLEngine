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
#include "Vertex.hpp"

class GameObject : public GameObjectPhysics, public GameObjectCollisions
{

public:

	GameObject(std::string name, std::string path, Shader* shader, ColliderType colliderType);
	GameObject(std::string name, Mesh mesh, Shader* shader, ColliderType colliderType);

	~GameObject();

	virtual void genBuffers();
	virtual void updateBuffers();

	virtual void draw();
	virtual void drawWireframe();

	void loadTexture(const char* path);
	static void loadTexture(const char* path, GLuint* texture);

	void loadCubeMap(std::vector<const char*> faces);
	static void loadCubeMap(std::vector<const char*> faces, GLuint* texture);

	bool getIsHidden() const;
	bool getIsDrawWireframe() const;
	bool* getIsDrawWireframePtr();

	glm::vec3 getPosition() const;
	glm::vec3* getPositionPtr();
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	glm::vec3* getScalePtr();

	virtual glm::mat4 getModelMatrix() const;

	GLuint getTexture() const;
	GLenum getTextureType() const;

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

private:

	std::string name;

	GLuint vbo, ebo, texture;
	GLenum textureType;

	glm::vec3 rotation, scale;

	Shader* m_shader;

	Material* material;

	Mesh mesh;

	bool isHidden, isDrawWireframe;

};