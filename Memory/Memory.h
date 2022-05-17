#pragma once

#include "Coin/Numeric.h"
#include "Coin/String.h"
#include "Coin/Object.h"

#include "Pointer/Pointer.h"
#include "Function/Function.h"
#include "Class/Class.h"

void to_json_edt(json& j, const std::shared_ptr<Coin>& p);


class CoinTable
{
	std::unordered_map<std::string, std::shared_ptr<Coin>> table = {};

public:
	CoinTable() {}

	void putCoin(std::string name, NumType numericType);												// Num
	void putCoin(std::string name);																		// Str
	void putCoin(std::string name, std::string storingType);											// Templated
	void putCoin(std::string name, std::shared_ptr<Class> objClass, std::shared_ptr<CoinTable> fields);	// Obj
	bool coinExists(std::string name);

	std::shared_ptr<Coin> getCoin(std::string name);
	std::unordered_map<std::string, std::shared_ptr<Coin>>& getTable();

	friend void to_json(json&, const CoinTable&);
};

class FunctionTable
{
	std::unordered_map<std::string, std::shared_ptr<Function>> table = {};

public:
	FunctionTable() {}

	void putFunction(std::string name, std::shared_ptr<CoinTable> attributes, std::vector<std::string> attributesOrder, std::shared_ptr<Coin> returnValue, std::shared_ptr <RPNVect> body);
	bool functionExists(std::string name);

	std::shared_ptr<Function> getFunction(std::string name);
	std::unordered_map<std::string, std::shared_ptr<Function>>& getTable();

	friend void to_json(json&, const FunctionTable&);
};

class ClassTable
{
	std::unordered_map<std::string, std::shared_ptr<Class>> table = {};

public:
	ClassTable() {}

	void putClass(std::string name, std::shared_ptr<CoinTable> fields, std::shared_ptr<FunctionTable> methods);
	bool classExists(std::string name);

	std::shared_ptr<Class> getClass(std::string name);
	std::unordered_map<std::string, std::shared_ptr<Class>>& getTable();

	friend void to_json(json&, const ClassTable&);
};