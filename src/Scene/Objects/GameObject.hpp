#pragma once
#include <iostream>
#include <format>
#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GameObjectPhysics.h"
#include "GameObjectCollisions.h"

#include "../../Collider.hpp"

#include "../../Utils/ObjectLoader.hpp"

class GameObject : public GameObjectPhysics, public GameObjectCollisions
{

public:

	GameObject(std::string name, std::string path, ShaderOLD* pShader, ColliderType colliderType);
	GameObject(std::string name, Mesh mesh, ShaderOLD* pShader, ColliderType colliderType);

	GameObject(const GameObject& other);

	~GameObject();

	virtual void genBuffers();
	virtual void updateBuffers();

	virtual void initParameterAtlas()
	{
		parameterAtlas.insert({ std::hash<std::string>()("modelMatrix"), ShaderOLD::Parameter<glm::mat4>("modelMatrix", 1, &modelMatrix) });

		parameterAtlas.insert({ std::hash<std::string>()("material.ambient"), ShaderOLD::Parameter<glm::vec3>("material.ambient", 1, m_material.getAmbientPtr()) });
		parameterAtlas.insert({ std::hash<std::string>()("material.diffuse"), ShaderOLD::Parameter<glm::vec3>("material.diffuse", 1, m_material.getDiffusePtr()) });
		parameterAtlas.insert({ std::hash<std::string>()("material.specular"), ShaderOLD::Parameter<glm::vec3>("material.specular", 1, m_material.getSpecularPtr()) });
		parameterAtlas.insert({ std::hash<std::string>()("material.shininess"), ShaderOLD::Parameter<float>("material.shininess", 1, m_material.getShininessPtr()) });
	}

	void draw();

	void loadTexture(const char* path);
	static void loadTexture(const char* path, GLuint& texture);

	void loadCubeMap(std::vector<const char*> faces);
	static void loadCubeMap(std::vector<const char*> faces, GLuint* texture);

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

	GLuint getTexture() const;
	GLenum getTextureType() const;

	ShaderOLD* getShaderPtr() const;

	Material* getMaterialPtr();

	Mesh& getMesh();
	Mesh* getMeshPtr();

	std::unordered_map<size_t, ShaderOLD::UniformType>& getParameterAtlas() { return parameterAtlas; }

	// setter
	void setPosition(glm::vec3 position);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);
	void setIsHidden(bool isHidden);
	void setShader(ShaderOLD* shader);
	void setIsOutline(bool isOutline);
	void setName(std::string name);

	// imgui pointer functions
	float** getRotationPtr();

protected:

	GLuint vao;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	Mesh mesh;

	glm::mat4 modelMatrix;

	std::unordered_map<size_t, ShaderOLD::UniformType> parameterAtlas;

private:

	std::string m_name;

	GLuint m_vbo, m_ebo, m_texture;
	GLenum m_textureType;

	ShaderOLD* m_pShader;

	Material m_material;

	bool m_hidden, m_outline;
};