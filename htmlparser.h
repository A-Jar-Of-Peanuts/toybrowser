#ifndef HTMLPARSER_H   
#define HTMLPARSER_H

#include <string>
#include <vector>
#include "nodestruct.h"

Node* parse(std::string str);

class Parser {
    public:
        unsigned pos;
        std::string input;
        char nextChar();
        bool startsWith(char c);
        bool eof();
        char consumeChar();
        std::string consumeWhile(bool (*func)(char c));
        void consumeWhitespace();
        std::string parseTagAttr();
        Node* parseNode();
        Node* parseText();
        Node* parseElement();
        std::vector<Node*> parseNodes();
        Parser(std::string str, unsigned i);
        
};

#endif