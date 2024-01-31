#include "ImguiRenderer.hpp"
#include "ObjectLoader.hpp"

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
					GameObject* obj = new GameObject(std::format("Cube{}", renderer->getObjects().size()), "\\Assets\\Objects\\Cube.obj", "debug", ColliderType::BoundingBox);
					obj->setIsPhysicsEnabled(true);

					renderer->addObject(obj);
				}
			}
			else if (ImGui::MenuItem("Sphere"))
			{
				auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
				if (value != nullptr)
				{
					auto renderer = static_cast<ObjectRenderer*>(value);
					GameObject* obj = new GameObject(std::format("Sphere{}", renderer->getObjects().size()), "\\Assets\\Objects\\Sphere.obj", "debug", ColliderType::Circular);
					obj->setIsPhysicsEnabled(true);

					renderer->addObject(obj);
				}
			}
			else if (ImGui::MenuItem("StressTest"))
			{
				auto value = RendererPipeline::getRendererMap().find(RendererType::Object)->second;
				if (value != nullptr)
				{
					auto renderer = static_cast<ObjectRenderer*>(value);
					GameObject* obj = new GameObject(std::format("StressTest{}", renderer->getObjects().size()), "\\Assets\\Objects\\StressTest.obj", "debug", ColliderType::BoundingBox);
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
		else if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Add Ortho"))
			{
				Camera* camera = new Camera(std::format("Camera{}", RendererPipeline::getCameras().size()), true);
				camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

				RendererPipeline::addCamera(camera);
			}

			if (ImGui::MenuItem("Add Pers"))
			{
				Camera* camera = new Camera(std::format("Camera{}", RendererPipeline::getCameras().size()), false);
				camera->setPosition(glm::vec3(0.0f, 0.0f, 1.0f));

				RendererPipeline::addCamera(camera);
			}

			ImGui::Checkbox("Show Debug Info", &m_showDebugInfo);

			for (size_t i = 0; i < RendererPipeline::getCameras().size(); i++)
			{
				if (ImGui::Checkbox(RendererPipeline::getCameras()[i]->getName().c_str(), new bool(i == RendererPipeline::getActiveCameraIndex())))
				{
					RendererPipeline::setActiveCamera(i);
				}
			}


			ImGui::EndMenu();
		}
		else if (ImGui::BeginMenu("Test"))
		{
			if (ImGui::Button("Test File"))
			{
				Mesh mesh;

				ObjectLoader::LoadObjFile("\\Assets\\Objects\\Cube.obj", &mesh);
				auto test = 0;
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
				float* color[4] = { &selectedObject->getMaterialPtr()->getColorPtr()->x, &selectedObject->getMaterialPtr()->getColorPtr()->y, &selectedObject->getMaterialPtr()->getColorPtr()->z, &selectedObject->getMaterialPtr()->getColorPtr()->w };

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
					ImGui::Text("Color");
					ImGui::SameLine();
					ImGui::SetCursorPosX(ImGui::GetCursorPosX());
					ImGui::ColorEdit4("Color", *color, ImGuiColorEditFlags_NoInputs);

					ImGui::Text(std::format("Loaded Shader: {}", selectedObject->getShaderPtr()->getName()).c_str());

					static char loadShaderBuffer[256] = "";

					ImGui::InputText("##2354", loadShaderBuffer, IM_ARRAYSIZE(loadShaderBuffer));
					ImGui::SameLine();
					if (ImGui::Button("Load Shader"))
					{
						selectedObject->setShader(new Shader(loadShaderBuffer));
					}

					ImGui::Image((void*)(intptr_t)selectedObject->getTexture(), ImVec2(150, 150));
					ImGui::SameLine();
					ImGui::BeginChild("Container", ImVec2(0, 150));
					{
						ImGui::BeginChild("Textures", ImVec2(0, 120), true);
						{
							renderFolderStructure("\\Assets\\Textures");
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
						ImGui::Text("Velocity");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat3("##2", *vel);

						ImGui::Text("Linear Drag");
						ImGui::SameLine();
						ImGui::SetCursorPosX(ImGui::GetCursorPosX());
						ImGui::InputFloat("##3", selectedObject->getLinearDragPtr());

						ImGui::Checkbox("Gravity", selectedObject->getIsGravityEnabledPtr());

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