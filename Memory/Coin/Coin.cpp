#include "Coin.h"


CoinType Coin::getCoinType()
{
	return this->type;
}
std::string Coin::getName()
{
	return this->name;
}
bool Coin::isConst()
{
	return this->Const;
}
void Coin::setName(std::string name)
{
	this->name = name;
}
void Coin::setConst(bool Const)
{
	this->Const = Const;
}
void Coin::setID(uint32_t id)
{
	this->ID = id;
}

std::string getName(CoinType type)
{
	switch (type)
	{
	case CoinType::NUMERIC:		return "numeric";
	case CoinType::STRING:		return "string";
	case CoinType::POINTER:		return "pointer";
	case CoinType::OBJECT:		return "object";
	case CoinType::VOID:		return "void";
	case CoinType::ERR_TYPE:	return "err-type";
	}
}