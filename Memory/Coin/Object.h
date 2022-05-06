#pragma once

#include "Coin.h"
#include "../Function/Function.h"
#include "../Class/Class.h"

class CoinTable;
class FunctionTable;


class Object : public Coin
{
	std::shared_ptr<Class> objClass;

	std::shared_ptr<CoinTable> fields;
	std::shared_ptr<FunctionTable> methods;

public:
	Object(std::string name, std::shared_ptr<Class> objClass, std::shared_ptr<CoinTable> fields) :
		Coin(CoinType::OBJECT, name), objClass(objClass), fields(fields), methods(objClass->getMethods()){}

	std::string getType();
	std::string str();

	std::shared_ptr<CoinTable> getFields();
	std::shared_ptr<FunctionTable> getMethods();
};