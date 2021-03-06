#include "Object.h"


Object::~Object()
{
	fields = nullptr;
}

void Object::upgradeToFull(std::shared_ptr<Class> objClass)
{
	this->objClass = objClass;
	this->methods = objClass->getMethods();
}

std::string Object::getType()
{
	return this->objClass.lock()->getName();
}
std::string Object::str()
{
	return ((Const) ? "const " : "") + objClass.lock()->getName() + " " + name;
}

std::shared_ptr<CoinTable> Object::getFields()
{
	return this->fields;
}
std::shared_ptr<FunctionTable> Object::getMethods()
{
	return this->methods.lock();
}
std::string Object::getDelayedClass()
{
	return className;
}

json Object::to_json_edt()
{
	json buf1; to_json(buf1, *fields);
	json j =
	{
		{"CoinType", type},
		{"name", name},
		{"ID", ID},
		{"Const", Const},
		{"data",
			{
				{"className", objClass.lock()->getName()},
				{"fields", buf1}
			}
		}
	};

	return j;
}
