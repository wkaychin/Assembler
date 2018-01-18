/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Account: masc0833
Team:    Alabama       
Assignment: prog2
FileName: opcode_error_exception.h
Class: CS530, Spring 2016

*/


#ifndef OPCODE_ERROR_EXCEPTION_H
#define OPCODE_ERROR_EXCEPTION_H
#include <string>

using namespace std;

class opcode_error_exception {

public:
    opcode_error_exception(string s) {
        message = s;        
        }
        
    opcode_error_exception() {
        message = "An error has occurred";
        }
        
    string getMessage() {
        return message;
    }
    
private:
    string message;
};    
#endif
