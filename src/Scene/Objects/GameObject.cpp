#include "GameObject.hpp"

GameObject::GameObject(std::string name, Mesh mesh, Material* pMaterial, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->m_name = name;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(5.0f);
	this->m_hidden = false;
	this->m_outline = false;

	this->m_pMaterial = pMaterial;
	this->mesh = mesh;

	genBuffers();
	initParameterAtlas();
}

GameObject::GameObject(std::string name, std::string path, Material* pMaterial, ColliderType colliderType) : GameObjectCollisions(this, colliderType), GameObjectPhysics(this)
{
	this->m_name = name;
	this->position = glm::vec3(0.0f);
	this->rotation = glm::vec3(0.0f);
	this->scale = glm::vec3(5.0f);
	this->m_hidden = false;
	this->m_outline = false;

	this->m_pMaterial = pMaterial;

	ObjectLoader::load_model_mesh_assimp(path.c_str(), mesh);

	genBuffers();
	initParameterAtlas();
}

GameObject::GameObject(const GameObject& other) : GameObjectCollisions(this, other.collider.getColliderType()), GameObjectPhysics(this)
{
	this->m_name = other.m_name + "_dup";
	this->position = other.position;
	this->rotation = other.rotation;
	this->scale = other.scale;
	this->m_hidden = other.m_hidden;
	this->m_outline = other.m_outline;

	this->m_pMaterial = other.m_pMaterial;
	this->mesh = Mesh(other.mesh);

	genBuffers();
	initParameterAtlas();
}

GameObject::~GameObject()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);
}

void GameObject::genBuffers()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->mesh.getVerticesPtr()->size(), this->mesh.getVerticesPtr()->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->mesh.getIndicesPtr()->size(), this->mesh.getIndicesPtr()->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex)); // position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); // normal
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)); // uv
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent)); // tangent
	glEnableVertexAttribArray(3);
	
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent)); // bitangent
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObject::updateBuffers()
{
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->mesh.getVerticesPtr()->size(), this->mesh.getVerticesPtr()->data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * this->mesh.getIndicesPtr()->size(), this->mesh.getIndicesPtr()->data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, vertex)); // position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); // normal
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv)); // uv
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent)); // tangent
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent)); // bitangent
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GameObject::draw()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(this->mesh.getIndicesPtr()->size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

bool GameObject::getHidden() const { return this->m_hidden; }
bool GameObject::getIsOutline() const { return this->m_outline; }

glm::vec3 GameObject::getPosition() const { return this->position; }
glm::vec3* GameObject::getPositionPtr() { return &this->position; }
glm::vec3 GameObject::getRotation() const { return rotation; }
glm::vec3 GameObject::getScale() const { return scale; }
glm::vec3* GameObject::getScalePtr() { return &scale; }

glm::mat4 GameObject::getModelMatrix()
{
	modelMatrix = glm::rotate(
		glm::rotate(
			glm::rotate(
				glm::scale(
					glm::translate(
						glm::mat4(1.0f), this->position),
					this->scale),
				glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f)),
			glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f)),
		glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));

	return modelMatrix;
}

Material* GameObject::getMaterialPtr() { return m_pMaterial; }

Mesh& GameObject::getMesh() { return this->mesh; }
Mesh* GameObject::getMeshPtr() { return &this->mesh; }

void GameObject::setPosition(glm::vec3 position) { this->position = position; }
void GameObject::setRotation(glm::vec3 rotation) { this->rotation = rotation; }
void GameObject::setScale(glm::vec3 scale) { this->scale = scale; }

void GameObject::setIsHidden(bool isHidden) { this->m_hidden = isHidden; }
void GameObject::setIsOutline(bool isOutline) { this->m_outline = isOutline; }

std::string GameObject::getName() const { return m_name; }
void GameObject::setName(std::string name) { m_name = name; }
void GameObject::setMaterial(Material* pMaterial) { m_pMaterial = pMaterial; };

// imgui ptr functions

float** GameObject::getRotationPtr()
{
	static float* rotation[3] = { &this->rotation.x, &this->rotation.y, &this->rotation.z };
	return rotation;
}