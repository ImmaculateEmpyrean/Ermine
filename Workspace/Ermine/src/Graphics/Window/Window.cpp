#include "stdafx.h"
#include "Window.h"

//This Is The Reason Why This Class Is Best Treated As An OpengL Version of The Window...
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Graphics/Renderer/OpenGLErrorChecker.h"

#include "imgui.h"
#include "InputSystem/imgui_impl_opengl3.h"
#include "InputSystem/imgui_impl_glfw.h"

#include "InputSystem/GlfwKeyCallbacks.h"

//In Future Do a Better Job By Moving This Function Somewhere else...
static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Ermine::Window::Window(std::string WindowTitle, std::pair<int, int> WindowDiamensions)
{
    glfwSetErrorCallback(glfw_error_callback);
    /* Initialize the library */
    if (!glfwInit())
    {
        STDOUTDefaultLog_Error("Failed To Initilize Glfw Hence Quitting Program");
        exit(-1); //Exit The Program If We Fail Something So Vital
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    WinPtr = glfwCreateWindow(WindowDiamensions.first, WindowDiamensions.second,
        WindowTitle.c_str(), NULL, NULL);
      
    if (!WinPtr)
    {
        STDOUTDefaultLog_Error("Failed To Draw Window Hence Quitting Program")
        glfwTerminate();
        exit(-1); //Exit The Program If We Fail Something So Vital
    }

    glfwMakeContextCurrent(WinPtr); //Made The Created Window Current Context..
    glfwSetFramebufferSizeCallback(WinPtr, framebuffer_size_callback);
    //glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        STDOUTDefaultLog_Error("Failed To Initialize GLAD Hence Quitting Program..");
        glfwTerminate();
        exit(-1);
    }
        
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    auto io = &ImGui::GetIO();
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

    //io->Fonts->AddFontFromFileTTF("AgencyFb.ttf", 20.0f);

    
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

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    //GLCall(glEnable(GL_DEPTH_TEST));


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

bool Ermine::Window::ShouldIQuit()
{
    return glfwWindowShouldClose(WinPtr);
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
