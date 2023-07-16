#include "Lexer.h"

void operator << (std :: ostream&out,Token &t){
      out << "(" << t.value  << " " << t.Type <<  " " <<  t.startPos  <<  " " << t.endPos <<  ")"  << " ";
}

Token :: Token(){
    this->value = ""; 
    this->Type = ""; 
    this->startPos = 0; 
    this->endPos= 0; 
    this->Line = 0;
}
Token :: Token(string value,string type,int startPos,int endPos,int Line){
    this->value = value; 
    this->Type = type; 
    this->startPos = startPos; 
    this->endPos = endPos; 
    this->Line = Line; 
}
Tokenizer :: Tokenizer(string text){
    this->text = text;  
    this->currPointer = -1; 
}
bool Tokenizer :: advance(){
    if(this->currPointer+1 >= this->text.size()) return false;
    else{
        this->currPointer = this->currPointer+1; 
        return true;
    }
}

pair<string,bool> Tokenizer ::  MakeNum(int &r,int &c){
   char currentchar = this->text[this->currPointer]; 

   string num = ""; 
   num.push_back(currentchar); 
   c += 1; 
   bool flag = false; 
   while(advance()){
      char currentchar = this->text[this->currPointer]; 
      if((currentchar-'0' >= 0) and (currentchar-'0' <= 9)){
        num.push_back(currentchar); 
        c += 1; 
      }
      else if(currentchar == '.'){
        flag = true; 
        num.push_back('.'); 
        c += 1; 
      }
      else{
         this->currPointer -= 1; 
         break; 
      }
   }
   return make_pair(num,flag); 
}

pair<string,bool> Tokenizer :: MakeWord(int &r,int &c){

   char currentchar = this->text[this->currPointer]; 
  
   vector<string> keywords; 
   keywords.push_back("VAR");
   keywords.push_back("AND"); 
   keywords.push_back("OR"); 
   keywords.push_back("NOT"); 
   keywords.push_back("IF"); 
   keywords.push_back("ELIF"); 
   keywords.push_back("ELSE"); 
   keywords.push_back("FOR");
   keywords.push_back("WHILE"); 
   keywords.push_back("TO"); 
   keywords.push_back("STEP"); 
   keywords.push_back("FUNC"); 

   string word = ""; 
   word.push_back(currentchar); 
   c += 1; 
   bool flag = false; 
   while(advance()){
      char currentchar = this->text[this->currPointer]; 
      if(((currentchar-'a') >= 0 && (currentchar-'a')<= 25) or ((currentchar-'A') >= 0 && (currentchar-'A' <= 25))){
        word.push_back(currentchar); 
        c += 1; 
      }
      else{
         this->currPointer -= 1; 
         break; 
      }
   }
   for(string s : keywords){
    if(word == s){
        flag = true; 
    }
   }
   
   return make_pair(word,flag); 
    
}

