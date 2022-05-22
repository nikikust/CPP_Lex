#pragma once
#include "Coin.h"


class Void : public Coin
{
	friend class Interpreter;

public:
	Void(std::string name, bool Const = true) : Coin(CoinType::VOID, name, Const) {}

	std::string getType();
	std::string str();

	std::shared_ptr<Coin> clone();
};
