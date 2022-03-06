#pragma once
#include <string>
#include <vector>
#include <list>

#include "DSL_Lexer.h"

struct TokenNode
{
	Token Node;
	std::list<TokenNode> childs;

	TokenNode() : Node(Token("##PARSER", "Programm")) {}
};

int buildTree(std::vector<std::unique_ptr<Token>>& Tokens, TokenNode& tree);
