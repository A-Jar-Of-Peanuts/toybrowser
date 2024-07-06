#ifndef NODESTRUCT_H   
#define NODESTRUCT_H

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

class NodeType{
    public:
        std::string type;
};

struct Node {
    std::vector<Node*> children;
    NodeType nt;

    Node(std::vector<Node*> c, NodeType n) {
        children = c;
        nt = n;
    }
};

Node* text(std::vector<Node*> children, std::string t);
Node* element(std::vector<Node*> children, std::string tn, std::unordered_map<std::string, std::string> am);
Node* document(std::vector<Node*> children, std::string t);
Node* comment(std::string t);

#endif