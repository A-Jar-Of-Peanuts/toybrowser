#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include "painting.h"

#include "../examples/fetcher.h"
#include "../parsers/cssparser.h"
#include "../parsers/htmlparser.h"
#include "../parsers/layout.h"
#include "../parsers/nodestruct.h"
#include "../parsers/selector.h"
#include "../parsers/stylenode.h"
#include "../network/curlstuff.h"
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
#include <cmath>
#include <future>

using namespace std;

// Global variables
float OUTERWINDOWWIDTH = 800;
float OUTERWINDOWHEIGHT = 800;
float INNERWINDOWX = 50;
float INNERWINDOWY = 50;
float INNERWINDOWWIDTH = 700;
float INNERWINDOWHEIGHT = 700;

ImFont *font;
ImColor bodycolor = IM_COL32(255, 255, 255, 255);
LayoutBox* lb;
string urlcurrent = "";
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

void draw(LayoutBox *lb, int r, std::unordered_map<std::string, Value *> inherit, float ScrollX, float ScrollY)
{
    auto drawList = ImGui::GetWindowDrawList();

    ImGuiIO &io = ImGui::GetIO();
    io.FontGlobalScale = 1;
    if (inherit.find("font-size") != inherit.end())
    {
        io.FontGlobalScale = lenToFloat(inherit["font-size"]->toString());
    }

    ImVec2 Pos(lb->dimensions.content.x-ScrollX, lb->dimensions.content.y-ScrollY);
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
        if (lb->box->nt.name == "Element" && lb->box->nt.type.find("body") != string::npos)
        {
            tuple<int, int, int> t = hexToRGB(lb->box->properties["background-color"]->toString());
            bodycolor = IM_COL32(get<0>(t),
                                 get<1>(t),
                                 get<2>(t),
                                 255);
        }
    }
    if (lb->box->properties.find("font-size") != lb->box->properties.end())
    {
        inherit["font-size"] = lb->box->properties["font-size"];
    }

    for (int i = 0; i < lb->children.size(); i++)
    {
        draw(lb->children[i], r + 50 + (i * 1), inherit, ScrollX, ScrollY);
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
        std::cout << "Search Query: " << searchQuery << std::endl;
        urlcurrent = searchQuery;
        future<string> htmlfuture = GetHtmlContent(searchQuery);
        string html = htmlfuture.get();
        //std::cout<< "html: " << html << std::endl;
        lb = setup(html);
    }
}

std::future<std::string> getCSS(Node *i) {
    // Check if the current node is a Link element with a stylesheet
    if (i->nt.name == "Link" && i->nt.type.find("stylesheet") != std::string::npos) {
        std::string link = i->nt.type;
        link = link.substr(link.find("href:") + 5);  // Extract the href link
        link = link.substr(0, link.find(" "));       // Extract the actual link URL

        // Launch async task to get the CSS content
        return std::async(std::launch::async, [link]() {
            cout << urlcurrent+link;
            return GetHtmlContent(urlcurrent + link).get();  // Fetch CSS asynchronously
        });
    }

    // If no CSS is found, recursively search the children asynchronously
    if (!i->children.empty()) {
        std::vector<std::future<std::string>> futures;
        for (int j = 0; j < i->children.size(); j++) {
            futures.push_back(getCSS(i->children[j]));
        }

        // Wait for each child future to complete and return the first non-empty result
        for (auto& future : futures) {
            std::string css = future.get();
            if (!css.empty()) {
                return std::async(std::launch::async, [css]() {
                    return css;  // Return the first found CSS
                });
            }
        }
    }

    // Return an empty string if no CSS is found
    return std::async(std::launch::async, []() {
        return std::string("");
    });
}

LayoutBox *setup(string html)
{
    // html = filetostring("examples/ex1.html");
    string css = filetostring("examples/default.css");
    Node *n = parseHTML(html);

    future<string> futurecss = getCSS(n);

    css = css+ futurecss.get();
    cout << css << "what";
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
    dim.content.width = INNERWINDOWWIDTH;
    dim.content.x = INNERWINDOWX;
    dim.content.y = INNERWINDOWY;
    root->layout(dim, std::unordered_map<std::string, Value *>());
    return root;
}

int main()
{
    // Set GLFW error callback
    glfwSetErrorCallback(glfwErrorCallback);
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
    GLFWwindow *window = glfwCreateWindow(OUTERWINDOWWIDTH, OUTERWINDOWHEIGHT, "YanBrowser", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();      // (void)io;
    io.DisplaySize = ImVec2(OUTERWINDOWWIDTH, OUTERWINDOWHEIGHT); // Set display size
    ImGui::StyleColorsDark();

    const GLubyte *renderer = glGetString(GL_RENDERER); // Get renderer string
    const GLubyte *version = glGetString(GL_VERSION);   // Get version string

    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 120"); // Use a compatible GLSL version for OpenGL 2.1

    font = io.Fonts->AddFontFromFileTTF("assets/Roboto_Mono/RobotoMono-VariableFont_wght.ttf", 12);
    lb = setup("<html></html>");

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));                                                        
        ImGui::SetNextWindowSize(ImVec2(OUTERWINDOWWIDTH, OUTERWINDOWHEIGHT)); 
        ImGui::Begin("OuterWindow", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        auto drawList = ImGui::GetWindowDrawList();
        drawList->AddRectFilled(ImVec2(0, 0), ImVec2(OUTERWINDOWWIDTH, OUTERWINDOWHEIGHT), IM_COL32(255, 255, 255, 255)); 
        RenderSearchBarAndButton();

        ImGui::SetNextWindowPos(ImVec2(INNERWINDOWX, INNERWINDOWY));                                                                 
        ImGui::SetNextWindowSize(ImVec2(INNERWINDOWWIDTH, INNERWINDOWHEIGHT)); 
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));                                          
        ImGui::Begin("InnerWindow", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        float ScrollX = ImGui::GetScrollX();
        float ScrollY = ImGui::GetScrollY();

        ImVec2 contentSize = ImVec2(std::max(INNERWINDOWWIDTH, (float)lb->dimensions.content.width), std::max(INNERWINDOWHEIGHT, (float)lb->dimensions.content.height));
        ImGui::BeginChild("ScrollableRegion", contentSize, true, ImGuiWindowFlags_HorizontalScrollbar);
        auto drawList2 = ImGui::GetWindowDrawList();
        
        drawList2->AddRectFilled(ImVec2(INNERWINDOWX, INNERWINDOWY), ImVec2(contentSize.x+INNERWINDOWX, contentSize.y+INNERWINDOWY), bodycolor); 

        draw(lb, 0, std::unordered_map<std::string, Value *>(), ScrollX, ScrollY);

        ImGui::EndChild();
        ImGui::End();         
        ImGui::PopStyleVar(); 

        ImGui::End(); 

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