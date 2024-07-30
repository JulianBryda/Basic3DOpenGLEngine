#include "GameObject.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GameObject::GameObject(std::string name, Mesh mesh, Shader* shader, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->m_name = name;
	this->position = glm::vec3(0.0f);
	this->m_rotation = glm::vec3(0.0f);
	this->m_scale = glm::vec3(5.0f);
	this->m_hidden = false;
	this->m_drawWireframe = false;
	this->m_outline = false;

	this->m_pMaterial = new Material(glm::vec4(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
	this->m_pShader = shader;
	this->m_mesh = mesh;

	genBuffers();
}

GameObject::GameObject(std::string name, std::string path, Shader* shader, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->m_name = name;
	this->position = glm::vec3(0.0f);
	this->m_rotation = glm::vec3(0.0f);
	this->m_scale = glm::vec3(5.0f);
	this->m_hidden = false;
	this->m_drawWireframe = false;
	this->m_outline = false;

	this->m_pMaterial = new Material(glm::vec4(0.1f), glm::vec3(1.0f), glm::vec3(1.0f), 1.0f);
	this->m_pShader = shader;

	ObjectLoader::load_model_mesh_assimp(path.c_str(), &m_mesh);

	genBuffers();
}

GameObject::~GameObject()
{
	// delete this->m_pShader;
	delete this->m_pMaterial;
}

void GameObject::genBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->m_mesh.getVerticesPtr()->size(), this->m_mesh.getVerticesPtr()->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->m_mesh.getIndicesPtr()->size(), this->m_mesh.getIndicesPtr()->data(), GL_STATIC_DRAW);

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

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->m_mesh.getVerticesPtr()->size(), this->m_mesh.getVerticesPtr()->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->m_mesh.getIndicesPtr()->size(), this->m_mesh.getIndicesPtr()->data(), GL_STATIC_DRAW);

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
	glDrawElements(GL_TRIANGLES, this->m_mesh.getIndicesPtr()->size(), GL_UNSIGNED_INT, 0);
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
	loadTexture(path, &this->m_texture);
	this->m_textureType = GL_TEXTURE_2D;
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
	loadCubeMap(faces, &this->m_texture);
	this->m_textureType = GL_TEXTURE_CUBE_MAP;
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
	for (int i = 0; i < faces.size(); i++)
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


bool GameObject::getHidden() const { return this->m_hidden; }
bool GameObject::getDrawWireframe() const { return this->m_drawWireframe; }
bool* GameObject::getDrawWireframePtr() { return &this->m_drawWireframe; }
bool GameObject::getIsOutline() const { return this->m_outline; }

glm::vec3 GameObject::getPosition() const { return this->position; }
glm::vec3* GameObject::getPositionPtr() { return &this->position; }
glm::vec3 GameObject::getRotation() const { return m_rotation; }
glm::vec3 GameObject::getScale() const { return m_scale; }
glm::vec3* GameObject::getScalePtr() { return &m_scale; }

glm::mat4 GameObject::getModelMatrix() const
{
	return glm::rotate(
		glm::rotate(
			glm::rotate(
				glm::scale(
					glm::translate(
						glm::mat4(1.0f), this->position),
					this->m_scale),
				glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f)),
			glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f)),
		glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f));
}

GLuint GameObject::getTexture() const { return this->m_texture; }
GLenum GameObject::getTextureType() const { return this->m_textureType; }

Shader* GameObject::getShaderPtr() const { return m_pShader; }

Material* GameObject::getMaterialPtr() const { return m_pMaterial; }

Mesh GameObject::getMesh() const { return this->m_mesh; }
Mesh* GameObject::getMeshPtr() { return &this->m_mesh; }

void GameObject::setPosition(glm::vec3 position) { this->position = position; }
void GameObject::setRotation(glm::vec3 rotation) { this->m_rotation = rotation; }
void GameObject::setScale(glm::vec3 scale) { this->m_scale = scale; }

void GameObject::setIsHidden(bool isHidden) { this->m_hidden = isHidden; }
void GameObject::setIsOutline(bool isOutline) { this->m_outline = isOutline; }

void GameObject::setShader(Shader* shader)
{
	if (this->m_pShader != nullptr) delete this->m_pShader;
	this->m_pShader = shader;
}

std::string GameObject::getName() const { return m_name; }