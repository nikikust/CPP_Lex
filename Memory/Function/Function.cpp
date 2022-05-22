#include "Function.h"
#include <iostream>

Function::~Function()
{
	attributes = nullptr;
	returnValue = nullptr;
	body = nullptr;
}


void to_json(json& j, const Function& p)
{
	j = json
	{
		{"name", p.name},
		{"attributes", p.attributesInit},
		{"attributesOrder", p.attributesOrder},
		{"attributesNamesOrder", p.attributesNamesOrder},
		{"returnValue", p.returnValue->str()},
		{"body", p.body}
	};
}
void from_json(const json& j, Function& p)
{
	j.at("name").get_to(p.name);
	j.at("attributes").get_to(p.attributesInit);
	j.at("attributesOrder").get_to(p.attributesOrder);
	j.at("attributesNamesOrder").get_to(p.attributesNamesOrder);
	j.at("returnValue").get_to(p.retValue);
	j.at("body").get_to(p.body);
}

std::string Function::getName()
{
	return this->name;
}
std::shared_ptr<Coin> Function::getReturnValue()
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
std::vector<std::string> Function::getAttributesOrder()
{
	return this->attributesOrder;
}
