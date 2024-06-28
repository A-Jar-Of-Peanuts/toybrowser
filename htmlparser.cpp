#include <string>
#include "nodestruct.h"
#include "htmlparser.h"

using namespace std;

bool Parser::eof() {
    return pos >= input.size();
}

char Parser::nextChar() {
    return input[pos];
}

bool Parser::startsWith(char c) {
    return  input[pos] == c;
}

char Parser::consumeChar() {
    return input[pos++];
}

string Parser::consumeWhile(bool (*func)(char c)) {
    string res = "";
    while(func(nextChar())) {
        res += consumeChar();
    }
    return res;
}

void Parser::consumeWhitespace() {
    consumeWhile([](char c)->bool{return c == ' ';});
}

string Parser::parseTagAttr() {
    return consumeWhile([](char c)->bool{return isalpha(c) || isdigit(c);});
}

Node* Parser::parseNode() {
    Node* n;
    if (nextChar() == '<') {
        n = parseElement();
    } else {
        n = parseText();
    }
    return n;
}

Node* Parser::parseElement() {
    
}

Node* Parser::parseText() {
    string t = consumeWhile([](char c)->bool{return c != '<';});
    return text(vector<Node*>(), t);
}

Parser::Parser(string str, unsigned i) {
    input = str;
    pos = i;
}

Node* parse(string str) {
    Parser p(str, 0);
    return p.parseNode();
}