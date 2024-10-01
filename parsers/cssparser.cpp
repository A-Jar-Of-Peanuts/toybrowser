#include "cssparser.h"
#include "selector.h"
#include <vector>
#include <string> 
#include <cctype>
#include <stdexcept>
#include <iostream>

using namespace std;

CSSParser::CSSParser(string str, unsigned i) {
    this->input = str;
    this->pos = i;
}

bool CSSParser::eof() {
    return pos >= input.size();
}

char CSSParser::nextChar() {
    return input[pos];
}

char CSSParser::consumeChar() {
    //cout << input[pos+1];
    return input[pos++];
}

bool CSSParser::startsWith(string c) {
    if (pos + c.length() > input.length()) {
        return false;
    }
    return input.substr(pos, c.length()) == c;
}

bool CSSParser::startsWithcs(string c) {
    if (pos + c.length() > input.length()) {
        return false;
    }
    for (int i = 0; i < c.length(); i++) {
        if (tolower(c[i]) != tolower(input[i + pos])) {
            return false;
        }
    }
    return true;
}

string CSSParser::consumeWhile(bool (*func)(char c)) {
    string res = "";
    while (!eof() && func(nextChar())) {
        res += consumeChar();
    }
    return res;
}

void CSSParser::consumeWhitespace() {
    consumeWhile([](char c) -> bool { return isspace(c); });
}

SimpleSelector* CSSParser::parseSimpleSelector() {
    SimpleSelector* sel = new SimpleSelector("", "", vector<string>());
    bool flag = true;
    while (!eof() && flag) {
        char c = nextChar();
        switch (c) {
            case '#':
                consumeChar();
                sel->id = parseIdentifier();
                break;
            case '.':
                consumeChar();
                sel->classes.push_back(parseIdentifier());
                break;
            case '*':
                // Universal selector
                consumeChar();
                break;
            default:
                if (validIdentifierChar(c)) {
                    sel->tagName = parseIdentifier();
                } else {
                    if (c != '{' && c!=',') {
                        throw runtime_error("Unexpected character while parsing simple selector, got "+string(1,c));
                    } 
                    flag = false;
                }
                break;
        }
    }
    //cout << sel->tagName << " SEL\n";
    return sel;
}

string CSSParser::parseIdentifier() {
    return consumeWhile([](char c) -> bool { return isalnum(c) || c == '-' || c == '_'; });
}

vector<Selector*> CSSParser::parseSelectors() {
    vector<Selector*> selectors;
    while (!eof()) {
        selectors.push_back(parseSimpleSelector());
        //cout<<selectors[selectors.size()-1]->tagName<<"PSEL\n";
        consumeWhitespace();
        char c = nextChar();
        switch (c) {
            case ',':
                consumeChar();
                consumeWhitespace();
                break;
            case '{':
                return selectors; // End of selectors list
            default:
                throw runtime_error("Unexpected character while parsing selectors");
        }
    }
    return selectors;
}

Declaration* CSSParser::parseDeclaration() {
    string pname = parseIdentifier();
    consumeWhitespace();
    char temp = consumeChar();
    if (temp != ':') {
        throw runtime_error(pname+" Expected ':' after property name, got " + std::string(1,temp));
    }
    consumeWhitespace();
    Value* value = parseValue();  // Updated to return Value* instead of Value
    consumeWhitespace();
    if (consumeChar() != ';') {
        throw runtime_error("Expected ';' after property value");
    }
    return new Declaration(pname, value);
}

vector<Declaration*> CSSParser::parseDeclarations() {
    vector<Declaration*> dec;
    while (!eof() && nextChar()!='}') {
        consumeWhitespace();
        if (nextChar() == '}') break; // End of declarations
        dec.push_back(parseDeclaration());
    }
    return dec;
}

Value* CSSParser::parseValue() {
    char c = nextChar();
    if (isdigit(c)) {
        return parseLength();  // Updated to return Value* instead of Value
    } else if (c == '#') {
        return parseColor();  // Updated to return Value* instead of Value
    } else {
        Keyword* val = new Keyword(parseIdentifier()); // Return a pointer to Keyword
        return val;
    }
}

Value* CSSParser::parseLength() {
    string number = consumeWhile([](char c) -> bool { return isdigit(c) || c == '.'; });
    string unit = parseIdentifier();
    return new Length(stof(number), unit);  // Return a pointer to Length
}

Value* CSSParser::parseColor() {
    consumeChar(); // Consume '#'
    string hex = consumeWhile([](char c) -> bool { return isxdigit(c); });
    if (hex.length() != 3 && hex.length() != 6) {
        throw runtime_error("Invalid color code length");
    }
    return new Color("#" + hex);  // Return a pointer to Color
}

Rule* CSSParser::parseRule() {
    vector<Selector*> sel = parseSelectors();
    consumeChar(); // Consume '{'
    vector<Declaration*> dec = parseDeclarations();
    consumeChar(); // Consume '}'
    Rule* r = new Rule();
    r->selectors = sel;
    r->declarations = dec;
    return r;
}

bool CSSParser::validIdentifierChar(char c) {
    return isalnum(c) || c == '-' || c == '_';
}

std::vector<Rule*> CSSParser::parseRules() {
    vector<Rule*> r;
    while (!eof()) {
        consumeWhitespace();
        r.push_back(parseRule());
    }
    return r;
}

std::vector<Rule*> parseCSS(string s) {
    CSSParser p(s, 0);
    return p.parseRules();
}
