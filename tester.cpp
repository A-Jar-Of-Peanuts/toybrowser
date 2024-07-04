#include <string>
#include <iostream>
#include "nodestruct.h"
#include "htmlparser.h"


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

int main() {
    printer(parse("<!DOCTYPE html><html class = \"b\" id = \"a\"><head><title>test</title></head><body><p>Hello World</p></body></html>"), 1);
}

