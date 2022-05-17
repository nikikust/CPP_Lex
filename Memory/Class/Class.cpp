#include "Class.h"



Class::~Class()
{
	fields = nullptr;
	methods = nullptr;
}

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

void to_json(json& j, const Class& p)
{
	json buf1; to_json(buf1, *p.fields);
	json buf2; to_json(buf2, *p.methods);
	j = json
	{
		{"name", p.name},
		{"fields", buf1},
		{"methods", buf2}
	};
}
