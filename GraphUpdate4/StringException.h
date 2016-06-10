#ifndef STRINGEXCEPTION_H
#define STRINTEXCEPTION_H
#include <string>

struct StringException : public std::exception
{
	std::string s;
	StringException(std::string ss):s(ss){}
	const char* what() const throw() {return s.c_str();}
};
#endif
