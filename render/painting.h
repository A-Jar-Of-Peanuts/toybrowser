#ifndef PAINTING_H
#define PAINTING_H

#include <string>
#include <tuple>
#include <unordered_map>
#include <future>
#include "../parsers/layout.h"

std::tuple<int, int, int> hexToRGB(unsigned int hexValue);
float lenToFloat(const std::string &input);
void draw(void *lb, int r, std::unordered_map<std::string, void *> inherit, float ScrollX, float ScrollY);
void RenderSearchBarAndButton();
LayoutBox* setup(std::string html);
std::future<std::string> getCSS(Node *i);
void loadFonts(Node *i);

#endif // PAINTING_H