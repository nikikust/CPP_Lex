#include <iostream>

#include "AST.h"
#include "../profile.h"

//#define ___DEBUG
//#define _output

AST::AST()
{
	root = std::make_shared<Node>(Node(Token(), TokenType::SPECIAL, nullptr));
	local_root = root;
	cursor = root;
	root->nextNamespace();
}
AST::~AST()
{
	root->clearMemory();
	root = nullptr;
	local_root = nullptr;
	local_roots.clear();
	cursor = nullptr;
}

std::map<size_t, std::shared_ptr<CoinTable>>& AST::getVariables()
{
	return root->getVariables();
}
FunctionTable& AST::getFunctions()
{
	return root->getFunctions();
}
ClassTable& AST::getClasses()
{
	return root->getClasses();
}

void AST::ret()
{
	cursor = local_root;
#ifdef ___DEBUG
	std::cout << "ret, changed to " << local_root->getValue() << "\n";

	this->printTree();
	std::cout << "\n---------------------------------------------------------\n";
#endif
}
void AST::goDown()
{
	local_roots.push_back(local_root);
	local_root = cursor;

#ifdef ___DEBUG
	std::cout << "goDown, changed to " << local_root->getValue() << "\n";
#endif
}
void AST::addProbe()
{
	auto newProbe = std::make_shared<Node>(Token("Probe"), TokenType::PROBE, cursor);
	cursor = cursor->addChild(newProbe);
}
void AST::addNode()
{
	 auto node = std::make_shared<Node>(Token(), TokenType::SPECIAL, cursor);
	 cursor = cursor->addChild(node);
}
bool AST::goUp()
{
	if (!local_roots.empty()) // && local_root != cursor->getParent() ???
	{
		if (local_root->getType() == TokenType::OPERATION || local_root->getType() == TokenType::PROBE)
		{
			cursor = local_root;
			local_root = local_roots.back();
			local_roots.pop_back();
		}
		else
		{
			local_root = local_roots.back();
			local_roots.pop_back();
			cursor = local_root;
		}

#ifdef ___DEBUG
		std::cout << "goUp, changed to " << local_root->getValue() << "\n";

		this->printTree();
		std::cout << "\n---------------------------------------------------------\n";
#endif
		return 1;
	}
	return 0;
}
void AST::checkForFull(std::shared_ptr<Token> next)
{
#ifdef ___DEBUG
	std::cout << "Check for full of " << cursor->getValue() << " : Next token is " << next->value << "\n";
#endif
	while (cursor->isFull() && 
			(((cursor->getValue() == "if" || cursor->getValue() == "elif") && (cursor->getChilds().size() >= 3 || next->value != "elif" && next->value != "else")) ||
			(cursor->getValue() != "if" && cursor->getValue() != "elif" && cursor->getType() == TokenType::SPECIAL)))
	{
#ifdef ___DEBUG
		std::cout << "Node " << cursor->getValue() << " is Full!!! Next token is " << next->value << "\n";
#endif
		goUp();
	}
}