pair<string,bool> Tokenizer :: MakeEquals(int &r,int &c){
    char currentchar = this->text[this->currPointer]; 

    string comp = ""; 
    comp.push_back(currentchar); 
    c += 1;
    bool flag = false;

    while(advance()){
        char currentchar = this->text[this->currPointer]; 
        if(currentchar == '='){
            comp.push_back(currentchar); 
            flag = true; 
            c += 1; 
            break; 
        }
        else{
           this->currPointer -= 1; 
           break;
        }
    }

    return make_pair(comp,flag); 

    
}
pair<string,bool> Tokenizer :: MakeReturn(int &r,int &c){
    char currentchar = this->text[this->currPointer]; 

    string comp = ""; 
    comp.push_back(currentchar); 
    c += 1;
    bool flag = false;

    while(advance()){
        char currentchar = this->text[this->currPointer]; 
        if(currentchar == '>'){
            comp.push_back(currentchar); 
            flag = true; 
            c += 1; 
            break; 
        }
        else{
           this->currPointer -= 1; 
           break;
        }
    }

    return make_pair(comp,flag); 

    
}
// Add != operator
vector<Token> Tokenizer::tokenize(){
    vector<Token> tokens; 
    int row = 0; 
    int col = 0; 
     while(this->advance()){
     
        char currentChr = this->text[this->currPointer]; 
        if(currentChr == '+'){
            Token t("+",PLUS,col,col,row); 
            col += 1; 
            tokens.push_back(t); 
        }
        else if(currentChr == '-'){

            int start = col; 
            pair<string,bool>  p = MakeReturn(row,col); 
            col -= 1; 
            if(p.second){
                Token t(p.first,"RETURN",start,col,row); 
                tokens.push_back(t); 
            }
            else{
                Token t(p.first,"MINUS",col,col,row); 
                tokens.push_back(t); 
            }
            col += 1;
        }
        else if(currentChr == '*'){
               Token t("*",MUL,col,col,row); 
               col += 1; 
               tokens.push_back(t); 
        }
        else if(currentChr== '/'){
             Token t("/",DIV,col,col,row); 
             col += 1; 
             tokens.push_back(t); 
        }
        else if(currentChr == ')'){
            Token t(")",RIGHTPARAN,col,col,row); 
            col += 1; 
            tokens.push_back(t); 
        }
        else if(currentChr == '('){
            Token t("(",LEFTPARAN,col,col,row); 
             col += 1; 
            tokens.push_back(t); 
        }
        else if(currentChr == ','){
            Token t(",","COMMA",col,col,row); 
             col += 1; 
            tokens.push_back(t); 
        }
        else if(currentChr== ' '){
            col += 1; 
            continue;
        }
        else if(currentChr == '\n'){
            // increment the line counter; at this use the line counter to update the current Line
            col = 0; 
            row += 1;
            continue; 
        }
        else if(currentChr == '='){
            int start = col; 
            pair<string,bool>  p = MakeEquals(row,col); 
            col -= 1; 
            if(p.second){
                Token t(p.first,"IS_EQUAL",start,col,row); 
                tokens.push_back(t); 
            }
            else{
                Token t(p.first,"EQUALS",col,col,row); 
                tokens.push_back(t); 
            }
            col += 1;
           
        }
        else if(currentChr == '>'){
              int start = col; 
            pair<string,bool>  p = MakeEquals(row,col); 
            col -= 1; 
            if(p.second){
                Token t(p.first,"GREQUALS",start,col,row); 
                tokens.push_back(t); 
            }
            else{
                Token t(p.first,"GR",col,col,row); 
                tokens.push_back(t); 
            }
            col += 1;
                        
        }
        else if(currentChr == '<'){
            int start = col; 
            pair<string,bool>  p = MakeEquals(row,col); 
            col -= 1; 
            if(p.second){
                Token t(p.first,"LSEQUALS",start,col,row); 
                tokens.push_back(t); 
            }
            else{
                Token t(p.first,"LS",col,col,row); 
                tokens.push_back(t); 
            }
            col += 1;
                    
        }
    
        else if((currentChr-'0') >= 0 and (currentChr -'0') <= 9){
            // its a number; 
            int start = col;  
            pair<string,bool> num = MakeNum(row,col); 
            col -= 1; 
            if(num.second){
                // float
                Token t(num.first,FLOAT,start,col,row); 
                tokens.push_back(t); 
            }
            else{
                // int
                Token t(num.first,INT,start,col,row); 
                tokens.push_back(t); 
            }
            col += 1; 
        }
        else if(((currentChr-'a') >= 0 && (currentChr-'a')<= 25) or ((currentChr-'A') >= 0 && (currentChr-'A' <= 25))){
            int start = col; 
            pair<string,bool> p =  MakeWord(row,col); 
            col -= 1; 
            if(p.second){
                // keyword
                Token t(p.first,"KEYWORD",start,col,row); 
                tokens.push_back(t); 
            }
            else{
                Token t(p.first,"ID",start,col,row); 
                tokens.push_back(t); 
            }
            col += 1; 
        }
        else{
             cout << "IillegalCharacter :" << currentchar << endl; 
             break; 
        }
     }
     Token t("EOF","EOF",col,col,row); 
     tokens.push_back(t); 

     return tokens;
}
Tokenizer :: ~Tokenizer(){
   // free the memory allocated
}