#ifndef NODESTRUCT_H   
#define NODESTRUCT_H

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include "selector.h"

class NodeType{
    public:
        std::string name;
        std::string type;
};

struct Node {
    const std::string INLINE = "inline";
    const std::string BLOCK = "block";
    const std::string NONE = "none";

    std::unordered_map<std::string, Value*> properties;
    std::vector<Node*> children; 
    NodeType nt;

    Node(std::vector<Node*> c, NodeType n) {
        children = c;
        nt = n;
    }

    std::pair<bool, Value*> value(std::string str);
    std::string display();
};

Node* text(std::vector<Node*> children, std::string t);
Node* element(std::vector<Node*> children, std::string tn, std::unordered_map<std::string, std::string> am);
Node* document(std::vector<Node*> children, std::string t);
Node* comment(std::string t);

#endif