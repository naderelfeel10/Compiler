
#include <iostream>
#include <vector>
#include <string>
#include "lexer.hpp"
#include <regex>
#include <vector>
#include <algorithm> 
using namespace std;
/*
enum TokenType {
    ID, ASSIGN, INT, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, SEMICOLON, END
};*/


/*
program         → statement_list

statement_list  → INDENT { statement } DEDENT

statement       → assignment_stmt   //done
                | aug_assignment_stmt //done
                | func_def  //done
                | if_stmt //done
                | for_stmt //done
                | class_def //done
                | while_stmt //done
                | return_stmt //done
                | func_call  // done
                | pass_stmt //done
                | expr_stmt //done

assignment_stmt → ID "=" expr ";"
aug_assignment_stmt → ID ("+=" | "-=" | "*=" | "/=") expr
func_def        → "def" ID "(" [param_list] ")" ":" statement_list
if_stmt → "if" expr ":" suite
          { "elif" expr ":" suite }
          [ "else" ":" suite ]

for_stmt        → "for" ID "in" expr ":" statement_list

class_def       → "class" ID ":" statement_list

param_list      → ID { "," ID }


while_stmt      → "while" expr ":" statement_list

return_stmt     → "return" [expr]
pass_stmt       → "pass" 
expr_stmt       → expr

expr            → or_expr
or_expr         → and_expr { "or" and_expr }
and_expr        → not_expr { "and" not_expr }
not_expr        → [ "not" ] comparison
comparison      → expr [ ( "==" | "!=" | ">" | "<" | ">=" | "<=" ) expr ]

expr      → term { ( "+" | "-" ) term }
term            → factor { ( "*" | "/" ) factor }
factor          → ID | INT | DOUBLE | STRING | expr |"(" expr ")" | func_call | list

func_call       → ID "(" [arg_list] ")"
arg_list        → expr { "," expr }

list → "[" [ arg_list ] "]"
*/

struct ParseTreeNode {
    std::string label;
    std::vector<ParseTreeNode*> children;

    ParseTreeNode(const std::string& lbl) : label(lbl) {}

    ~ParseTreeNode() {
        for (auto child : children) delete child;
    }

    void addChild(ParseTreeNode* child) {
        children.push_back(child);
    }
};


class Parser {
    std::vector<TOKEN*> tokens; // Vector of pointers
    int current;

public:
    Parser(std::vector<TOKEN*> t) : tokens(t), current(0) {}
std::vector<std::string> function_names2 = {
    "abs", "all", "any", "ascii", "bin", "bool", "breakpoint", "bytearray",
    "bytes", "callable", "chr", "classmethod", "compile", "complex", "delattr",
    "dict", "dir", "divmod", "enumerate", "eval", "exec", "filter", "float",
    "format", "frozenset", "getattr", "globals", "hasattr", "hash", "help",
    "hex", "id", "input", "int", "isinstance", "issubclass", "iter", "len",
    "list", "locals", "map", "max", "memoryview", "min", "next", "object",
    "oct", "open", "ord", "pow", "print", "property", "range", "repr",
    "reversed", "round", "set", "setattr", "slice", "sorted", "staticmethod",
    "str", "sum", "super", "tuple", "type", "vars", "zip", "__import__"
};
    std::vector<string> list_names;

    TOKEN* peek() {
        if (current < tokens.size()) return tokens[current];
        static TOKEN endToken = { TOKEN_EOF, "",0 };
        return &endToken;
    }

    TOKEN* advance() {
        if (current < tokens.size()) return tokens[current++];
        static TOKEN endToken = { TOKEN_EOF, "",0 };
        return &endToken;
    }


    bool check_regex(const std::string& line,string regex) {
    std::regex func_regex(regex);
    return std::regex_match(line, func_regex);
}


    bool match(type expected) {
        if (peek()->TYPE == expected) {
            advance();
            return true;
        }
        return false;
    }

    void parse() {
        cout << "Starting parsing...\n";
        //assignment_stmt();
       // statement_list();
                try {
    ParseTreeNode* tree = statement_list();
    printParseTree(tree);
    delete tree;
} catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
        cout << "Parsing complete!\n";
        cout << "No Parsing Errors\n";

    }


