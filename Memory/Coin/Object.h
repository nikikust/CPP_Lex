#pragma once

#include "Coin.h"
#include "../Function/Function.h"
#include "../Class/Class.h"

class CoinTable;
class FunctionTable;

void to_json(json& j, const CoinTable& p);


class Object : public Coin
{
	friend class Interpreter;
	std::weak_ptr<Class> objClass;
	std::string className = "";

	std::shared_ptr<CoinTable> fields;
	std::weak_ptr<FunctionTable> methods;

public:
	Object(std::string name, std::shared_ptr<Class> objClass, std::shared_ptr<CoinTable> fields, bool Const = false) :
		Coin(CoinType::OBJECT, name, Const), objClass(objClass), fields(fields), methods(objClass->getMethods()){}
	Object(std::string name, std::string className, bool Const = false) :
		Coin(CoinType::OBJECT, name, Const), className(className) {}
	~Object();

	void upgradeToFull(std::shared_ptr<Class> objClass);

	std::string getType();
	std::string str();

	std::shared_ptr<CoinTable> getFields();
	std::shared_ptr<FunctionTable> getMethods();
	std::string getDelayedClass();

	void set(std::shared_ptr<Object> coin);	// Check Memory.cpp for implementation

	json to_json_edt();

	std::shared_ptr<Coin> clone();
};