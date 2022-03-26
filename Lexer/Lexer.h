#pragma once
#include <string>
#include <vector>
#include <memory>


enum class TokensEnum
{
	BRACKET = 1,
	NUMBER_WITH_LIT,
	NUMBER_F,
	NUMBER,
	NAME,
	SPLITTER,
	OP_EQUAL,
	OP_ASSIGN,
	OP_UNAR,
	OP_BINAR,
	STRING,
	KEYWORD,
	TYPE
};

struct Token {
	TokensEnum type;
	std::string value;
	std::string gotIn = "lexer";

	Token(TokensEnum i_type, std::string i_value) : type(i_type), value(i_value) {}
};

int getTokens(std::vector<std::shared_ptr<Token>>& Tokens, std::string fileName = "prog1.prog");

std::string getName(TokensEnum token);