#pragma once
#include <memory>

#include "../Lexer/Lexer.h"
#include "Node/Node.h"


class AST
{
	std::shared_ptr<Node> root;
	
	nodeVect local_roots;
	std::shared_ptr<Node> local_root;

	std::shared_ptr<Node> cursor;

public:
	AST();

	void nextStatement();
	void goDown();
	bool goUp();
	
	bool addToken(Token& token, TokenType type);

	void printTree();
	std::string genRPN_str(bool full = false);
	nodeVect RPN();
};