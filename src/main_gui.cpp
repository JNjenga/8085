#include <imgui.h>
#include <string>
#include <sstream>
#include <cctype>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Forward declarations
static void ShowMenuBar();
static void ShowCodeViewer();
static void ShowMemViewer();
static void ShowDisassembley();
static void ShowCPU();

// Globals
static bool should_close = false;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


// Main code
int main(int, char**)
{
    const int WIDTH  = 1280;
    const int HEIGHT = 720;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "8085 emulator", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    bool show_demo = true;

    // Main loop
    while (!glfwWindowShouldClose(window) && !should_close)
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);


        if(ImGui::Begin("Fullscreen app window", nullptr, flags))
        {
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ShowMenuBar();
            ShowCodeViewer();
            ImGui::SameLine();
            ShowCPU();
            ShowMemViewer();
        }
        ImGui::End();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

static void ShowMenuBar()
{
    if(ImGui::BeginMainMenuBar())
    {
        if(ImGui::BeginMenu("File"))
        {
            if(ImGui::MenuItem("Exit"))
            {
                should_close = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
static void ShowDisassembley()
{
    if(ImGui::BeginChild("Disassebley", ImVec2(ImGui::GetContentRegionAvail().x * 0.495, ImGui::GetContentRegionAvail().y * 0.7), ImGuiWindowFlags_None))
    {
        ImGui::SeparatorText("Disasm Viewer");
        ImGuiTableFlags flags = ImGuiTableFlags_Borders |  ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg;
        if(ImGui::BeginTable("DisasmCode", 3, flags, ImVec2(0.0f, 0.0f)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Opcode", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("Operand", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableHeadersRow();

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("0x600");
            ImGui::TableNextColumn();
            ImGui::Text("JMP");
            ImGui::TableNextColumn();
            ImGui::Text("0x5000");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("0x600");
            ImGui::TableNextColumn();
            ImGui::Text("ADD A");
            ImGui::TableNextColumn();
            ImGui::Text("-");

            ImGui::EndTable();
        }

        ImGui::EndChild();
    }
}

static void ShowCodeViewer()
{
    ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
    if(ImGui::BeginChild("Tabs", ImVec2(ImGui::GetContentRegionAvail().x * 0.4, ImGui::GetContentRegionAvail().y * 0.7), ImGuiWindowFlags_None))
    {
        ImGui::SeparatorText("Code Editor");
        if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
        {
            if (ImGui::BeginTabItem("asm.txt"))
            {
                static char text[1024 * 16] =
                    "/*\n"
                    " The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
                    " the hexadecimal encoding of one offending instruction,\n"
                    " more formally, the invalid operand with locked CMPXCHG8B\n"
                    " instruction bug, is a design flaw in the majority of\n"
                    " Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
                    " processors (all in the P5 microarchitecture).\n"
                    "*/\n\n"
                    "label:\n"
                    "\tlock cmpxchg8b eax\n";


                ImGuiInputTextFlags input_flags = ImGuiInputTextFlags_AllowTabInput;
                ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y), input_flags);
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ShowDisassembley();
    }
}

static void ShowMemViewer()
{
    static uint8_t mem[1 << 16] = {0};
    if(ImGui::BeginChild("Mem editor", ImVec2(ImGui::GetContentRegionAvail().x * 0.7, ImGui::GetContentRegionAvail().y)))
    {
        ImGui::SeparatorText("Mem Editor");
        size_t mem_index = 0;
        size_t cols = 0x10;
        ImGuiTableFlags flags = ImGuiTableFlags_Borders |  ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg;
        std::string hexval;

        if(ImGui::BeginTable("MemViewer", cols*2+1, flags, ImVec2(0.0f, 0.0f)))
        {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Address",ImGuiTableColumnFlags_WidthFixed);
            std::stringstream ss;
            for(int j = 0; j < cols; j ++)
            {
                ss.str("");
                ss << std::hex << j;
                ImGui::TableSetupColumn(ss.str().c_str());
            }
            ImGui::TableHeadersRow();
            size_t mem_size = 1 << 16;
            for(int i = 0; i < mem_size; i += 0x10)
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::Text("0x%04X", i);
                std::string ascii_string;
                uint8_t val;
                for(int j = 0; j < cols; j ++)
                {
                    val = mem[mem_index++];
                    if(std::isprint(val))
                        ascii_string += val;
                    else
                        ascii_string += '.';

                    ImGui::TableNextColumn();
                    ImGui::Text("%02X", val);
                }
                for(auto & c : ascii_string)
                {
                    std::string cval(1, c);
                    ImGui::TableNextColumn();
                    ImGui::Text(cval.c_str());
                }
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }
}

static void ShowCPU()
{
    char* str0 = "0x00\0";

    if(ImGui::BeginChild("CPU", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 0.7), ImGuiWindowFlags_None))
    {
        ImGui::SeparatorText("CPU");

        ImGui::SeparatorText("Registers");

        ImGui::InputText("A: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("B: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("C: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("D: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("E: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("F: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("H: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("L: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("PSW: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("PC: ", str0, IM_ARRAYSIZE(str0));
        ImGui::InputText("SP: ", str0, IM_ARRAYSIZE(str0));

        ImGui::Separator();
        ImGui::Button("Assemble");
        ImGui::SameLine();
        ImGui::Button("Reset");
        ImGui::SameLine();
        ImGui::Button("Run");
        ImGui::SameLine();
        ImGui::Button("Step");
        ImGui::SameLine();
        ImGui::Button("Pause");


        ImGui::EndChild();
    }
    
}
