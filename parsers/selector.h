#ifndef SELECTOR_H   
#define SELECTOR_H

#include <string> 
#include <vector>

enum un {cm, mm, in, px, pt, pc};

class Value {
    public:
        std::string type; 
        virtual std::string toString() const = 0;
}; 

struct Declaration {
    public:
        std::string name; 
        Value* value; 
        Declaration(std::string name, Value* value);
};

class Selector{
    public: 
        std::string tagName; 
        std::string id; 
        std::vector<std::string> classes; 
};

class SimpleSelector : public Selector {
    public: 
        // std::string tagName ; 
        // std::string id ; 
        // std::vector<std::string> classes; 
        SimpleSelector(std::string tn, std::string i, std::vector<std::string> cl);
};

class Keyword : public Value{
    public: 
        std::string kw; 
        Keyword(std::string k);
        std::string toString() const override;
};

// class Color : public Value {
//     public:
//         int r, g, b, a; 
//         Color(int r, int g, int b, int a);
// }; 

class Color : public Value {
    public:
        std::string hex; 
        Color(std::string hex);
        std::string toString() const override;
}; 

class Length : public Value {
    public:
        int len;
        std::string un;
        Length(int len, std::string un);
        std::string toString() const override;
};

class Unit : public Value {
    public:
        un units; 
        Unit(un units);
};

struct Rule{
    std::vector<Selector*> selectors;
    std::vector<Declaration*> declarations;
};

struct Stylesheet {
    std::vector<Rule*> rules;
};



#endif