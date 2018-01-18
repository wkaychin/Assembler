/* 
Members: Christian Barajas, Cody Cosby, Kevin Nguyen, Wanqi Chin
Account: masc0833
Team:    Alabama       
Assignment: prog2
FileName: sicxe_asm.h
Class: CS530, Spring 2016

*/

#ifndef SICXE_ASM_H
#define SICXE_ASM_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <iomanip>
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "file_parser.h"
#include "file_parse_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

const string directives[] = {"START","END","BYTE","WORD","RESB","RESW","BASE",
					"NOBASE","EQU"};
					
const string registers[] = {"A","X","L","B","S","T","PC","SW"};

const string reg_values[] = {"0","1","2","3","4","5","8","9"};

class sicxe_asm{

	public: 
	
		//ctor
		sicxe_asm();
		
		//dtor
		~sicxe_asm();
		
		// creates .lis file for file
		void assemble(string); 
	
	private:
		
		struct to_print{
			int line;
			string address;
			string label;
			string opcode;
			string operand;
			string machine;
			flag flags;
		};
		
		struct flag
		{
			bool n;
			bool i;
			bool x;
			bool b;
			bool p;
			bool e;
		}
		
		file_parser *fparse;
		opcodetab *optable;
		symtab *symbtable;
		vector<to_print> list_vector;
		int pc;//program counter
		int locctr; // location counter
		int base;
		string program; // program name
		int current_line;
		
		
		// parses file for tokens
		void toParse(string);
		
		// generates line number, address, label, opcode, and operand in .lis file 
		bool first_pass();
		
		bool second_pass();
	
		// returns true if there are more lines to parse
		bool has_next_line();
		
		
		// changes locctr based on what instruction was processed
		int next_location(to_print &);
		
	
		// creates .lis file
		void create_listing_file(string);
		
		
		// converts filename.asm to filename.lis
		string change_filename(string);
	
		
		// determines type of operand
		int type_of_operand(string);
		
		
		
		//`````` CONVERSIONS`````````````````
		
		
		// converts string to upper case
		string to_upper(string);
		
		// converts string to int
		int string_to_int(string);
		
		// converts string hexadecimal to a decimal integer
		int hex_to_int(string);
		
		// converts an int value to a string
		int int_to_string(int);
		
		// converts decimal string to an integer
		int dec_to_int(string);
		
		// converts decimal integer to string hexadecimal
		string int_to_hex(int,int);
		
		// converts hexadecimal value into a binary value
		string hex_to_binary(string);
		
		string binary_to_hex(string);
		
		string bool_to_string(bool);
		
		
		//```````BIT FLAGGING```````````````````````
	
};

#endif


/* Stuff to add to header file
const string directives[] = {"START","END","BYTE","WORD","RESB","RESW","BASE",
                    "NOBASE","EQU"};
                    
const string registers[] = {"A","X","L","B","S","T","PC","SW"};

const string reg_values[] = {"0","1","2","3","4","5","8","9"};
*/
string test_opcode;
string test_operand;
