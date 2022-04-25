#pragma once
#include <memory>
#include "../../Lexer/Lexer.h"
#include <iostream>

enum class TokenType
{
	PROBE, //EMPTY node
	IDENTIFIER,
	CONSTANT,
	OPERATION,
	ROUNDS, // ()
	SQUARES, // []
	TYPE,
	SPECIAL // Root
};

class Node;

typedef std::vector<std::shared_ptr<Node>> nodeVect;

class Node
{
	Token token;
	TokenType type;

	std::shared_ptr<Node> parent;
	nodeVect childs;

public:
	Node(Token i_token, TokenType i_type, std::shared_ptr<Node> i_parent) : token(i_token), type(i_type), parent(i_parent) {}

	std::shared_ptr<Node> addChild(std::shared_ptr<Node> child);
	std::shared_ptr<Node> replaceLastChild(std::shared_ptr<Node> child);

	void setParent(std::shared_ptr<Node>);
	void setName(std::string name);

	nodeVect& getChilds();
	std::shared_ptr<Node> getLastChild();
	std::shared_ptr<Node> getParent();
	TokenType getType();
	std::string getValue();

	int getRang();
	bool isEmpty();
	bool isFull();
	
	std::string str(int level, std::shared_ptr<Node> me, std::shared_ptr<Node> local_root, std::shared_ptr<Node> cursor);
	std::string RPN_str(bool full);
	nodeVect RPN();
};

std::string getName(TokenType token);