#pragma once

#include "../Lexer/Lexer.h"
#include "Coin/Coin.h"


class VarTable
{
	std::unordered_map<std::string, Coin> table;

public:
	bool putCoin(std::string name, CoinType type);
	Coin& getCoin(std::string name);
};