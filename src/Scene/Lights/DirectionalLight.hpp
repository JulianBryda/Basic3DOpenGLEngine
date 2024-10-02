#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Light.hpp"

class DirectionalLight : public Light
{

public:


	DirectionalLight(const char* name, glm::vec3 color) : Light(name, color, LightType::Directional)
	{
		projection = glm::mat4(1.f);
	}

	~DirectionalLight()
	{

	}


	// getter
	inline glm::vec3 getDirection() const { return this->position; }
	inline glm::vec3* getDirectionPtr() { return &this->position; }

	glm::mat4 getViewMatrix() override
	{
		return glm::lookAt(this->position, glm::vec3(0.f), glm::vec3(0.f, 1.f, 0.f));
	}

	glm::mat4 getProjectionMatrix() override
	{
		return projection;
	}

	glm::mat4 getProjectionMatrix(glm::vec3 min, glm::vec3 max) override
	{
		const glm::mat4 view = getViewMatrix();

		glm::vec3 corners[8] =
		{
			glm::vec3(min.x, min.y, min.z),
			glm::vec3(max.x, min.y, min.z),
			glm::vec3(min.x, max.y, min.z),
			glm::vec3(max.x, max.y, min.z),
			glm::vec3(min.x, min.y, max.z),
			glm::vec3(max.x, min.y, max.z),
			glm::vec3(min.x, max.y, max.z),
			glm::vec3(max.x, max.y, max.z)
		};

		glm::vec3 minView = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 maxView = glm::vec3(std::numeric_limits<float>::lowest());

		for (int i = 0; i < 8; ++i)
		{
			const glm::vec4 transformedCorner = view * glm::vec4(corners[i], 1.0);
			minView = glm::min(minView, glm::vec3(transformedCorner));
			maxView = glm::max(maxView, glm::vec3(transformedCorner));
		}

		const float left = minView.x;
		const float right = maxView.x;
		const float bottom = minView.y;
		const float top = maxView.y;
		const float near = -maxView.z;
		const float far = -minView.z;

		projection = glm::ortho(left, right, bottom, top, near, far);
		return projection;
	}

	// setter
	inline void setDirection(glm::vec3 direction) { this->position = direction; }

private:

	glm::mat4 projection;
};