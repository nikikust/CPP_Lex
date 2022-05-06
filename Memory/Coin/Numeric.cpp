#include "Numeric.h"


void Numeric::set(double new_data)
{
	this->data = new_data;
}

std::string Numeric::getType()
{
	switch (numericType)
	{
	case NumType::INT:		return "int";
	case NumType::DOUBLE:	return "double";
	case NumType::BOOL:		return "bool";
	default:				return "error-type";
	};
}
std::string Numeric::str()
{
	switch (numericType)
	{
	case NumType::INT:		return "int " + name + " = " + std::to_string((int)data);
	case NumType::DOUBLE:	return "double " + name + " = " + std::to_string(data);
	case NumType::BOOL:		return "bool " + name + " = " + std::to_string((bool)data);
	default:				return "error-type " + name + " = " + std::to_string(data);
	}
}
int Numeric::getInt()
{
	return (int)data;
}
double Numeric::get()
{
	return data;
}
