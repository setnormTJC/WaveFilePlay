#include "MyException.h"

MyException::MyException(const std::string& message, const std::string& filename, const int lineNumber)
	:message(message), filename(filename), lineNumber(lineNumber)
{

}

const char* MyException::what() const noexcept
{
	message =
		message +
		"\nin file: " + filename +
		"\non line: " + std::to_string(lineNumber);

	return message.c_str(); 

}

