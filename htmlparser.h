#ifndef HTMLPARSER_H   
#define HTMLPARSER_H

#include <string>
#include <vector>
#include "nodestruct.h"
#include <unordered_map>
#include <utility>

Node* parse(std::string str);

class Parser {
    public:
        std::vector<Node*> parseNodes();
        Parser(std::string str, unsigned i);

    private:
        // current position in input 
        unsigned pos;
        // string of input
        std::string input;
        // get next character without consuming it
        char nextChar();
        // returns true if next characters equals to param
        bool startsWith(std::string c);
        // returns true if end of file
        bool eof();
        // returns next character & consumes it
        char consumeChar();
        // keep consuming until fnc returns false, and then returns string
        std::string consumeWhile(bool (*func)(char c));
        // keep consuming until next character isn't a whitepace
        void consumeWhitespace();
        // parse a tag or attribute name
        std::string parseTagAttr();
        // parse a list of name="value" pairs separated by whitespace
        std::unordered_map<std::string, std::string> parseAttrs();
        // parse a single name="value" pair
        std::pair<std::string,std::string> parseAttr();
        // parse a quaoted value
        std::string parseAttrVal();
        // parse a single node
        Node* parseNode();
        // parse a text node
        Node* parseText();
        // parse a single element, including open tag, contents, closing tag
        Node* parseElement();
        
};

#endif