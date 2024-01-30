#include "RendererPipeline.hpp"
#include "ObjectRenderer.hpp"
#include "EnvironmentRenderer.hpp"

std::multimap<RendererType, Renderer*> RendererPipeline::rendererMap;
std::vector<Camera*> RendererPipeline::cameras;
int RendererPipeline::activeCameraIndex;

void RendererPipeline::addObjectToRenderer(GameObject* object, RendererType type)
{
	switch (type)
	{
	case Object:
		static_cast<ObjectRenderer*>(getRendererPtr(type))->addObject(object);
		break;
	case Environment:
		static_cast<EnvironmentRenderer*>(getRendererPtr(type))->addObject(object);
		break;
	default:
		throw std::exception("Type is not supported!");
		break;
	}
}