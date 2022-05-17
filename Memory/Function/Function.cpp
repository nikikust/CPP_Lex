#include "Function.h"


Function::~Function()
{
	attributes = nullptr;
	returnValue = nullptr;
	body = nullptr;
}


void to_json(json& j, const Function& p)
{
	json buf1; to_json(buf1, *p.attributes);
	json buf2; to_json_edt2(buf2, p.returnValue);

	j = json
	{
		{"name", p.name},
		{"attributes", buf1},
		{"attributesOrder", p.attributesOrder},
		{"returnValue", buf2},
		{"body", p.body}
	};
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