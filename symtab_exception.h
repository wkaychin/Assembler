/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Account: masc0833
Team:    Alabama       
Assignment: prog3
FileName: symtab_exception.h
Class: CS530, Spring 2016
*/
#ifndef SYMTAB_EXCEPTION_H
#define SYMTAB_EXCEPTION_H

using namespace std;

class symtab_exception {

	public:
	    symtab_exception(string s) {
        	message = s;        
        	}

	    symtab_exception() {
        	message = "An error has occurred";
        	}

	    string getMessage() {
        	return message;
	    }

	private:
	    string message;
};

#endif
