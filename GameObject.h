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

	GameObject(std::string name, std::string path, Shader* pShader, ColliderType colliderType);
	GameObject(std::string name, Mesh mesh, Shader* pShader, ColliderType colliderType);

	~GameObject();

	virtual void genBuffers();
	virtual void updateBuffers();

	virtual void draw();
	virtual void drawWireframe();

	void loadTexture(const char* path);
	static void loadTexture(const char* path, GLuint* texture);

	void loadCubeMap(std::vector<const char*> faces);
	static void loadCubeMap(std::vector<const char*> faces, GLuint* texture);

	bool getHidden() const;
	bool getDrawWireframe() const;
	bool* getDrawWireframePtr();
	bool getIsOutline() const;

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
	void setIsOutline(bool isOutline);

	std::string getName() const;

protected:

	GLuint vao;

	glm::vec3 position;

private:

	std::string m_name;

	GLuint m_vbo, m_ebo, m_texture;
	GLenum m_textureType;

	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	Shader* m_pShader;

	Material* m_pMaterial;

	Mesh m_mesh;

	bool m_hidden, m_drawWireframe, m_outline;

};