# Makefile for the project

# Compiler
CXX = g++
CXXFLAGS = -std=c++11 -I. -Iimgui -Iimgui/examples/libs/glfw/include -I/usr/local/include

PKG_CONFIG := pkg-config
GLFW_CFLAGS := $(shell $(PKG_CONFIG) --cflags glfw3)
GLFW_LDFLAGS := $(shell $(PKG_CONFIG) --libs glfw3)

# Source files
SRC = render/painting.cpp \
      parsers/nodestruct.cpp \
      parsers/htmlparser.cpp \
      parsers/cssparser.cpp \
      parsers/selector.cpp \
	  parsers/stylenode.cpp \
	  parsers/layout.cpp \
	  imgui/imgui.cpp \
	  imgui/imgui_demo.cpp \
	  imgui/imgui_draw.cpp \
	  imgui/imgui_widgets.cpp \
	  imgui/imgui_tables.cpp \
	  imgui/backends/imgui_impl_glfw.cpp \
	  imgui/backends/imgui_impl_opengl3.cpp

LIBS = -framework OpenGL -ldl -lglfw3

# Header files
HEADERS = parsers/cssparser.h \
          parsers/selector.h \
	  	parsers/stylenode.h \
		parsers/htmlparser.h \
		parsers/selector.h \
		parsers/layout.h

# Output executable
TARGET = main

# Build target
all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(GLFW_LDFLAGS) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

# Clean target
clean:
	$(RM) $(TARGET)

.PHONY: all clean
