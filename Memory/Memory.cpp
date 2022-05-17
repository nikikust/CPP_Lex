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

std::shared_ptr<Coin> CoinTable::getCoin(std::string name)
{
	return table.at(name);
}
std::unordered_map<std::string, std::shared_ptr<Coin>>& CoinTable::getTable()
{
	return this->table;
}

void to_json_edt2(json& j, const std::shared_ptr<Coin>& p)
{
	switch (p->getCoinType())
	{
	case CoinType::NUMERIC:
		j.push_back(std::dynamic_pointer_cast<Numeric, Coin>(p)->to_json_edt());
		break;
	case CoinType::STRING:
		j.push_back(std::dynamic_pointer_cast<String, Coin>(p)->to_json_edt());
		break;
	case CoinType::POINTER:
		j.push_back(std::dynamic_pointer_cast<Pointer, Coin>(p)->to_json_edt());
		break;
	case CoinType::OBJECT:
		j.push_back(std::dynamic_pointer_cast<Object, Coin>(p)->to_json_edt());
		break;
	}
}

void to_json(json& j, const CoinTable& p)
{
	j = { p.table };
}


// --- Functions --- //

void FunctionTable::putFunction(std::string name, std::shared_ptr<CoinTable> attributes, std::vector<std::string> attributesOrder,
								std::shared_ptr<Coin> returnValue, std::shared_ptr <RPNVect> body)
{
	table.insert({ name, std::make_shared<Function>(name, attributes, attributesOrder, returnValue, body) });
}
bool FunctionTable::functionExists(std::string name)
{
	return this->table.contains(name);
}

std::shared_ptr<Function> FunctionTable::getFunction(std::string name)
{
	return table.at(name);
}
std::unordered_map<std::string, std::shared_ptr<Function>>& FunctionTable::getTable()
{
	return this->table;
}

void to_json(json& j, const FunctionTable& p)
{
	j = { p.table };
}

// --- Classes --- //

void ClassTable::putClass(std::string name, std::shared_ptr<CoinTable> fields, std::shared_ptr<FunctionTable> methods)
{
	table.insert({ name, std::make_shared<Class>(name, fields, methods) });
}
bool ClassTable::classExists(std::string name)
{
	return this->table.contains(name);
}

std::shared_ptr<Class> ClassTable::getClass(std::string name)
{
	return table.at(name);
}
std::unordered_map<std::string, std::shared_ptr<Class>>& ClassTable::getTable()
{
	return this->table;
}

void to_json(json& j, const ClassTable& p)
{
	j = { p.table };
}
