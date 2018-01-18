/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Account: masc0833
Team:    Alabama       
Assignment: prog3
FileName: symtab.h
Class: CS530, Spring 2016

*/


#ifndef SYMTAB_H
#define SYMTAB_H
#include <string>
#include <map>
#include <sstream>

using namespace std;

class symtab {
    public:
        // constructor
	symtab();
	
	// destructor
	~symtab();
	
	bool contains_symbol(string);
	void insert_symbol(string,string,unsigned int);
	string get_value(string);
        
    private:
        // your variables and private methods go here
	
	map<string,string> symbol_table;
	stringstream sstream;
	void error_handling(string,unsigned int);
};

#endif 
