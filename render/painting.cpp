#include "../parsers/layout.h"
#include "../parsers/nodestruct.h"
#include "../parsers/htmlparser.h"
#include "../parsers/cssparser.h"
#include "../parsers/selector.h"
#include "../parsers/stylenode.h"
#include "../examples/fetcher.h"

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <OpenGL/gl3.h>
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>

using namespace std;
// Error callback for GLFW
void glfwErrorCallback(int error, const char* description) {
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

std::tuple<int, int, int> hexToRGB(std::string hex) {
    int r = 0, g = 0, b = 0;

    if (hex[0] == '#' && hex.length() == 7) {
        std::stringstream ss;
        ss << std::hex << hex.substr(1);  

        unsigned int hexValue;
        ss >> hexValue;

        r = (hexValue >> 16) & 0xFF; 
        g = (hexValue >> 8) & 0xFF;  
        b = hexValue & 0xFF;         
    }

    return std::make_tuple(r, g, b);
}

void draw(LayoutBox* lb, int r) {
    auto drawList = ImGui::GetBackgroundDrawList();
    ImVec2 Pos(lb->dimensions.content.x, lb->dimensions.content.y); 
    ImVec2 Size(lb->dimensions.content.width, lb->dimensions.content.height); 

    if (lb->box->nt.name == "Element") {
        if (lb->box->properties.find("color")!=lb->box->properties.end()) {
            tuple<int,int,int> t = hexToRGB(lb->box->properties["color"]->toString());
            drawList->AddRectFilled(Pos,
            ImVec2(Pos.x + Size.x, Pos.y + Size.y),
            IM_COL32(get<0>(t), get<1>(t), get<2>(t), 255));
        } 
    } else if (lb->box->nt.name == "Text") {
        ImVec2 textPos(Pos.x + Size.x / 2.0f, Pos.y + Size.y / 2.0f);
        ImVec2 textSize = ImGui::CalcTextSize(lb->box->nt.type.c_str());
        drawList->AddText(ImVec2(textPos.x - textSize.x / 2.0f, textPos.y - textSize.y / 2.0f), IM_COL32(255, 7, 255, 255), lb->box->nt.type.c_str());
    } else {
        drawList->AddRectFilled(Pos,
            ImVec2(Pos.x + Size.x, Pos.y + Size.y),
            IM_COL32(r, r, r, 255));
    }

    for(int i = 0; i<lb->children.size(); i++) {
        draw(lb->children[i], r+50+(i*1));
    }
}

LayoutBox* setup() {
    string html = filetostring("examples/ex1.html");
    string css = filetostring("examples/ex1.css");
    cout << html;
    Node* n = parseHTML(html);
    vector<Rule*> r = parseCSS(css);
    makeStyle(n, r);
    LayoutBox* root;
    if (n->nt.name == "Document") {
        root  = buildLayoutTree(n->children[0]);
    } else {
        root  = buildLayoutTree(n);
    }
    //LayoutBox* root = buildLayoutTree(n);
    Dimensions dim;
    dim.content.width = 800;
    root->layout(dim);
    return root;
}

int main() {
    // Set GLFW error callback
    glfwSetErrorCallback(glfwErrorCallback);
    LayoutBox* lb = setup();
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set the desired OpenGL version to 2.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "YanBrowser", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // (void)io;
    io.DisplaySize = ImVec2(800, 600); // Set display size
    ImGui::StyleColorsDark();

    const GLubyte* renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte* version = glGetString(GL_VERSION);   // Get version string

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120"); // Use a compatible GLSL version for OpenGL 2.1

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        draw(lb, 0);

        // Rendering
        ImGui::Render();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
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