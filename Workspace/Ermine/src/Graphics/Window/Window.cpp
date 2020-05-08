#include "stdafx.h"
#include "Window.h"

//This Is The Reason Why This Class Is Best Treated As An OpengL Version of The Window...
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "InputSystem/imgui_impl_opengl3.h"
#include "InputSystem/imgui_impl_glfw.h"

#include "InputSystem/GlfwKeyCallbacks.h"

//In Future Do a Better Job By Moving This Function Somewhere else...
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Ermine::Window::Window(std::string WindowTitle, std::pair<int, int> WindowDiamensions)
{
    glfwSetErrorCallback(glfw_error_callback);
    /* Initialize the library */
    if (!glfwInit())
    {
        STDOUTDefaultLog_Error("Failed To Initilize Glfw Hence Quitting Program");
        exit(-1); //Exit The Program If We Fail Something So Vital
    }

    WinPtr = glfwCreateWindow(WindowDiamensions.first, WindowDiamensions.second,
        WindowTitle.c_str(), NULL, NULL);
      
    if (!WinPtr)
    {
        STDOUTDefaultLog_Error("Failed To Draw Window Hence Quitting Program")
        glfwTerminate();
        exit(-1); //Exit The Program If We Fail Something So Vital
    }

    glfwMakeContextCurrent(WinPtr); //Made The Created Window Current Context..
    glfwSwapInterval(1);

    if(!gladLoadGL())//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //This Seems To Be Working
    {
        STDOUTDefaultLog_Error("Failed To Initialize GLAD Hence Quitting Program..")
        glfwTerminate();
        exit(-1); //Exit The Program If we Fail Something So Vital
    }
        
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); //ImGuiIO& io = ImGui::GetIO(); (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    io->Fonts->AddFontFromFileTTF("AgencyFb.ttf", 20.0f);

    
    //Start Set Callbacks To Sense Events...
    glfwSetKeyCallback(WinPtr, key_callback);
    glfwSetCharCallback(WinPtr, character_callback);
    glfwSetCursorPosCallback(WinPtr, cursor_position_callback);
    glfwSetMouseButtonCallback(WinPtr, mouse_button_callback);
    glfwSetScrollCallback(WinPtr, scroll_callback); 
    //Ended Set Callbacks To Sense Events...

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(WinPtr, true);
    ImGui_ImplOpenGL3_Init("#version 130");

#if defined(ER_DEBUG_DEVELOP) || defined(ER_DEBUG_SHIP)
    STDOUTDefaultLog_Trace("Initialized Window Printing Log Info : ");
    STDOUTDefaultLog_Trace("Vendor graphic card: {}", glGetString(GL_VENDOR));
    STDOUTDefaultLog_Trace("Renderer: {}", glGetString(GL_RENDERER));
    STDOUTDefaultLog_Trace("Version GL: {}", glGetString(GL_VERSION));
    STDOUTDefaultLog_Trace("Version GLSL: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
#endif

}

Ermine::Window::~Window()
{
    //glfwTerminate(); //Glfw Must Be Terminated With The Window.. Though In The Future One May Choose To Move This Into App Class..
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

Ermine::Window::Window(Window&& rhs)
    :
    WinPtr(rhs.WinPtr)
{
    rhs.WinPtr = nullptr;
}

Ermine::Window& Ermine::Window::operator=(Window&& rhs)
{
    WinPtr = rhs.WinPtr;
    rhs.WinPtr = nullptr;

    return *this;
}

void Ermine::Window::PreNewFrameProcess()
{
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /*Can BE Removed Section*/
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Ermine::Window::PostNewFrameProcess()
{
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(WinPtr, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    //glClearColor(0.2f,0.2f,0.2f,1.0f);
    //glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }

    glfwSwapBuffers(WinPtr);
}
