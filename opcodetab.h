/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Account: masc0833
Team:    Alabama       
Assignment: prog2
FileName: opcodetab.h
Class: CS530, Spring 2016

*/


#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

        // ctor
        // creates a new dictionary structure and loads all of the opcodes for 
        // the SIC/XE architecture into the table.  Use the STL
        // map for this.
        opcodetab::opcodetab(){
		//  loads map with opcodes in the form of (Value in Hex, Size)
		for(int i = 0; i < AMT_OF_OPCODES; i++){
			codes[opcodes[i]] = pair<string,int>(hex_vals[i],sizes[i]);
		}
	
	}
        
        // takes a SIC/XE opcode and returns the machine code 
        // equivalent as a two byte string in hexadecimal.
        // Example:  get_machine_code("ADD") returns the value 18
        // Note that opcodes may be prepended with a '+'.
        // throws an opcode_error_exception if the opcode is not 
        // found in the table.
        string opcodetab::get_machine_code(string Str){
		// checks if opcode begins with '+' sign, if so it gets erased
		if(Str[0] == '+'){
			Str = Str.substr(1);
		}
		
		// changes every element into an upper case letter
		for(unsigned int i = 0; i < Str.size(); i++){
			Str[i] = toupper(Str[i]);
		}
		
		// finds the code in the map
		codes_iter = codes.find(Str);
		
		// if the end of the map is hit while looking for the code, then error is thrown
		if(codes_iter == codes.end()){
			throw opcode_error_exception("Opcode " + Str + " not found.");
		}
		
		else{
			hex_value = codes_iter -> second.first;
		}
		return hex_value;
	
	} 
        
        // takes a SIC/XE opcode and returns the number of bytes 
        // needed to encode the instruction, which is an int in
        // the range 1..4.
        // NOTE: the opcode must be prepended with a '+' for format 4.
        // throws an opcode_error_exception if the opcode is not 
        // found in the table.        
        int opcodetab::get_instruction_size(string Str){
		plus_sign_flag = 0;
		
		// checks if opcode begins with '+' sign, if so it gets erased
		// '+' sign flag becomes set to 1
		if(Str[0] == '+'){
			Str = Str.substr(1);
			plus_sign_flag = 1;
		}
		
		// changes every element into an upper case letter
		for(unsigned int i = 0; i < Str.size(); i++){
			Str[i] = toupper(Str[i]);
		}
		
		// looks for opcode in map
		codes_iter = codes.find(Str);
		
		// error if opcode not found
		if(codes_iter == codes.end()){
			throw opcode_error_exception("Opcode " + Str + " not found.");
		}
		
		// looks for size of command
		opcode_size = codes_iter -> second.second;
		
		// if '+' sign flag is set, and opcode size is 3, then it can be incremented to 4
		if(plus_sign_flag == 1 && opcode_size == 3){
			opcode_size++;
		}
		
		// otherwise if opcode size is not 3, then it can not be incremented, error thrown
		// only opcodes size of 3 can be incremented to 4
		else if(plus_sign_flag == 1 && opcode_size !=3){
			throw opcode_error_exception("Opcode " + Str + " is not valid opcode format");
		}
		
		return opcode_size;
		
	}
                        
    


