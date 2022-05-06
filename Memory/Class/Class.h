#pragma once
#include "../Coin/Coin.h"
#include "../Function/Function.h"

class CoinTable;
class FunctionTable;


class Class
{
	std::string name;

	std::shared_ptr<CoinTable> fields;
	std::shared_ptr<FunctionTable> methods;

public:
	Class(std::string name, std::shared_ptr<CoinTable> fields, std::shared_ptr<FunctionTable> methods) :
		  name(name), fields(fields), methods(methods) {}

	std::string getName();

	std::shared_ptr<CoinTable> getFields();
	std::shared_ptr<FunctionTable> getMethods();
};