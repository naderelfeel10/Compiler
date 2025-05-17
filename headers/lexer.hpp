#ifndef __LEXER_H
#define __LEXER_H

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <iomanip>



using namespace std;

enum type {
    TOKEN_INT,
    TOKEN_DOUBLE,
    TOKEN_ID,
    TOKEN_EQUALS,
    TOKEN_ARE,
    TOKEN_SEMICOLON,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_LEFT_SQR,
    TOKEN_RIGHT_SQR,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_QUOTES,
    TOKEN_COMMA,
    TOKEN_COLON,
    TOKEN_DOUBLE_Q,
    TOKEN_SINGLE_Q,
    TOKEN_FUNCTION,
    TOKEN_CALL,
    TOKEN_ARGUMENTS,
    TOKEN_REL_EQUALS,
    TOKEN_REL_NOTEQUALS,
    TOKEN_REL_LESSTHAN,
    TOKEN_REL_LESSTHANEQUALS,
    TOKEN_REL_GREATERTHAN,
    TOKEN_REL_GREATERTHANEQUALS,
    TOKEN_MATH,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_RANGE,
    TOKEN_TILL, 
    TOKEN_TO,
    TOKEN_AS,
    TOKEN_REFERENCE,
    TOKEN_INDENT,
    TOKEN_DOT,
    TOKEN_EOF,
    TOKEN_COMMENT,
    TOKEN_NEWLINE,
    END
};

struct TOKEN {
    enum type TYPE;
    std::string VALUE;
    int line;
};

std::string typeToString(enum type TYPE) {
    switch (TYPE) {
        case TOKEN_ID: return "TOKEN_ID";
        case TOKEN_INT: return "TOKEN_INT";
        case TOKEN_DOUBLE: return "TOKEN_DOBLE";
        case TOKEN_EQUALS: return "TOKEN_EQUALS";
        case TOKEN_ARE: return "TOKEN_ARE";
        case TOKEN_SEMICOLON: return "TOKEN_SEMICOLON";
        case TOKEN_COLON: return "TOKEN_COLON";
        case TOKEN_LEFT_PAREN: return "TOKEN_LEFT_PAREN";
        case TOKEN_RIGHT_PAREN: return "TOKEN_RIGHT_PAREN";
        case TOKEN_LEFT_SQR: return "TOKEN_LEFT_SQR";
        case TOKEN_RIGHT_SQR: return "TOKEN_RIGHT_SQR";
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_STRING: return "TOKEN_STRING";
        case TOKEN_QUOTES: return "TOKEN_QUOTES";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_FUNCTION: return "TOKEN_FUNCTION";
        case TOKEN_CALL: return "TOKEN_CALL";
        case TOKEN_ARGUMENTS: return "TOKEN_ARGUMENTS";
        case TOKEN_REL_EQUALS: return "TOKEN_REL_EQUALS";
        case TOKEN_REL_NOTEQUALS: return "TOKEN_REL_NOTEQUALS";
        case TOKEN_REL_LESSTHAN: return "TOKEN_REL_LESSTHAN";
        case TOKEN_REL_LESSTHANEQUALS: return "TOKEN_REL_LESSTHANEQUALS";
        case TOKEN_REL_GREATERTHAN: return "TOKEN_REL_GREATERTHAN";
        case TOKEN_REL_GREATERTHANEQUALS: return "TOKEN_REL_GREATERTHANEQUALS";
        case TOKEN_MATH: return "TOKEN_MATH";
        case TOKEN_PLUS: return "TOKEN_PLUS";
        case TOKEN_MINUS: return "TOKEN_MINUS";
        case TOKEN_MUL: return "TOKEN_MUL";
        case TOKEN_DIV: return "TOKEN_DIV";
        case TOKEN_RANGE: return "TOKEN_RANGE";
        case TOKEN_TILL: return "TOKEN_TILL";
        case TOKEN_TO: return "TOKEN_TO";
        case TOKEN_AS: return "TOKEN_AS";
        case TOKEN_REFERENCE: return "TOKEN_REFERENCE";
        case TOKEN_INDENT: return "TOKEN_INDENT";
        case TOKEN_EOF: return "TOKEN_EOF";
        case TOKEN_DOT: return "TOKEN_DOT";
        case TOKEN_DOUBLE_Q: return "TOKEN_DOUBLE_Q"; // Handling double quote
        case TOKEN_SINGLE_Q: return "TOKEN_SINGLE_Q"; // Handling single quote
        case TOKEN_COMMENT: return "TOKEN_COMMENT"; 
        case TOKEN_NEWLINE: return "TOKEN_NEWLINE"; 

        default: return "UNRECOGNIZED TOKEN";
    }
}


