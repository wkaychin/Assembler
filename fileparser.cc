/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Team:    Alabama       
Account: masc0833
Assignment: prog1
FileName: file_parser.cc
Class: CS530, Spring 2016

*/ 
 
#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

file_parser::file_parser(string file) {
   f = file;
   }
   
file_parser::~file_parser() {
   }

void file_parser::read_file() {
   open_f();
   
   string str; // str is the current line being read
   ostringstream line_number; // used to convert current line number to a string
   line *temp; // structure used to store tokens
   
   for(unsigned int i = 0; i < lines.size(); i++) { // i is line number, starting from line 0, iterate for every line in file
      
      unsigned int position = 0; // position is used to point to chars in the current line
      temp = new line(); // set all tokens to empty strings
      
      str = lines[i]; // str now contains the a string of the current 
      
      // Check for label
      if(isalnum(str[0])) { // check if first character is an alphanumeric, if it is it's a label
         if(!isalpha(str[0])) { 
	    line_number << i+1;
	    throw file_parse_exception("Error on line " + line_number.str() + ": Label does not begin with a letter.");
	 }
	 do {
	    if(!isalnum(str[position])) { // check for non-alphanumeric characters
	       line_number << i+1;
	       throw file_parse_exception("Error on line " + line_number.str() + ": Label contains a non-alphanumeric character.");
	    }
	    temp->label += str[position++]; // add character to label
	 }
	 while((!isspace(str[position])) && (position < str.length()) &&  (position < 8) && (str[position] != '.')); // only first 8 characters are used for label
	 
	 if(!isspace(str[position])) {
	    while((!isspace(str[position])) && (position < str.length())) {
               position++; // skip extra label characters
            }
	 }
      }
      
      while((isspace(str[position])) && (position < str.length())) {
         position++; // skip leading spaces
      }
      
      if(position == str.length()) { // reached end of line
         tokens.push_back(*temp); 
	 continue; // go to next line;
      }
      
      // Check for opcode
      while((!isspace(str[position])) &&  (str[position] != '.') && position < str.length()) {
      // If quote is read, everything inside the quote is added to token
         if(str[position] == '\'') { 
            do {
               temp->opcode += str[position++];
	       if(position == str.length()) {
		   line_number << i+1;
	           throw file_parse_exception("Error on line " + line_number.str() + ": Token missing a \'.");
        	   break;
	       }
            }
            while(((str[position] != '\'')));
	    temp->operand += str[position]; // add ending quote
	 }
         else{
	    temp->opcode += str[position]; // character is part of opcode
	 }   
	 position++;
      }
      
      while((isspace(str[position])) && (position < str.length())) {
         position++;
      }
      
      if(position >= str.length()) {
         tokens.push_back(*temp);
	 continue;
      }
      
      // Check for operand
       while((!isspace(str[position])) &&  (str[position] != '.') && position < str.length()) {
       // If quote is read, everything inside the quote is added to token
         if(str[position] == '\'') {
            do {
               temp->operand += str[position++];
	       if(position == str.length()) {
		   line_number << i+1;
	           throw file_parse_exception("Error on line " + line_number.str() + ": Token missing a \'.");
        	   break;
	       }
            }
            while(((str[position] != '\'')));
	    temp->operand += str[position]; // add ending quote
	 }
         else{
	    temp->operand += str[position]; // character is part of operand
	 }   
	 position++;
      }
      
      while((isspace(str[position])) && (position < str.length())) {
         position++;
      }
      
      if(position == str.length()) {
         tokens.push_back(*temp);
	 continue;
      }
      
      // Check for comment
      if(str[position] == '.') {
         for (unsigned int j = position; j < str.length(); j++) { // everything after a '.' is a comment
	    temp->comment += str[position++];
	 }
      }
      
      if(position == str.length()) {
         tokens.push_back(*temp);
	 continue;
      }
      
      else {
         line_number << i+1;
	 throw file_parse_exception("Error on line " + line_number.str() + ": Too many tokens.");
      }
      
   }
}

string file_parser::get_token(unsigned int x, unsigned int y){
    ostringstream tokenstring;
    tokenstring << x << ',' << y;

    if(x < tokens.size()){
    	line item = tokens.at(x);
    	
    	if(y == 0)
    	    return item.label;
    	if(y == 1)
    	    return item.opcode;
    	if(y == 2)
    	    return item.operand;
    	if(y == 3)
    	    return item.comment;
    }
    throw file_parse_exception("Error Token (" + tokenstring.str() + ") does not exist");
    return 0;
}

void file_parser::print_file(){
   vector<line>::iterator vector_iter;
        
   for(vector_iter = tokens.begin(); vector_iter != tokens.end(); advance(vector_iter,1)){
       cout << vector_iter->label << "\t" << vector_iter->opcode << "\t";
       cout << vector_iter->operand << "\t" << vector_iter->comment << endl;
   }
}

int file_parser::size(){ 
   return (int) tokens.size(); 
}

void file_parser::open_f(){
   infile.open(f.c_str());
   if(!infile) {
      throw file_parse_exception("Error: File not found.");
   }
      
   string current_line;
   while(!infile.eof()) {
      getline(infile, current_line);
      lines.push_back(current_line);
   }
   
   infile.close();
}
