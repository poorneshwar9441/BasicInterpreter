#pragma once

#include<string> 
#include<vector> 
#include<utility>
#include<iostream>

#define INT "INT"
#define FLOAT "FLOAT"
#define PLUS "PLUS"
#define MINUS "MINUS"
#define LEFTPARAN "LEFTPARAN"
#define RIGHTPARAN "RIGHTPARAN"
#define DIV "DIV"
#define MUL "MUL"

using namespace std; 



class Token{
    public:
     string value; 
     string Type; 
     int startPos; // start position of the token in the line
     int endPos;  // end position of the token in the line
     int Line;   // line number

     Token(); 

     Token(string value,string Type,int startPos,int endPos,int Line); 

}; 


class Tokenizer{

   public: 
     string text; 
     char currentchar; 
     int currPointer; 
     Tokenizer(string text);
     bool advance();
     vector<Token> tokenize(); 
     ~Tokenizer(); 
     pair<string,bool> MakeNum(int &r,int &c); 
     pair<string,bool>  MakeWord(int &r,int &c);
     pair<string,bool> MakeEquals(int &r,int &c);
     pair<string,bool> MakeReturn(int &r,int &c);
}; 




void  operator << (std::ostream& out, Token &t);