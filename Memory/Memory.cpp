#include "Memory.h"

// --- Coins --- //
void CoinTable::putCoin(std::string name, NumType numericType)
{
	table.insert({ name, std::make_shared<Numeric>(name, numericType) });
}
void CoinTable::putCoin(std::string name)
{
	table.insert({ name, std::make_shared<String>(name) });
}
void CoinTable::putCoin(std::string name, std::string storingType)
{
	table.insert({ name, std::make_shared<Pointer>(name, storingType) });
}
void CoinTable::putCoin(std::string name, std::shared_ptr<Class> objClass, std::shared_ptr<CoinTable> fields)
{
	table.insert({ name, std::make_shared<Object>(name, objClass, fields) });
}

bool CoinTable::coinExists(std::string name)
{
	return this->table.contains(name);
}

std::unordered_map<std::string, std::shared_ptr<Coin>> CoinTable::getTable()
{
	return this->table;
}


// --- Functions --- //

void FunctionTable::putFunction(std::string name, std::shared_ptr<CoinTable> attributes, std::string returnValue, std::shared_ptr <RPNVect>  body)
{
	table.insert({ name, std::make_shared<Function>(name, attributes, returnValue, body) });
}
bool FunctionTable::functionExists(std::string name)
{
	return this->table.contains(name);
}

std::unordered_map<std::string, std::shared_ptr<Function>> FunctionTable::getTable()
{
	return this->table;
}
