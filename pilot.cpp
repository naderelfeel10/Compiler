#include <iostream>
#include <fstream>
#include <sstream>
#include"headers/parser.hpp"
#include "headers/lexer.hpp"
#include "headers/symbol_table.hpp"
#include <algorithm>

int main (int argc , char** argv){
    
    if(argc < 2){
        std::cout << "Please supply the source file." << std::endl;
        exit(1);
    }

    std::cout << "Reading from the File name: " << argv[1] << std::endl;
    
    std::ifstream sourceFileStream(argv[1]);
    std::stringstream buffer;

    char tmp;
    while(sourceFileStream.get(tmp)){
        buffer << tmp;
    }

    std::string sourceCode = buffer.str();
    std::cout << "The source file is: " << sourceCode << std::endl;

    // Open a file to save the output
    std::ofstream outputFile("output.txt");

    // Redirect std::cout to outputFile
    std::streambuf* oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(outputFile.rdbuf());

    // Lexer and tokenization
    Lexer lexer(sourceCode);
    std::vector<TOKEN*> tokens = lexer.tokenize();
    int counter = 0;

    for (TOKEN* temp : tokens) {
        counter++;
        std::cout << counter << ") " << typeToString(temp->TYPE) << " (   " << temp->VALUE << "     )  " << std::endl;
    }

    std::string currentScope = "global";
    for (size_t i = 0; i < tokens.size(); ++i) {
        TOKEN* tok = tokens[i];
        std::string val = tok->VALUE;

        // Detect functions
        if (tok->TYPE == TOKEN_KEYWORD && val == "def") {
            if (i + 1 < tokens.size() && tokens[i + 1]->TYPE == TOKEN_ID) {
                std::string funcName = tokens[i + 1]->VALUE;
                addSymbol(funcName, "function", "global", tok->line);
                currentScope = funcName;

                // Parameters
                size_t j = i + 2;
                if (j < tokens.size() && tokens[j]->TYPE == TOKEN_LEFT_PAREN) {
                    j++;
                    while (j < tokens.size() && tokens[j]->TYPE != TOKEN_RIGHT_PAREN) {
                        if (tokens[j]->TYPE == TOKEN_ID) {
                            addSymbol(tokens[j]->VALUE, "parameter", funcName, tok->line);
                        }
                        j++;
                    }
                }
            }
        }

        // Variables or Constants or Arrays
        //||((tokens[i + 1]->TYPE == (TOKEN_PLUS || TOKEN_MINUS||TOKEN_MUL||TOKEN_DIV)) && (tokens[i + 1]->TYPE == TOKEN_EQUALS) ) )
        if (tok->TYPE == TOKEN_ID && i + 1 < tokens.size()&&  (tokens[i + 1]->TYPE == TOKEN_EQUALS)) {
            std::string kind = "variable";
            if (!val.empty() && all_of(val.begin(), val.end(), ::isupper)) {
                kind = "constant";
            } else if (i + 2 < tokens.size() && tokens[i + 2]->TYPE == TOKEN_LEFT_SQR) {
                kind = "array";
            }
            addSymbol(val, kind, currentScope, tok->line);
        }
    }

    printSymbolTable();
    Parser parser(tokens);
    parser.parse();
    std::cout << "End of the Program" << std::endl;

    // Restore the original std::cout buffer
    std::cout.rdbuf(oldCoutBuffer);

    outputFile.close();

    return 0;
}
