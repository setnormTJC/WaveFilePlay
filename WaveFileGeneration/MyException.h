#pragma once

#include<exception>
#include <string>

class MyException : public std::exception
{
private: 
	mutable std::string message;  //MUTABLE! -> because std::exception marks `what` as const 
	std::string filename; 
	int lineNumber; 

public: 

	MyException() = delete; 
	MyException(const std::string& message, const std::string& filename, const int lineNumber); 
	const char* what() const noexcept override; 
};

