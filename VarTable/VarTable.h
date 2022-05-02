#pragma once
#include <map>

#include "../Token/Token.h"
#include "Coin/Coin.h"

typedef std::unordered_map<std::string, std::shared_ptr<Coin>> Table;


class VarTable
{
	Table variables = {};

public:
	Table getTable();

	void putVar(std::string name, CoinVar::VarType type, size_t currentNamespace);
	std::shared_ptr<Coin> getVar(std::string name);

	bool varExists(std::string name);
	std::string getVarType(std::string name);
};