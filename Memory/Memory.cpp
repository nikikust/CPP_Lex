#include <iostream>

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
void CoinTable::putCoin(std::string name, bool isVoid, bool isVoid2)
{
	table.insert({ name, std::make_shared<Void>(name) });
}
void CoinTable::putDelayedObjectCoin(std::string name, std::string className)
{
	table.insert({ name, std::make_shared<Object>(name, className) });
}
void CoinTable::putCoin(std::shared_ptr<Coin> coin)
{
	table.insert({ coin->getName(), coin });
}

bool CoinTable::coinExists(std::string name)
{
	return this->table.contains(name);
}

void CoinTable::eraseCoin(std::string name)
{
	table.erase(name);
}
std::shared_ptr<CoinTable> CoinTable::clone()
{
	std::shared_ptr<CoinTable> newFields = std::make_shared<CoinTable>();
	for (auto& field : table)
	{
		newFields->putCoin(field.second->clone());
	}
	return newFields;
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
		j = (std::dynamic_pointer_cast<Numeric, Coin>(p)->to_json_edt());
		break;
	case CoinType::STRING:
		j = (std::dynamic_pointer_cast<String, Coin>(p)->to_json_edt());
		break;
	case CoinType::POINTER:
		j = (std::dynamic_pointer_cast<Pointer, Coin>(p)->to_json_edt());
		break;
	case CoinType::OBJECT:
		j = (std::dynamic_pointer_cast<Object, Coin>(p)->to_json_edt());
		break;
	}
}
void from_json_edt2(const json& j, std::shared_ptr<Coin>& p)
{
	CoinType type;
	std::string name;
	uint32_t ID;
	bool Const;

	j.at("CoinType").get_to(type);
	j.at("name").get_to(name);
	j.at("ID").get_to(ID);
	j.at("Const").get_to(Const);

	switch (type)
	{
	case CoinType::NUMERIC:
	{
		double data = 0;
		NumType numericType;

		j.at("data").at("data").get_to(data);
		j.at("data").at("NumType").get_to(numericType);

		auto coin = Numeric(name, numericType, Const);
		coin.setID(ID); coin.set(data);

		p = std::make_shared<Numeric>(coin);
	}
		break;
	case CoinType::STRING:
	{
		std::string data = "";

		j.at("data").at("data").get_to(data);

		auto coin = String(name, Const);
		coin.setID(ID); coin.set(data);

		p = std::make_shared<String>(coin);
	}
		break;
	case CoinType::POINTER:
	{
		std::string storingType = "";
		std::shared_ptr<Coin> data;

		j.at("data").at("storingType").get_to(storingType);
		//from_json_edt2(j.at("data").at("data"), data);
		
		auto coin = Pointer(name, storingType);
		coin.setID(ID); //coin.set(data);

		p = std::make_shared<Pointer>(name, storingType, Const);
	}
		break;
	case CoinType::OBJECT:
	{
		std::string className = "";
		j.at(0).at("data").at("className").get_to(className);
		p = std::make_shared<Object>(name, className, Const);
	}
		break;
	}
	/*
	case CoinType::POINTER:
		{
			std::shared_ptr<Coin> data = nullptr;
			std::string storingType;

			from_json_edt2(j.at("data").at("data"), data, classes);
			j.at(0).at("data").at("storingType").get_to(storingType);
			
			auto coin = Pointer(name, storingType, Const);
			coin.set(data);

			p = std::make_shared<Pointer>(coin);
		}
		break;
	case CoinType::OBJECT:
		{
			std::string className;
			std::shared_ptr<CoinTable> fields;

			j.at(0).at("data").at("className").get_to(className);
			j.at(0).at("data").at("fields").get_to(fields);

			p = std::make_shared<Object>(name, classes.getClass(className), fields, Const);
		}
		break;
	}*/
}


void to_json(json& j, const CoinTable& p)
{
	j = p.table;
}
void from_json(const json& j, CoinTable& p)
{
	j.get_to(p.table);
}


// --- Functions --- //

void FunctionTable::putFunction(std::string name, std::shared_ptr<CoinTable> attributes, 
								std::vector<std::string> attributesOrder, std::vector<std::string> attributesNamesOrder,
								std::shared_ptr<RPNVect> attributesInit, std::shared_ptr<Coin> returnValue, std::shared_ptr <RPNVect> body)
{
	table.insert({ name, std::make_shared<Function>(name, attributes, attributesOrder, attributesNamesOrder, attributesInit, returnValue, body) });
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
	j = p.table;
}
void from_json(const json& j, FunctionTable& p)
{
	j.get_to(p.table);
}

// --- Classes --- //

void ClassTable::putClass(std::string name, std::shared_ptr<CoinTable> fields, 
						  std::shared_ptr<RPNVect> fieldsInit, std::shared_ptr<FunctionTable> methods)
{
	table.insert({ name, std::make_shared<Class>(name, fields, fieldsInit, methods) });
}
void ClassTable::putClass(std::string name, std::shared_ptr<Class> i_class)
{
	table.insert({ name, i_class });
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
	j = p.table;
}
void from_json(const json& j, ClassTable& p)
{
	j.get_to(p.table);
}

// --- Unpacks from JSON --- //

void from_json(const json& j, Class& p)
{
	j.at("name").get_to(p.name);
	j.at("fields").get_to(p.fieldsInit);
	j.at("methods").get_to(p.methods);
}

// --- Additional --- //

void Object::set(std::shared_ptr<Object> coin)
{
	CoinTable buf1 = *coin->fields;
	this->fields = std::make_shared<CoinTable>(buf1);
}

std::shared_ptr<Coin> Object::clone()
{
	Object res(name, objClass.lock(), fields->clone(), Const);

	return std::make_shared<Object>(res);
}