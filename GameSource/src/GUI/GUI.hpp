#pragma once
#include "GUI.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


    void imguiBind::Init(GLFWwindow* window, const char* glsl_version)
    {
	  IMGUI_CHECKVERSION();
	  ImGui::CreateContext();
	  ImGuiIO& io = ImGui::GetIO();
	  // Setup Platform/Renderer bindings
	  ImGui_ImplGlfw_InitForOpenGL(window, true);
	  ImGui_ImplOpenGL3_Init(glsl_version);
	  ImGui::StyleColorsDark();
    }

    void imguiBind::NewFrame()
    {
	  // feed inputs to dear imgui, start new frame
	  ImGui_ImplOpenGL3_NewFrame();
	  ImGui_ImplGlfw_NewFrame();
	  ImGui::NewFrame();
    }

    void imguiBind::Update()
    {
	  ImGuiWindowFlags windowFlags =
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;

	  ImGui::Begin(
		"", NULL,
		windowFlags);  // Create a window called "Conan Logo" and append into it.
	  ImGui::End();
    }

    void imguiBind::Render()
    {
	  // Render dear imgui into screen
	  ImGui::Render();
	  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void imguiBind::Close()
    {
	  // Cleanup
	  ImGui_ImplOpenGL3_Shutdown();
	  ImGui_ImplGlfw_Shutdown();
	  ImGui::DestroyContext();
    }

    class CustomImGui : public imguiBind
    {
    public:
	  virtual void Update() override
	  {
		{

		    ImGuiWindowFlags tileSetsWindowFlags =
			  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		    ImGuiWindowFlags mapEditorWindowFlags =
			  ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
		}

		int i = 0;
		if(player.currentTarget)i = float(player.currentTarget->position.x);
		
		std::string ptrStr = std::to_string(i);
		ImGui::Text(ptrStr.c_str());

		std::string mobCount = "mobCount ";
		mobCount.append(std::to_string(gameState.mobs.mobList.size()));

		ImGui::Text(mobCount.c_str());

		std::string dtStr = std::to_string(deltaTime);

		ImGui::Text(dtStr.c_str());
	  }
	  std::vector<std::function<void()>> widgetDrawList;
    private:
    };


void drawImgui(imguiBind& imgui)
{
    imgui.NewFrame();
    imgui.Update();
    imgui.Render();
}