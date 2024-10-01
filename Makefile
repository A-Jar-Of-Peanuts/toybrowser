# Makefile for the project

# Compiler
CXX = g++
CXXFLAGS = -std=c++11

# Source files
SRC = parsers/tester.cpp \
      parsers/nodestruct.cpp \
      parsers/htmlparser.cpp \
      parsers/cssparser.cpp \
      parsers/selector.cpp \
	  parsers/stylenode.cpp 

# Header files
HEADERS = parsers/cssparser.h \
          parsers/selector.h \
	  	parsers/stylenode.h

# Output executable
TARGET = main

# Build target
all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean target
clean:
	$(RM) $(TARGET)

.PHONY: all clean
