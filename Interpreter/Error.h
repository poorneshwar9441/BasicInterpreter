#pragma once

#include "Parser.h"


// update the Error; 

class IIlegalCharError{
   public:
      IIlegalCharError(string filename,int pos_start,int pos_end,int LineNumber);
}; 


class InvalidSyntaxError{

  public: 
    string filename; 
    int pos_start; 
    int pos_end; 
    int LineNumber;
     InvalidSyntaxError(string filename,int pos_start,int pos_end,int LineNumber){
        this->filename = filename; 
        this->pos_start = pos_start; 
        this->pos_end = pos_end; 
        this->LineNumber = LineNumber;
     }
     
}; 