    void printParseTree(ParseTreeNode* node, const std::string& prefix = "", bool isLast = true) {
    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
    std::cout << node->label << std::endl;

    for (size_t i = 0; i < node->children.size(); ++i) {
        printParseTree(node->children[i], prefix + (isLast ? "    " : "│   "), i == node->children.size() - 1);
    }
}


    ParseTreeNode* statement_list() {
    ParseTreeNode* node = new ParseTreeNode("statement_list");
    while (peek()->TYPE != TOKEN_EOF) {
    ParseTreeNode* stmt_node = statement();
        node->addChild(stmt_node);
    }
    return node;
    }

    ParseTreeNode* statement() {
    ParseTreeNode* node = new ParseTreeNode("statement");
    
    int curr2 = current+1;
    cout<<current<<curr2;
    if (peek()->TYPE == TOKEN_ID && tokens[curr2]->TYPE == TOKEN_EQUALS) {
       ParseTreeNode* assign_node= assignment_stmt();
       node->addChild(assign_node);
        //assignment_stmt();
    } else if (peek()->TYPE == TOKEN_ID && ( tokens[curr2]->TYPE == TOKEN_PLUS
         || tokens[curr2]->TYPE == TOKEN_MINUS|| 
        tokens[curr2]->TYPE == TOKEN_MUL||
        tokens[curr2]->TYPE == TOKEN_DIV
        )) {
               ParseTreeNode* aug_assign_node= aug_assignment_stmt();
               node->addChild(aug_assign_node);
        //aug_assignment_stmt();
    }else if (peek()->VALUE == "def"){
            ParseTreeNode* func_node= func_def();
            node->addChild(func_node);
       // func_def();
    }else if ((peek()->VALUE=="if") ){
            ParseTreeNode* if_node= if_stmt();
            node->addChild(if_node);
        //if_stmt();
    }else if (peek()->VALUE=="for"){
                    ParseTreeNode* for_node= for_stmt();
            node->addChild(for_node);

    }else if (peek()->VALUE=="class"){
            ParseTreeNode* class_node= class_stmt();
            node->addChild(class_node);
        //class_stmt();
    }else if(peek()->VALUE=="while"){
            ParseTreeNode* while_node= while_stmt();
            node->addChild(while_node);
        //while_stmt();
    }else if (peek()->VALUE=="return"){
            ParseTreeNode* return_node= return_stmt();
            node->addChild(return_node);
            //return_stmt();
    }else if (peek()->VALUE=="pass"){
        advance();
    }
    else if (peek()->VALUE=="break"){
            node->addChild(new ParseTreeNode("\"break\""));
            advance();
            //return_stmt();
    }
        else if (peek()->VALUE=="continue"){
            node->addChild(new ParseTreeNode("\"continue\""));
            advance();
            //return_stmt();
    }
    else if(std::find(function_names2.begin(), function_names2.end(),peek()->VALUE ) != function_names2.end())
    {
                    ParseTreeNode* func_node= func_call();
            node->addChild(func_node);
            //func_call();
    }
    else if(std::find(list_names.begin(), list_names.end(),peek()->VALUE ) != list_names.end()){
                    ParseTreeNode* list_node= list_call();
            node->addChild(list_node);
          //list_call();
    }
    else {

        error("Unrecognized statement.");
    }
    cout<<"Exiting statement"<<endl;
    return node;
}
//aug_assignment_stmt → ID ("+=" | "-=" | "*=" | "/=") expr
ParseTreeNode* aug_assignment_stmt() {
    cout << "Entering aug_assignment_stmt()\n";
        ParseTreeNode* node = new ParseTreeNode("aug_assignment_stmt");
        string id = peek()->VALUE;
        node->addChild(new ParseTreeNode("ID ("+id+")"));
        
    if (match(TOKEN_ID)) {
        cout << "Matched ID\n";
        id= peek()->VALUE;
        if(match(TOKEN_MINUS)||match(TOKEN_PLUS)||match(TOKEN_MUL)||match(TOKEN_DIV)){
                node->addChild(new ParseTreeNode("MATH ID ("+id+")"));
                cout << "Matched ' + or - or * or / '\n";
                if (match(TOKEN_EQUALS)) { // check if the next token is '='
                node->addChild(new ParseTreeNode(""+id+"="));
                cout << "Matched ' += or -= or *= or /= '\n";  // a +=5
                ParseTreeNode* exp_node = expr();
                node->addChild(exp_node);
                }else {
                error("Expected '='"); //it has to be '=' op
                }
            }
}else {
            error("Expected identifier"); // it has to be id
        }
    cout << "Exiting aug_assignment_stmt()\n";
 return node;
}

//assignment_stmt → ID "=" expr
ParseTreeNode* assignment_stmt() {
        cout << "Entering assignment_stmt()\n";
        ParseTreeNode* node = new ParseTreeNode("assingment_stmt");
        string id = peek()->VALUE;
        node->addChild(new ParseTreeNode("ID ("+id+")"));

        if (match(TOKEN_ID)) { //check if it's id 
            cout << "Matched ID\n";

            if (match(TOKEN_EQUALS)) { // check if the next token is '='
             node->addChild(new ParseTreeNode("\"=\""));
                cout << "Matched '='\n";
                 id = peek()->VALUE;
                if(match(TOKEN_LEFT_SQR)){
                    node->addChild(new ParseTreeNode("\"[\""));
                    ParseTreeNode* argnode = arg_list();
                    node->addChild(argnode);

                
                    if(!match(TOKEN_RIGHT_SQR)){
                     node->addChild(new ParseTreeNode("\"]\""));
                        error("invalid list decalration");
                    }
                    cout<<"list created";
                    list_names.push_back(id);
                } 
                else if(std::find(function_names2.begin(), function_names2.end(),peek()->VALUE ) != function_names2.end())
                     {
                    ParseTreeNode* fncnode = func_call();
                    node->addChild(fncnode);
                    }

               else if(std::find(list_names.begin(), list_names.end(),peek()->VALUE ) != list_names.end()){
                    ParseTreeNode* listnode = list_call();
                    node->addChild(listnode);  
                       // list_call();
                       }
                       else if(match(TOKEN_STRING)){
                           node->addChild(new ParseTreeNode("String \""+id+"\""));
                       } 
                       else{
                           ParseTreeNode* exp_node = expr();
                           node->addChild(exp_node); 
                        //expr();
                        }
   // if yes call expression function                                                                              
            }

        }
         else {
            error("Expected identifier"); // it has to be id
        }

        cout << "Exiting assignment_stmt()\n"; // done
        return node;
    }

//func_def → "def" ID "(" [param_list] ")" ":" statement_list

ParseTreeNode* func_def() {
    ParseTreeNode* node = new ParseTreeNode("func_Call_stmt");
     node->addChild(new ParseTreeNode("\"def\""));
    
    string line = "def ";
    int curr = current + 1;

    while (curr < tokens.size() && tokens[curr]->VALUE != ":") {
        string tok = tokens[curr]->VALUE;
        line += (tok + " ");
        curr++;
    }
     node->addChild(new ParseTreeNode("funcName("+line+")"));
      
    if (curr >= tokens.size()) {
        error("':' not found in function definition");
        //return ;
    }
    node->addChild(new ParseTreeNode("\":\""));
    line += ":";

    cout << "line: " << line << endl;

    std::regex func_regex(R"(^\s*def\s+([a-zA-Z_][a-zA-Z0-9_]*)\s*\([^()]*\)\s*(->\s*[a-zA-Z_][a-zA-Z0-9_]*\s*)?:)");
    std::smatch match;

    if (std::regex_match(line, match, func_regex)) {
        std::cout << " Valid Python function definition\n";
        std::string func_name = match[1];  // Capture group for function name
        function_names2.push_back(func_name);
        std::cout << "Function name added: " << func_name << "\n";
    } else {
        std::cout << " Not a valid function definition\n";
    }

    current = curr + 1;  // move past ':'
    return node;
}

ParseTreeNode* func_call(){
     ParseTreeNode* node = new ParseTreeNode("\"func_call\"");
    
    string function_name=peek()->VALUE;
      node->addChild(new ParseTreeNode("functionName("+function_name+")"));

    //error(function_name);
    advance();
    if(match(TOKEN_LEFT_PAREN)){
        //error(peek()->VALUE);
        node->addChild(new ParseTreeNode("\"(\""));
     if(std::find(function_names2.begin(), function_names2.end(),peek()->VALUE ) != function_names2.end())
        {
            ParseTreeNode* func_node= func_call();
            node->addChild(func_node);
            //func_call();
        }
        else{
        ParseTreeNode* arg_node = arg_list();
        node->addChild(arg_node);
        //advance();
        //error(peek()->VALUE);
        }
        if(!match(TOKEN_RIGHT_PAREN)){
            advance();
            error(") missed while calling the function");
        }else{
        //error(peek()->VALUE);
        node->addChild(new ParseTreeNode("\")\""));

           cout<<function_name<<"function called ";
            //advance();
        //error(peek()->VALUE);
        }

    }else{
        
        error("( missed while calling the function");
    }
    return node;
}

ParseTreeNode* list_call(){
    ParseTreeNode* node = new ParseTreeNode("list_Call_stmt");
     node->addChild(new ParseTreeNode("\"list\""));

    string list_name=peek()->VALUE;
    advance();
    if(match(TOKEN_LEFT_SQR)){
        //error(peek()->VALUE);
         node->addChild(new ParseTreeNode("\"[\""));
        ParseTreeNode* exp_node = expr();
        node->addChild(exp_node);
        //expr();
        //advance();
        //error(peek()->VALUE);
        if(!match(TOKEN_RIGHT_SQR)){
            advance();
            error("] missed while calling the list");
        }else{
        //error(peek()->VALUE);
         node->addChild(new ParseTreeNode("\"]\""));
           cout<<list_name<<" list called ";
            //advance();
           //error(peek()->VALUE);
        if(peek()->VALUE == "="){
            advance();
         node->addChild(new ParseTreeNode("\"=\""));

            ParseTreeNode* exp_node = expr();
            node->addChild(exp_node);
            //expr();

        }
        }
    }else{
        
        error("[ missed while calling the list");
    }
    return node;
}

ParseTreeNode* while_stmt(){
    ParseTreeNode* node = new ParseTreeNode("while_stmt");
     node->addChild(new ParseTreeNode("\"while\""));

    advance();
    ParseTreeNode* comp_node = comparison();
    node->addChild(comp_node);
   //comparison();
if(peek()->VALUE==":"){
    node->addChild(new ParseTreeNode("\":\""));
    cout<<"#### valid while loop";
    advance();
}else{
    error(": missed from while ");
}
   return node;

}

ParseTreeNode* return_stmt(){
    ParseTreeNode* node = new ParseTreeNode("return_stmt");
     node->addChild(new ParseTreeNode("\"return\""));

   advance();
    ParseTreeNode* comp_node = comparison();
    node->addChild(comp_node);
   //comparison();
   return node;
   cout<<" ****** return stmt";
}

ParseTreeNode* if_stmt() {
    ParseTreeNode* node = new ParseTreeNode("if_stmt");
 if(peek()->VALUE=="if"){    
    node->addChild(new ParseTreeNode("\"if\""));
    advance();
    ParseTreeNode* compNode = comparison();  // Now returns a subtree
      node->addChild(compNode);
     //comparison();

}
if(peek()->VALUE==":"){
 cout<<endl<<" #$ if parsed"<<endl;
    node->addChild(new ParseTreeNode("\":\""));
 advance();

    while (peek()->TYPE != TOKEN_EOF) {
        if(peek()->VALUE=="elif" || peek()->VALUE=="else"){break;}
        ParseTreeNode* stmt_node = statement();
        node->addChild(stmt_node);
    }
     cout<<"leo23333"<<peek()->VALUE<<endl;
  while(peek()->VALUE=="elif"||peek()->VALUE=="else"){
    if(peek()->VALUE=="elif"){
     node->addChild(new ParseTreeNode("keyword (" + peek()->VALUE + ")"));
    advance();
    ParseTreeNode* comp_node = comparison();
    node->addChild(comp_node);
    //comparison();
    }else if (peek()->VALUE=="else"){
     node->addChild(new ParseTreeNode("keyword (" + peek()->VALUE + ")"));
        advance();}
    if(peek()->VALUE==":"){
       node->addChild(new ParseTreeNode("\":\""));
         cout<<endl<<" #$ elif or else parsed"<<endl;
         advance();
while (peek()->TYPE != TOKEN_EOF) {
        if(peek()->VALUE=="elif" || peek()->VALUE=="else"){break;}
            ParseTreeNode* stmt_node = statement();
            node->addChild(stmt_node);
            //statement();
    }    }else{
    error(" : missed");

    }
 }
}else{
    error(" : missed");
}

return node;

}

ParseTreeNode* for_stmt() {
    cout << "Entering for_stmt()\n";
    ParseTreeNode* node = new ParseTreeNode("for_stmt");

    if (peek()->VALUE != "for") error("Expected 'for'");
     node->addChild(new ParseTreeNode("\"for\""));
    advance();

    if (peek()->TYPE != TOKEN_ID) error("Expected identifier after 'for'");
     node->addChild(new ParseTreeNode("ID (" + peek()->VALUE + ")"));
    advance();

    if (peek()->VALUE != "in") error("Expected 'in'");
    node->addChild(new ParseTreeNode("\"in\""));
     advance();

      ParseTreeNode* exprNode = expr_for();  // Now returns a subtree
      node->addChild(exprNode);

    if (!match(TOKEN_COLON)) error("Expected ':' at end of for loop header");
    node->addChild(new ParseTreeNode("\":\""));

    cout << "Parsed valid for-statement\n";
    cout << "Exiting for_stmt()\n";
    return node;
}
ParseTreeNode* expr_for() {
    cout << "Entering expr_for()\n";
    ParseTreeNode* exprNode = new ParseTreeNode("expr");

    if (peek()->VALUE == "range") {
        ParseTreeNode* rangeNode = new ParseTreeNode("range_expr");
        rangeNode->addChild(new ParseTreeNode("\"range\""));
        advance(); // consume 'range'

        if (!match(TOKEN_LEFT_PAREN)) error("Expected '(' after 'range'");
        rangeNode->addChild(new ParseTreeNode("\"(\""));

        while (!match(TOKEN_RIGHT_PAREN)) {
            if (peek()->TYPE == TOKEN_INT) {
                rangeNode->addChild(new ParseTreeNode("INT (" + peek()->VALUE + ")"));
                advance();
                cout<<peek()->VALUE;
            } else if (peek()->TYPE == TOKEN_COMMA) {
                rangeNode->addChild(new ParseTreeNode("\",\""));
                advance();
            } else {
                error("Expected integer or ',' in range()");
            }
        }
        cout<<peek()->VALUE;
        --current;
        cout<<peek()->VALUE;

        if (!match(TOKEN_RIGHT_PAREN)) error("Expected ')' after range");
        rangeNode->addChild(new ParseTreeNode("\")\""));
        exprNode->addChild(rangeNode);

    } else if (match(TOKEN_LEFT_SQR)) {
        ParseTreeNode* listNode = new ParseTreeNode("list_expr");
        listNode->addChild(new ParseTreeNode("\"[\""));

        while (!match(TOKEN_RIGHT_SQR)) {
            if (peek()->TYPE == TOKEN_INT) {
                listNode->addChild(new ParseTreeNode("INT (" + peek()->VALUE + ")"));
                advance();
            } else if (peek()->TYPE == TOKEN_COMMA) {
                listNode->addChild(new ParseTreeNode("\",\""));
                advance();
            } else {
                error("Expected list item or ',' in list");
            }
        }
         current--;
        if (!match(TOKEN_RIGHT_SQR)) error("Expected ']' after list");
        listNode->addChild(new ParseTreeNode("\"]\""));
        exprNode->addChild(listNode);

    } else if (std::find(list_names.begin(), list_names.end(), peek()->VALUE) != list_names.end()) {
        ParseTreeNode* idNode = new ParseTreeNode("ID (" + peek()->VALUE + ")");
        exprNode->addChild(idNode);
        advance();

    } else if(match(TOKEN_STRING)){
            current--;
            exprNode->addChild(new ParseTreeNode("String \""+peek()->VALUE+"\""));
            advance();
        }else {
        error("Invalid expression in for-loop");
    }

    cout << "Exiting expr_for()\n";
    return exprNode;
}

ParseTreeNode* class_stmt(){
    ParseTreeNode* node = new ParseTreeNode("class_stmt");
     node->addChild(new ParseTreeNode("\"class\""));

    advance(); //skip class keyword
    cout<<"Yoo Nigga"<<peek()->VALUE<<peek()->TYPE;
    if(match(TOKEN_ID)){
        current--;
        node->addChild(new ParseTreeNode("ID (" + peek()->VALUE + ")"));
        advance();
        cout<<"Yoo Nigga"<<peek()->VALUE<<peek()->TYPE;
        if(match(TOKEN_COLON)){
        node->addChild(new ParseTreeNode("\":\""));
        }else{
            advance();
            error(": missed from class");
        }
    }else{
            advance();
        error("Invalid Class Name");

    }
    return node;
}

ParseTreeNode* arg_list() {
    cout << "Entering arg_list()\n";
    ParseTreeNode* node = new ParseTreeNode("arg_list");
    cout<<"ma nnigga00 "<<peek()->VALUE;
     
    ParseTreeNode* expNode = expr();
    node->addChild(expNode);
    //expr();  // parse the first expression

    //advance();
    while (peek()->TYPE == TOKEN_COMMA) {
        node->addChild(new ParseTreeNode("\",\""));
        advance();   // skip the comma
       expNode = expr();
       node->addChild(expNode);
      //  expr();      // parse the next expression
    }
   
    cout << "Exiting arg_list()\n";
        return node;

}

ParseTreeNode* comparison() {
    cout << "Entering comparison()\n";
    ParseTreeNode* node = new ParseTreeNode("compare_stmt");
    ParseTreeNode* exprNode =  expr();  // First handle the left-hand arithmetic expression
    node->addChild(exprNode);

    // If next token is a comparison operator
    if (peek()->TYPE == TOKEN_REL_GREATERTHAN || peek()->TYPE == TOKEN_REL_LESSTHAN ||
        (peek()->TYPE==TOKEN_REL_GREATERTHANEQUALS) || peek()->TYPE == TOKEN_REL_LESSTHANEQUALS ||
        peek()->TYPE == TOKEN_REL_EQUALS || peek()->TYPE == TOKEN_REL_NOTEQUALS) {
        advance();   // consume the operator
         exprNode =  expr();  // First handle the left-hand arithmetic expression
      node->addChild(exprNode);
       // expr();      // parse the right-hand expression
    }

    cout << "Exiting comparison()\n";
        return node;

}

ParseTreeNode* expr() {
        cout << "Entering expr()\n";
        ParseTreeNode* node = new ParseTreeNode("expr_stmt");
        ParseTreeNode* termNode = term();
        node->addChild(termNode);

        while (peek()->TYPE == TOKEN_MINUS || peek()->TYPE == TOKEN_PLUS) {  // check if the current token is '+' or '-'
            node->addChild(new ParseTreeNode("\""+peek()->VALUE+"\""));
         advance();  // if yes move to next token
         termNode = term();
        node->addChild(termNode);
            //term();    // then call the term again 
            // a = 5 + 10*20*14-4
        }
        while(peek()->VALUE=="and" ||peek()->VALUE=="or" || peek()->VALUE=="not"){
            node->addChild(new ParseTreeNode("\""+peek()->VALUE+"\""));
            advance();
         termNode = expr_stmt();
        node->addChild(termNode);
            //expr_stmt();
        }
        cout << "Exiting expr()\n";
        return node;

    }

ParseTreeNode* term() {
        cout << "Entering term()\n";
        ParseTreeNode* node = new ParseTreeNode("term_stmt");
        ParseTreeNode* termNode = factor();
        node->addChild(termNode);
        //factor(); // calls factor to check the right hand
        // a = 5 
        while (peek()->TYPE == TOKEN_MUL || peek()->TYPE == TOKEN_DIV) { // check if the current token is '*' or '/'
            node->addChild(new ParseTreeNode("\""+peek()->VALUE+"\""));

            advance();  // if yes move to next token
            //ParseTreeNode* termNode = factor();
            termNode = factor();
            node->addChild(termNode);
                   // factor();  // then call factor again to check the right hand 
        // a = 5*10  or   a = 5 * b  or 5 *(3+4)
            
        }
        cout << "Exiting term()\n";
        return node;

    }

ParseTreeNode* factor() {
        cout << "Entering factor()\n";
        ParseTreeNode* node = new ParseTreeNode("factor_stmt");
        string id = peek()->TYPE==TOKEN_INT?"INT": peek()->TYPE==TOKEN_DOUBLE?"DOUBLE":"null";
        if (match(TOKEN_INT)||match(TOKEN_DOUBLE)) { // check if int 
            current--;
           node->addChild(new ParseTreeNode(""+id+" (" + peek()->VALUE + ")"));
            cout << "Matched INT\n";
            advance();
        } else if (match(TOKEN_ID)) { // if not 'int' check if another id like this :  a = b
            current--;
           node->addChild(new ParseTreeNode("ID (" + peek()->VALUE + ")"));
            advance();
            cout << "Matched ID\n";
        } else if (match(TOKEN_LEFT_PAREN)) { //else check if expression like : a = (3+5)
            node->addChild(new ParseTreeNode("\"(\""));
            ParseTreeNode* compNode = comparison();
            node->addChild(compNode);
            //comparison(); // call expression
            if (!match(TOKEN_RIGHT_PAREN)) error("Expected ')'");
            node->addChild(new ParseTreeNode("\")\""));

        }
         else {
            error("Expected INT, ID, or '('");
        }
        cout << "Exiting factor()\n";
        return node;

    }

ParseTreeNode* expr_stmt() {
        ParseTreeNode* node = new ParseTreeNode("expr_stmt");

        ParseTreeNode* or_expr_node =or_expr();
        node->addChild(or_expr_node);
        cout << "Parsed expression statement successfully.\n";
        return node;
    }

ParseTreeNode* or_expr() {
        ParseTreeNode* node = new ParseTreeNode("or_expr_stmt");
        node->addChild(new ParseTreeNode("OR"));
        ParseTreeNode* or_expr_node =and_expr();
        node->addChild(or_expr_node);
       // and_expr();
        while (peek()->VALUE=="or"){
            or_expr_node = and_expr();
            node->addChild(or_expr_node);
            //and_expr();
            cout << "Parsed 'or' expression.\n";
        
        }
        return node;

    }
ParseTreeNode* and_expr() {
        ParseTreeNode* node = new ParseTreeNode("and_expr_stmt");
        node->addChild(new ParseTreeNode("AND"));

        ParseTreeNode* not_expr_node =not_expr();
        node->addChild(not_expr_node);
        //not_expr();
        while (peek()->VALUE=="and") {
         not_expr_node =not_expr();
         node->addChild(not_expr_node);

           // not_expr();
            cout << "Parsed 'and' expression.\n";
            
        }
        return node;

    }
ParseTreeNode* not_expr() {
        ParseTreeNode* node = new ParseTreeNode("not_expr_stmt");
        node->addChild(new ParseTreeNode("NOT"));

        if ((peek()->VALUE=="not")) {
            ParseTreeNode* not_exp_node = not_expr();
            node->addChild(not_exp_node);
            //not_expr();
            cout << "Parsed 'not' expression.\n";
        } else {
            ParseTreeNode* comp_node = comparison();
            node->addChild(comp_node);
            //comparison();
        }
        return node;
    }


    void error(const string& msg) {
        cerr << "Parse error: " << msg << " at token '" << peek()->VALUE << "'\n";
        exit(1);
    }
};
