#ifndef CSSPARSER_H   
#define CSSPARSER_H

#include <string>
#include <vector>
#include "selector.h"
#include <unordered_map>
#include <utility>

class CSSParser {
    public:
        std::vector<Rule*> parseRules();
        CSSParser(std::string str, unsigned i);

    private:
        // current position in input 
        unsigned pos;
        // string of input
        std::string input;
        // get next character without consuming it
        char nextChar();
        // returns true if next characters equals to param
        bool startsWith(std::string c);
        // startWith case sensitive
        bool startsWithcs(std::string c);
        // returns true if end of file
        bool eof();
        // returns next character & consumes it
        char consumeChar();
        // keep consuming until fnc returns false, and then returns string
        std::string consumeWhile(bool (*func)(char c));
        // keep consuming until next character isn't a whitepace
        void consumeWhitespace();
        // parser one simple selector
        SimpleSelector* parseSimpleSelector();
        // parse a group of selectors separated by ,
        std::vector<Selector*> parseSelectors();
        // parse an identifier (element/class/id/etc)
        std::string parseIdentifier();
        //parse a declaration
        Declaration* parseDeclaration();
        //parse declarations
        std::vector<Declaration*> parseDeclarations();
        // parse value
        Value* parseValue();
        // parse a rule
        Rule* parseRule();
        //parse length value
        Value* parseLength();
        // check if valid color 
        Value* parseColor(); 
        //check if valid char
        bool validIdentifierChar(char c);

        
};

std::vector<Rule*> parseCSS(std::string s);
#endif