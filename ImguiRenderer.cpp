#include "ImguiRenderer.hpp"
#include "ObjectLoader.hpp"

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

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("Object Manager", &m_showObjectManager);

			ImGui::Checkbox("Light Manager", &m_showLightManager);

			ImGui::Checkbox("Asset Manager", &m_showAssetManager);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				GameObject* obj = new GameObject(std::format("Cube{}", RendererManager::getInstance().getTotalObjectCount()), ".\\Assets\\Objects\\Cube.obj", ShaderLib::getDebugShaderPtr(), ColliderType::BoundingBox);
				obj->setIsPhysicsEnabled(true);

				RendererManager::getInstance().addObject(obj, RendererType::Object);

			}
			if (ImGui::MenuItem("Sphere"))
			{
				GameObject* obj = new GameObject(std::format("Sphere{}", RendererManager::getInstance().getTotalObjectCount()), ".\\Assets\\Objects\\Sphere.obj", ShaderLib::getDebugShaderPtr(), ColliderType::Circular);
				obj->setIsPhysicsEnabled(true);

				RendererManager::getInstance().addObject(obj, RendererType::Object);
			}
			if (ImGui::MenuItem("StressTest"))
			{
				GameObject* obj = new GameObject(std::format("StressTest{}", RendererManager::getInstance().getTotalObjectCount()), ".\\Assets\\Objects\\StressTest.obj", ShaderLib::getDebugShaderPtr(), ColliderType::BoundingBox);
				obj->setIsPhysicsEnabled(true);

				RendererManager::getInstance().addObject(obj, RendererType::Object);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Object Manager", &m_showObjectManager);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Lighting"))
		{
			if (ImGui::MenuItem("PointLight"))
			{
				Scene* activeScene = RendererManager::getInstance().getActiveScene();
				std::string* name = new std::string();
				name->assign(std::format("PointLight{}", activeScene->getPointLights().size()));

				auto light = new PointLight(name->c_str(), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f));
				activeScene->addPointLight(light);
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Light Manager", &m_showLightManager);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Viewport"))
		{
			ImGui::Checkbox("Show Debug Info", &m_showDebugInfo);

			if (ImGui::BeginMenu("Camera"))
			{
				if (ImGui::MenuItem("Add Pers"))
				{
					Camera* camera = new Camera(std::format("Camera{}", RendererManager::getInstance().getActiveScene()->getCameras().size()), false);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					RendererManager::getInstance().getActiveScene()->addCamera(camera);
				}

				if (ImGui::MenuItem("Add Ortho", nullptr, nullptr, false))
				{
					Camera* camera = new Camera(std::format("Camera{}", RendererManager::getInstance().getActiveScene()->getCameras().size()), true);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					RendererManager::getInstance().getActiveScene()->addCamera(camera);
				}

				for (auto& camera : RendererManager::getInstance().getActiveScene()->getCameras())
				{
					if (ImGui::Checkbox(camera->getName().c_str(), new bool(camera == RendererManager::getInstance().getActiveScene()->getActiveCamera())))
					{
						RendererManager::getInstance().getActiveScene()->setActiveCamera(camera);
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Render Mode"))
			{
				if (ImGui::Checkbox("Debug", new bool(RendererManager::getInstance().getRenderMode() == RenderMode::Debug)))
				{
					RendererManager::getInstance().setRenderMode(RenderMode::Debug);
				}

				if (ImGui::Checkbox("Render", new bool(RendererManager::getInstance().getRenderMode() == RenderMode::Render)))
				{
					RendererManager::getInstance().setRenderMode(RenderMode::Render);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::Button("Test File"))
			{
				OPENFILENAMEA ofn{};       // common dialog box structure
				char szFile[260];       // buffer for file name
				HWND hwnd = NULL;       // owner window

				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFile = szFile;
				// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
				// use the contents of szFile to initialize itself.
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(szFile);
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex = 1;
				ofn.lpstrFileTitle = NULL;
				ofn.nMaxFileTitle = 0;
				ofn.lpstrInitialDir = NULL;
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

				// Display the Open dialog box
				if (GetOpenFileNameA(&ofn) == TRUE)
				{
					//TODO IF USING SHADERLIB IN GAMEOBJECT, OBJECT WILL DELETE SHADERLIB SINCE IT DELETES ALSO ITS SHADER POINTER
					GameObject* obj = new GameObject(std::format("Object{}", RendererManager::getInstance().getTotalObjectCount()), ofn.lpstrFile, ShaderLib::getDebugShaderPtr(), ColliderType::BoundingBox);
					obj->setIsPhysicsEnabled(true);

					RendererManager::getInstance().addObject(obj, RendererType::Object);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ImguiRenderer::renderDebugInfo()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 windowPadding = style.WindowPadding;

	style.WindowRounding = 0.0f;
	style.WindowPadding = ImVec2(3.0f, 3.0f);

	ImGui::SetNextWindowPos(ImVec2(0.0f, 18.0f));
	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 20.0f));

	ImGui::Begin("FPS Bar", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
	{
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
	}
	ImGui::End();

	style.WindowRounding = 5.0f;
	style.WindowPadding = windowPadding;
}

void ImguiRenderer::renderObjectManager()
{
	ImGui::Begin("Object Manager", nullptr);
	{
		ImGui::BeginChild("Objects", ImVec2(200, 0), true);
		{
			if (ImGui::TreeNode("Objects"))
			{
				auto renderer = static_cast<ObjectRenderer*>(RendererManager::getInstance().getRenderer(RendererType::Object));

				for (auto& obj : renderer->getObjects())
				{
					ImGui::Selectable(obj->getName().c_str(), m_selectedObject == obj);
					if (ImGui::IsItemClicked())
					{
						if (m_selectedObject != nullptr)
						{
							m_selectedObject->setIsOutline(false);
						}

						m_selectedObject = obj;
						obj->setIsOutline(true);
					}
				}


				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Environment"))
			{
				auto renderer = static_cast<EnvironmentRenderer*>(RendererManager::getInstance().getRenderer(RendererType::Environment));

				for (auto& obj : renderer->getObjects())
				{
					ImGui::Selectable(obj->getName().c_str(), m_selectedObject == obj);
					if (ImGui::IsItemClicked())
					{
						m_selectedObject = obj;
					}
				}


				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Detail View", ImVec2(0, 0), true);
		{
			if (m_selectedObject != nullptr)
			{
				float* pos[3] = { &m_selectedObject->getPositionPtr()->x, &m_selectedObject->getPositionPtr()->y, &m_selectedObject->getPositionPtr()->z };
				float* scale[3] = { &m_selectedObject->getScalePtr()->x, &m_selectedObject->getScalePtr()->y, &m_selectedObject->getScalePtr()->z };
				float* ambient[3] = { &m_selectedObject->getMaterialPtr()->getAmbientPtr()->x, &m_selectedObject->getMaterialPtr()->getAmbientPtr()->y, &m_selectedObject->getMaterialPtr()->getAmbientPtr()->z };
				float* diffuse[3] = { &m_selectedObject->getMaterialPtr()->getDiffusePtr()->x, &m_selectedObject->getMaterialPtr()->getDiffusePtr()->y, &m_selectedObject->getMaterialPtr()->getDiffusePtr()->z };
				float* specular[3] = { &m_selectedObject->getMaterialPtr()->getSpecularPtr()->x, &m_selectedObject->getMaterialPtr()->getSpecularPtr()->y, &m_selectedObject->getMaterialPtr()->getSpecularPtr()->z };

				ImGui::Text("Selected Object: %s", m_selectedObject->getName().c_str());
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
					ImGui::InputFloat3("##1", *m_selectedObject->getRotationPtr());

					ImGui::Text("Scale");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX());
					ImGui::InputFloat3("##2", *scale);

					ImGui::Checkbox("Draw Wireframe", m_selectedObject->getDrawWireframePtr());

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Material"))
				{
					ImGui::ColorEdit3("Ambient", *ambient, ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::ColorEdit3("Diffuse", *diffuse, ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::ColorEdit3("Specular", *specular, ImGuiColorEditFlags_NoInputs);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(100.0f);
					ImGui::InputFloat("Shininess", m_selectedObject->getMaterialPtr()->getShininessPtr());


					ImGui::Text(std::format("Loaded Shader: {}", m_selectedObject->getShaderPtr()->getName()).c_str());

					static char loadShaderBuffer[256] = "";

					ImGui::InputText("##2354", loadShaderBuffer, IM_ARRAYSIZE(loadShaderBuffer));
					ImGui::SameLine();
					if (ImGui::Button("Load Shader"))
					{
						std::cout << "DISABLED!\n";
						// selectedObject->setShader(new Shader(loadShaderBuffer));
					}

					ImGui::Image((void*)(intptr_t)m_selectedObject->getTexture(), ImVec2(150, 150));
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
							m_selectedObject->loadTexture(m_selectedTexturePath);
						}
					}
					ImGui::EndChild();

					ImGui::TreePop();
				}

				if (m_selectedObject->getIsPhysicsEnabled())
				{
					float* vel[3] = { &m_selectedObject->getVelocityPtr()->x, &m_selectedObject->getVelocityPtr()->y, &m_selectedObject->getVelocityPtr()->z };

					if (ImGui::TreeNode("Physics"))
					{
						ImGui::Text("Mass");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat("##4523", m_selectedObject->getMassPtr());

						ImGui::Text("Velocity");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##2", *vel);

						ImGui::Text("Linear Drag");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat("##3", m_selectedObject->getLinearDragPtr());

						if (ImGui::Checkbox("Gravity", m_selectedObject->getIsGravityEnabledPtr()))
						{
							if (m_selectedObject->getIsGravityEnabled()) m_selectedObject->setIsPullToObjectEnabled(false);
						}

						if (ImGui::Checkbox("ObjectPull", m_selectedObject->getIsPullToObjectEnabledPtr()))
						{
							if (m_selectedObject->getIsPullToObjectEnabled()) m_selectedObject->setIsGravityEnabled(false);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Collisions"))
					{
						if (ImGui::Checkbox("Collision Enabled", m_selectedObject->getIsCollisionEnabledPtr()))
						{
							m_selectedObject->checkBuffers();
						}

						if (m_selectedObject->getIsCollisionEnabled())
						{
							ImGui::Checkbox("Draw Collision Box", m_selectedObject->getIsDrawColliderPtr());

							if (ImGui::Checkbox("Hide objects to far away for collision", &m_highlightCloseCollidableObjects))
							{
								if (PhysicEngine::getFocusedGameObject() == nullptr)
									PhysicEngine::setFocusedGameObject(m_selectedObject);
								else
									PhysicEngine::setFocusedGameObject(nullptr);
							}

							ImGui::BeginChild("Collider", ImVec2(0, 0), true);
							{
								float* anchor[3] = { &m_selectedObject->getColliderPtr()->getAnchorPositionPtr()->x, &m_selectedObject->getColliderPtr()->getAnchorPositionPtr()->y , &m_selectedObject->getColliderPtr()->getAnchorPositionPtr()->z };
								float* colliderScale[3] = { &m_selectedObject->getColliderPtr()->getScalePtr()->x, &m_selectedObject->getColliderPtr()->getScalePtr()->y , &m_selectedObject->getColliderPtr()->getScalePtr()->z };

								ImGui::Text("Anchor");
								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX());
								ImGui::InputFloat3("##4", *anchor);

								ImGui::Text("Scale");
								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX());
								ImGui::InputFloat3("##5", *colliderScale);

								ImGui::Spacing();

								if (ImGui::Button("##6", ImVec2(40, 0))) m_selectedObject->snapColliderToObject();
								ImGui::SameLine();
								ImGui::Text("Snap Collider To Object");

								ImGui::Text("IMPORTANT! If collider scale = object scale collider won't be visible!");
							}
							ImGui::EndChild();
						}

						ImGui::TreePop();
					}

					if (ImGui::Button("Delete"))
					{
						RendererManager::getInstance().deleteObject(*m_selectedObject, Object);
						m_selectedObject = nullptr;
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
			if (ImGui::TreeNode("PointLights"))
			{
				for (auto& light : RendererManager::getInstance().getActiveScene()->getPointLights())
				{
					ImGui::Selectable(light->getName().data(), m_selectedLight == light);
					if (ImGui::IsItemClicked())
					{
						m_selectedLight = light;
					}
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("DirectionalLights"))
			{
				for (auto& light : RendererManager::getInstance().getActiveScene()->getDirectionalLights())
				{
					ImGui::Selectable(light->getName().data(), m_selectedLight == light);
					if (ImGui::IsItemClicked())
					{
						m_selectedLight = light;
					}
				}

				ImGui::TreePop();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Detail View", ImVec2(0, 0), true);
		{
			if (m_selectedLight != nullptr)
			{
				ImGui::Text("Selected Light: %s", m_selectedLight->getName());
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

						renderLightMaterialView();

						ImGui::InputFloat("CutOff", light->getCutOffPtr());
						ImGui::SameLine();
						ImGui::InputFloat("OuterCutOff", light->getOuterCutOffPtr());
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
					}

					ImGui::TreePop();
				}

				if (ImGui::Button("Delete"))
				{
					switch (m_selectedLight->getLightType())
					{
					case LightType::Point:
						RendererManager::getInstance().getActiveScene()->deletePointLight(static_cast<PointLight*>(m_selectedLight));
						break;
					default:
						break;
					}

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
						if (IconItem(i, entry.path().filename().string().c_str(), AssetManager::getInstance().getIcon("folder"), itemSize))
						{
							m_selectedAssetPath = entry.path().string();
						}
					}
					else
					{
						if (IconItem(i, entry.path().filename().string().c_str(), AssetManager::getInstance().getIcon(entry.path().extension().string()), itemSize))
						{
							//GameObject* obj = new GameObject(std::format("{}{}", items[i].name, RendererManager::getInstance().getTotalObjectCount()), items[i].path, ShaderLib::getDebugShaderPtr(), ColliderType::BoundingBox);
							//obj->setIsPhysicsEnabled(true);

							//RendererManager::getInstance().addObject(obj, RendererType::Object);
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

void ImguiRenderer::renderLightMaterialView()
{
	float* ambient[3] = { &m_selectedLight->getAmbientPtr()->x, &m_selectedLight->getAmbientPtr()->y, &m_selectedLight->getAmbientPtr()->z };
	float* diffuse[3] = { &m_selectedLight->getDiffusePtr()->x, &m_selectedLight->getDiffusePtr()->y, &m_selectedLight->getDiffusePtr()->z };
	float* specular[3] = { &m_selectedLight->getSpecularPtr()->x, &m_selectedLight->getSpecularPtr()->y, &m_selectedLight->getSpecularPtr()->z };

	ImGui::ColorEdit3("Ambient", *ambient, ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine();
	ImGui::ColorEdit3("Diffuse", *diffuse, ImGuiColorEditFlags_NoInputs);
	ImGui::SameLine();
	ImGui::ColorEdit3("Specular", *specular, ImGuiColorEditFlags_NoInputs);
}


void ImguiRenderer::setImguiStyle()
{
	ImGuiStyle& style = ImGui::GetStyle();

	// Colors
	ImVec4* colors = style.Colors;
	colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.24f, 0.29f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
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

	// Adjust other settings
	style.WindowMinSize = ImVec2(1.0f, 1.0f);
	style.WindowRounding = 5.0f;
	style.FrameRounding = 4.0f;
	style.GrabRounding = 3.0f;
	style.ScrollbarRounding = 2.0f;

}


GameObject* ImguiRenderer::m_selectedObject;