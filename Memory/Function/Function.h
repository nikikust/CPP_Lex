#pragma once
#include "../Coin/Coin.h"
#include "../../RPN/RPN_Element.h"


class CoinTable;
class ClassTable;

void to_json(json& j, const CoinTable& p);
void from_json(const json& j, CoinTable& p);


class Function
{
	friend class Interpreter;
	std::string name;

	std::shared_ptr<CoinTable> attributes;
	std::shared_ptr<RPNVect> attributesInit;
	std::vector<std::string> attributesOrder;
	std::vector<std::string> attributesNamesOrder;

	std::shared_ptr<Coin> returnValue;
	std::string retValue = "";

	std::shared_ptr <RPNVect> body;

public:
	Function(std::string name, std::shared_ptr<CoinTable> attributes, std::vector<std::string> attributesOrder, 
			 std::vector<std::string> attributesNamesOrder, std::shared_ptr<RPNVect> attributesInit,
			 std::shared_ptr<Coin> returnValue, std::shared_ptr <RPNVect> body) 
		:
			 name(name), attributes(attributes), attributesOrder(attributesOrder), 
			 attributesNamesOrder(attributesNamesOrder), attributesInit(attributesInit),
			 returnValue(returnValue), body(body) {}
	Function() : name("123") {}
	~Function();

	std::string getName();
	std::shared_ptr<Coin> getReturnValue();
	std::shared_ptr<RPNVect> getProgram();
	std::shared_ptr<CoinTable> getAttributes();
	std::vector<std::string> getAttributesOrder();
	
	friend void to_json(json&, const Function&);
	friend void from_json(const json&, Function&);
};