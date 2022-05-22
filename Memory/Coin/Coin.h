#pragma once
#include <unordered_map>
#include <memory>

#include "../../Token/Token.h"


enum class CoinType
{
	NUMERIC,
	STRING,
	POINTER,
	OBJECT,
	VOID,
	ERR_TYPE
};

class Coin
{
	friend class Interpreter;
protected:
	CoinType type;
	std::string name;
	uint32_t ID = 0;
	bool Const = false;

	inline static uint32_t coin_ID = 0;

public:
	Coin(CoinType type, std::string name, bool Const = false) :
		type(type), name(name), Const(Const), ID(coin_ID++) {}
	Coin() :
		type(CoinType::ERR_TYPE), name(""), Const(true) {}
	virtual ~Coin() = default;

	CoinType getCoinType();
	std::string getName();
	bool isConst();
	void setName(std::string name);
	void setConst(bool Const);
	void setID(uint32_t id);

	virtual std::string getType() = 0;
	virtual std::string str() = 0;

	virtual std::shared_ptr<Coin> clone() = 0;
};

std::string getName(CoinType type);