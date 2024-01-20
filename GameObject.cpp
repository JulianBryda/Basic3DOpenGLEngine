#include "GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GameObject::GameObject(std::string name, std::vector<glm::vec3> vertices, std::vector<GLuint> indices, std::string shader_name, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->name = name;
	this->vertices = vertices;
	this->indices = indices;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(5.0f);

	this->m_shader = new Shader(std::format("v_{}.glsl", shader_name).c_str(), std::format("f_{}.glsl", shader_name).c_str());

	genBuffers();
}

GameObject::GameObject(std::string name, std::string path, std::string shader_name, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->name = name;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(5.0f);

	this->m_shader = new Shader(std::format("v_{}.glsl", shader_name).c_str(), std::format("f_{}.glsl", shader_name).c_str());

	ObjectLoader::LoadObjFile(path, this->vertices, this->indices);

	genBuffers();
}

void GameObject::genBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObject::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObject::draw()
{
	// uncomment this to draw wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


void GameObject::loadTexture(std::string textureName)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(("Assets\\" + textureName).c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture \"" << textureName << "\"!\n";
	}

	stbi_image_free(data);
}

void GameObject::loadTexture(std::string textureName, GLuint* texture)
{
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(("Assets\\" + textureName).c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture \"" << textureName << "\"!\n";
	}

	stbi_image_free(data);
}

glm::vec3 GameObject::getPosition() const { return position; }
glm::vec3* GameObject::getPositionPtr() { return &position; }
glm::vec3 GameObject::getRotation() const { return rotation; }
glm::vec3 GameObject::getScale() const { return scale; }
glm::vec3* GameObject::getScalePtr() { return &scale; }

glm::mat4 GameObject::getModelMatrix() const { return glm::translate(glm::mat4(1.0f), position); }
glm::mat4 GameObject::getScaleMatrix() const { return glm::scale(glm::mat4(1.0f), scale); }

GLuint GameObject::getTexture() const { return texture; }

Shader* GameObject::getShaderPtr() const { return m_shader; }

void GameObject::setPosition(glm::vec3 position) { this->position = position; }
void GameObject::setRotation(glm::vec3 rotation) { this->rotation = rotation; }
void GameObject::setScale(glm::vec3 scale) { this->scale = scale; }

std::vector<glm::vec3> GameObject::getVertices() const { return vertices; }
std::vector<GLuint> GameObject::getIndices() const { return indices; }
std::string GameObject::getName() const { return name; }