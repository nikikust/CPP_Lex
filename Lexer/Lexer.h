#pragma once
#include <string>
#include <vector>
#include <memory>

class Token;

typedef std::vector<std::shared_ptr<Token>> tokenVect;

std::string operator*(std::string a, unsigned int b);
std::string& operator*=(std::string& a, unsigned int b);


enum class TokensEnum
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
	NODE
};

std::string getName(TokensEnum token);

class Token {
public:
	TokensEnum type = TokensEnum::NODE;
	std::string value = "node";
	size_t line = 0;
	size_t position = 0;

	std::string gotIn = "lexer";

	Token() {}
	Token(std::string name) : value(name) {}
	Token(TokensEnum i_type, std::string i_value, size_t i_line, size_t i_position) : type(i_type), value(i_value), line(i_line), position(i_position) {}

	std::string str() { return "TOKEN: Type: '" + getName(this->type) + "' Value: '" + this->value + "' at (" + std::to_string(this->line) + "," + std::to_string(this->position + 1) + ")"; }
};

int getTokens(tokenVect& Tokens, std::string fileName = "prog1.prog");