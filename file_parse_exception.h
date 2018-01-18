/* 
Members: Christian Barajas, Christian Cosby, Kevin Nguyen, Wanqi Chan
Team:    Alabama       
Account: masc0833
Assignment: prog1
FileName: file_parse_exception.h
Class: CS530, Spring 2016

*/
   

#ifndef FILE_PARSE_EXCEPTION_H
#define FILE_PARSE_EXCEPTION_H
#include <string>

using namespace std;

class file_parse_exception {

public:
    file_parse_exception(string s) {
        message = s;        
        }
        
    file_parse_exception() {
        message = "An error has occurred";
        }
        
    string getMessage() {
        return message;
    }
    
private:
    string message;
};    
#endif
