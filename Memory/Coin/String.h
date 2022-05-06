#pragma once
#include "Coin.h"

class String : public Coin
{
	std::string data = "";

public:
	String(std::string name) : Coin(CoinType::STRING, name) {}

	std::string getType();
	std::string str();

	void set(std::string new_data);
	std::string get();
};