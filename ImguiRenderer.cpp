#include "ImguiRenderer.hpp"
#include "ObjectLoader.hpp"

#include <windows.h>
#include <commdlg.h>
#include <iostream>

void ImguiRenderer::renderMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Cube"))
			{
				auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
				if (value != nullptr)
				{
					auto renderer = static_cast<ObjectRenderer*>(value);
					GameObject* obj = new GameObject(std::format("Cube{}", renderer->getObjects().size()), ".\\Assets\\Objects\\Cube.obj", ShaderLib::getDebugShaderPtr(), ColliderType::BoundingBox);
					obj->setIsPhysicsEnabled(true);

					renderer->addObject(obj);
				}
			}
			if (ImGui::MenuItem("Sphere"))
			{
				auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
				if (value != nullptr)
				{
					auto renderer = static_cast<ObjectRenderer*>(value);
					GameObject* obj = new GameObject(std::format("Sphere{}", renderer->getObjects().size()), ".\\Assets\\Objects\\Sphere.obj", ShaderLib::getDebugShaderPtr(), ColliderType::Circular);
					obj->setIsPhysicsEnabled(true);

					renderer->addObject(obj);
				}
			}
			if (ImGui::MenuItem("StressTest"))
			{
				auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
				if (value != nullptr)
				{
					auto renderer = static_cast<ObjectRenderer*>(value);
					GameObject* obj = new GameObject(std::format("StressTest{}", renderer->getObjects().size()), ".\\Assets\\Objects\\StressTest.obj", ShaderLib::getDebugShaderPtr(), ColliderType::BoundingBox);
					obj->setIsPhysicsEnabled(true);

					renderer->addObject(obj);
				}
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
				Scene* activeScene = RendererPipeline::getActiveScenePtr();
				std::string* name = new std::string();
				name->assign(std::format("PointLight{}", activeScene->getPointLightsPtr()->size()));

				activeScene->addPointLight(new PointLight(name->c_str(), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)));
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::Checkbox("Light Manager", &showLightManager);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Viewport"))
		{
			ImGui::Checkbox("Show Debug Info", &m_showDebugInfo);

			if (ImGui::BeginMenu("Camera"))
			{
				if (ImGui::MenuItem("Add Pers"))
				{
					Camera* camera = new Camera(std::format("Camera{}", RendererPipeline::getActiveScenePtr()->getCamerasPtr()->size()), false);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					RendererPipeline::getActiveScenePtr()->addCamera(camera);
				}

				if (ImGui::MenuItem("Add Ortho", nullptr, nullptr, false))
				{
					Camera* camera = new Camera(std::format("Camera{}", RendererPipeline::getActiveScenePtr()->getCamerasPtr()->size()), true);
					camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

					RendererPipeline::getActiveScenePtr()->addCamera(camera);
				}

				for (int i = 0; i < RendererPipeline::getActiveScenePtr()->getCamerasPtr()->size(); i++)
				{
					if (ImGui::Checkbox(RendererPipeline::getActiveScenePtr()->getCamerasPtr()->at(i)->getName().c_str(), new bool(i == RendererPipeline::getActiveScenePtr()->getActiveCameraIndex())))
					{
						RendererPipeline::getActiveScenePtr()->setActiveCamera(i);
					}
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Render Mode"))
			{
				if (ImGui::MenuItem("Debug"))
				{
					RendererPipeline::setRenderMode(RenderMode::Debug);
				}

				if (ImGui::MenuItem("Render"))
				{
					RendererPipeline::setRenderMode(RenderMode::Render);
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::Button("Test File"))
			{
				OPENFILENAMEA ofn;       // common dialog box structure
				char szFile[260];       // buffer for file name
				HWND hwnd = NULL;       // owner window
				HANDLE hf;              // file handle

				// Initialize OPENFILENAME
				ZeroMemory(&ofn, sizeof(ofn));
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
					auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
					if (value != nullptr)
					{
						auto renderer = static_cast<ObjectRenderer*>(value);
						GameObject* obj = new GameObject(std::format("Cube{}", renderer->getObjects().size()), ofn.lpstrFile, ShaderLib::getDebugShaderPtr(), ColliderType::BoundingBox);
						obj->setIsPhysicsEnabled(true);

						renderer->addObject(obj);
					}
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
				auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
				if (value != nullptr)
				{
					auto renderer = static_cast<ObjectRenderer*>(value);

					for (size_t i = 0; i < renderer->getObjects().size(); i++)
					{
						auto obj = renderer->getObjects()[i];

						ImGui::Selectable(obj->getName().c_str(), selectedObject == obj);
						if (ImGui::IsItemClicked())
						{
							selectedObject = obj;
						}
					}
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Environment"))
			{
				auto value = RendererPipeline::getRendererMap().find(RendererType::Environment)->second;
				if (value != nullptr)
				{
					auto renderer = static_cast<EnvironmentRenderer*>(value);

					for (size_t i = 0; i < renderer->getObjects().size(); i++)
					{
						auto obj = renderer->getObjects()[i];

						ImGui::Selectable(obj->getName().c_str(), selectedObject == obj);
						if (ImGui::IsItemClicked())
						{
							selectedObject = obj;
						}
					}
				}

				ImGui::TreePop();
			}
		}
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Detail View", ImVec2(0, 0), true);
		{
			if (selectedObject != nullptr)
			{
				float* pos[3] = { &selectedObject->getPositionPtr()->x, &selectedObject->getPositionPtr()->y, &selectedObject->getPositionPtr()->z };
				float* scale[3] = { &selectedObject->getScalePtr()->x, &selectedObject->getScalePtr()->y, &selectedObject->getScalePtr()->z };
				float* ambient[3] = { &selectedObject->getMaterialPtr()->getAmbientPtr()->x, &selectedObject->getMaterialPtr()->getAmbientPtr()->y, &selectedObject->getMaterialPtr()->getAmbientPtr()->z };
				float* diffuse[3] = { &selectedObject->getMaterialPtr()->getDiffusePtr()->x, &selectedObject->getMaterialPtr()->getDiffusePtr()->y, &selectedObject->getMaterialPtr()->getDiffusePtr()->z };
				float* specular[3] = { &selectedObject->getMaterialPtr()->getSpecularPtr()->x, &selectedObject->getMaterialPtr()->getSpecularPtr()->y, &selectedObject->getMaterialPtr()->getSpecularPtr()->z };

				ImGui::Text("Selected Object: %s", selectedObject->getName().c_str());
				ImGui::Separator();

				if (ImGui::TreeNode("Object"))
				{
					ImGui::Text("Position");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX());
					ImGui::InputFloat3("##0", *pos);

					ImGui::Text("Scale");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX());
					ImGui::InputFloat3("##1", *scale);

					ImGui::Checkbox("Draw Wireframe", selectedObject->getIsDrawWireframePtr());

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
					ImGui::InputFloat("Shininess", selectedObject->getMaterialPtr()->getShininessPtr());


					ImGui::Text(std::format("Loaded Shader: {}", selectedObject->getShaderPtr()->getName()).c_str());

					static char loadShaderBuffer[256] = "";

					ImGui::InputText("##2354", loadShaderBuffer, IM_ARRAYSIZE(loadShaderBuffer));
					ImGui::SameLine();
					if (ImGui::Button("Load Shader"))
					{
						std::cout << "DISABLED!\n";
						// selectedObject->setShader(new Shader(loadShaderBuffer));
					}

					ImGui::Image((void*)(intptr_t)selectedObject->getTexture(), ImVec2(150, 150));
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
							selectedObject->loadTexture(selectedTexturePath);
						}
					}
					ImGui::EndChild();

					ImGui::TreePop();
				}

				if (selectedObject->getIsPhysicsEnabled())
				{
					float* vel[3] = { &selectedObject->getVelocityPtr()->x, &selectedObject->getVelocityPtr()->y, &selectedObject->getVelocityPtr()->z };

					if (ImGui::TreeNode("Physics"))
					{
						ImGui::Text("Mass");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat("##4523", selectedObject->getMassPtr());

						ImGui::Text("Velocity");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##2", *vel);

						ImGui::Text("Linear Drag");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat("##3", selectedObject->getLinearDragPtr());

						if (ImGui::Checkbox("Gravity", selectedObject->getIsGravityEnabledPtr()))
						{
							if (selectedObject->getIsGravityEnabled()) selectedObject->setIsPullToObjectEnabled(false);
						}

						if (ImGui::Checkbox("ObjectPull", selectedObject->getIsPullToObjectEnabledPtr()))
						{
							if (selectedObject->getIsPullToObjectEnabled()) selectedObject->setIsGravityEnabled(false);
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Collisions"))
					{
						if (ImGui::Checkbox("Collision Enabled", selectedObject->getIsCollisionEnabledPtr()))
						{
							selectedObject->checkBuffers();
						}

						if (selectedObject->getIsCollisionEnabled())
						{
							ImGui::Checkbox("Draw Collision Box", selectedObject->getIsDrawColliderPtr());

							if (ImGui::Checkbox("Hide objects to far away for collision", &highlightCloseCollidableObjects))
							{
								if (PhysicEngine::getFocusedGameObject() == nullptr)
									PhysicEngine::setFocusedGameObject(selectedObject);
								else
									PhysicEngine::setFocusedGameObject(nullptr);
							}

							ImGui::BeginChild("Collider", ImVec2(0, 0), true);
							{
								float* anchor[3] = { &selectedObject->getColliderPtr()->getAnchorPositionPtr()->x, &selectedObject->getColliderPtr()->getAnchorPositionPtr()->y , &selectedObject->getColliderPtr()->getAnchorPositionPtr()->z };
								float* colliderScale[3] = { &selectedObject->getColliderPtr()->getScalePtr()->x, &selectedObject->getColliderPtr()->getScalePtr()->y , &selectedObject->getColliderPtr()->getScalePtr()->z };

								ImGui::Text("Anchor");
								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX());
								ImGui::InputFloat3("##4", *anchor);

								ImGui::Text("Scale");
								ImGui::SameLine();
								ImGui::SetCursorPosX(ImGui::GetCursorPosX());
								ImGui::InputFloat3("##5", *colliderScale);

								ImGui::Spacing();

								if (ImGui::Button("##6", ImVec2(40, 0))) selectedObject->snapColliderToObject();
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
						auto value = RendererPipeline::getRendererPtr(RendererType::Object);
						if (value != nullptr)
						{
							auto renderer = static_cast<ObjectRenderer*>(value);

							PhysicEngine::removeObject(selectedObject);
							renderer->removeObject(selectedObject);

							delete selectedObject;
							selectedObject = nullptr;
						}
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
				for (size_t i = 0; i < RendererPipeline::getActiveScenePtr()->getPointLightsPtr()->size(); i++)
				{
					auto light = RendererPipeline::getActiveScenePtr()->getPointLightsPtr()->at(i);

					ImGui::Selectable(light->getName(), selectedLight == light);
					if (ImGui::IsItemClicked())
					{
						selectedLight = light;
					}
				}

				ImGui::TreePop();
			}
			if (ImGui::TreeNode("DirectionalLights"))
			{
				auto light = RendererPipeline::getActiveScenePtr()->getDirectionalLightPtr();

				ImGui::Selectable(light->getName(), selectedLight == light);
				if (ImGui::IsItemClicked())
				{
					selectedLight = light;
				}

				ImGui::TreePop();
			}
		}
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("Detail View", ImVec2(0, 0), true);
		{
			if (selectedLight != nullptr)
			{
				ImGui::Text("Selected Light: %s", selectedLight->getName());
				ImGui::Separator();

				if (ImGui::TreeNode("Light"))
				{
					if (selectedLight->getLightType() == LightType::Point)
					{
						PointLight* light = static_cast<PointLight*>(selectedLight);
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
					else if (selectedLight->getLightType() == LightType::Spot)
					{
						SpotLight* light = static_cast<SpotLight*>(selectedLight);
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
						DirectionalLight* light = static_cast<DirectionalLight*>(selectedLight);
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
					switch (selectedLight->getLightType())
					{
					case LightType::Point:
						RendererPipeline::getActiveScenePtr()->deletePointLight(static_cast<PointLight*>(selectedLight));
						break;
					default:
						break;
					}

					selectedLight = nullptr;
				}
			}
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void ImguiRenderer::renderLightMaterialView()
{
	float* ambient[3] = { &selectedLight->getAmbientPtr()->x, &selectedLight->getAmbientPtr()->y, &selectedLight->getAmbientPtr()->z };
	float* diffuse[3] = { &selectedLight->getDiffusePtr()->x, &selectedLight->getDiffusePtr()->y, &selectedLight->getDiffusePtr()->z };
	float* specular[3] = { &selectedLight->getSpecularPtr()->x, &selectedLight->getSpecularPtr()->y, &selectedLight->getSpecularPtr()->z };

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


GameObject* ImguiRenderer::selectedObject;