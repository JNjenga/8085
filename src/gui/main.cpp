#include <imgui.h>
#include <string>
#include <sstream>
#include <cctype>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <stdio.h>
#include <fstream>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include "app.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// Forward declarations
static std::string read_source_code(const char* path);

// Globals
static bool should_close = false;

// App prooperties
static appname8085::AppName appname;

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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
    bool close_cpu = false;
    bool close_disassembley = false;
    char* str0 = "0x00\0";
    ImGuiTableFlags disassembley_flags = ImGuiTableFlags_Borders |  ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg;
    static bool close_mem_viewer= false;
    static char file_name[128] = "exampleasm/hello.asm";
    lib8085::Processor* cpu = appname.get_cpu();

    int reg_a = 0;
    int reg_b = 0;
    int reg_c = 0;
    int reg_d = 0;
    int reg_e = 0;
    int reg_h = 0;
    int reg_l = 0;

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    bool show_demo = true;
    float x_offset = 0;
    float y_offset = 0;
    float size_of_menu_bar;
    ImGuiID dockspace_id;

    // Main loop
    while (!glfwWindowShouldClose(window) && !should_close)
    {
        reg_a = cpu->reg_a;
        reg_b = cpu->reg_b;
        reg_c = cpu->reg_c;
        reg_d = cpu->reg_d;
        reg_e = cpu->reg_e;
        reg_h = cpu->reg_h;
        reg_l = cpu->reg_l;

        x_offset = 0;
        y_offset = 0;
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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
            x_offset += ImGui::GetWindowSize().x;
            y_offset += ImGui::GetWindowSize().y;
            size_of_menu_bar = ImGui::GetWindowSize().y;
            ImGui::EndMainMenuBar();
        }

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, 0));

        if(ImGui::Begin("ControlButtons", nullptr, ImGuiWindowFlags_NoDecoration))
        {
            ImGui::SetNextWindowSize(ImVec2(0, 40));
            ImGui::InputText("file path", file_name, IM_ARRAYSIZE(file_name));

            ImGui::SameLine();
            if(ImGui::Button("Assemble"))
            {
                // Read file

                std::string code = read_source_code(file_name);
                std::cout << code << std::endl;
                appname.assemble(code);
            }

            ImGui::SameLine();
            if(ImGui::Button("Reset"))
            {
                appname.reset();
            }

            ImGui::SameLine();
            ImGui::Button("Run");

            ImGui::SameLine();
            if(ImGui::Button("Step"))
            {
                appname.step();
            }

            ImGui::SameLine();
            ImGui::Button("Pause");

            x_offset += ImGui::GetWindowSize().x;
            y_offset += ImGui::GetWindowSize().y;
            ImGui::End();
        }

        ImGui::SetNextWindowPos(ImVec2(0, y_offset));
        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - y_offset + size_of_menu_bar));

        if(ImGui::Begin("MainApp", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus))
        {
            dockspace_id = ImGui::GetID("MainApp");
            ImGui::DockSpace(dockspace_id);

            //
            // Render CPU
            // 

            if(ImGui::Begin("Cpu", &close_cpu, ImGuiWindowFlags_NoCollapse))
            {

                ImGui::SeparatorText("Registers");

                if(ImGui::InputInt("A: ", &reg_a, 1))
                {
                    cpu->reg_a = static_cast<uint8_t>(reg_a);
                }

                if(ImGui::InputInt("B: ", &reg_b, 1))
                {
                    cpu->reg_b = static_cast<uint8_t>(reg_b);
                }

                if(ImGui::InputInt("C: ", &reg_c, 1))
                {
                    cpu->reg_c = static_cast<uint8_t>(reg_c);
                }

                if(ImGui::InputInt("D: ", &reg_d, 1))
                {
                    cpu->reg_d = static_cast<uint8_t>(reg_d);
                }

                if(ImGui::InputInt("E: ", &reg_e, 1))
                {
                    cpu->reg_e = static_cast<uint8_t>(reg_e);
                }

                if(ImGui::InputInt("H: ", &reg_h, 1))
                {
                    cpu->reg_h = static_cast<uint8_t>(reg_h);
                }

                if(ImGui::InputInt("L: ", &reg_l, 1))
                {
                    cpu->reg_l = static_cast<uint8_t>(reg_l);
                }

                ImGui::InputInt("PSW: ", (int*)&cpu->reg_c, 1);
                ImGui::InputInt("PC: ",  (int*)&cpu->program_counter, 1);
                ImGui::InputInt("SP: ",  (int*)&cpu->stack_pointer, 1);

                ImGui::SeparatorText("");
                // Status register
                ImGui::Checkbox("Sign     ", &cpu->sign);
                ImGui::SameLine();
                ImGui::Checkbox("Zero     ", &cpu->zero);
                ImGui::SameLine();
                ImGui::Checkbox("Aux Carry", &cpu->auxiliary_carry);

                ImGui::Checkbox("Parity   ", &cpu->parity);
                ImGui::SameLine();
                ImGui::Checkbox("Carry    ", &cpu->carry);



                ImGui::End();
            }

            if(ImGui::Begin("Disassembley", &close_disassembley, ImGuiWindowFlags_NoCollapse))
            {
                const int number_of_cols = 2;
                if(ImGui::BeginTable("DisasmCode", number_of_cols, disassembley_flags, ImVec2(0.0f, 0.0f)))
                {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    // ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Address");
                    ImGui::TableSetupColumn("Instruction");
                    ImGui::TableHeadersRow();

                    uint64_t address;

                    std::map<uint64_t, std::string> disassembly = appname.get_disassembly();

                    auto loc = disassembly.find(cpu->program_counter);

                    int start = std::distance(disassembly.begin(), loc);

                    if(start < 10)
                        start = 0;
                    else
                        start -= 10;

                    int end = start + 15;

                    if(end > disassembly.size())
                        end = disassembly.size();

                    std::map<uint64_t, std::string>::iterator it_start = disassembly.begin();
                    std::map<uint64_t, std::string>::iterator it_end   = disassembly.begin();

                    std::advance(it_start, start);
                    std::advance(it_end, end);

                    for(; it_start != it_end; it_start++)
                    {
                        ImGui::TableNextRow();

                        // Set background color
                        address = it_start->first;

                        if(address == cpu->program_counter)
                        {
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, ImGui::GetColorU32(ImVec4(0.7f, 0.3f, 0.3f, 1.00f)));
                        }

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("0x%04X", address);
                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text(it_start->second.c_str());
                    }
                    ImGui::EndTable();
                }

                ImGui::End();
            }

            if(ImGui::Begin("MemViewer", &close_mem_viewer, ImGuiWindowFlags_NoCollapse))
            {
                size_t mem_index = 0;
                size_t cols = 0x10;
                ImGuiTableFlags flags = ImGuiTableFlags_Borders |  ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg;
                std::string hexval;

                if(ImGui::BeginTable("MemViewerTable", cols*2+1, flags, ImVec2(0.0f, 0.0f)))
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
                    size_t mem_size = 1 << 5;
                    for(int i = 0; i < mem_size; i += 0x10)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::Text("0x%04X", i);
                        std::string ascii_string;
                        uint8_t val;
                        for(int j = 0; j < cols; j ++)
                        {
                            val = cpu->mem[mem_index++];
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
                ImGui::End();
            }

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

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

std::string read_source_code(const char* path)
{
    std::ifstream file(path);
    std::string line;
    std::string code = "";

    if(!file.bad())
    {
        while(std::getline(file, line))
        {
            code += line + "\n";
        }

        code += "\n";
    }
    else
    {
        std::cout << "Couldn't read file\n";
    }
    std::cout << "Read code:\n";
    std::cout << code ;
    std::cout << "==================\n";

    file.close();

    return code;
}
