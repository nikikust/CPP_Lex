#pragma once
#include <string>

enum class TokensEnum	// For LEXER
{
	PUNCTUATOR = 1,
	COMMENT,
	FLOAT,
	INT,
	BOOL,
	IDENTIFIER,
	STRING,
	OP_BINAR,
	NEWLINE,
	OP_UNAR_PREF,
	OP_UNAR_POST,
	KEYWORD,
	SIMPLETYPE,
	_EOF,
};

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

std::string getName(TokensEnum token);

class Token {
public:
	TokensEnum type = TokensEnum::PUNCTUATOR;
	std::string value = "node";
	size_t line = 0;
	size_t position = 0;

	std::string gotIn = "lexer";

	Token() {}
	Token(std::string name) : value(name) {}
	Token(TokensEnum i_type, std::string i_value, size_t i_line, size_t i_position) : type(i_type), value(i_value), line(i_line), position(i_position) {}

	std::string str() { return "TOKEN: Type: '" + getName(this->type) + "' Value: '" + this->value + "' at (" + std::to_string(this->line) + "," + std::to_string(this->position) + ")"; }
};
