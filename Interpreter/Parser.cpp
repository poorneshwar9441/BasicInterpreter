#include "Parser.h"


// if the end is not EOF Then Throw an error; 
Node :: Node(string type,Token value,Node *left,Node *right){
        this->Type = type; 
        this->value = value; 
        this->left  = left; 
        this->right = right; 

        this->pos_start = value.startPos; 
        this->pos_end = value.endPos; 
        this->Line = value.Line; 
}

Parser :: Parser(vector<Token> tokens){
   this->tokens = tokens; 
   this->currPos = 0; 
   this->currToken = this->tokens[0];
}

void Parser::advance(){
    if(this->currPos+1 < tokens.size()){
        this->currPos += 1;
        this->currToken = this->tokens[this->currPos]; 
    }
    else{
        this->currToken = this->tokens[this->currPos]; 
    }
}
 //Node(string type,string value,Node *left,Node *right);

ParserResult Parser :: IfExpr(){


    Token t = this->currToken; 
    this->advance(); 

    vector<Node*> children; 

    ParserResult res1 = this->expression(); 
    if(not res1.status){
        return res1; 
    }
    if(this->currToken.value != "THEN"){
        ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED THEN",currToken.startPos,currToken.endPos,currToken.Line)}; 
        return p; 
    }
    else{
        this->advance();
        
        ParserResult res2 = this->expression(); 
        if(not res2.status){
            return res2; 
        }
        children.push_back(new Node("IF_CASE",t,res1.node,res2.node)); 
    }
    
    while(this->currToken.value == "ELIF"){
        this->advance(); 
        ParserResult res1 = this->expression();
        if(not res1.status){
            return res1; // propagate the error backwords; 
        }
        if(this->currToken.value != "THEN"){
             ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED THEN",currToken.startPos,currToken.endPos,currToken.Line)}; 
             return p; 
        }
        else{
            this->advance();
            ParserResult res2 = this->expression(); 
            if(not res2.status){
                return res2; 
            }
            children.push_back(new Node("ELIF_CASE",t,res1.node,res2.node)); 
        }
    }
    if(this->currToken.value == "ELSE"){
          this->advance(); 
          ParserResult res = this->expression(); 
          if(not res.status){
            return res; // propagate the error to parent functions; 
          }
          children.push_back(new Node("ELSE_CASE",t,res.node,NULL)); 
    }
    
    Node *n = new Node("IF_ELSE_LADDER",t,NULL,NULL); 
    n->children = children; 
    ParserResult p = {true,n, InvalidSyntaxError("",0,0,0)}; 

    return p; 
}


ParserResult Parser :: WhileExpr(){
    Token t = this->currToken; 
    this->advance(); 
    ParserResult p1 = this->expression();  // Grab the next expression
    if(not p1.status){
        return p1; 
    }
    if(this->currToken.value != "THEN"){
         ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED THEN",currToken.startPos,currToken.endPos,currToken.Line)};
         return p; 
    }
    else{
        this->advance(); 
        ParserResult p2 = this->expression(); // Grab the next expression 
        if(not p2.status){
            return p2; 
        }
        Node *n = new Node("WHILE",t,p1.node,p2.node); 
        ParserResult p = {true,n, InvalidSyntaxError("",0,0,0)}; 
        return p; 
    }
    
}

