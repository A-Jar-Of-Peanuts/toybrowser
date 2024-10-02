#include "fetcher.h"
#include <iostream>
#include <fstream>
#include <string>


std::string filetostring(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        return "Error: The file '" + filePath + "' was not found.";
    }

    std::string content;
    std::string line;

    while (std::getline(file, line)) {
        content += line + "\n"; // Append each line with a newline character
    }

    return content;
}