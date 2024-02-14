#include "GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


GameObject::GameObject(std::string name, Mesh mesh, Shader* shader, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->name = name;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(5.0f);
	this->isHidden = false;
	this->isDrawWireframe = false;

	this->material = new Material(glm::vec4(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
	this->m_shader = shader;
	this->mesh = mesh;

	genBuffers();
}

GameObject::GameObject(std::string name, std::string path, Shader* shader, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->name = name;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(5.0f);
	this->isHidden = false;
	this->isDrawWireframe = false;

	this->material = new Material(glm::vec4(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
	this->m_shader = shader;

	ObjectLoader::load_model_mesh_assimp(path.c_str(), &mesh);

	genBuffers();
}

GameObject::~GameObject()
{
	delete this->m_shader;
	delete this->material;
}

void GameObject::genBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->mesh.getVerticesPtr()->size(), this->mesh.getVerticesPtr()->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->mesh.getIndicesPtr()->size(), this->mesh.getIndicesPtr()->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObject::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->mesh.getVerticesPtr()->size(), this->mesh.getVerticesPtr()->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->mesh.getIndicesPtr()->size(), this->mesh.getIndicesPtr()->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObject::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, this->mesh.getIndicesPtr()->size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameObject::drawWireframe()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	this->draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GameObject::loadTexture(const char* path)
{
	loadTexture(path, &this->texture);
	this->textureType = GL_TEXTURE_2D;
}

void GameObject::loadTexture(const char* path, GLuint* texture)
{
	if (*texture == 0) glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, *texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture \"" << path << "\"!\n";
	}

	stbi_image_free(data);
}

void GameObject::loadCubeMap(std::vector<const char*> faces)
{
	loadCubeMap(faces, &this->texture);
	this->textureType = GL_TEXTURE_CUBE_MAP;
}

void GameObject::loadCubeMap(std::vector<const char*> faces, GLuint* texture)
{
	if (*texture == 0) glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, *texture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, nrChannels;
	unsigned char* data;
	for (size_t i = 0; i < faces.size(); i++)
	{
		data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
}


bool GameObject::getIsHidden() const { return this->isHidden; }
bool GameObject::getIsDrawWireframe() const { return this->isDrawWireframe; }
bool* GameObject::getIsDrawWireframePtr() { return &this->isDrawWireframe; }

glm::vec3 GameObject::getPosition() const { return this->position; }
glm::vec3* GameObject::getPositionPtr() { return &this->position; }
glm::vec3 GameObject::getRotation() const { return rotation; }
glm::vec3 GameObject::getScale() const { return scale; }
glm::vec3* GameObject::getScalePtr() { return &scale; }

glm::mat4 GameObject::getModelMatrix() const { return glm::scale(glm::translate(glm::mat4(1.0f), this->position), this->scale); }

GLuint GameObject::getTexture() const { return this->texture; }
GLenum GameObject::getTextureType() const { return this->textureType; }

Shader* GameObject::getShaderPtr() const { return m_shader; }

Material* GameObject::getMaterialPtr() const { return material; }

Mesh GameObject::getMesh() const { return this->mesh; }
Mesh* GameObject::getMeshPtr() { return &this->mesh; }

void GameObject::setPosition(glm::vec3 position) { this->position = position; }
void GameObject::setRotation(glm::vec3 rotation) { this->rotation = rotation; }
void GameObject::setScale(glm::vec3 scale) { this->scale = scale; }

void GameObject::setIsHidden(bool isHidden) { this->isHidden = isHidden; }

void GameObject::setShader(Shader* shader)
{
	if (this->m_shader != nullptr) delete this->m_shader;
	this->m_shader = shader;
}

std::string GameObject::getName() const { return name; }