bool AST::addToken(Token& token, TokenType type)
{
	auto newNode = std::make_shared<Node>(token, type, cursor);

	if (type == TokenType::_EOF)
	{
		root->addChild(newNode);
		return 1;
	}

#ifdef ___DEBUG
	std::cout << token.str() << " | " << getName(type) << " | Current: " << getName(cursor->getType()) << "\n";
	std::cout << "Root vect:";
	for (std::shared_ptr<Node> node : local_roots)
		std::cout << " " << node->getValue();
	std::cout << "\n";
#endif
	
	switch (cursor->getType())
	{
	case TokenType::OPERATION:
		if (type == TokenType::OPERATION)
		{
			if (cursor->getValue() == "." && token.value == "(")
			{
				std::cout << colorText(31) << "\nTREE: Expected name instead of rounds!!!" << colorText();
				return 0;
			}

			if (cursor->getRang() == 3 && newNode->getRang() == 3)
			{
				newNode->setParent(cursor);
				if (cursor->getChilds().empty())
					cursor = cursor->addChild(newNode);
				else
				{
					newNode->addChild(cursor->getLastChild());
					cursor->getLastChild()->setParent(newNode);
					cursor = cursor->replaceLastChild(newNode);
				}
			}
			else if (token.value == "(")
			{
				goDown();
				addProbe();
			} 
			else if (newNode->getRang() >= cursor->getRang())
			{
				while (cursor->getParent() != local_root && newNode->getRang() >= cursor->getParent()->getRang())
					cursor = cursor->getParent();

				newNode->setParent(cursor->getParent());
				newNode->addChild(cursor);
				cursor->setParent(newNode);
				cursor = newNode->getParent()->replaceLastChild(newNode);
			}
			else if (cursor->isFull())
			{
				newNode->setParent(cursor);
				newNode->addChild(cursor->getLastChild());
				cursor->getLastChild()->setParent(newNode);
				cursor = cursor->replaceLastChild(newNode);
			}
			else
				cursor = cursor->addChild(newNode);
		}
		else if ((type == TokenType::IDENTIFIER || type == TokenType::CONSTANT))
		{
			if (cursor->getValue() == "." && type == TokenType::CONSTANT)
			{
				std::cout << colorText(31) << "\nTREE: Expected name instead of constant!!!" << colorText();
				return 0;
			}
			while (cursor->isFull())
				cursor = cursor->getLastChild();

			newNode->setParent(cursor);
			cursor = cursor->addChild(newNode);
		}
		break;
	case TokenType::IDENTIFIER:
		if (token.value == "(" && cursor->getParent()->getType() == TokenType::TYPE)
		{
			goUp();
			newNode->setName("function_declaration");
			newNode->setParent(cursor);
			newNode->addChild(cursor->getLastChild());
			newNode->setType(TokenType::SPECIAL);
			cursor->getLastChild()->setParent(newNode);
			cursor = cursor->replaceLastChild(newNode);
			goDown();
			addNode();
			goDown();
		}
		else if (type == TokenType::OPERATION)
		{
			while (cursor->getParent() != local_root && newNode->getRang() >= cursor->getParent()->getRang())
				cursor = cursor->getParent();
			newNode->addChild(cursor);
			newNode->setParent(cursor->getParent());
			cursor->setParent(newNode);
			cursor = newNode->getParent()->replaceLastChild(newNode);

			if (token.value == "(" || token.value == "[")
				goDown();
		}
		else
		{
			std::cout << "\nTREE: Expected operation instead of: " + token.str();
			return 0;
		}
		break;
	case TokenType::CONSTANT:
		if (type == TokenType::OPERATION)
		{
			if (cursor->getValue() == "." || cursor->getValue() == "(" || (token.type != TokensEnum::STRING && cursor->getValue() == "["))
			{
				std::cout << "\nTREE: Expected name instead of: " << cursor->getToken().str();
				return 0;
			}
			while (cursor->getParent() != local_root && newNode->getRang() >= cursor->getParent()->getRang())
				cursor = cursor->getParent();

			newNode->setParent(cursor->getParent());
			newNode->addChild(cursor);
			cursor->setParent(newNode);
			cursor = newNode->getParent()->replaceLastChild(newNode);
		}
		else
		{
			std::cout << "\nTREE: Expected operation instead of: " + token.str();
			return 0;
		}
		break;
	case TokenType::SPECIAL:
		if (token.value == "(")
		{
			if (cursor->getValue() == "node" || cursor->getValue() == "{" ||
				cursor->getValue() == "return")
			{
				addProbe();
				goDown();
				addProbe();
			}
			else
				goDown();
		}
		else if (cursor->getValue() == "<")
		{
			cursor = cursor->addChild(newNode);
			if (newNode->getToken().type == TokensEnum::TEMPLATEDTYPE)
				goDown();
		}
		else if (type == TokenType::SPECIAL || type == TokenType::TYPE)
		{
			cursor = cursor->addChild(newNode);
			goDown();
			if (token.value == "break" || token.value == "continue")
				goUp();
		}
		else if (cursor->getValue() == "class")
		{
			if (type == TokenType::IDENTIFIER)
				cursor->addChild(newNode);
			else if (type == TokenType::SPECIAL)
				cursor = cursor->addChild(newNode);
		}
		else
			cursor = cursor->addChild(newNode);
		break;
	case TokenType::TYPE:
		if (type == TokenType::IDENTIFIER)
			cursor = cursor->addChild(newNode);
		else if (type == TokenType::TYPE)
		{
			goUp();
			newNode->setParent(cursor);
			cursor->addChild(newNode);
			goDown();
		}
		else if (type == TokenType::SPECIAL)
		{
			cursor = cursor->addChild(newNode);
			goDown();
		}
		else
		{
			std::cout << "\nTREE: Expected identifier after type instead of: " + token.str();
			return 0;
		}
		break;
	case TokenType::PROBE:
		if (token.value == "(")
		{
			goDown();
			addProbe();
		}
		else
		{
			if (!cursor->isEmpty())
			{
				newNode->addChild(cursor->getLastChild());
				newNode->getLastChild()->setParent(newNode);
			}
			newNode->setParent(cursor->getParent());
			cursor = cursor->getParent()->replaceLastChild(newNode);
			if (type == TokenType::TYPE)
				goDown();
		}
		break;
	}

#ifdef ___DEBUG
	this->printTree();
	std::cout << "\n---------------------------------------------------------\n";
#endif

	return 1;
}

void AST::printTree(bool newView)
{
	std::cout << "\no\n" << root->str(root, local_root, cursor, newView);
}
std::string AST::genRPN_str(bool full)
{
	return root->RPN_str(full);
}
nodeVect AST::RPN()
{
	bool fixed = this->root->fixLinks(root);
	auto out = this->root->RPN(root, 0, true);
	this->rpn_ok = root->getState();
	if (fixed)
		std::cout << colorText(31) << "\n----------------------\nFIXED LINKS IN TREE!!!\n----------------------\n" << colorText();
#ifdef _output
	else
		std::cout << colorText(32) << "\n-------------------------\nNO FIXED LINKS IN TREE!!!\n-------------------------\n" << colorText();
#endif
	return out;
}

void AST::showVars()
{
	root->showVars();
}
void AST::showFunctions()
{
	this->root->showFunctions();
}
void AST::showClasses()
{
	this->root->showClasses();
}
