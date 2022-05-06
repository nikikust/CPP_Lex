#include "String.h"


std::string String::getType()
{
	return "string";
}
std::string String::str()
{
	return "string " + name + " = \"" + data + "\"";
}

void String::set(std::string new_data)
{
	this->data = new_data;
}
std::string String::get()
{
	return data;
}
