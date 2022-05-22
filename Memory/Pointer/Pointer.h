#pragma once
#include "../Coin/Coin.h"


void to_json(json& j, const std::shared_ptr<Coin>& p);

class Pointer : public Coin
{
	friend class Interpreter;
	std::shared_ptr<Coin> data = nullptr;
	std::string storingType;

public:
	Pointer(std::string name, std::string storingType, bool Const = false) : Coin(CoinType::POINTER, name, Const), storingType(storingType) {}
	~Pointer();

	std::string getType();
	std::string getStoringType();
	std::string str();

	void set(std::shared_ptr<Coin> new_data, std::string storingType = "");
	std::shared_ptr<Coin> get();

	json to_json_edt();

	std::shared_ptr<Coin> clone();
};
