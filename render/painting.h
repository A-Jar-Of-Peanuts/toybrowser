#ifndef PAINTING_H
#define PAINTING_H

#include <string>
#include <tuple>
#include <unordered_map>
#include <../parsers/layout.h>

std::tuple<int, int, int> hexToRGB(unsigned int hexValue);
float lenToFloat(const std::string &input);
void draw(void *lb, int r, std::unordered_map<std::string, void *> inherit, float ScrollX, float ScrollY);
void RenderSearchBarAndButton();
LayoutBox* setup(std::string html);

#endif // PAINTING_H