ParserResult Parser :: ForExpr(){

    Token t = this->currToken; 
    Node *n = new Node("FOR",t,NULL,NULL); 
    this->advance(); 
    if(this->currToken.Type != "ID"){
      ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED ID",currToken.startPos,currToken.endPos,currToken.Line)};
      return p; // Expected Identifier; 
    }
    else{
        n->var_name = this->currToken.value; 
        this->advance(); 
        if(this->currToken.value != "="){
            ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED Equals",currToken.startPos,currToken.endPos,currToken.Line)};
            return p; 
        }
        else{
            this->advance(); 
            ParserResult p1 = this->expression(); // grab the next expression 
            if(not p1.status){
                return p1; 
            }  
            n->start = p1.node;
            
            if(this->currToken.value != "TO"){
               ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED TO",currToken.startPos,currToken.endPos,currToken.Line)};
               return p; 
                
            }
            else{
                this->advance(); 
                ParserResult p = this->expression(); // grab the next end expression; 
                n->end = p.node; 
                
                if(this->currToken.value == "STEP"){
                     this->advance(); 
                     ParserResult step = this->expression(); // grab the next expression; 
                     if(not step.status){
                        return step;  // propagate the error to parent;
                     }
                     n->step = step.node; 
                }
                if(this->currToken.value != "THEN"){
                      ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED THEN",currToken.startPos,currToken.endPos,currToken.Line)};
                     return p; 
                
                }
                else{
                    this->advance(); 
                    ParserResult p = this->expression(); 
                    if(not p.status){
                        return p; 
                    }
                    n->todo = p.node; 
                }
               
            }
        }
    }
    ParserResult to_return = {true,n, InvalidSyntaxError("",0,0,0)}; 
    return to_return; 
}
// ParserResult Parser :: funcdef(){
//     Token t = this->currToken; 
//     this->advance(); 
  

//     vector<Token*> children; 
//     Node *body = NULL;

//     string name = ""; 
//     if(this->currToken.Type == "ID"){
//         name = this->currToken.value; 
//         this->advance(); 
//     }

//     if(this->currToken.value != "("){
//         ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED (",currToken.startPos,currToken.endPos,currToken.Line)};
//         return p; 
//     }
//     else{
//         this->advance(); 
        
//         while (this->currToken.Type == "ID"){
//             children.push_back(this->currToken); 
//             this->advance(); 
//         }
//         if(this->currToken.value != ")"){
//              ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED )",currToken.startPos,currToken.endPos,currToken.Line)};
//              return p; 
//         }
//         else{
//             this->advance(); 
//             if(this->currToken.value != "->"){
//                 ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED )",currToken.startPos,currToken.endPos,currToken.Line)};
//                  return p; 
//             }
//             else{
//                 this->advance(); 
//                 ParserResult p  = this->expression(); 
//                 if(not p.status){
//                     return p; 
//                 }
//                 body = p.node; 
//             }
//         }
//     }
//     Node *n = new Node("FuncDef",t,NULL,NULL); 
//     n->ids = children; 
//     n->body = body; 
    

//     ParserResult to_return = {true,n, InvalidSyntaxError("",0,0,0)}; 

//     return to_return; 
// }
ParserResult Parser :: factor(){
    Token t = this->currToken; 
    if(t.Type == INT || t.Type == FLOAT){
           Node *n = new Node("factor",t,NULL,NULL);
           ParserResult p = {true,n, InvalidSyntaxError("",0,0,0)}; 

           this->advance(); 

           return p; 
    }

    else if(t.value == "IF"){
        return this->IfExpr(); 
    }
    else if(t.value == "WHILE"){
        return this->WhileExpr();
    }
    else if(t.value == "FOR"){
         return this->ForExpr(); 
    }
    else if(t.value == "("){
          this->advance();
          ParserResult p = this->expression(); 
          if(not p.status){
             return p; 
          }

          if(this->currToken.value != ")"){
      
               ParserResult p1 = {false,NULL, InvalidSyntaxError("Bracket expected",currToken.startPos,currToken.endPos,currToken.Line)}; 
               this->advance(); 
               return p1; 
          }
          else{
              this->advance();
              return p; 
          }
    }

     else if(this->currToken.Type == "ID"){
         
         Node *n = new Node("VarAccess",this->currToken,NULL,NULL); 
         this->advance(); 
         ParserResult p = {true,n, InvalidSyntaxError("",0,0,0)}; 
         return p; 
    }
   
  

    ParserResult p = {false,NULL, InvalidSyntaxError("EXPECTED FLOAT OR INT",t.startPos,t.endPos,t.Line)}; 
    this->advance();
    return p; 
}

ParserResult Parser :: term(){
    ParserResult res = factor(); 
    if(not res.status){
        return res;  // propagate the error to parent function; 
    }
    Node* left = res.node; 


    while(this->currToken.Type == MUL || this->currToken.Type == DIV){
         Token curr = this->currToken; 
         this->advance();
        ParserResult res = factor(); 
        if(not res.status){
            return res;  // propagate the error to parent function;
        }
        Node *right = res.node; 
        left = new Node("Binop",curr,left,right); 
    }
    ParserResult p = {true,left, InvalidSyntaxError("",0,0,0)}; 
    
    return p; 
}


