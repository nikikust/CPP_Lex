#pragma once

#include "Coin/Numeric.h"
#include "Coin/String.h"
#include "Coin/Object.h"
#include "Coin/Void.h"

#include "Pointer/Pointer.h"
#include "Function/Function.h"
#include "Class/Class.h"


class CoinTable
{
	std::unordered_map<std::string, std::shared_ptr<Coin>> table = {};

public:
	CoinTable() {}

	void putCoin(std::string name, NumType numericType);												// Num
	void putCoin(std::string name);																		// Str
	void putCoin(std::string name, std::string storingType);											// Templated
	void putCoin(std::string name, std::shared_ptr<Class> objClass, std::shared_ptr<CoinTable> fields);	// Obj
	void putCoin(std::shared_ptr<Coin> coin);															// Fast
	void putCoin(std::string name, bool isVoid, bool isVoid2);											// Void
	void putDelayedObjectCoin(std::string name, std::string className);									// class A { ptr<A> ... }
	bool coinExists(std::string name);

	void eraseCoin(std::string name);

	std::shared_ptr<Coin> getCoin(std::string name);
	std::unordered_map<std::string, std::shared_ptr<Coin>>& getTable();

	std::shared_ptr<CoinTable> clone();

	friend void to_json(json&, const CoinTable&);
	friend void from_json(const json&, CoinTable&);
};

class FunctionTable
{
	std::unordered_map<std::string, std::shared_ptr<Function>> table = {};

public:
	FunctionTable() {}

	void putFunction(std::string name, std::shared_ptr<CoinTable> attributes, 
					 std::vector<std::string> attributesOrder, std::vector<std::string> attributesNamesOrder,
					 std::shared_ptr<RPNVect> attributesInit, std::shared_ptr<Coin> returnValue, std::shared_ptr <RPNVect> body);
	bool functionExists(std::string name);

	std::shared_ptr<Function> getFunction(std::string name);
	std::unordered_map<std::string, std::shared_ptr<Function>>& getTable();

	friend void to_json(json&, const FunctionTable&);
	friend void from_json(const json&, FunctionTable&);
};

class ClassTable
{
	std::unordered_map<std::string, std::shared_ptr<Class>> table = {};

public:
	ClassTable() {}

	void putClass(std::string name, std::shared_ptr<CoinTable> fields, std::shared_ptr<RPNVect> fieldsInit, 
				  std::shared_ptr<FunctionTable> methods);
	bool classExists(std::string name);

	std::shared_ptr<Class> getClass(std::string name);
	std::unordered_map<std::string, std::shared_ptr<Class>>& getTable();

	friend void to_json(json&, const ClassTable&);
	friend void from_json(const json&, ClassTable&);
};

void from_json(const json& j, Class& p);