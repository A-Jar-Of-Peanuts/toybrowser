#include <string>
#include <iostream>
#include "nodestruct.h"
#include "htmlparser.h"
#include "cssparser.h"
#include "selector.h"
#include "stylenode.h"


using namespace std;
void printer(Node* n, int num) {
    if (n == nullptr) {
        return;
    }

    for(int i = 0; i<num; i++) {
        cout<<"|";
    }
    cout<<n->nt.type<<endl;

    for(int i = 0; i<n->children.size(); i++) {
        printer(n->children[i], num+1);
    }
}

void pr(vector<Rule*> r) {
    for(int i = 0; i<r.size(); i++) {
        for(int j = 0; j<r[i]->selectors.size(); j++) {
            cout<<r[i]->selectors[j]->tagName<<", ";
        }
        for(int j = 0; j<r[i]->declarations.size(); j++) {
            cout<<r[i]->declarations[j]->name<<" ";
            cout<<r[i]->declarations[j]->value->toString()<<" ";
        }
        cout<<"\n";
    }
}

void printer2(Node* n, int num) {
    if (n == nullptr) {
        return;
    }

    for(int i = 0; i<num; i++) {
        cout<<"|";
    }
    cout<<n->nt.type<<" CSS: ";

    for (const auto& pair : n->properties) {
        std::cout << pair.first << ": " << pair.second->toString() << " ";
    }

    cout <<endl;
    
    for(int i = 0; i<n->children.size(); i++) {
        printer2(n->children[i], num+1);
    }
}

int main() {
    Node* n = parseHTML("<!doctype html><html ><head><title>test</title></head><body><p  class = \"b\" id = \"a\">Hello World</p></body><!--this is a comment!--></html>");
    vector<Rule*> r = parseCSS("title{color:#ffffff; display:inline;}p{color:#cc0000; margin:auto;} ");
    //vector<Rule*> r = parseCSS("h1{}");
    printer(n, 1);
    pr(r);
    makeStyle(n, r);
    printer2(n, 1);
}

