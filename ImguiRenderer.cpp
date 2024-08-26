#include "ImguiRenderer.hpp"
#include "ObjectLoader.hpp"
#include "GameObjectConstructor.hpp"
#include "GlobalConfig.hpp"
#include "StatTracker.hpp"

#include <windows.h>
#include <commdlg.h>
#include <iostream>


void ImguiRenderer::renderMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ColoredText("Managers", IM_COL32(130, 130, 130, 255));

			ImGui::Checkbox("Object", &m_objectManager);

			ImGui::Checkbox("Light", &m_lightManager);

			ImGui::Checkbox("Asset", &m_assetManager);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Create"))
		{
			ColoredText("Objects", IM_COL32(130, 130, 130, 255));

			if (ImGui::MenuItem("Plane"))
			{
				GameObject* obj = new GameObject(std::format("Plane{}", RendererManager::getInstance().getActiveScene()->getObjects().size()), ".\\Assets\\Objects\\Plane.obj", ShaderLib::getRenderShaderPtr(), ColliderType::BoundingBox);
				obj->setIsPhysicsEnabled(true);

				RendererManager::getInstance().addObject(obj);
			}
			if (ImGui::MenuItem("Cube"))
			{
				GameObject* obj = new GameObject(std::format("Cube{}", RendererManager::getInstance().getActiveScene()->getObjects().size()), ".\\Assets\\Objects\\Cube.obj", ShaderLib::getRenderShaderPtr(), ColliderType::BoundingBox);
				obj->setIsPhysicsEnabled(true);

				RendererManager::getInstance().addObject(obj);

			}
			if (ImGui::MenuItem("Sphere"))
			{
				GameObject* obj = new GameObject(std::format("Sphere{}", RendererManager::getInstance().getActiveScene()->getObjects().size()), ".\\Assets\\Objects\\Sphere.obj", ShaderLib::getRenderShaderPtr(), ColliderType::Circular);
				obj->setIsPhysicsEnabled(true);

				RendererManager::getInstance().addObject(obj);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ColoredText("Lights", IM_COL32(130, 130, 130, 255));

			if (ImGui::MenuItem("PointLight"))
			{
				Scene* activeScene = RendererManager::getInstance().getActiveScene();
				std::string* name = new std::string();
				name->assign(std::format("PointLight{}", activeScene->getLights().size()));

				auto light = new PointLight(name->c_str(), glm::vec3(1.f));
				activeScene->addLight(light);
			}

			if (ImGui::MenuItem("SpotLight"))
			{
				Scene* activeScene = RendererManager::getInstance().getActiveScene();
				std::string* name = new std::string();
				name->assign(std::format("SpotLight{}", activeScene->getLights().size()));

				auto light = new SpotLight(name->c_str(), glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f));
				activeScene->addLight(light);
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ColoredText("Display", IM_COL32(130, 130, 130, 255));

			ImGui::Checkbox("FPS Graph", &m_fpsGraph);

			ImGui::Checkbox("Scene Stats", &m_sceneStats);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ColoredText("Debug", IM_COL32(130, 130, 130, 255));

			if (ImGui::Checkbox("Vsync", new bool(Config::g_settings->vsyncEnabled)))
			{
				Config::g_settings->vsyncEnabled = !Config::g_settings->vsyncEnabled;
				glfwSwapInterval(Config::g_settings->vsyncEnabled);
			}

			if (ImGui::BeginMenu("Visualizers"))
			{
				bool triangleDebug = Config::g_settings->debugMode == Config::DebugMode::Triangles;
				if (ImGui::RadioButton("Triangle View", triangleDebug))
				{
					if (!triangleDebug)
					{
						RendererManager::getInstance().setDebugShader(ShaderLib::getDebugTriangleShaderPtr());
						Config::g_settings->debugMode = Config::DebugMode::Triangles;
					}
					else
					{
						RendererManager::getInstance().setDebugShader(ShaderLib::getDebugShaderPtr());
						Config::g_settings->debugMode = Config::DebugMode::None;
					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				bool overdrawDebug = Config::g_settings->debugMode == Config::DebugMode::Overdraw;
				if (ImGui::RadioButton("Overdraw View", overdrawDebug))
				{
					if (!overdrawDebug)
					{
						RendererManager::getInstance().setDebugShader(ShaderLib::getDebugOverdrawShaderPtr());
						Config::g_settings->debugMode = Config::DebugMode::Overdraw;
					}
					else
					{
						RendererManager::getInstance().setDebugShader(ShaderLib::getDebugShaderPtr());
						Config::g_settings->debugMode = Config::DebugMode::None;
					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				bool wireframeDebug = Config::g_settings->debugMode == Config::DebugMode::Wireframe;
				if (ImGui::RadioButton("Wireframe View", wireframeDebug))
				{
					if (!wireframeDebug)
					{
						RendererManager::getInstance().setDebugShader(ShaderLib::getDebugWireframeShaderPtr());
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						Config::g_settings->debugMode = Config::DebugMode::Wireframe;
					}
					else
					{
						RendererManager::getInstance().setDebugShader(ShaderLib::getDebugShaderPtr());
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						Config::g_settings->debugMode = Config::DebugMode::None;
					}

				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Viewport"))
		{
			ColoredText("Camera", IM_COL32(130, 130, 130, 255));

			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("Perspective"))
				{
					Camera* camera = new Camera(std::format("Camera{}", RendererManager::getInstance().getActiveScene()->getCameras().size()), false);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					RendererManager::getInstance().getActiveScene()->addCamera(camera);
				}

				if (ImGui::MenuItem("Orthographic", nullptr, nullptr, false))
				{
					Camera* camera = new Camera(std::format("Camera{}", RendererManager::getInstance().getActiveScene()->getCameras().size()), true);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					RendererManager::getInstance().getActiveScene()->addCamera(camera);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Cameras"))
			{
				for (auto& camera : RendererManager::getInstance().getActiveScene()->getCameras())
				{
					if (ImGui::Checkbox(camera->getName().c_str(), new bool(camera == RendererManager::getInstance().getActiveScene()->getActiveCamera())))
					{
						RendererManager::getInstance().getActiveScene()->setActiveCamera(camera);
					}
				}

				ImGui::EndMenu();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ColoredText("Render", IM_COL32(130, 130, 130, 255));

			if (ImGui::RadioButton("Debug", Config::g_settings->renderMode == Config::RenderMode::Debug))
			{
				RendererManager::getInstance().setDebugShader(ShaderLib::getDebugShaderPtr());
				Config::g_settings->renderMode = Config::RenderMode::Debug;
			}

			if (ImGui::RadioButton("Render", Config::g_settings->renderMode == Config::RenderMode::Render))
			{
				Config::g_settings->renderMode = Config::RenderMode::Render;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ImguiRenderer::renderObjectManager()
{
	ImGui::Begin("Object Manager", nullptr);
	{
		ImGui::BeginChild("Objects", ImVec2(200, 0), true);
		{
			auto& objects = RendererManager::getInstance().getActiveScene()->getObjects();
			for (int i = 0; i < objects.size(); i++)
			{
				auto obj = objects[i];

				ImGui::Selectable(obj->getName().c_str(), isSelected(obj));
				if (ImGui::IsItemClicked())
				{
					if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && m_selectedObjects.size() > 0)
					{
						int startIndex = getIndex<GameObject*>(objects, m_selectedObjects[0]);
						int endIndex = i;
						if (startIndex > endIndex)
						{
							int temp = startIndex;
							startIndex = endIndex;
							endIndex = temp;
						}

						m_selectedObjects.clear();
						for (int j = startIndex; j <= endIndex; j++)
						{
							addSelectedObject(objects[j]);
						}
					}
					else
					{
						setSelectedObject(obj);
					}
				}
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Detail View", ImVec2(0, 0), true);
		{
			if (m_selectedObjects.size() > 0)
			{
				GameObject* object = getSelectedObject();

				float* pos[3] = { &object->getPositionPtr()->x, &object->getPositionPtr()->y, &object->getPositionPtr()->z };
				float* scale[3] = { &object->getScalePtr()->x, &object->getScalePtr()->y, &object->getScalePtr()->z };
				float* ambient[3] = { &object->getMaterialPtr()->getAmbientPtr()->x, &object->getMaterialPtr()->getAmbientPtr()->y, &object->getMaterialPtr()->getAmbientPtr()->z };
				float* diffuse[3] = { &object->getMaterialPtr()->getDiffusePtr()->x, &object->getMaterialPtr()->getDiffusePtr()->y, &object->getMaterialPtr()->getDiffusePtr()->z };
				float* specular[3] = { &object->getMaterialPtr()->getSpecularPtr()->x, &object->getMaterialPtr()->getSpecularPtr()->y, &object->getMaterialPtr()->getSpecularPtr()->z };

				ImGui::Text("Selected Object: %s", object->getName().c_str());
				ImGui::Separator();

				if (ImGui::TreeNode("Object"))
				{
					ImGui::Text("Position");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX());
					ImGui::InputFloat3("##0", *pos);

					ImGui::Text("Rotation");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX());
					ImGui::InputFloat3("##1", *object->getRotationPtr());

					ImGui::Text("Scale");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX());
					ImGui::InputFloat3("##2", *scale);

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Material"))
				{
					ImGui::Text("Shader: %s", object->getShaderPtr()->getName().c_str());

					ImGui::ColorEdit3("Ambient", *ambient, ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::ColorEdit3("Diffuse", *diffuse, ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::ColorEdit3("Specular", *specular, ImGuiColorEditFlags_NoInputs);

					ImGui::SetNextItemWidth(80.f);
					ImGui::InputFloat("Shininess", object->getMaterialPtr()->getShininessPtr());

					ImGui::Image((void*)(intptr_t)object->getTexture(), ImVec2(150, 150));
					ImGui::SameLine();
					ImGui::BeginChild("Container", ImVec2(0, 150));
					{
						ImGui::BeginChild("Textures", ImVec2(0, 120), true);
						{
							renderFolderStructure(".\\Assets\\Textures");
						}
						ImGui::EndChild();

						if (ImGui::Button("Load Texture"))
						{
							object->loadTexture(m_selectedTexturePath);
						}
					}
					ImGui::EndChild();

					ImGui::TreePop();
				}

				if (object->getIsPhysicsEnabled())
				{
					float* vel[3] = { &object->getVelocityPtr()->x, &object->getVelocityPtr()->y, &object->getVelocityPtr()->z };

					if (ImGui::TreeNode("Physics"))
					{
						ImGui::Text("Mass");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat("##4523", object->getMassPtr());

						ImGui::Text("Velocity");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##2", *vel);

						ImGui::Text("Linear Drag");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat("##3", object->getLinearDragPtr());

						if (ImGui::Checkbox("Gravity", object->getIsGravityEnabledPtr()))
						{
							if (object->getIsGravityEnabled()) object->setIsPullToObjectEnabled(false);
						}

						if (ImGui::Checkbox("ObjectPull", object->getIsPullToObjectEnabledPtr()))
						{
							if (object->getIsPullToObjectEnabled()) object->setIsGravityEnabled(false);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Collisions"))
					{
						if (ImGui::Checkbox("Collision Enabled", object->getIsCollisionEnabledPtr()))
						{
							object->checkBuffers();
						}

						if (object->getIsCollisionEnabled())
						{
							ImGui::Checkbox("Draw Collision Box", object->getIsDrawColliderPtr());

							if (ImGui::Checkbox("Hide objects to far away for collision", &m_highlightCloseCollidableObjects))
							{
								if (PhysicEngine::getFocusedGameObject() == nullptr)
									PhysicEngine::setFocusedGameObject(object);
								else
									PhysicEngine::setFocusedGameObject(nullptr);
							}

							ImGui::BeginChild("Collider", ImVec2(0, 0), true);
							{
								float* anchor[3] = { &object->getColliderPtr()->getAnchorPositionPtr()->x, &object->getColliderPtr()->getAnchorPositionPtr()->y , &object->getColliderPtr()->getAnchorPositionPtr()->z };
								float* colliderScale[3] = { &object->getColliderPtr()->getScalePtr()->x, &object->getColliderPtr()->getScalePtr()->y , &object->getColliderPtr()->getScalePtr()->z };

								ImGui::Text("Anchor");
								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX());
								ImGui::InputFloat3("##4", *anchor);

								ImGui::Text("Scale");
								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX());
								ImGui::InputFloat3("##5", *colliderScale);

								ImGui::Spacing();

								if (ImGui::Button("##6", ImVec2(40, 0))) object->snapColliderToObject();
								ImGui::SameLine();
								ImGui::Text("Snap Collider To Object");

								ImGui::Text("IMPORTANT! If collider scale = object scale collider won't be visible!");
							}
							ImGui::EndChild();
						}

						ImGui::TreePop();
					}

					ImGui::Separator();

					ImGui::Text("Indices: %i", object->getMesh().getIndices().size());
					ImGui::Text("Vertices: %i", object->getMesh().getVertices().size());

					if (ImGui::Button("Delete"))
					{
						RendererManager::getInstance().deleteObject(*object);
						removeSelectedObject(object);
					}
				}
			}
		}
		ImGui::EndChild();

	}
	ImGui::End();
}

void ImguiRenderer::renderLightManager()
{
	ImGui::Begin("Light Manager", nullptr);
	{
		ImGui::BeginChild("Lights", ImVec2(200, 0), true);
		{
			for (auto& light : RendererManager::getInstance().getActiveScene()->getLights())
			{
				ImGui::Selectable(light->getName().data(), m_selectedLight == light);
				if (ImGui::IsItemClicked())
				{
					m_selectedLight = light;
				}
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Detail View", ImVec2(0, 0), true);
		{
			if (m_selectedLight != nullptr)
			{
				ImGui::Text(std::format("Selected Light: {}", m_selectedLight->getName()).c_str());
				ImGui::Separator();

				if (ImGui::TreeNode("Light"))
				{
					if (m_selectedLight->getLightType() == LightType::Point)
					{
						PointLight* light = static_cast<PointLight*>(m_selectedLight);
						float* pos[3] = { &light->getPositionPtr()->x, &light->getPositionPtr()->y, &light->getPositionPtr()->z };

						ImGui::Text("Position");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##0", *pos);

						renderLightMaterialView();

						ImGui::SetNextItemWidth(80.0f);
						ImGui::InputFloat("Constant", light->getConstantPtr());
						ImGui::SameLine();
						ImGui::SetNextItemWidth(80.0f);
						ImGui::InputFloat("Linear", light->getLinearPtr());
						ImGui::SameLine();
						ImGui::SetNextItemWidth(80.0f);
						ImGui::InputFloat("Quadratic", light->getQuadraticPtr());

						if (ImGui::Button("Render Scene from Light"))
						{
							//! This creates a memory leak but womp womp

							Camera* camera = new Camera("", false);
							camera->setAnchor(glm::vec3(0.f));
							camera->setPosition(m_selectedLight->getPosition());
							camera->setProjectionMatrix(m_selectedLight->getProjectionMatrix());
							camera->setViewMatrix(m_selectedLight->getViewMatrix());
							camera->setImmutable(true);

							RendererManager::getInstance().getActiveScene()->setActiveCamera(camera);
						}
					}
					else if (m_selectedLight->getLightType() == LightType::Spot)
					{
						SpotLight* light = static_cast<SpotLight*>(m_selectedLight);
						float* pos[3] = { &light->getPositionPtr()->x, &light->getPositionPtr()->y, &light->getPositionPtr()->z };
						float* dir[3] = { &light->getDirectionPtr()->x, &light->getDirectionPtr()->y, &light->getDirectionPtr()->z };

						ImGui::Text("Position");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##0", *pos);

						ImGui::Text("Direction");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##1", *dir);

						ImGui::SetNextItemWidth(80.0f);
						ImGui::InputFloat("Inner Cone", light->getInnerConePtr());
						ImGui::SameLine();
						ImGui::SetNextItemWidth(80.0f);
						ImGui::InputFloat("Outer Cone", light->getOuterConePtr());

						renderLightMaterialView();

						if (ImGui::Button("Render Scene from Light"))
						{
							//! This creates a memory leak but womp womp

							auto light = static_cast<SpotLight*>(m_selectedLight);

							Camera* camera = new Camera("", false);
							camera->setAnchor(light->getPosition() + light->getDirection());
							camera->setPosition(light->getPosition());
							camera->setProjectionMatrix(light->getProjectionMatrix());
							camera->setViewMatrix(light->getViewMatrix());
							camera->setImmutable(true);

							RendererManager::getInstance().getActiveScene()->setActiveCamera(camera);
						}
					}
					else
					{
						DirectionalLight* light = static_cast<DirectionalLight*>(m_selectedLight);
						float* dir[3] = { &light->getDirectionPtr()->x, &light->getDirectionPtr()->y, &light->getDirectionPtr()->z };

						ImGui::Text("Direction");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##0", *dir);

						renderLightMaterialView();

						ImGui::Text("Shadow Quality");
						ImGui::SameLine();
						ImGui::InputFloat("##355", light->getShadowQualityPtr());
						ImGui::SameLine();
						if (ImGui::Button("Apply"))
						{
							light->updateShadows();
						}

						if (ImGui::Button("Render Scene from Light"))
						{
							//! This creates a memory leak but womp womp

							Camera* camera = new Camera("", false);
							camera->setAnchor(glm::vec3(0.f));
							camera->setPosition(m_selectedLight->getPosition());
							camera->setProjectionMatrix(m_selectedLight->getProjectionMatrix());
							camera->setViewMatrix(m_selectedLight->getViewMatrix());
							camera->setImmutable(true);

							RendererManager::getInstance().getActiveScene()->setActiveCamera(camera);
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::Button("Delete"))
				{
					RendererManager::getInstance().getActiveScene()->deleteLight(m_selectedLight);

					m_selectedLight = nullptr;
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void ImguiRenderer::renderAssetManager()
{
	ImGui::Begin("Asset Manager", nullptr);
	{
		ImGui::BeginChild("Folder View", ImVec2(200, 0), ImGuiChildFlags_Border);
		{
			renderAssetFolderStructure(".\\Assets");
		}
		ImGui::EndChild();

		ImGui::SameLine();

		if (m_selectedAssetPath != "")
		{
			ImGui::BeginChild("Data View");
			{
				const float itemSize = 120.0f;
				const int numColumns = ceilf(ImGui::GetWindowWidth() * 0.8f / itemSize);

				ImGui::Columns(numColumns, nullptr, false);

				auto iterator = std::filesystem::directory_iterator(m_selectedAssetPath);

				int i = 0;
				for (auto& entry : iterator)
				{
					if (entry.is_directory())
					{
						if (IconItem(i, entry.path().filename().string().c_str(), AssetManager::getInstance().getIcon(".folder"), itemSize))
						{
							m_selectedAssetPath = entry.path().string();
						}
					}
					else
					{
						if (IconItem(i, entry.path().filename().string().c_str(), AssetManager::getInstance().getIcon(entry.path().extension().string()), itemSize))
						{
							if (entry.path().extension().string() == ".obj")
							{
								std::vector<GameObject*> objects = GameObjectConstructor::createGameObjects(entry.path().string().c_str());
								for (GameObject* obj : objects)
								{
									obj->setIsPhysicsEnabled(true);
									RendererManager::getInstance().addObject(obj);
								}
							}
						}
					}

					i++;
				}

				ImGui::Columns(1); // Reset columns back to one
			}
			ImGui::EndChild();
		}
	}
	ImGui::End();
}

void ImguiRenderer::renderFpsGraph()
{
	ImGui::Begin("FPS Graph", nullptr, ImGuiWindowFlags_NoDocking);
	{
		static bool lines = true, fills = true;
		static int history = 10;

		ImGui::Checkbox("Lines", &lines);
		ImGui::SameLine();
		ImGui::Checkbox("Fills", &fills);
		ImGui::SameLine();
		ImGui::SliderInt("History", &history, 1, g_stats->maxHistory);

		if (ImPlot::BeginPlot("Real-Time FPS", ImGui::GetContentRegionAvail()))
		{
			double start = max(g_stats->fpsHistory.size() - static_cast<float>(history), 0);
			double end = g_stats->fpsHistory.size();

			ImPlot::SetupAxisLimits(ImAxis_X1, start, end - 1, ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, g_stats->minFps, g_stats->maxFps * 1.5f, ImGuiCond_Always);

			if (lines)
			{
				ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, -1), 2.f);
				ImPlot::PlotLine("FPS", g_stats->fpsHistory.data(), g_stats->fpsHistory.size());
				ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, -1), 2.f);
				ImPlot::PlotLine("Frame Time", g_stats->frameTimeHistory.data(), g_stats->frameTimeHistory.size());
				ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, -1), 2.f);
				ImPlot::PlotLine("CPU Time", g_stats->cpuTimeHistory.data(), g_stats->cpuTimeHistory.size());
				ImPlot::SetNextLineStyle(ImVec4(0, 0, 0, -1), 2.f);
				ImPlot::PlotLine("GPU Time", g_stats->gpuTimeHistory.data(), g_stats->gpuTimeHistory.size());
			}

			if (fills)
			{
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PlotShaded("FPS", g_stats->fpsHistory.data(), g_stats->fpsHistory.size());
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PlotShaded("Frame Time", g_stats->frameTimeHistory.data(), g_stats->frameTimeHistory.size());
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PlotShaded("CPU Time", g_stats->cpuTimeHistory.data(), g_stats->cpuTimeHistory.size());
				ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
				ImPlot::PlotShaded("GPU Time", g_stats->gpuTimeHistory.data(), g_stats->gpuTimeHistory.size());
			}

			ImPlot::EndPlot();
		}
	}
	ImGui::End();
}

void ImguiRenderer::renderSceneStats()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 150));
	ImGui::Begin("Scene Stats", nullptr);
	{
		ImGui::Text("Objects: %i", RendererManager::getInstance().getActiveScene()->getObjects().size());
		ImGui::Text("Env-Objects: %i", RendererManager::getInstance().getActiveScene()->getEnvObjects().size());

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Lights: %i", RendererManager::getInstance().getActiveScene()->getLights().size());

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Vertices: %i", RendererManager::getInstance().getActiveScene()->countVertices());
		ImGui::Text("Indices: %i", RendererManager::getInstance().getActiveScene()->countIndices());

	}
	ImGui::End();
	ImGui::PopStyleColor();
}

bool ImguiRenderer::IconItem(int id, const char* text, GLuint imageId, const float itemSize)
{
	ImGui::PushID(id); // Push a unique ID for each item

	// Calculate positions for icon and text
	ImVec2 itemPos = ImGui::GetCursorScreenPos();
	float iconSize = itemSize / 2;
	float textHeight = itemSize / 4;
	float iconPosX = itemPos.x + (itemSize - iconSize) / 2;
	float iconPosY = itemPos.y + (itemSize - iconSize - textHeight) / 2; // Center vertically

	ImGui::SetCursorScreenPos(ImVec2(iconPosX, iconPosY));
	ImGui::Image((void*)(intptr_t)imageId, ImVec2(iconSize, iconSize));

	// Calculate text position (under the icon)
	float textPosX = itemPos.x + (itemSize - ImGui::CalcTextSize(text).x) / 2;
	float textPosY = itemPos.y + (itemSize + iconSize + textHeight) / 2.1f; // Adjust vertically as needed

	ImGui::SetCursorScreenPos(ImVec2(textPosX, textPosY));
	ImGui::Text(text);

	// Calculate selectable area position
	ImGui::SetCursorScreenPos(ImVec2(itemPos.x, itemPos.y));
	bool isSelected = ImGui::Selectable("", false, 0, ImVec2(itemSize, itemSize));

	ImGui::NextColumn();

	ImGui::PopID(); // Pop the unique ID

	return isSelected;
}

void ImguiRenderer::ColoredText(const char* text, ImU32 color)
{
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGui::Text(text);
	ImGui::PopStyleColor();
}

void ImguiRenderer::renderLightMaterialView()
{
	float* color[3] = { &m_selectedLight->getColorPtr()->x, &m_selectedLight->getColorPtr()->y, &m_selectedLight->getColorPtr()->z };

	ImGui::ColorEdit3("Color", *color, ImGuiColorEditFlags_NoInputs);
}


void ImguiRenderer::setImguiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	// Colors
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.29f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.44f, 0.75f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.20f, 0.44f, 0.75f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.50f, 0.50f, 0.50f, 0.60f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.44f, 0.75f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.44f, 0.75f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.24f, 0.52f, 0.88f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.85f);
	colors[ImGuiCol_Border] = ImVec4(0.f, 0.f, 0.f, 0.f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.f, 0.f, 0.f, 0.f);

	// Adjust other settings
	style.WindowMinSize = ImVec2(20.f, 20.0f);
	style.WindowRounding = 5.0f;
	style.FrameRounding = 4.0f;
	style.GrabRounding = 3.0f;
	style.ScrollbarRounding = 2.0f;
}


std::vector<GameObject*> ImguiRenderer::m_selectedObjects;