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
	nodeVect toJump;


public:
	bool rpn_ok = true;
	AST();

	void ret();
	void goDown();
	void addProbe();
	void addNode();
	bool goUp();
	void checkForFull(std::shared_ptr<Token> next);
	
	bool addToken(Token& token, TokenType type);

	void printTree(bool newView = true);
	std::string genRPN_str(bool full = false);
	nodeVect RPN();

	void showVars();
};