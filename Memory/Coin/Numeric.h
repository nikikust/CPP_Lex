#pragma once
#include "Coin.h"

enum class NumType
{
	INT,
	DOUBLE,
	BOOL
};
class Numeric : public Coin
{
	friend class Interpreter;
	double data = 0;
	NumType numericType;

public:
	Numeric(std::string name, NumType type, bool Const = false) : Coin(CoinType::NUMERIC, name, Const), numericType(type) {}

	std::string getType();
	std::string str();

	void set(double new_data);
	int getInt();
	double get();

	json to_json_edt();

	std::shared_ptr<Coin> clone();
};