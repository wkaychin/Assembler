/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Team:    Alabama       
Account: masc0833
Assignment: prog1
FileName: file_parser.h
Class: CS530, Spring 2016

*/ 
 

#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include <cstring>
#include <ctype.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace std;

class file_parser {
    public:
        // ctor, filename is the parameter.  A driver program will read
        // the filename from the command line, and pass the filename to
        // the file_parser constructor.  Filenames must not be hard-coded.
        file_parser(string); 
        
        // dtor
        ~file_parser();
        
        // reads the source file, storing the information is some
        // auxiliary data structure you define in the private section. 
        // Throws a file_parse_exception if an error occurs.
        // if the source code file fails to conform to the above
        // specification, this is an error condition.     
        void read_file();    
        
        // returns the token found at (row, column).  Rows and columns
        // are zero based.  Returns the empty string "" if there is no 
        // token at that location. column refers to the four fields
        // identified above.
        string get_token(unsigned int, unsigned int);
        
        // prints the source code file to stdout.  Should not repeat 
        // the exact formatting of the original, but uses tabs to align
        // similar tokens in a column. The fields should match the 
        // order of token fields given above (label/opcode/operands/comments)
        void print_file();
        
        // returns the number of lines in the source code file
        int size();
        
    private:
        // your variables and private methods go here
	struct line {
           string label;
           string opcode;
           string operand;
           string comment;
	
	   line() {
	      label = opcode = operand = comment = "";
	   }
    	};
	
	vector<string> lines; // stores lines from file being read
	vector<line> tokens; // stores tokens from each line
	ifstream infile;
	string f; // variable to store filename of file being opened
        void open_f(); // function used to open file and store contents into a vector
};

#endif    
