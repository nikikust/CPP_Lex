#pragma once

#include "../Coin/Coin.h"
#include "../Function/Function.h"


class CoinTable;
class FunctionTable;

void to_json(json& j, const FunctionTable& p);
void from_json(const json& j, FunctionTable& p);

class Class
{
	friend class Interpreter;
	std::string name;

	std::shared_ptr<CoinTable> fields;
	std::shared_ptr<RPNVect> fieldsInit;

	std::shared_ptr<FunctionTable> methods;

public:
	Class(std::string name, std::shared_ptr<CoinTable> fields, std::shared_ptr<RPNVect> fieldsInit, std::shared_ptr<FunctionTable> methods) :
		  name(name), fields(fields), fieldsInit(fieldsInit), methods(methods) {}
	~Class();
	Class() : name("123") {}

	std::string getName();

	std::shared_ptr<CoinTable> getFields();
	std::shared_ptr<FunctionTable> getMethods();

	friend void to_json(json&, const Class&);
	friend void from_json(const json&, Class&);
};
