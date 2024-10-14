#include "layout.h"

#include <vector>
#include <string>
#include "nodestruct.h"
#include "selector.h"
#include <cmath>
#include <iostream>
#include <sstream>


using namespace std;

float lenToF(const std::string& input) {
    std::string digits;
    std::string letters;

    // Iterate through each character in the input string
    for (char ch : input) {
        if (std::isdigit(ch)) { // Check if the character is a digit
            digits.push_back(ch);
        } else if (std::isalpha(ch)) { // Check if the character is a letter
            letters.push_back(ch);
        }
    }

    float number = std::stof(digits);

    return number;
}

float calculateTextHeight(float fontSize, const std::string& text, float maxWidth) {
    // For Roboto Mono, we assume a more direct relationship between font size and character width
    float charWidth = fontSize * 0.43f; // Adjust this multiplier based on actual measurements or desired spacing
    
    // Line height for Roboto Mono, considering it's a monospace font
    float lineHeight = fontSize; // Adjusted for better approximation for monospace
    
    float totalHeight = 0;
    std::istringstream wordsStream(text);
    std::string word;
    float currentLineWidth = 0;

    while (wordsStream >> word) {
        // Check for newline characters within words and split accordingly
        size_t newlinePos = word.find('\n');
        if (newlinePos != std::string::npos) {
            // Handle text before the newline
            std::string beforeNewline = word.substr(0, newlinePos);
            float beforeNewlineWidth = beforeNewline.length() * charWidth;
            if (currentLineWidth + beforeNewlineWidth > maxWidth) {
                totalHeight += lineHeight; // Add height for the current line
                currentLineWidth = 0; // Reset for a new line
            }
            totalHeight += lineHeight; // Add height for the line ending with a newline character
            currentLineWidth = 0; // Reset for a new line

            // Handle text after the newline as a new word
            word = word.substr(newlinePos + 1);
        }

        float wordWidth = word.length() * charWidth;
        
        // Check if adding the next word exceeds the max width
        if (currentLineWidth + wordWidth > maxWidth) {
            totalHeight += lineHeight; // Add the height of the current line and reset
            currentLineWidth = 0;
        }
        
        // Account for a space if the current line is not empty
        if (currentLineWidth > 0) {
            currentLineWidth += charWidth; // Width of a space in a monospace font
        }
        
        currentLineWidth += wordWidth;
    }
    
    // Add the height of the last line if there's any text left
    if (currentLineWidth > 0) {
        totalHeight += lineHeight;
    }
    
    return totalHeight;
}


LayoutBox* buildLayoutTree(Node* node) {
    string dis = node->display();
    LayoutBox* lb = new LayoutBox(); 
    lb->dimensions = Dimensions();
    
    lb->box = node;
    if (dis == node->INLINE) {
        lb->boxType = node->INLINE;
    } else if (dis == node->TEXT) {
        lb->boxType = node->TEXT;
    } else if (dis == node->BLOCK) {
        lb->boxType = node->BLOCK;
    }
    for (int i = 0; i<node->children.size(); i++) {
        //
            string d = node->children[i]->display();
            if (d != node->NONE) {
                if (dis == node->INLINE) {
                    // TODO create anon block or smt idk
                    
                    lb->children.push_back(buildLayoutTree(node->children[i]));
                } else if (dis == node->BLOCK) {
                    lb->children.push_back(buildLayoutTree(node->children[i]));
                } else if (dis == node->NONE) {
                    cout <<"whaaa";

                } else if (dis == node->TEXT) {
                    lb->children.push_back(buildLayoutTree(node->children[i]));
                }
            }  
    }
    return lb; 
}

LayoutBox::LayoutBox() {

}

void LayoutBox::layout(Dimensions dim, std::unordered_map<std::string, Value*> inherit) {
    if (box->properties.find("font-size")!=box->properties.end()) {
        inherit["font-size"] = box->properties["font-size"];
    }
    if (boxType == box->INLINE) {
        layoutInline(dim, inherit);
    } else if (boxType == box->TEXT) {
        layoutText(dim, inherit);
    } else if (boxType == box->BLOCK) {
        layoutBlock(dim, inherit);
    }
}

