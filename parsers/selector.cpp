#include "selector.h"
#include <vector>
#include <string>

using namespace std;

Keyword::Keyword(string k) {
    kw = k; 
    type = "Keyword";
}
string Keyword::toString() const {
        return this->kw;
}

Declaration::Declaration(std::string name, Value* value) {
    this->name = name;
    this->value = value;
}

// Color::Color(int r, int g, int b, int a) {
//     this->r = r; 
//     this->g = g; 
//     this->b = b; 
//     this->a = a; 
// }

Length::Length(int l, string u) {
    this->len = l;
    this->un = u;
}

string Length::toString() const {
        return to_string(this->len) + this->un;
}

Color::Color(string h) {
    this->hex = h;
}

string Color::toString() const {
        return this->hex;
}

Unit::Unit(un units) {
    this->units = units; 
}

SimpleSelector::SimpleSelector(string tn, string i, vector<string> cl) {
    tagName = tn; 
    id = i; 
    classes = cl; 
}