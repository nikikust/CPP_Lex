#pragma once
#include <memory>
#include <iostream>

#include "../../Lexer/Lexer.h"
#include "../../VarTable/VarTable.h"


static std::unordered_map<std::string, CoinVar::VarType> toVarType =
{
	{"bool", CoinVar::VarType::BOOL},
	{"int", CoinVar::VarType::INT},
	{"double", CoinVar::VarType::DOUBLE},
	{"string", CoinVar::VarType::STRING},
	{"ptr", CoinVar::VarType::POINTER}
};

class Node;

typedef std::vector<std::shared_ptr<Node>> nodeVect;

class Node
{
	Token token;
	TokenType type;

	std::shared_ptr<Node> parent;
	nodeVect childs;

	inline static nodeVect toJump;
	inline static nodeVect toBreak;
	inline static nodeVect toContinue;

	inline static std::map<size_t, std::shared_ptr<VarTable>> variables;
	inline static size_t currentNamespace = 0;
	inline static size_t maxNamespace = 0;
	inline static std::vector<size_t> lastLoopNamespace;
	inline static bool StateOK = true;

	size_t jumper;

public:
	Node(Token i_token, TokenType i_type, std::shared_ptr<Node> i_parent, size_t jumpTo = 0) :
		 token(i_token), type(i_type), parent(i_parent), jumper(jumpTo) {}

	void breakCode();

	std::shared_ptr<Node> addChild(std::shared_ptr<Node> child);
	std::shared_ptr<Node> replaceLastChild(std::shared_ptr<Node> child);

	void setParent(std::shared_ptr<Node>);
	void setName(std::string name);
	void setJumper(size_t me);

	nodeVect& getChilds();
	std::shared_ptr<Node> getFirstChild();
	std::shared_ptr<Node> getLastChild();
	std::shared_ptr<Node> getParent();
	size_t getJumper();
	TokenType getType();
	std::string getValue();
	Token getToken();
	bool getState();
	

	int getRang();
	bool isEmpty();
	bool isFull();
	
	std::string str(std::shared_ptr<Node> me, std::shared_ptr<Node> local_root, std::shared_ptr<Node> cursor, bool newView = true, std::string offset_string = "", bool is_last = true);
	std::string RPN_str(bool full);
	nodeVect RPN(std::shared_ptr<Node> me, size_t current_size, bool clean = false);

	void requestBreak(std::shared_ptr<Node> me);
	void requestContinue(std::shared_ptr<Node> me);
	void loadCycleRequests(size_t indexBreak, size_t indexContinue);

	void nextNamespace();
	nodeVect previousNamespace();
	void showVars();
	bool varExists(std::string name);
};

std::string getName(TokenType token);