#pragma once
#include <string>
#include <vector>
#include <memory>


enum class TokensEnum
{
	BRACKET = 1,
	NUMBER_WITH_LIT,
	NUMBER,
	NAME,
	SPLITTER,
	OP_UNAR,
	OP_BINAR,
	OP_FAST,
	STRING,
	KEYWORD,
	TYPE
};

struct Token {
	TokensEnum type;
	std::string value;

	Token(TokensEnum i_type, std::string i_value) : type(i_type), value(i_value) {}
};

int getTokens(std::vector<std::unique_ptr<Token>>& Tokens, std::string fileName = "prog1.prog");