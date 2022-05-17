#pragma once
#include <string>
#include <vector>
#include <memory>

#include "../Memory/InitJSON.h"

class RPN_Element;
typedef std::vector<std::shared_ptr<RPN_Element>> RPNVect;


enum class TokenType	// For PARSER and next
{
	PROBE,		//EMPTY node
	IDENTIFIER,
	CONSTANT,
	OPERATION,
	TYPE,		// void, int, MyType
	JMP,		// JUMP
	CJM,		// conditional JUMP
	SPECIAL,	// Root
	CLEANER,	// remove 2+2; etc from interpreter stack
	DESTRUCTOR,
	_EOF
};

class RPN_Element
{
	std::string value;
	size_t line = 0;
	size_t position = 0;
	TokenType type;
	size_t jumper = 0;

public:
	RPN_Element(std::string value, TokenType type, size_t line = 0, size_t position = 0, size_t jumper = 0) :
				value(value), type(type), line(line), position(position), jumper(jumper) {}

	RPN_Element(std::string i_value = "' '", TokenType i_type = TokenType::CLEANER) : value(value), type(i_type) {}

	std::string getValue();
	size_t getLine();
	size_t getPosition();
	TokenType getType();
	size_t getJumper();

	friend void to_json(json&, const RPN_Element&);
};

