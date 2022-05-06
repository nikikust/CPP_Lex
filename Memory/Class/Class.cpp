#include "Class.h"


std::string Class::getName()
{
	return this->name;
}
std::shared_ptr<CoinTable> Class::getFields()
{
	return this->fields;
}
std::shared_ptr<FunctionTable> Class::getMethods()
{
	return this->methods;
}
