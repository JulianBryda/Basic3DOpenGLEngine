#include "RendererPipeline.hpp"

std::multimap<RendererType, Renderer*> RendererPipeline::rendererMap;
std::vector<Camera*> RendererPipeline::cameras;
int RendererPipeline::activeCameraIndex;