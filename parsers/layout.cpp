#include "layout.h"

#include <vector>
#include <string>
#include "nodestruct.h"
#include "selector.h"

using namespace std;

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

void LayoutBox::layout(Dimensions dim) {
    if (boxType == box->INLINE) {
        layoutInline(dim);
    } else if (boxType == box->TEXT) {
        layoutText(dim);
    }
}

void LayoutBox::layoutText(Dimensions dim) {
    dimensions = dim;
    if (dimensions.content.height < 100) {
        dimensions.content.height = 100;
    }
}
void LayoutBox::layoutInline(Dimensions dim) {
    // TODO get margin properties idk lol dgaf
    // Length* width = new Length(10, "px");

    // Length* margin_left = new Length(0, "px");
    // Length* margin_right = new Length(0, "px");
    // Length* border_left = new Length(0, "px");
    // Length* border_right = new Length(0, "px");
    // Length* padding_left = new Length(0, "px");
    // Length* padding_right = new Length(0, "px");

    // int total = width->len + margin_left->len + margin_right->len+border_left->len + border_right->len+
    // padding_left->len + padding_right->len;
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
        children[i]->layout(dimensions);
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
    // content.x+=margin.left;
    //content.y+=margin.top;
    // content.width-=(margin.left+margin.right);
    //content.height-=(margin.top+margin.bottom);
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

