#pragma once
#include <string>

#include "../AST/Node/Node.h"

class RPN_Element
{
	std::string value;
	size_t line = 0;
	size_t position = 0;
	TokenType type;
	size_t jumper = 0;

public:
	RPN_Element(Node node) : value(node.getValue()), line(node.getToken().line), position(node.getToken().position), type(node.getType()), jumper(node.getJumper()) {}
	RPN_Element(std::string i_value = "' '", TokenType i_type = TokenType::CLEANER) : value(value), type(i_type) {}

	std::string getValue();
	size_t getLine();
	size_t getPosition();
	TokenType getType();
	size_t getJumper();
};

