#pragma once
#include "../Coin/Coin.h"


class Pointer : public Coin
{
	std::shared_ptr<Coin> data = nullptr;
	std::string storingType;

public:
	Pointer(std::string name, std::string storingType) : Coin(CoinType::POINTER, name), storingType(storingType) {}

	std::string getType();
	std::string str();

	void set(std::shared_ptr<Coin> new_data, std::string storingType);
	std::shared_ptr<Coin> get();
	std::string getStoringType();
};
