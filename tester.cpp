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
    cout<<n->nt.print();

    for(int i = 0; i<n->children.size(); i++) {
        printer(n->children[i], num+1);
    }
}

int main() {
    printer(parse("hello"), 1);
}
