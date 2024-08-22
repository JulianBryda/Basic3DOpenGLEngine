#pragma once
#include <format>
#include <filesystem>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "RendererBase.hpp"
#include "RendererManager.hpp"
#include "ObjectRenderer.hpp"
#include "Primitives.hpp"
#include "PhysicEngine.hpp"
#include "AssetManager.hpp"
#include "IconsFontAwesome5.h"

#include "PointLight.hpp"
#include "SpotLight.hpp"	

class ImguiRenderer : public RendererBase
{

public:

	ImguiRenderer(GLFWwindow* window) : RendererBase(RendererType::UserInterface)
	{
		m_pWindow = window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		io.Fonts->AddFontDefault();
		float baseFontSize = 13.0f;
		float iconFontSize = baseFontSize * 2.0f / 3.0f;

		// merge in icons from Font Awesome
		static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.GlyphMinAdvanceX = iconFontSize;
		io.Fonts->AddFontFromFileTTF(".\\Assets\\Fonts\\fa-solid-900.ttf", iconFontSize, &icons_config, icons_ranges);
		// use FONT_ICON_FILE_NAME_FAR if you want regular instead of solid

		setImguiStyle();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	~ImguiRenderer()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void render(Scene* activeScene) override
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// render functions here
		renderMenuBar();
		if (m_showDebugMenu) renderDebugMenu();
		if (m_showObjectManager) renderObjectManager();
		if (m_showLightManager) renderLightManager();
		if (m_showAssetManager) renderAssetManager();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void renderFolderStructure(const char* path)
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_directory())
			{
				if (ImGui::TreeNode(entry.path().filename().string().c_str()))
				{
					renderFolderStructure(entry.path().string().c_str());

					ImGui::TreePop();
				}
			}
			else
			{
				if (ImGui::Selectable(entry.path().filename().string().c_str(), !std::strcmp(m_selectedTexturePath, entry.path().string().c_str())))
				{
					std::strcpy(m_selectedTexturePath, entry.path().string().c_str());
				}
			}
		}
	}

	void renderAssetFolderStructure(const char* path)
	{
		renderAssetFolderStructure(std::filesystem::directory_iterator(path));
	}

	void renderAssetFolderStructure(const std::filesystem::directory_entry entry)
	{
		renderAssetFolderStructure(std::filesystem::directory_iterator(entry));
	}

	void renderAssetFolderStructure(std::filesystem::directory_iterator iterator)
	{
		for (const auto& entry : iterator)
		{
			if (entry.is_directory())
			{
				bool subDirectories = false;

				for (const auto& subEntry : std::filesystem::directory_iterator(entry))
				{
					if (subEntry.is_directory())
					{
						subDirectories = true;
						break;
					}
				}

				if (subDirectories)
				{
					if (ImGui::TreeNode(entry.path().filename().string().c_str()))
					{
						if (ImGui::IsItemClicked())
						{
							m_selectedAssetPath = entry.path().string();
						}

						renderAssetFolderStructure(entry);

						ImGui::TreePop();
					}
				}
				else
				{
					if (ImGui::Selectable(entry.path().filename().string().c_str(), false))
					{
						m_selectedAssetPath = entry.path().string();
					}
				}
			}
		}
	}

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

	// static

	static void addSelectedObject(GameObject* object)
	{
		if (object == nullptr) return;

		object->setIsOutline(true);
		m_selectedObjects.push_back(object);
	}

	static void removeSelectedObject(GameObject* object)
	{
		for (size_t i = 0; i < m_selectedObjects.size(); i++)
		{
			if (m_selectedObjects[i] == object)
			{
				m_selectedObjects.erase(m_selectedObjects.begin() + i);
				break;
			}
		}
	}

	static void setSelectedObject(GameObject* object)
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

	static std::vector<GameObject*>& getSelectedObjects()
	{
		return m_selectedObjects;
	}

	static GameObject* getSelectedObject()
	{
		return m_selectedObjects[m_selectedObjects.size() - 1];
	}

private:

	void setImguiStyle();

	void renderMenuBar();
	void renderDebugMenu();

	void renderObjectManager();
	void renderLightManager();
	void renderAssetManager();

	void renderLightMaterialView();

	// custom components 
	bool IconItem(int id, const char* text, GLuint imageId, const float itemSize);

	char m_selectedTexturePath[_MAX_PATH] = "";
	std::string m_selectedAssetPath = "";

	bool m_showDebugMenu = false,
		m_showObjectManager = false,
		m_highlightCloseCollidableObjects = false,
		m_showLightManager = false,
		m_showAssetManager = false;

	Light* m_selectedLight;

	GLFWwindow* m_pWindow;

	// static
	static std::vector<GameObject*> m_selectedObjects;
};