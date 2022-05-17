#pragma once
#include "../Coin/Coin.h"
#include "../../RPN/RPN_Element.h"


class CoinTable;
void to_json(json& j, const CoinTable& p);
//void to_json(json& j, const std::shared_ptr<Coin>& ptr);


class Function
{
	std::string name;

	std::shared_ptr<CoinTable> attributes;
	std::vector<std::string> attributesOrder;

	std::shared_ptr<Coin> returnValue;

	std::shared_ptr <RPNVect> body;

public:
	Function(std::string name, std::shared_ptr<CoinTable> attributes, std::vector<std::string> attributesOrder,
		     std::shared_ptr<Coin> returnValue, std::shared_ptr <RPNVect> body) :
			name(name), attributes(attributes), attributesOrder(attributesOrder), returnValue(returnValue), body(body) {}
	~Function();

	std::string getName();
	std::shared_ptr<Coin> getReturnValue();
	std::shared_ptr<RPNVect> getProgram();
	std::shared_ptr<CoinTable> getAttributes();
	std::vector<std::string> getAttributesOrder();

	// Coin call(CoinTable attributes);
	
	friend void to_json(json&, const Function&);
};