class Lexer {
public:
    Lexer(std::string sourceCode)
        : source(sourceCode), cursor(0), size(sourceCode.length()), current(sourceCode.at(cursor)), lineNumber(1), charNumber(1) {}


    char advance() {
        if (cursor < size) {
            char temp = current;
            cursor++;
            charNumber++;
            current = (cursor < size) ? source[cursor] : '\0';
            return temp;
        }
        return '\0';
    }

    void checkAndSkip() {
        while (current == ' ' || current == '\n' || current == '\t' || current == '\r') {
            if (current == '\n') {
                lineNumber++;
                charNumber = 1;
            }
            advance();
        }
    }

    char seek(int offset) {
        if (cursor + offset >= size)
            return 0;
        else
            return source[cursor + offset];
    }

    bool isPythonKeyword(const std::string& word) {
        static const std::unordered_set<std::string> pythonKeywords = {
            "False", "None", "True", "and", "as", "assert", "break", "class", "continue","print",
            "def", "del", "elif", "else", "except", "finally", "for", "from", "global", "if",
            "import", "in", "is", "lambda", "nonlocal", "not", "or", "pass", "raise", "return",
            "try", "while", "with", "yield", "async", "await", "match", "case","range"
        };
        return pythonKeywords.find(word) != pythonKeywords.end();
    }

    TOKEN* tokenizeID() {
        std::stringstream buffer;
        buffer << advance();

        while (isalnum(current) || current == '_') {
            buffer << advance();
        }

        TOKEN* newToken = new TOKEN();
        newToken->VALUE = buffer.str();
        newToken->line = lineNumber;

        
        if (isPythonKeyword(newToken->VALUE)) {
            newToken->TYPE = TOKEN_KEYWORD;
        } else {
            newToken->TYPE = TOKEN_ID;
            
            //addSymbol( newToken->VALUE, kind, scope,line)
        }
        return newToken;
    }

TOKEN* tokenizeNUMBER() {
    std::stringstream buffer;
    bool isDouble = false;

    while (isdigit(current)) {
        buffer << advance();
    }

    if (current == '.' && isdigit(seek(1))) {
        isDouble = true;
        buffer << advance(); // consume the '.'

        while (isdigit(current)) {
            buffer << advance();
        }
    }

    TOKEN* newToken = new TOKEN();
    newToken->TYPE = isDouble ? TOKEN_DOUBLE : TOKEN_INT;
    newToken->VALUE = buffer.str();
    newToken->line = lineNumber;

    return newToken;
}
TOKEN* tokenizeSTRING(char quoteType) {
    advance();
    std::stringstream buffer;

    while (cursor < size && current != quoteType) {
        if (current == '\\') {  
            advance();
            if (current == 'n') buffer << '\n';
            else if (current == 't') buffer << '\t';
            else if (current == 'r') buffer << '\r';
            else if (current == quoteType) buffer << quoteType;
            else buffer << '\\' << current;
        } else {
            buffer << current;
        }
        advance();
    }

    if (current != quoteType) {
        std::cerr << "[!] LEXER ERROR: Unterminated string literal at line " << lineNumber << "\n";
        exit(1);
    }

    advance(); // Skip closing quote

    TOKEN* newToken = new TOKEN();
    newToken->TYPE = TOKEN_STRING;
    newToken->VALUE = buffer.str();
    newToken->line = lineNumber;
    return newToken;
}

  
    TOKEN* tokenizeSPECIAL(enum type TYPE) {

        TOKEN* newToken = new TOKEN();
        newToken->TYPE = TYPE;
        newToken->VALUE = std::string(1, advance());
        newToken->line = lineNumber;

       //std::cout<<newToken->TYPE << newToken->VALUE;
        return newToken;
    }