void LayoutBox::layoutText(Dimensions dim, std::unordered_map<std::string, Value*> inherit) {
    dimensions = dim;
    if (inherit.find("font-size")!=inherit.end()) {
        dimensions.content.height = calculateTextHeight(lenToF(inherit["font-size"]->toString()), box->nt.type, dim.content.width);

    } else {
        dimensions.content.height = calculateTextHeight(12, box->nt.type, dim.content.width);
    }
}

void LayoutBox::layoutBlock(Dimensions dim, std::unordered_map<std::string, Value*> inherit) {
    this->dimensions.margin.left = 5;
    this->dimensions.margin.right = 5;
    this->dimensions.margin.top = 5;
    this->dimensions.margin.bottom = 5;

    if (box->properties.find("margin-left")!=box->properties.end()) {
        this->dimensions.margin.left = lenToF(box->properties["margin-left"]->toString());
    }
    if (box->properties.find("margin-right")!=box->properties.end()) {
        this->dimensions.margin.right = lenToF(box->properties["margin-right"]->toString());
    }   
    if (box->properties.find("margin-top")!=box->properties.end()) {
        this->dimensions.margin.top = lenToF(box->properties["margin-top"]->toString());
    }
    if (box->properties.find("margin-bottom")!=box->properties.end()) {
        this->dimensions.margin.bottom = lenToF(box->properties["margin-bottom"]->toString());
    }
    if (box->properties.find("margin")!=box->properties.end()) {
        float margin = lenToF(box->properties["margin"]->toString());
        this->dimensions.margin.left = margin;
        this->dimensions.margin.right = margin;
        this->dimensions.margin.top = margin;
        this->dimensions.margin.bottom = margin;
    }

    this->dimensions.content.x = dim.content.x + this->dimensions.margin.left;
    this->dimensions.content.y = dim.content.y + dim.content.height + this->dimensions.margin.top;
    this->dimensions.content.width = dim.content.width;

    if (dimensions.content.x+dimensions.content.width+dimensions.margin.right>dim.content.width+dim.content.x) {
        int difference = (dimensions.content.x+dimensions.content.width+dimensions.margin.right)-(dim.content.width+dim.content.x);
        dimensions.content.width-=difference;
    } else {
        dimensions.content.width+=dimensions.margin.right;
    }

    if (children.size() == 0) {
        dimensions.content.height = 100;
    }
    
    for(int i = 0; i<children.size(); i++) {
        children[i]->layout(dimensions, inherit);
        dimensions.content.height+=children[i]->dimensions.marg().height;
    }
}

void LayoutBox::layoutInline(Dimensions dim, std::unordered_map<std::string, Value*> inherit) {
    // TODO get margin properties idk lol dgaf
    this->dimensions.margin.left = 15;
    this->dimensions.margin.right = 15;
    this->dimensions.margin.top = 15;
    this->dimensions.margin.bottom = 15;

    this->dimensions.content.x = dim.content.x + this->dimensions.margin.left;
    this->dimensions.content.y = dim.content.y + dim.content.height + this->dimensions.margin.top;
    this->dimensions.content.width = dim.content.width;

    if (dimensions.content.x+dimensions.content.width+dimensions.margin.right>dim.content.width+dim.content.x) {
        int difference = (dimensions.content.x+dimensions.content.width+dimensions.margin.right)-(dim.content.width+dim.content.x);
        dimensions.content.width-=difference;
    } else {
        dimensions.content.width+=dimensions.margin.right;
    }

    if (children.size() == 0) {
        dimensions.content.height = 100;
    }
    
    for(int i = 0; i<children.size(); i++) {
        children[i]->layout(dimensions, inherit);
        dimensions.content.height+=children[i]->dimensions.marg().height;
    }

}

Rect Dimensions::marg() {
    Rect r;
    r.x = content.x+margin.left;
    r.y = content.y +margin.top;
    r.width = content.width+margin.left+margin.right;
    r.height = content.height+margin.bottom+margin.top;
    return r;
}

Dimensions::Dimensions() {
    content = Rect();
    padding = EdgeSizes();
    border = EdgeSizes();
    margin = EdgeSizes();
}

EdgeSizes::EdgeSizes() {
    left = 0;
    right = 0;
    top = 0;
    bottom = 0;
}

Rect::Rect() {
    x = 0;
    y = 0;
    width = 0;
    height = 0;
}

