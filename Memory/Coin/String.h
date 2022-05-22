#pragma once
#include "Coin.h"


class String : public Coin
{
	friend class Interpreter;
	std::string data = "";

public:
	String(std::string name, bool Const = false) : Coin(CoinType::STRING, name, Const) {}

	std::string getType();
	std::string str();

	void set(std::string new_data);
	std::string get();

	json to_json_edt();

	std::shared_ptr<Coin> clone();
};