    std::vector<TOKEN*> tokenize() {
        std::vector<TOKEN*> tokens;
        bool notEOF = true;
        int comment_line = 1000000000;
        while (cursor < size && notEOF) {
            checkAndSkip();
            if(lineNumber==comment_line){
                advance();
                continue;
            }
            if (isalpha(current) || current == '_') {
                tokens.push_back(tokenizeID());
                continue;
            }

            if (isdigit(current)) {
                tokens.push_back(tokenizeNUMBER());
                continue;
            }
          if (current == '"') {
             tokens.push_back(tokenizeSTRING('"'));
             continue;
           }
             if (current == '\'') {
              tokens.push_back(tokenizeSTRING('\''));
             continue;
                }
            
            switch (current) {
                case '=':
                    if (seek(1) == '=')
                    { tokens.push_back(tokenizeSPECIAL(TOKEN_REL_EQUALS));advance();}
                    else tokens.push_back(tokenizeSPECIAL(TOKEN_EQUALS));
                    break;
                case '!':
                    if (seek(1) == '=') {tokens.push_back(tokenizeSPECIAL(TOKEN_REL_NOTEQUALS));advance();}
                    else tokens.push_back(tokenizeSPECIAL(TOKEN_MATH)); 
                    break;
                case '<':
                    if (seek(1) == '=') {tokens.push_back(tokenizeSPECIAL(TOKEN_REL_LESSTHANEQUALS));advance();}
                    else tokens.push_back(tokenizeSPECIAL(TOKEN_REL_LESSTHAN));
                    break;
                case '>':
                    if (seek(1) == '=') {tokens.push_back(tokenizeSPECIAL(TOKEN_REL_GREATERTHANEQUALS));advance();}
                    else tokens.push_back(tokenizeSPECIAL(TOKEN_REL_GREATERTHAN));
                    break;
                case '+': tokens.push_back(tokenizeSPECIAL(TOKEN_PLUS)); break;
                case '-': tokens.push_back(tokenizeSPECIAL(TOKEN_MINUS)); break;
                case '*':tokens.push_back(tokenizeSPECIAL(TOKEN_MUL)); break;
                case '#':
                    comment_line=lineNumber;
                    cout<<comment_line<<lineNumber;
                    //tokens.push_back(tokenizeSPECIAL(TOKEN_COMMENT)); 
                    break;
                case '%': tokens.push_back(tokenizeSPECIAL(TOKEN_MATH)); break;
                case ':': tokens.push_back(tokenizeSPECIAL(TOKEN_COLON)); break;
                case ',': tokens.push_back(tokenizeSPECIAL(TOKEN_COMMA)); break;
                case '.': tokens.push_back(tokenizeSPECIAL(TOKEN_DOT)); break;
                case '(': tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_PAREN)); break;
                case ')': tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_PAREN)); break;
                case '[': tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_SQR)); break;
                case ']': tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_SQR)); break;
                case '{': tokens.push_back(tokenizeSPECIAL(TOKEN_LEFT_BRACE)); break;
                case '}': tokens.push_back(tokenizeSPECIAL(TOKEN_RIGHT_BRACE)); break;
                case '@': tokens.push_back(tokenizeSPECIAL(TOKEN_REFERENCE)); break;
                case '&': tokens.push_back(tokenizeSPECIAL(TOKEN_MATH)); break;
                case '|': tokens.push_back(tokenizeSPECIAL(TOKEN_MATH)); break;
                case '^': tokens.push_back(tokenizeSPECIAL(TOKEN_MATH)); break;
                case '~': tokens.push_back(tokenizeSPECIAL(TOKEN_MATH)); break;
                case '\\':{
                    if(seek(1) == 'n') advance();
                    break;
                }
                case '/':{
                    tokens.push_back(tokenizeSPECIAL(TOKEN_DIV));
                    break;
                }
                default:
                if (current == '\n') {
                    tokens.push_back(tokenizeSPECIAL(TOKEN_NEWLINE));
                } else {
                    std::cout << "[!] LEXER ERROR: Undefined char '"<< current << "'\n";
                    std::cout << "Line number: " << lineNumber << ", Char number: " << charNumber << "\n";
                    exit(1);
                }
            }
        }

        return tokens;
    }

private:
    std::string source;
    int cursor;
    int size;
    char current;
    int lineNumber;
    int charNumber;
};

#endif