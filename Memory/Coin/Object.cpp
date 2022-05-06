#include "Object.h"


std::string Object::getType()
{
	return this->objClass->getName();
}
std::string Object::str()
{
	return objClass->getName() + " " + name;
}

std::shared_ptr<CoinTable> Object::getFields()
{
	return this->fields;
}
std::shared_ptr<FunctionTable> Object::getMethods()
{
	return this->methods;
}