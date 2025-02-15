#pragma once
#include <imgui.h>
#include "../../Landscape.hpp"
#include "../Renderer.hpp"

class ObjectManager
{
public:

	ObjectManager(GLFWwindow* window)
	{
		m_pWindow = window;
	}

	~ObjectManager()
	{

	}

	void renderObjectList()
	{
		ImGui::Begin("Object List");
		{
			auto& objects = Renderer::getInstance().getActiveScene()->getObjects();
			for (int i = 0; i < objects.size(); i++)
			{
				auto& object = objects[i];

				ImGui::Selectable(object->getName().c_str(), isSelected(object));
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
						setSelectedObject(object);
					}
				}
			}
		}
		ImGui::End();
	}

	void renderObjectProperties()
	{
		ImGui::Begin("Object Properties");
		{
			if (m_selectedObjects.size() > 0)
			{
				GameObject* object = getSelectedObject();

				float* pos[3] = { &object->getPositionPtr()->x, &object->getPositionPtr()->y, &object->getPositionPtr()->z };
				float* scale[3] = { &object->getScalePtr()->x, &object->getScalePtr()->y, &object->getScalePtr()->z };

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
					if (ImGui::BeginCombo("Material", object->getMaterialPtr()->getName().c_str()))
					{
						for (auto& shader : MaterialLib::g_materials)
						{
							bool isSelected = object->getMaterialPtr() == shader.second;
							if (ImGui::Selectable(shader.second->getName().c_str(), isSelected))
								object->setMaterial(shader.second);
							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					ImGui::BeginListBox("Textures");
					{
						for (auto& texture : object->getMaterialPtr()->getTextures())
						{
							ImGui::BeginChild(texture->getId(), {50, 70});
							{
								ImGui::Image((void*)(intptr_t)texture->getId(), { 50, 50 });
								ImGui::Text("%i", texture->getId());
							}
							ImGui::EndChild();
							ImGui::SameLine();
						}
					}
					ImGui::EndListBox();

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

							//if (ImGui::Checkbox("Hide objects to far away for collision", &m_highlightCloseCollidableObjects))
							//{
							//	if (PhysicEngine::getFocusedGameObject() == nullptr)
							//		PhysicEngine::setFocusedGameObject(object);
							//	else
							//		PhysicEngine::setFocusedGameObject(nullptr);
							//}

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
				}
				else
				{
					Landscape* landscape = dynamic_cast<Landscape*>(object);
					if (landscape)
					{
						if (ImGui::TreeNode("Landscape"))
						{
							float* flatColor[3] = { &landscape->getFlatColorPtr()->x, &landscape->getFlatColorPtr()->y, &landscape->getFlatColorPtr()->z };
							float* slopeColor[3] = { &landscape->getSlopeColorPtr()->x, &landscape->getSlopeColorPtr()->y, &landscape->getSlopeColorPtr()->z };

							ImGui::ColorEdit3("Flat Color", *flatColor, ImGuiColorEditFlags_NoInputs);
							ImGui::SameLine();
							ImGui::ColorEdit3("Slope Color", *slopeColor, ImGuiColorEditFlags_NoInputs);

							ImGui::Text("Slope Start");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::SetNextItemWidth(150.0f);
							ImGui::SliderFloat("##32543", landscape->getSlopeStartPtr(), 0.f, 1.f);
							ImGui::SameLine();
							ImGui::Text("Slope End");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::SetNextItemWidth(150.0f);
							ImGui::SliderFloat("##3646", landscape->getSlopeEndPtr(), 0.f, 1.f);

							ImGui::Spacing();
							ColoredText("Terrain Generation", IM_COL32(130, 130, 130, 255));
							ImGui::Separator();

							int* resolution[2] = { landscape->getResolutionXPtr(), landscape->getResolutionYPtr() };

							ImGui::Text("Seed");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputInt("##582345", landscape->getSeedPtr());

							ImGui::Text("Detail");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputInt("##2946", landscape->getOctavesPtr());

							ImGui::Text("Resolution");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputInt2("##58275", *resolution);

							ImGui::Text("Frequency");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputFloat("##58245", landscape->getFrequencyPtr());

							ImGui::Text("Height");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputFloat("##2498", landscape->getHeightPtr());

							if (ImGui::Button("Update"))
							{
								landscape->update();
							}

							ImGui::Spacing();
							ColoredText("Hydraulic erosion", IM_COL32(130, 130, 130, 255));
							ImGui::Separator();

							static int dropCount = 0;
							static float depositionRate = 0.3f;
							static float evapRate = 0.001f;
							static float friction = 0.05f;

							ImGui::Text("Deposition Rate");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputFloat("##289467", &depositionRate);

							ImGui::Text("Evap Rate");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputFloat("##35355", &evapRate);

							ImGui::Text("Friction");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputFloat("##35366", &friction);

							ImGui::Spacing();

							ImGui::Text("Droplet Count");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::InputInt("##3875", &dropCount);

							if (ImGui::Button("Simulate"))
							{
								landscape->simulateErosion(dropCount, depositionRate, evapRate, friction);
							}

							ImGui::Spacing();
							ColoredText("Terrain Modifier", IM_COL32(130, 130, 130, 255));
							ImGui::Separator();

							static float smoothingFactor = 1.f;

							ImGui::Text("Smoothing Factor");
							ImGui::SameLine();
							ImGui::SetCursorPosX(ImGui::GetCursorPosX());
							ImGui::SliderFloat("##83965", &smoothingFactor, 0, 1);

							if (ImGui::Button("Smoothen"))
							{
								landscape->smoothenHeightmap(smoothingFactor);
							}

							ImGui::TreePop();
						}
					}
				}

				ImGui::Spacing();
				ImGui::Separator();

				ImGui::Text("Indices: %i", object->getMesh().getIndices().size());
				ImGui::Text("Vertices: %i", object->getMesh().getVertices().size());

				if (ImGui::Button("Delete"))
				{
					Renderer::getInstance().deleteObject(*object);
					removeSelectedObject(object);
				}
			}
		}
		ImGui::End();
	}


	void addSelectedObject(GameObject* object)
	{
		if (object == nullptr) return;

		object->setIsOutline(true);
		m_selectedObjects.push_back(object);
	}

	void removeSelectedObject(GameObject* object)
	{
		for (int i = 0; i < m_selectedObjects.size(); i++)
		{
			if (m_selectedObjects[i] == object)
			{
				m_selectedObjects.erase(m_selectedObjects.begin() + i);
				break;
			}
		}
	}

	void setSelectedObject(GameObject* object)
	{
		for (auto& obj : m_selectedObjects)
		{
			obj->setIsOutline(false);
		}

		m_selectedObjects.clear();

		if (object != nullptr)
		{
			object->setIsOutline(true);
			m_selectedObjects.push_back(object);
		}
	}

	std::vector<GameObject*>& getSelectedObjects()
	{
		return m_selectedObjects;
	}

	GameObject* getSelectedObject()
	{
		if (m_selectedObjects.size() == 0) return nullptr;

		return m_selectedObjects[m_selectedObjects.size() - 1];
	}

private:

	bool isSelected(GameObject* object)
	{
		for (int i = 0; i < m_selectedObjects.size(); i++)
		{
			if (m_selectedObjects[i] == object)
			{
				return true;
			}
		}

		return false;
	}

	template <typename T>
	int getIndex(std::vector<T>& vector, T& value)
	{
		for (int i = 0; i < vector.size(); i++)
		{
			if (vector[i] == value)
			{
				return i;
			}
		}

		return -1;
	}

	void ColoredText(const char* text, ImU32 color)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, color);
		ImGui::Text(text);
		ImGui::PopStyleColor();
	}

	GLFWwindow* m_pWindow;
	std::vector<GameObject*> m_selectedObjects;

};