#include <string>
#include "nodestruct.h"
#include "htmlparser.h";

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

