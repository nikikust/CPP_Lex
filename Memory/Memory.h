#pragma once

#include "Coin/Numeric.h"
#include "Coin/String.h"
#include "Coin/Object.h"

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
	bool coinExists(std::string name);

	std::unordered_map<std::string, std::shared_ptr<Coin>> getTable();
};

class FunctionTable
{
	std::unordered_map<std::string, std::shared_ptr<Function>> table = {};

public:
	FunctionTable() {}

	void putFunction(std::string name, std::shared_ptr<CoinTable> attributes, std::string returnValue, std::shared_ptr <RPNVect> body);
	bool functionExists(std::string name);

	std::unordered_map<std::string, std::shared_ptr<Function>> getTable();
};