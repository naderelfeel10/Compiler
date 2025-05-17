#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <unordered_set>

struct Symbol {
    std::string name;
    std::string kind;  // variable, constant, function, parameter, array
    std::string scope; // global, or function name
    int line;
};

std::vector<Symbol> symbolTable;
std::unordered_set<std::string> symbolNames;

void addSymbol(const std::string& name, const std::string& kind, const std::string& scope, int line) {
    if (symbolNames.find(name + scope) == symbolNames.end()) {
        symbolTable.push_back({name, kind, scope, line});
        symbolNames.insert(name + scope);
    }
}

void printSymbolTable() {
    std::cout << "\nSymbol Table:\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << "| " << std::setw(12) << "Name" << " | " << std::setw(10) << "Kind"
              << " | " << std::setw(10) << "Scope" << " | " << std::setw(6) << "Line" << " |\n";
    std::cout << "--------------------------------------------------------\n";
    for (const auto& sym : symbolTable) {
        std::cout << "| " << std::setw(12) << sym.name
                  << " | " << std::setw(10) << sym.kind
                  << " | " << std::setw(10) << sym.scope
                  << " | " << std::setw(6) << sym.line << " |\n";
    }
    std::cout << "--------------------------------------------------------\n";
}

#endif
