#pragma once

#include "Coin.h"
#include "../Function/Function.h"
#include "../Class/Class.h"

class CoinTable;
class FunctionTable;

void to_json(json& j, const CoinTable& p);


class Object : public Coin
{
	std::weak_ptr<Class> objClass;

	std::shared_ptr<CoinTable> fields;
	std::weak_ptr<FunctionTable> methods;

public:
	Object(std::string name, std::shared_ptr<Class> objClass, std::shared_ptr<CoinTable> fields, bool Const = false) :
		Coin(CoinType::OBJECT, name, Const), objClass(objClass), fields(fields), methods(objClass->getMethods()){}
	~Object();

	std::string getType();
	std::string str();

	std::shared_ptr<CoinTable> getFields();
	std::shared_ptr<FunctionTable> getMethods();

	json to_json_edt();
};