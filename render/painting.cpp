#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions

#include "../examples/fetcher.h"
#include "../parsers/cssparser.h"
#include "../parsers/htmlparser.h"
#include "../parsers/layout.h"
#include "../parsers/nodestruct.h"
#include "../parsers/selector.h"
#include "../parsers/stylenode.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#ifdef _WIN32
#include <GL/gl.h>
#elif defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

ImFont *font;
// Error callback for GLFW
void glfwErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

std::tuple<int, int, int> hexToRGB(std::string hex)
{
    int r = 0, g = 0, b = 0;

    if (hex[0] == '#' && hex.length() == 7)
    {
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

float lenToFloat(const std::string &input)
{
    std::string digits;
    std::string letters;

    // Iterate through each character in the input string
    for (char ch : input)
    {
        if (std::isdigit(ch))
        { // Check if the character is a digit
            digits.push_back(ch);
        }
        else if (std::isalpha(ch))
        { // Check if the character is a letter
            letters.push_back(ch);
        }
    }

    // returns the float value of the digits divided by the original default font size (12)
    float number = std::stof(digits) / 12;

    return number;
}

void draw(LayoutBox *lb, int r, std::unordered_map<std::string, Value *> inherit)
{
    auto drawList = ImGui::GetWindowDrawList();

    ImGuiIO &io = ImGui::GetIO();
    io.FontGlobalScale = 1;
    if (inherit.find("font-size") != inherit.end())
    {
        io.FontGlobalScale = lenToFloat(inherit["font-size"]->toString());
    }

    ImVec2 Pos(lb->dimensions.content.x, lb->dimensions.content.y);
    ImVec2 Size(lb->dimensions.content.width, lb->dimensions.content.height);

    if (lb->box->nt.name == "Element")
    {
        if (lb->box->properties.find("background-color") != lb->box->properties.end())
        {
            tuple<int, int, int> t = hexToRGB(lb->box->properties["background-color"]->toString());
            drawList->AddRectFilled(Pos,
                                    ImVec2(Pos.x + Size.x, Pos.y + Size.y),
                                    IM_COL32(get<0>(t), get<1>(t), get<2>(t), 255));
        }
        else if (inherit.find("background-color") != inherit.end())
        {
            tuple<int, int, int> t = hexToRGB(inherit["background-color"]->toString());
            drawList->AddRectFilled(Pos,
                                    ImVec2(Pos.x + Size.x, Pos.y + Size.y),
                                    IM_COL32(get<0>(t), get<1>(t), get<2>(t), 255));
        }
    }
    else if (lb->box->nt.name == "Text")
    {
        ImGui::PushFont(font);

        ImColor textColor = ImColor(0, 0, 0, 255);
        if (inherit.find("color") != inherit.end())
        {
            tuple<int, int, int> t = hexToRGB(inherit["color"]->toString());
            textColor = ImColor(get<0>(t), get<1>(t), get<2>(t), 255);
        }

        std::istringstream wordsStream(lb->box->nt.type);
        std::string word;
        std::string currentLine;
        int maxWidth = lb->dimensions.content.width;
        ImVec2 textPos(Pos.x + Size.x / 2.0f, Pos.y);

        while (wordsStream >> word)
        {
            std::string temp = currentLine + word + (currentLine.empty() ? "" : " ");
            ImVec2 textSize = ImGui::CalcTextSize(temp.c_str());
            // Check if adding the next word exceeds the max width
            if (textSize.x > maxWidth)
            {
                // Print the current line if it's not empty
                if (!currentLine.empty())
                {
                    ImVec2 ts = ImGui::CalcTextSize(currentLine.c_str());
                    drawList->AddText(ImVec2(textPos.x - ts.x / 2.0f, textPos.y), textColor, currentLine.c_str());
                    currentLine.clear();
                }
                // If the word itself exceeds the max width, split it
                while (word.length() > maxWidth)
                {
                    ImVec2 ts = ImGui::CalcTextSize(word.substr(0, maxWidth).c_str());
                    drawList->AddText(ImVec2(textPos.x - ts.x / 2.0f, textPos.y), textColor, word.substr(0, maxWidth).c_str());
                    word = word.substr(maxWidth);
                }
                currentLine = word;
                textPos.y += textSize.y;
            }
            else
            {
                // Add the word to the current line, with a space if it's not the first word
                if (!currentLine.empty())
                    currentLine += " ";
                currentLine += word;
            }
        }

        if (!currentLine.empty())
        {
            ImVec2 textSize = ImGui::CalcTextSize(currentLine.c_str());
            drawList->AddText(ImVec2(textPos.x - textSize.x / 2.0f, textPos.y), textColor, currentLine.c_str());
        }

        ImGui::PopFont();
    }
    else
    {
        drawList->AddRectFilled(Pos, ImVec2(Pos.x + Size.x, Pos.y + Size.y),
                                IM_COL32(r, r, r, 255));
    }

    // inherit properties
    if (lb->box->properties.find("color") != lb->box->properties.end())
    {
        inherit["color"] = lb->box->properties["color"];
    }
    if (lb->box->properties.find("background-color") != lb->box->properties.end())
    {
        inherit["background-color"] = lb->box->properties["background-color"];
    }
    if (lb->box->properties.find("font-size") != lb->box->properties.end())
    {
        inherit["font-size"] = lb->box->properties["font-size"];
    }

    for (int i = 0; i < lb->children.size(); i++)
    {
        draw(lb->children[i], r + 50 + (i * 1), inherit);
    }
}

void RenderSearchBarAndButton()
{
    // Static variable to hold the search query
    static char searchQuery[256] = "";

    // Create a text input for the search query
    ImGui::InputText("##searchbar", searchQuery, IM_ARRAYSIZE(searchQuery), ImGuiInputTextFlags_EnterReturnsTrue);

    // Same line to have the button next to the search bar
    ImGui::SameLine();

    // Button handling
    if (ImGui::Button("Enter"))
    {
        // TODO: Implement what happens when the button is pressed
        // For now, just print the search query to the console
        std::cout << "Search Query: " << searchQuery << std::endl;
    }
}
LayoutBox *setup()
{
    string html = filetostring("examples/ex1.html");
    string css = filetostring("examples/ex1.css");
    Node *n = parseHTML(html);
    vector<Rule *> r = parseCSS(css);
    makeStyle(n, r);
    LayoutBox *root;
    if (n->nt.name == "Document")
    {
        root = buildLayoutTree(n->children[0]);
    }
    else
    {
        root = buildLayoutTree(n);
    }
    Dimensions dim;
    dim.content.width = 800;
    root->layout(dim, std::unordered_map<std::string, Value *>());
    return root;
}

int main()
{
    // Set GLFW error callback
    glfwSetErrorCallback(glfwErrorCallback);
    LayoutBox *lb = setup();
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set the desired OpenGL version to 2.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(500, 500, "YanBrowser", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();       // (void)io;
    io.DisplaySize = ImVec2(500, 500); // Set display size
    ImGui::StyleColorsDark();

    const GLubyte *renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // Get version string

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120"); // Use a compatible GLSL version for OpenGL 2.1

    font = io.Fonts->AddFontFromFileTTF("assets/Roboto_Mono/RobotoMono-VariableFont_wght.ttf", 12);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));                              // Position at the top-left corner of the outer window
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y)); // Full width and 50px height for the search bar area
        ImGui::Begin("OuterWindow", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus|ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

        RenderSearchBarAndButton();

        // Ensure the inner window displays its colors correctly without being darkened out
        ImGui::SetNextWindowPos(ImVec2(0, 50));                                                            // Just below the search bar
        ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x + 50, ImGui::GetIO().DisplaySize.y - 50)); // Full width, height adjusted for search bar
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));                                    // Remove padding to avoid any background peeking through
        ImGui::Begin("InnerWindow", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        glClearColor(255, 255, 255, 1);
        glClear(GL_COLOR_BUFFER_BIT);


        // Ensure the draw function is called within the inner window context
        draw(lb, 0, std::unordered_map<std::string, Value *>());

        ImGui::End();         // End "InnerWindow"
        ImGui::PopStyleVar(); // Reset style to previous state

        ImGui::End(); // End "OuterWindow"

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