#include <iostream>

#include "VarTable.h"


Table VarTable::getTable()
{
	return this->variables;
}

void VarTable::putVar(std::string name, CoinVar::VarType type, size_t currentNamespace)
{ 
	variables.insert({ name, std::make_shared<CoinVar>(name, type, currentNamespace) });
}
std::shared_ptr<Coin> VarTable::getVar(std::string name)
{
	if (variables.contains(name))
		return variables[name];
}
bool VarTable::varExists(std::string name)
{
	return variables.contains(name);
}
std::string VarTable::getVarType(std::string name)
{
	return getVar(name)->getType();
}
