# Specify the minimum version for CMake
cmake_minimum_required(VERSION 3.10)

# Project's name
project(ToyBrowserProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(. imgui imgui/examples/libs/glfw/include /usr/local/include)

# Include CPM.cmake
include(cmake/CPM.cmake)

# Add GLFW via CPM
CPMAddPackage(
  NAME glfw
  GIT_TAG 3.3.4 # Specify the version of GLFW you want
  GITHUB_REPOSITORY glfw/glfw
  OPTIONS
    "GLFW_BUILD_DOCS OFF"
    "GLFW_BUILD_TESTS OFF"
    "GLFW_BUILD_EXAMPLES OFF"
)

CPMAddPackage(
  NAME imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.2  # pick a version
)


# Source files
set(SOURCES
    render/painting.cpp
    parsers/nodestruct.cpp
    parsers/htmlparser.cpp
    parsers/cssparser.cpp
    parsers/selector.cpp
    parsers/stylenode.cpp
    parsers/layout.cpp
    examples/fetcher.cpp
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

# Add an executable based on the source files
add_executable(ToyBrowser ${SOURCES})

target_link_libraries(ToyBrowser glfw)

# Platform-specific libraries
if(APPLE)
    target_link_libraries(ToyBrowser glfw ${GLFW_LIBRARIES} "-framework OpenGL" "-ldl")
elseif(WIN32)
    target_link_libraries(ToyBrowser glfw ${GLFW_LIBRARIES} opengl32)
elseif(UNIX AND NOT APPLE) # Linux
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(ToyBrowser glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} ${X11_LIBRARIES} dl pthread)
endif()