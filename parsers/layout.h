#ifndef LAYOUT_H   
#define LAYOUT_H

#include <vector>
#include <string>
#include "nodestruct.h"

struct Rect {
    int x;
    int y;
    int width;
    int height;
    Rect();
};

struct EdgeSizes {
    int left;
    int right;
    int top;
    int bottom;
    EdgeSizes();
};

struct Dimensions{
    Rect content;
    EdgeSizes padding;
    EdgeSizes border;
    EdgeSizes margin;
    Dimensions();
    Rect marg();
};

struct LayoutBox {
    const std::string INLINE = "inline";
    const std::string BLOCK = "block";
    const std::string NONE = "none";
    const std::string TEXT = "text";

    Dimensions dimensions;
    std::vector<LayoutBox*> children;
    std::string boxType;
    Node* box;
    LayoutBox();
    void layout(Dimensions dim);
    void layoutBlock(Dimensions dim);
    void layoutInline(Dimensions dim);
    void layoutText(Dimensions dim);
};

LayoutBox* buildLayoutTree(Node* node);

#endif