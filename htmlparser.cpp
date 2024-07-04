#include <string>
#include "nodestruct.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <utility>
#include "htmlparser.h"

using namespace std;

bool Parser::eof() {
    return pos >= input.size();
}

char Parser::nextChar() {
    return input[pos];
}

bool Parser::startsWith(string c) {
    if (pos+c.length() > input.length()) {
        return false;
    }
    return  input.substr(pos, c.length()) == c;
}

char Parser::consumeChar() {
    return input[pos++];
}

string Parser::consumeWhile(bool (*func)(char c)) {
    string res = "";
    while(!eof() && func(nextChar())) {
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
    //return text(vector<Node*>(), "");
    //assert(consumeChar() == '<');
    consumeChar();
    string tag_name = parseTagAttr();
    unordered_map<string, string> attrs = parseAttrs();
    consumeChar();
   // assert(consumeChar() == '>');

    vector<Node*> children = parseNodes();

    // assert(consumeChar() == '<');
    // assert(consumeChar() == '/');
    // assert(parseTagAttr() == tag_name);
    // assert(consumeChar() == '>');

    consumeChar();
    consumeChar();
    parseTagAttr();
    consumeChar();

    return element(children, tag_name, attrs);
}

unordered_map<string, string> Parser::parseAttrs() {
    unordered_map<string, string> m;
    while(true) {
        consumeWhitespace();
        if (nextChar() == '>') {
            break;
        }
        pair<string, string> p = parseAttr();
        m[p.first] = p.second;
    }
    return m;
}

pair<string,string> Parser::parseAttr() {
    string name = parseTagAttr();
    // cout<<nextChar()<<endl;
    // assert(nextChar() == '=');
    consumeWhitespace();
    consumeChar();
    string value = parseAttrVal();
    return make_pair(name, value);
}

string Parser::parseAttrVal() {
    consumeWhitespace();
    char openQuote = consumeChar();
    //assert(openQuote == '"' || openQuote == '\'');
    string val = consumeWhile([](char c)->bool{return c!= '"' && c!= '\'';});
    consumeChar();
    return val;
}

Node* Parser::parseText() {
    string t = consumeWhile([](char c)->bool{return c != '<';});
    return text(vector<Node*>(), t);
}

Parser::Parser(string str, unsigned i) {
    input = str;
    pos = i;
}

vector<Node*> Parser::parseNodes() {
    vector<Node*> v;
    while(!eof() && !startsWith("</")) {
        v.push_back(parseNode());
    }
    return v;
}

Node* parse(string str) {
    Parser p(str, 0);
    return p.parseNodes()[0];
}