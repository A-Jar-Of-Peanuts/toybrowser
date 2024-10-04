cmake_minimum_required(VERSION 3.10)

project(toybrowser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.89.2  # pick a version
)

CPMAddPackage(
  NAME glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.3.4  # pick a version fr
)

file(GLOB_RECURSE SRC_FILES
    "render/*.cpp"
    "parsers/*.cpp"
    "examples/*.cpp"
)

file(GLOB_RECURSE HEADER_FILES
    "parsers/*.h"
    "examples/*.h"
)

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEADER_FILES}
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${glfw_SOURCE_DIR}/include ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} glfw opengl32)
elseif (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME} glfw ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKit_LIBRARY} ${CoreVideo_LIBRARY})
else()
    target_link_libraries(${PROJECT_NAME} glfw GL)
endif()