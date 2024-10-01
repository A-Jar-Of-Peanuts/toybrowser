#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include "nodestruct.h"

using namespace std;

class Document: public NodeType {
    public:
        string text;
        Document(string t) {
            name = "Document";
            text = t;
            type = "DOCTYPE " + text;
        }
};

class Text: public NodeType {
    public:
        string text;
        Text(string t) {
            name = "Text";
            text = t;
            type = text;
        }
};

class Element: public NodeType {
    public:
        string tagname;
        unordered_map<string, string> atmap;
        Element(string tn, unordered_map<string, string> am) {
            name = "Element";
            tagname = tn;
            atmap = am;
            type = "tagname:" + tagname;
            for(auto& it : atmap) {
                type += " " + it.first + ":" + it.second;
            }
        }
};

class Comment: public NodeType{
    public:
        string text;
        Comment(string t) {
            name = "Comment";
            text = t;
            type = text;
        }
};

Node* text(vector<Node*> children, string t) {
    Text tx(t);
    Node* nd = new Node(children, tx);
    return nd;
}

Node* element(vector<Node*> children, string tn, unordered_map<string, string> am) {
    Element elem(tn, am);
    Node* nd = new Node(children, elem);
    return nd;
}

Node* document(vector<Node*> children, string t) {
    Document d(t);
    Node* nd = new Node(children, d);
    return nd;
}

Node* comment(string t) {
    Comment c(t);
    Node* nd = new Node(vector<Node*>(), c);
    return nd;
}

std::pair<bool, Value*> Node::value(std::string str) {
    if (properties.find(str)!=properties.end()) {
        return make_pair(true, properties[str]);
    } else {
        return make_pair(false, nullptr);
    }
}

string Node::display() {
    // TODO when display value class has been implemented

    // pair<bool, Value*> p = value("display");
    // if (p.first) {
    //     return p.second;
    // } else {
    //     return INLINE;
    // }
    if (nt.name == "Element") {
        return INLINE;
    } else {
        return NONE;
    }
}

std::string display();