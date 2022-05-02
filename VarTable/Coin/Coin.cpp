#include "Coin.h"


std::string Coin::getType()
{
	switch (this->type)
	{
	case CoinType::VAR:
		return getName(this->type);
	default:
		return "abrakadabra";
	}
}
std::string Coin::getCoinName()
{
	return this->name;
}

bool CoinVar::get(int& a)
{
	switch (this->varType)
	{
	case VarType::INT:
		a = INT;
		return true;
	case VarType::DOUBLE:
		a = DOUBLE;
		return true;
	case VarType::BOOL:
		a = BOOL;
		return true;
	case VarType::STRING:
		a = std::stoi(STRING);
		return true;
	}
	return false;
}
bool CoinVar::get(double& a)
{
	switch (this->varType)
	{
	case VarType::INT:
		a = INT;
		return true;
	case VarType::DOUBLE:
		a = DOUBLE;
		return true;
	case VarType::BOOL:
		a = BOOL;
		return true;
	case VarType::STRING:
		a = std::stod(STRING);
		return true;
	}
	return false;
}
bool CoinVar::get(bool& a)
{
	switch (this->varType)
	{
	case VarType::INT:
		a = INT;
		return true;
	case VarType::DOUBLE:
		a = DOUBLE;
		return true;
	case VarType::BOOL:
		a = BOOL;
		return true;
	case VarType::STRING:
		a = (bool)std::stoi(STRING);
		return true;
	}
	return false;
}
bool CoinVar::get(std::string& a)
{
	switch (this->varType)
	{
	case VarType::INT:
		a = std::to_string(INT);
		return true;
	case VarType::DOUBLE:
		a = std::to_string(DOUBLE);
		return true;
	case VarType::BOOL:
		a = std::to_string(BOOL);
		return true;
	case VarType::STRING:
		a = STRING;
		return true;
	}
	return false;
}
bool CoinVar::get(std::shared_ptr<Coin>& a)
{
	if (this->varType == VarType::POINTER && POINTER != nullptr)
	{
		a = POINTER;
		return true;
	}
	return false;
}
CoinVar::VarType CoinVar::getType()
{
	return this->varType;
}
std::string CoinVar::str()
{
	switch (this->varType)
	{
	case CoinVar::VarType::BOOL: {
		bool a; get(a);
		return "VAR - 'bool' : " + getCoinName() + " = " + std::to_string(a);
	}
	case CoinVar::VarType::INT: {
		int a; get(a);
		return "VAR - 'int' : " + getCoinName() + " = " + std::to_string(a);
	}
	case CoinVar::VarType::DOUBLE: {
		double a; get(a);
		return "VAR - 'double' : " + getCoinName() + " = " + std::to_string(a);
	}
	case CoinVar::VarType::STRING: {
		std::string a; get(a);
		return "VAR - 'string' : " + getCoinName() + " = " + a;
	}
	case CoinVar::VarType::POINTER: {
		return "VAR - 'pointer' : " + getCoinName();
	}
	}
	return "NULL";
}


bool CoinFunction::call(std::vector<Coin> attributes)
{
	return false;
}

std::string CoinObject::getType()
{
	return this->getType();
}


std::string getName(Coin::CoinType coin)
{
	switch (coin)
	{
	case Coin::CoinType::VAR:		return "variable";
	case Coin::CoinType::FUNCTION:	return "function";
	case Coin::CoinType::CLASS:		return "class";
	case Coin::CoinType::OBJECT:	return "object";
	case Coin::CoinType::VOID:		return "void";
	default:						return "UNKNOWN";
	}
}
