#pragma once

#include "../Coin/Coin.h"
#include "../Function/Function.h"


class CoinTable;
class FunctionTable;

void to_json(json& j, const FunctionTable& p);
void to_json(json& j, const CoinTable& p);

class Class
{
	std::string name;

	std::shared_ptr<CoinTable> fields;
	std::shared_ptr<FunctionTable> methods;

public:
	Class(std::string name, std::shared_ptr<CoinTable> fields, std::shared_ptr<FunctionTable> methods) :
		  name(name), fields(fields), methods(methods) {}
	~Class();

	std::string getName();

	std::shared_ptr<CoinTable> getFields();
	std::shared_ptr<FunctionTable> getMethods();

	friend void to_json(json&, const Class&);
};
