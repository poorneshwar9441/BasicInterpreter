#pragma once
#include "Parser.h"
#include<unordered_map>



class Variable_Table{
    public:
      unordered_map<string,float> map; 
      Variable_Table *parent; 
}; 

// This keeps track of the current context; 
class Context{
    public:
      string name; 
      Context *Parent;  // Parent Context; 
      Variable_Table *Table; 
      Context(Variable_Table *Table);

};


class Interpreter{

    public: 
      Context *context; // Global context; 

      Interpreter(Context *context){
        this->context = context; 
      }

      float visit(Node *root,Context *context);
      float eval(Node *root); 
     
}; 