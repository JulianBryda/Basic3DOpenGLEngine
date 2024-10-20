#pragma once
#include <format>
#include <filesystem>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "implot.h"

#include "../../Core/PhysicEngine.hpp"

#include "../../Utils/AssetManager.hpp"

#include "../../ThirdParty/IconsFontAwesome5.h"

#include "../../Scene/Lights/PointLight.hpp"
#include "../../Scene/Lights/SpotLight.hpp"	
#include "../../Scene/Scene.hpp"	

#include "../../Window/AppWindow.hpp"

#include "ShaderNodeEditor.hpp"
#include "ObjectManager.hpp"

class UserInterface
{

public:

	UserInterface()
	{
		m_pWindow = AppWindow::window;

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		float baseFontSize = 16.0f;
		float iconFontSize = baseFontSize * 2.0f / 3.0f;

		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arial.ttf", baseFontSize);

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
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
		ImGui_ImplOpenGL3_Init();

		m_pObjectManager = new ObjectManager(m_pWindow);
	}

	~UserInterface()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();

		delete m_pObjectManager;
	}

	static UserInterface& getInstance()
	{
		static UserInterface instance;
		return instance;
	}

	void render(Scene* activeScene)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

		// render functions here
		renderMenuBar();

		if (m_objectList) m_pObjectManager->renderObjectList();
		if (m_objectProperties) m_pObjectManager->renderObjectProperties();
		if (m_lightManager) renderLightManager();
		if (m_assetManager) renderAssetManager();
		
		if (m_fpsGraph) renderFpsGraph();
		if (m_sceneStats) renderSceneStats();

		static ShaderNodeEditor nodeEditor;

		if (m_nodeEditor) nodeEditor.render();

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

	// object manager redirections

	void addSelectedObject(GameObject* object)
	{
		m_pObjectManager->addSelectedObject(object);
	}

	void removeSelectedObject(GameObject* object)
	{
		m_pObjectManager->removeSelectedObject(object);
	}

	void setSelectedObject(GameObject* object)
	{
		m_pObjectManager->setSelectedObject(object);
	}

	std::vector<GameObject*>& getSelectedObjects()
	{
		return m_pObjectManager->getSelectedObjects();
	}

	GameObject* getSelectedObject()
	{
		return m_pObjectManager->getSelectedObject();
	}

private:

	void setImguiStyle();

	void renderMenuBar();

	void renderLightManager();
	void renderAssetManager();

	void renderFpsGraph();
	void renderSceneStats();

	void renderLightMaterialView();

	// custom components 
	bool IconItem(int id, const char* text, GLuint imageId, const float itemSize);
	inline void ColoredText(const char* text, ImU32 color);

	char m_selectedTexturePath[_MAX_PATH] = "";
	std::string m_selectedAssetPath = "";

	bool m_objectList = false,
		m_objectProperties = false,
		m_highlightCloseCollidableObjects = false,
		m_lightManager = false,
		m_assetManager = false,
		m_fpsGraph = false,
		m_sceneStats = false,
		m_nodeEditor = false;

	Light* m_selectedLight;

	GLFWwindow* m_pWindow;

	ObjectManager* m_pObjectManager;
};