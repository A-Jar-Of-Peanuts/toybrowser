#ifndef HTMLPARSER_H   
#define HTMLPARSER_H

#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>

class Parser {
    public:
        unsigned pos;
        std::string input;
        char nextChar();
        bool startsWith(char c);
        bool eof();
        char consumeChar();
};

#endif