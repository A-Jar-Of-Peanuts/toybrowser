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

class Link: public NodeType{
    public:
        unordered_map<string, string> atmap;
        Link(unordered_map<string, string> am) {
            name = "Link";
            atmap = am;
            type = "";
            for(auto& it : atmap) {
                type += " " + it.first + ":" + it.second;
            }
        }
};

class Meta: public NodeType {
    public:
        unordered_map<string, string> atmap;
        Meta(unordered_map<string, string> am) {
            name = "Meta";
            atmap = am;
            type = "";
            for(auto& it : atmap) {
                type += " " + it.first + ":" + it.second;
            }
        }
};

class Script: public NodeType {
    public:
        string text;
        Script(string t) {
            name = "Script";
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

Node* link(unordered_map<string, string> am) {
    Link l(am);
    Node* nd = new Node(vector<Node*>(), l);
    return nd;
}

Node* meta(unordered_map<string, string> am) {
    Meta m(am);
    Node* nd = new Node(vector<Node*>(), m);
    return nd;
}

Node* script(string t) {
    Script s(t);
    Node* nd = new Node(vector<Node*>(), s);
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

    if (nt.name == "Element") {
        pair<bool, Value*> p = value("display");
        if (p.first) {
            return p.second->toString();
        } else {
            return BLOCK;
        }
    } else if (nt.name == "Text") {
        return TEXT;
    }else {
        return NONE;
    }
}

std::string display();