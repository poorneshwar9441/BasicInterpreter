#include "eval.h"

float Interpreter :: visit(Node *root,Context *context){
    if(root->Type ==  "Binop"){

        if(root->value.value == "*"){
           return visit(root->left,context) * visit(root->right,context); 
        }
        else if(root->value.value == "-"){
           return visit(root->left,context) - visit(root->right,context); 
        }
        else if(root->value.value == "+"){
            return visit(root->left,context) + visit(root->right,context); 
        }
        else if(root->value.value == "/"){
            // Raise RTerror divsion by 0
            return visit(root->left,context)/visit(root->right,context); 
        }
        else if(root->value.value == "=="){
             return visit(root->left,context) == visit(root->right,context); 
        }
        else if(root->value.value == ">="){
             return visit(root->left,context) >= visit(root->right,context); 
        }
        else if(root->value.value == ">"){
             return visit(root->left,context) > visit(root->right,context); 
        }
        else if(root->value.value == "<"){
            return visit(root->left,context) < visit(root->right,context); 
        }
        else if(root->value.value == "AND"){
             return visit(root->left,context) and visit(root->right,context); 
        }
        else if(root->value.value == "OR"){
             return visit(root->left,context) || visit(root->right,context); 
        }
    }
    else if(root->Type == "factor"){
        return stof(root->value.value);
    }
    else if(root->Type == "VarAssign"){
        
       return this->context->Table->map[root->value.value] = visit(root->left,context);
    }
    else if(root->Type == "Uniop"){
        if(root->value.value == "NOT"){
            return not (visit(root->left,context)); 
        }
    }
    else if(root->Type == "VarAccess"){
        Context *curr = context; 
        while(curr == NULL || curr->Table->map.find(root->value.value) == curr->Table->map.end()){
           curr = curr->Parent; 
        }
        if(curr == NULL){
            // Raise Variable Not Found Error; 
        }
        return curr->Table->map[root->value.value]; 
    }
    else if(root->Type == "IF_ELSE_LADDER"){
        for(Node *childs : root->children){
            if(childs->Type == "IF_CASE" || childs->Type == "ELIF_CASE"){
                 if(visit(childs->left,context)){
                     return visit(childs->right,context); 
                 }
            }
            else if(childs->Type == "ELSE_CASE"){
                 return visit(childs->left,context); 
            }
        }
    }
    else if(root->Type == "WHILE"){
        int val = 0; 
        while(visit(root->left,context)){
            val =  visit(root->right,context); 
        }
        return val;
    }
    else if(root->Type == "FOR"){
      string var_name =  root->var_name; 
      this->context->Table->map[var_name] = visit(root->start,context); 
      
      int val = 0; 
      while(this->context->Table->map[var_name] <= visit(root->end,context)){
            val = visit(root->todo,context); 
            if(root->step != NULL)
               this->context->Table->map[var_name] += visit(root->step,context); 
            else 
               this->context->Table->map[var_name] += 1; 
      }
      return val; 
    }
    else{
        return 0;
    }
}

float Interpreter :: eval(Node *root){
    return visit(root,this->context); 
}

Context :: Context(Variable_Table *Table){
   this->Table = Table; 
}