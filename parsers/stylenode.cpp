#include "stylenode.h"
#include "selector.h"
#include "nodestruct.h"
#include <sstream>
#include <vector>
#include <string>

using namespace std;

std::pair<bool, std::string> getAttributeValue(const std::string& input, const std::string& attribute) {
    std::unordered_map<std::string, std::string> attrMap;
    std::istringstream ss(input);
    std::string pair;

    // Split the input string into key-value pairs
    while (ss >> pair) {
        size_t pos = pair.find(':');
        if (pos != std::string::npos) {
            // Get the attribute and value
            std::string key = pair.substr(0, pos);
            std::string value = pair.substr(pos + 1);
            attrMap[key] = value; // Store in the map
        }
    }

    // Check if the attribute exists and return its value
    auto it = attrMap.find(attribute);
    if (it != attrMap.end()) {
        return {true, it->second}; // Found, return true and the value
    }
    return {false, ""}; // Not found, return false
}

bool matchesSimpleSelector(Node* node, Selector* sel) {
    if (node->nt.name == "Element") {
        std::pair<bool, std::string> p = getAttributeValue(node->nt.type, "tagname");
        if (p.first && p.second == sel->tagName) {
            return true;
        }

        p = getAttributeValue(node->nt.type, "class");
        if (p.first) {
            for(int i = 0; i<sel->classes.size(); i++) {
                if (sel->classes[i] == p.second) {
                    return true;
                }
            }
        }

        p = getAttributeValue(node->nt.type, "id");
        if (p.first && p.second == sel->id) {
            return true;
        }
    }
    return false;
}

bool matches(Node* node, Selector* sel) {
    return matchesSimpleSelector(node, sel);
}

void assignProperties(Node* node, vector<Rule*> rul) {
    for(int i = 0; i<rul.size(); i++) {
        for(int j = 0; j<rul[i]->selectors.size(); j++) {
            if(matches(node, rul[i]->selectors[j])) {
                for(int k = 0; k<rul[i]->declarations.size(); k++) {
                    node->properties[rul[i]->declarations[k]->name] = rul[i]->declarations[k]->value;
                }
                break;
            }
        }
    }
}

void makeStyle(Node* node, vector<Rule*> rul) {
    assignProperties(node, rul);
    for(int i = 0; i<node->children.size(); i++) {
       makeStyle(node->children[i], rul);
    }
}

