#pragma once
#include <unordered_map>
#include <memory>

#include "../../Token/Token.h"


enum class CoinType
{
	NUMERIC,
	STRING,
	POINTER,
	OBJECT
};
class Coin
{
protected:
	CoinType type;
	std::string name;
	bool Const = false;

public:
	Coin(CoinType type, std::string name, bool Const = false) :
		type(type), name(name), Const(Const) {}

	CoinType getCoinType();
	std::string getName();
	bool isConst();
	void setName(std::string name);

	virtual std::string getType() = 0;
	virtual std::string str() = 0;

	virtual ~Coin() = default;
};
