#include "UserInterface.hpp"

#include "../../Utils/ObjectLoader.hpp"
#include "../../Utils/GameObjectConstructor.hpp"
#include "../../Utils/StatTracker.hpp"

#include "../../Globals/GlobalConfig.hpp"

#include "../Renderer.hpp"
#include "../../Landscape.hpp"

#include "../PostProcess/PostProcess.hpp"

#include <iostream>


void UserInterface::renderMenuBar()
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

			ImGui::Checkbox("Object List", &m_objectList);

			ImGui::Checkbox("Object Properties", &m_objectProperties);

			ImGui::Checkbox("Light", &m_lightManager);

			ImGui::Checkbox("Asset", &m_assetManager);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Create"))
		{
			ColoredText("Objects", IM_COL32(130, 130, 130, 255));

			if (ImGui::MenuItem("Plane"))
			{
				GameObject* obj = new GameObject(std::format("Plane{}", Renderer::getInstance().getActiveScene()->getObjects().size()), ".\\Assets\\Objects\\Plane.obj", MaterialLib::get("render"), ColliderType::AABB);
				obj->setIsPhysicsEnabled(true);

				Renderer::getInstance().addObject(obj);
			}
			if (ImGui::MenuItem("Cube"))
			{
				GameObject* obj = new GameObject(std::format("Cube{}", Renderer::getInstance().getActiveScene()->getObjects().size()), ".\\Assets\\Objects\\Cube.obj", MaterialLib::get("render"), ColliderType::AABB);
				obj->setIsPhysicsEnabled(true);

				Renderer::getInstance().addObject(obj);

			}
			if (ImGui::MenuItem("Sphere"))
			{
				GameObject* obj = new GameObject(std::format("Sphere{}", Renderer::getInstance().getActiveScene()->getObjects().size()), ".\\Assets\\Objects\\Sphere.obj", MaterialLib::get("render"), ColliderType::Sphere);
				obj->setIsPhysicsEnabled(true);

				Renderer::getInstance().addObject(obj);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ColoredText("Landscape", IM_COL32(130, 130, 130, 255));

			if (ImGui::MenuItem("Landscape"))
			{
				Landscape* obj = new Landscape(std::format("Landscape{}", Renderer::getInstance().getActiveScene()->getObjects().size()));

				Renderer::getInstance().addObject(obj);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ColoredText("Lights", IM_COL32(130, 130, 130, 255));

			if (ImGui::MenuItem("PointLight"))
			{
				Scene* activeScene = Renderer::getInstance().getActiveScene();
				std::string* name = new std::string();
				name->assign(std::format("PointLight{}", activeScene->getLights().size()));

				auto light = new PointLight(name->c_str(), glm::vec3(1.f));
				activeScene->addLight(light);
			}

			if (ImGui::MenuItem("SpotLight"))
			{
				Scene* activeScene = Renderer::getInstance().getActiveScene();
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
						Renderer::getInstance().setDebugMaterial(MaterialLib::get("debugTriangle"));
						Config::g_settings->debugMode = Config::DebugMode::Triangles;
					}
					else
					{
						Renderer::getInstance().setDebugMaterial(MaterialLib::get("debug"));
						Config::g_settings->debugMode = Config::DebugMode::None;
					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				bool overdrawDebug = Config::g_settings->debugMode == Config::DebugMode::Overdraw;
				if (ImGui::RadioButton("Overdraw View", overdrawDebug))
				{
					if (!overdrawDebug)
					{
						Renderer::getInstance().setDebugMaterial(MaterialLib::get("debugOverdraw"));
						Config::g_settings->debugMode = Config::DebugMode::Overdraw;
					}
					else
					{
						Renderer::getInstance().setDebugMaterial(MaterialLib::get("debug"));
						Config::g_settings->debugMode = Config::DebugMode::None;
					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				bool wireframeDebug = Config::g_settings->debugMode == Config::DebugMode::Wireframe;
				if (ImGui::RadioButton("Wireframe View", wireframeDebug))
				{
					if (!wireframeDebug)
					{
						Renderer::getInstance().setDebugMaterial(MaterialLib::get("debugWireframe"));
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						Config::g_settings->debugMode = Config::DebugMode::Wireframe;
					}
					else
					{
						Renderer::getInstance().setDebugMaterial(MaterialLib::get("debug"));
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						Config::g_settings->debugMode = Config::DebugMode::None;
					}
				}

				bool normalDebug = Config::g_settings->debugMode == Config::DebugMode::Normals;
				if (ImGui::RadioButton("Normal View", normalDebug))
				{
					Renderer::getInstance().setDebugMaterial(MaterialLib::get("debug"));

					if (!normalDebug)
					{
						PostProcess::getInstance().visualizeNormals = true;
						Config::g_settings->debugMode = Config::DebugMode::Normals;
					}
					else
					{
						PostProcess::getInstance().visualizeNormals = false;
						Config::g_settings->debugMode = Config::DebugMode::None;
					}

					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ColoredText("Shader", IM_COL32(130, 130, 130, 255));

			if (ImGui::RadioButton("Node Editor", m_nodeEditor == true))
			{
				m_nodeEditor = !m_nodeEditor;
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
					Camera* camera = new Camera(std::format("Camera{}", Renderer::getInstance().getActiveScene()->getCameras().size()), false);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					Renderer::getInstance().getActiveScene()->addCamera(camera);
				}

				if (ImGui::MenuItem("Orthographic", nullptr, nullptr, false))
				{
					Camera* camera = new Camera(std::format("Camera{}", Renderer::getInstance().getActiveScene()->getCameras().size()), true);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					Renderer::getInstance().getActiveScene()->addCamera(camera);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Cameras"))
			{
				for (auto& camera : Renderer::getInstance().getActiveScene()->getCameras())
				{
					if (ImGui::Checkbox(camera->getName().c_str(), new bool(camera == Renderer::getInstance().getActiveScene()->getActiveCamera())))
					{
						Renderer::getInstance().getActiveScene()->setActiveCamera(camera);
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
				Renderer::getInstance().setDebugMaterial(MaterialLib::get("debug"));
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

void UserInterface::renderLightManager()
{
	ImGui::Begin("Light Manager", nullptr);
	{
		ImGui::BeginChild("Lights", ImVec2(200, 0), true);
		{
			for (auto& light : Renderer::getInstance().getActiveScene()->getLights())
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

							Renderer::getInstance().getActiveScene()->setActiveCamera(camera);
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

							Renderer::getInstance().getActiveScene()->setActiveCamera(camera);
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

							Renderer::getInstance().getActiveScene()->setActiveCamera(camera);
						}
					}

					ImGui::TreePop();
				}

				if (ImGui::Button("Delete"))
				{
					Renderer::getInstance().getActiveScene()->deleteLight(m_selectedLight);

					m_selectedLight = nullptr;
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void UserInterface::renderAssetManager()
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
				const int numColumns = ceil(ImGui::GetWindowWidth() * 0.8f / itemSize);

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
									Renderer::getInstance().addObject(obj);
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

void UserInterface::renderFpsGraph()
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
			double start = std::max(static_cast<int>(g_stats->fpsHistory.size()) - history, 0);
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

void UserInterface::renderSceneStats()
{
	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 150));
	ImGui::Begin("Scene Stats", nullptr, ImGuiWindowFlags_NoDocking);
	{
		ImGui::Text("Objects: %i", Renderer::getInstance().getActiveScene()->getObjects().size());
		ImGui::Text("Env-Objects: %i", Renderer::getInstance().getActiveScene()->getEnvObjects().size());

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Lights: %i", Renderer::getInstance().getActiveScene()->getLights().size());

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		ImGui::Text("Vertices: %i", Renderer::getInstance().getActiveScene()->countVertices());
		ImGui::Text("Indices: %i", Renderer::getInstance().getActiveScene()->countIndices());

	}
	ImGui::End();
	ImGui::PopStyleColor();
}

bool UserInterface::IconItem(int id, const char* text, GLuint imageId, const float itemSize)
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

void UserInterface::ColoredText(const char* text, ImU32 color)
{
	ImGui::PushStyleColor(ImGuiCol_Text, color);
	ImGui::Text(text);
	ImGui::PopStyleColor();
}

void UserInterface::renderLightMaterialView()
{
	float* color[3] = { &m_selectedLight->getColorPtr()->x, &m_selectedLight->getColorPtr()->y, &m_selectedLight->getColorPtr()->z };

	ImGui::ColorEdit3("Color", *color, ImGuiColorEditFlags_NoInputs);
}


void UserInterface::setImguiStyle()
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