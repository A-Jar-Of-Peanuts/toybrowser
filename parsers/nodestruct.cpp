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
            text = t;
            type = "DOCTYPE " + text;
        }
};

class Text: public NodeType {
    public:
        string text;
        Text(string t) {
            text = t;
            type = text;
        }
};

class Element: public NodeType {
    public:
        string tagname;
        unordered_map<string, string> atmap;
        Element(string tn, unordered_map<string, string> am) {
            tagname = tn;
            atmap = am;
            type = tagname;
            for(auto& it : atmap) {
                type += " " + it.first + ": " + it.second;
            }
        }
};

class Comment: public NodeType{
    public:
        string text;
        Comment(string t) {
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