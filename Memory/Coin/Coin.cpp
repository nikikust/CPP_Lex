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
