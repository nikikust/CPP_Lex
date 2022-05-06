#include "Function.h"


std::string Function::getReturnValueType()
{
	return this->returnValue;
}
std::shared_ptr <RPNVect> Function::getProgram()
{
	return this->body;
}
std::shared_ptr<CoinTable> Function::getAttributes()
{
	return this->attributes;
}