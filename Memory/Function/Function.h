#pragma once
#include "../Coin/Coin.h"
#include "../../RPN/RPN_Element.h"

class CoinTable;


class Function
{
	std::string name;

	std::shared_ptr<CoinTable> attributes;
	std::string returnValue;

	std::shared_ptr <RPNVect> body;

public:
	Function(std::string name, std::shared_ptr<CoinTable> attributes, std::string returnValue, std::shared_ptr <RPNVect> body) :
			name(name), attributes(attributes), returnValue(returnValue), body(body) {}

	std::string getReturnValueType();
	std::shared_ptr <RPNVect> getProgram();
	std::shared_ptr<CoinTable> getAttributes();

	// Coin call(CoinTable attributes);
};