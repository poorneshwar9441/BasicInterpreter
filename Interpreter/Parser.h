#pragma once
#include<iostream> 
#include "Lexer.h"
#include "Error.h"
using namespace std; 

// #define BinaryOpNode // 2 childs
// #define NumberNode // 0 childs
// #define UniaryOpNode // 1 child 


class Node{
  public: 
    string Type; 
    Token value; 
    Node *left; 
    Node *right; 
    int pos_start; 
    int pos_end; 
    int Line;

    // vairaibles for for expr Node;
    string var_name; 
    Node* step; 
    Node *start; 
    Node *end;
    Node *todo; 


    // variables for function definition
    Node *body; // for storing body of the function; 
    vector<Token*> ids;

    vector<Node *> children; // can be used for IF NODE; 
   

    Node(string type,Token value,Node *left,Node *right);
}; 

struct ParserResult{
    bool status; // true if successful
    Node *node; // return value on success; 
    InvalidSyntaxError error;  // error to display; 
}; 

class Parser{

   public:
    vector<Token> tokens; 

    int currPos; 
    Token currToken; 

  


    Parser(vector<Token> tokens); 
    ParserResult expression(); 
    ParserResult term(); 
    ParserResult factor(); 
    ParserResult CompExpression(); 
    ParserResult  ArthEpression(); 
    ParserResult  IfExpr();
    ParserResult  WhileExpr();
    ParserResult ForExpr();
    ParserResult  funcdef(); 
    void advance(); 
    ~Parser(); // free the allocated memory
}; 

ostream& operator << (std :: ostream&out, Node *p);
   