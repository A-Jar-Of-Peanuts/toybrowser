#include <string>
#include "nodestruct.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <unordered_map>
#include <utility>
#include "htmlparser.h"

using namespace std;

bool HTMLParser::eof()
{
    return pos >= input.size();
}

char HTMLParser::nextChar()
{
    return input[pos];
}

bool HTMLParser::startsWith(string c)
{
    if (pos + c.length() > input.length())
    {
        return false;
    }
    return input.substr(pos, c.length()) == c;
}

bool HTMLParser::startsWithcs(string c)
{
    if (pos + c.length() > input.length())
    {
        return false;
    }
    for (int i = 0; i < c.length(); i++)
    {
        if (tolower(c[i]) != tolower(input[i + pos]))
        {
            return false;
        }
    }
    return true;
}

char HTMLParser::consumeChar()
{
    return input[pos++];
}

string HTMLParser::consumeWhile(bool (*func)(char c))
{
    string res = "";
    while (!eof() && func(nextChar()))
    {
        res += consumeChar();
    }
    return res;
}

void HTMLParser::consumeWhitespace()
{
    consumeWhile([](char c) -> bool
                 { return isspace(c); });
}

string HTMLParser::parseTagAttr()
{
    return consumeWhile([](char c) -> bool
                        { return isalpha(c) || isdigit(c); });
}

Node *HTMLParser::parseNode()
{
    Node *n;
    consumeWhitespace();
    if (startsWithcs("<!--"))
    {
        n = parseCom();
    }
    else if (startsWithcs("<!DOCTYPE"))
    {
        n = parseDoc();
    }
    else if (startsWithcs("<link"))
    {
        n = parseLink();
    } else if (startsWithcs("<meta")) {
        n = parseMeta();
    } else if (startsWithcs("<script>")) {
        n = parseScript();
    }
    else if (nextChar() == '<')
    {
        n = parseElement();
    }
    else
    {
        n = parseText();
    }
    consumeWhitespace();
    return n;
}

Node* HTMLParser::parseScript() {
    for(int i = 0; i<8; i++) {
        consumeChar();
    }
    consumeWhitespace();

    string t = consumeWhile([](char c) -> bool
                            { return c != '<'; });

    for(int i = 0; i<8; i++) {
        consumeChar();
    }
    return script(t);
}

Node* HTMLParser::parseMeta() {
    for(int i = 0; i<5; i++) {
        consumeChar();
    }
    consumeWhitespace();

    unordered_map<string, string> attrs = parseAttrs();
    consumeWhitespace();
    if (nextChar() == '/') {
        consumeChar(); // Skip '/'
        consumeChar(); // Skip '>'
        consumeWhitespace();
    } else if (nextChar() == '>') {
        consumeChar(); // Skip '>'
        consumeWhitespace();
    }
    return meta(attrs);
}

Node* HTMLParser::parseLink() {
    for (int i = 0; i < 5; i++)
    {
        consumeChar();
    }
    consumeWhitespace();
    
    unordered_map<string, string> attrs = parseAttrs();
    consumeWhitespace();
    if (nextChar() == '/')
    {
        consumeChar(); // Skip '/'
        consumeChar(); // Skip '>'
        consumeWhitespace();
    } else if (nextChar() == '>')
    {
        consumeChar(); // Skip '>'
        consumeWhitespace();
    }

    return link(attrs);
}

Node *HTMLParser::parseCom()
{
    for (int i = 0; i < 4; i++)
    {
        consumeChar();
    }
    string t = "";
    while (!startsWith("-->"))
    {
        t += consumeChar();
    }
    for (int i = 0; i < 3; i++)
    {
        consumeChar();
    }
    return comment(t);
}

Node *HTMLParser::parseDoc()
{
    for (int i = 0; i < 9; i++)
    {
        consumeChar();
    }
    consumeWhitespace();
    string t = consumeWhile([](char c) -> bool
                            { return c != '>'; });
    consumeChar();
    vector<Node *> children = parseNodes();
    return document(children, t);
}

Node *HTMLParser::parseElement()
{
    consumeWhitespace();
    consumeChar(); // Skip '<'
    string tag_name = parseTagAttr();
    unordered_map<string, string> attrs = parseAttrs();
    consumeWhitespace();

    if (nextChar() == '/')
    {
        consumeChar(); // Skip '/'
        consumeChar(); // Skip '>'
        consumeWhitespace();
        return element({}, tag_name, attrs);
    }

    consumeChar(); // Skip '>'
    consumeWhitespace();

    vector<Node *> children = parseNodes();
    consumeWhitespace();

    // Skip "</tag_name>"
    consumeChar();  // Skip '<'
    consumeChar();  // Skip '/'
    parseTagAttr(); // Read tag_name
    consumeChar();  // Skip '>'
    consumeWhitespace();
    return element(children, tag_name, attrs);
}

unordered_map<string, string> HTMLParser::parseAttrs()
{
    unordered_map<string, string> m;
    while (true)
    {
        consumeWhitespace();
        if (nextChar() == '>' || nextChar() == '/')
        {
            break;
        }
        pair<string, string> p = parseAttr();
        m[p.first] = p.second;
    }
    return m;
}

pair<string, string> HTMLParser::parseAttr()
{
    consumeWhitespace();
    string name = parseTagAttr();
    // cout<<nextChar()<<endl;
    // assert(nextChar() == '=');
    consumeWhitespace();
    consumeChar();
    string value = parseAttrVal();
    return make_pair(name, value);
}

string HTMLParser::parseAttrVal()
{
    consumeWhitespace();
    if (nextChar() == '"' || nextChar() == '\'')
    {
        char openQuote = consumeChar();
        // assert(openQuote == '"' || openQuote == '\'');
        string val = consumeWhile([](char c) -> bool
                                { return c != '"' && c != '\''; });
        consumeChar();
        return val;
    } else {
        string val = parseTagAttr();
        return val;
    }

}

Node *HTMLParser::parseText()
{
    string t = consumeWhile([](char c) -> bool
                            { return c != '<'; });
    return text(vector<Node *>(), t);
}

HTMLParser::HTMLParser(string str, unsigned i)
{
    input = str;
    pos = i;
}

vector<Node *> HTMLParser::parseNodes()
{
    vector<Node *> v;
    while (!eof() && !startsWith("</"))
    {
        consumeWhitespace();
        v.push_back(parseNode());
        consumeWhitespace();
    }
    return v;
}

Node *parseHTML(string str)
{
    HTMLParser p(str, 0);
    return p.parseNodes()[0];
}