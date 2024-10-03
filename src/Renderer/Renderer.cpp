#include "Renderer.hpp"

#include "../Renderer/Environment/Environment.hpp"
#include "../Renderer/Lighting/Lighting.hpp"
#include "../Renderer/Object/Object.hpp"
#include "../Renderer/PostProcess/PostProcess.hpp"
#include "../Renderer/UI/UserInterface.hpp"

void Renderer::render()
{
	//! render order:
	//!		Environment,
	//!		Lighting,
	//!		Object,
	//!		PostProcess,
	//!		UI

	Environment::render(m_activeScene);

	Lighting::render(m_activeScene);

	Object::render(m_activeScene);

	PostProcess::getInstance().render(m_activeScene);

	UserInterface::getInstance().render(m_activeScene);
}