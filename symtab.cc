/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Account: masc0833
Team:    Alabama       
Assignment: prog3
FileName: symtab.cc
Class: CS530, Spring 2016

*/

#include <string>
#include "symtab.h"
#include "symtab_exception.h"

using namespace std;	

//constructor
symtab::symtab(){}
//deconstructor
symtab::~symtab(){}

// checks symbol table to see if a specific string exists in the table 
bool symtab::contains_symbol(string s){
	map<string,string>::iterator contains_iter;
	contains_iter= symbol_table.find(s);
	if (contains_iter != symbol_table.end()){
		return true;
	}
	else{
		return false;
	}
	
}
// adds to symbol table and assigns an address to each line of code.
void symtab::insert_symbol(string label,string address,unsigned int lineNum){
	map<string,string>::iterator symbol_iter; //if symbol_iter equals the end of map symbol_table label is not a duplicate and therefore can be added
	symbol_iter= symbol_table.find(label);
	if(symbol_iter != symbol_table.end()){ 
		string error_s = "Symbol " + label + " already exists at that line. ";
		error_handling(error_s,lineNum);
	}
	symbol_table[label] = address;
}

// returns value using its associated key pair.
string symtab::get_value(string key){
	map<string,string>::iterator iter;
	iter= symbol_table.find(key);
	if( iter == symbol_table.end()){
		return "";
	}
	return symbol_table[key];
	
}
	
void symtab::error_handling(string s,unsigned int lineNum){
	sstream << lineNum;
	string lineString = sstream.str(); 
	throw symtab_exception(s + lineString + ".");
}

