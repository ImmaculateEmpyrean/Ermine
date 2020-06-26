#include "stdafx.h"
#include "MainWindow.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

#include "EngineUI/WindowHandler.h"

#include "EngineUI/DebugDevelopment/AssetsMenu/AddNewAssetsMenu/AddNewTextureWindow.h"
#include "EngineUI/DebugDevelopment/AssetsMenu/AddNewAssetsMenu/NewTileSet.h"
#include "EngineUI/DebugDevelopment/AssetsMenu/AddNewAssetsMenu/NewTileMap.h"

#include "EngineUI/DebugDevelopment/AssetsMenu/ViewAssetsMenu/TextureViewWizard.h"
#include "EngineUI/DebugDevelopment/AssetsMenu/ViewAssetsMenu/TilesetViewWizard.h"

Ermine::DebugMainWindow::DebugMainWindow()
{
    // Demonstrate the various window flags. Typically you would just use the default!
    bool no_titlebar = true;
    bool no_scrollbar = false;
    bool no_menu = false;
    bool no_move = true;
    bool no_resize = true;
    bool no_collapse = true;
    bool no_close = true;
    bool no_nav = true;
    bool no_background = true;
    bool no_bring_to_front = true;
    bool no_docking = true;

    WindowFlags = 0;
    if (no_titlebar)        WindowFlags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       WindowFlags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           WindowFlags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            WindowFlags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          WindowFlags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        WindowFlags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             WindowFlags |= ImGuiWindowFlags_NoNav;
    if (no_background)      WindowFlags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (no_docking)         WindowFlags |= ImGuiWindowFlags_NoDocking;
}

void Ermine::DebugMainWindow::Draw()
{
    int MainWindowWidth, MainWindowHeight, MainWindowXPosition, MainWindowYPosition;
    glfwGetWindowSize(glfwGetCurrentContext(), &MainWindowWidth,&MainWindowHeight);
    glfwGetWindowPos(glfwGetCurrentContext(), &MainWindowXPosition,&MainWindowYPosition);

   ImGui::Begin("Main Window DEBUG_DEVELOPMENT", (bool*)0, WindowFlags);

    ImGui::SetWindowSize({ (float)MainWindowWidth,(float)MainWindowHeight });
    ImGui::SetWindowPos({ (float)MainWindowXPosition,(float)MainWindowYPosition });
	ImGui::Text("You Will Be Working a Long time On Me ... ");


    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Quit", "", nullptr))
            {
                exit(0);
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::BeginMenu("Add New Assets"))
            {
                if (ImGui::MenuItem("Add Texture", "", nullptr))
                {
                    auto Handler = Ermine::WindowHandler::Get();
                    Handler->SubmitWindowFront(std::make_unique<Ermine::AddNewTextureWindow>(Ermine::AddNewTextureWindow()));
                }

                if (ImGui::MenuItem("Add TileSet", "", nullptr))
                {
                    auto Handler = Ermine::WindowHandler::Get();
                    Handler->SubmitWindowFront(std::make_unique<Ermine::AddNewTileSetWindow>(Ermine::AddNewTileSetWindow()));
                }
               
                if (ImGui::MenuItem("Add TileMap", "", nullptr))
                {
                    auto Handler = Ermine::WindowHandler::Get();
                    Handler->SubmitWindowFront(std::make_unique<Ermine::NewTileMap>(Ermine::NewTileMap()));
                }
                ImGui::EndMenu();
            }
            
            if (ImGui::BeginMenu("View Assets"))
            {
                if (ImGui::MenuItem("Texture"))
                {
                    auto Handler = Ermine::WindowHandler::Get();
                    Handler->SubmitWindowFront(std::make_unique<Ermine::TextureViewWizard>(Ermine::TextureViewWizard()));
                }


                if (ImGui::MenuItem("Tileset"))
                {
                    auto Handler = Ermine::WindowHandler::Get();
                    Handler->SubmitWindowFront(std::make_unique<Ermine::TilesetViewWizard>(Ermine::TilesetViewWizard()));
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    /*if(ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("FILE"))
        {
            static bool b1 = false, b2 = false;
            /*if (ImGui::MenuItem("Quit",nullptr,&b1))
            {
                exit(0);
            }
            ImGui::EndMenu();
        }
        /*if (ImGui::BeginMenu("Menu"))
        { 
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Examples"))
        {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }*/
    ImGui::End();
}
