#include "Lexer.h"
#include "Parser.h"
#include "eval.h"

int main(){
    
    
    Interpreter i(new Context(new Variable_Table())); 
    while(true){
        cout << "basic>"; 
        string in = ""; 
        getline(cin,in);  
        
        if(in == "exit"){
            break; 
        }
        else{
           Tokenizer t(in); 

        //    for(Token t : t.tokenize()){
        //       cout << t; 
        //    }
        //    cout << endl;
           
           vector<Token> toks = t.tokenize();
           //cout << toks.size() << endl;
           Parser P(toks);
           ParserResult R = P.expression(); 
           if(R.status){
             //cout << R.node << endl; 
             //Interpreter i(R.node,new Context(new Variable_Table())); 
             cout << i.eval(R.node) << endl;
             
           }
           else{
             cout << "Error : " << R.error.filename <<  endl; 
           }
        }
    }
}