ParserResult Parser :: ArthEpression(){

         ParserResult res = term(); 
            if(not res.status){
                return res;  // propagate the error to parent function; 
            }
            Node* left = res.node; 


            while(this->currToken.Type == PLUS || this->currToken.Type == MINUS){
                Token curr = this->currToken; 
                this->advance();
                ParserResult res = term(); 
                if(not res.status){
                    return res;  // propagate the error to parent function;
                }
                Node *right = res.node; 
                left = new Node("Binop",curr,left,right); 
            }

          ParserResult p = {true,left, InvalidSyntaxError("",0,0,0)}; 
         return p; 

}

ParserResult Parser :: CompExpression(){

    if(this->currToken.value == "NOT"){
         Token t = this->currToken; 
         this->advance(); 
         ParserResult p = CompExpression(); 
         if(not p.status){
            return p;  // propagate the error back; 
         }
         else{
              Node *n = new Node("Uniop",t,p.node,NULL); 
              ParserResult p1 = {true,n, InvalidSyntaxError("",0,0,0)}; 
              return p1; 
         }
    }
    else{
        ParserResult res = ArthEpression(); 
        if(not res.status){
            return res;  // propagate the error
        }
        Node *left = res.node; 

        while((this->currToken.value == ">") || (this->currToken.value == "<") || (this->currToken.value == "<=") || (this->currToken.value == ">=") || (this->currToken.value == "==")){
            Token curr = this->currToken; 
            this->advance(); 
            ParserResult res = ArthEpression(); 
            if(not res.status){
                return res;  // propagate the error to parent; 
            }
            Node *right = res.node; 
            left = new Node("Binop",curr,left,right); 
        }
        ParserResult p = {true,left, InvalidSyntaxError("",0,0,0)}; 
        return p; 
    }
     
}

ParserResult Parser :: expression(){
    
     if(this->currToken.value == "VAR"){
          this->advance(); 

          if(this->currToken.Type != "ID"){ 
               ParserResult p1 = {false,NULL, InvalidSyntaxError("Expected Identifier",currToken.startPos,currToken.endPos,currToken.Line)}; 
               this->advance(); 
               return p1; 
          }
          else{
              Token t = this->currToken;
              this->advance(); 
              if(this->currToken.value != "="){
                    ParserResult p1 = {false,NULL, InvalidSyntaxError("Expected Equal sign",currToken.startPos,currToken.endPos,currToken.Line)}; 
                    this->advance(); 
                    return p1;  
              }
              else{
                  //Token t = this->currToken; 
                  this->advance(); 
                  ParserResult res = this->expression(); 
                  if(not res.status){
                     return res; 
                  }

                  Node *n = new Node("VarAssign",t,res.node,NULL); 

                  ParserResult p = {true,n, InvalidSyntaxError("",0,0,0)}; 

                  return p; 
              }
          }
    }
    else{
           ParserResult res = CompExpression(); 
            if(not res.status){
                return res;  // propagate the error
            }
            Node *left = res.node; 

            while((this->currToken.value == "AND") || (this->currToken.value == "OR")){
                Token curr = this->currToken; 
                this->advance(); 
                ParserResult res = ArthEpression(); 
                if(not res.status){
                    return res;  // propagate the error to parent; 
                }
                Node *right = res.node; 
                left = new Node("Binop",curr,left,right); 
            }
            ParserResult p = {true,left, InvalidSyntaxError("",0,0,0)}; 
            return p;         
    }
}

ostream& operator << (std :: ostream&out, Node *p){
    if(p->Type == "factor"){
        out <<  p->value.value; 
    }
    else if(p->Type == "Binop"){
        out << "( "  << p->left << " " << p->value.value << " " << p->right << " )"; 
    }

    return out;
}


Parser :: ~Parser(){
    // Traverse the Tree and free each Node; 

}