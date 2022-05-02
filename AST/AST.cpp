#include <iostream>

#include "AST.h"

//#define _DEBUG


AST::AST()
{
	root = std::make_shared<Node>(Node(Token(), TokenType::SPECIAL, nullptr));
	local_root = root;
	cursor = root;
	root->nextNamespace();
}

void AST::ret()
{
	cursor = local_root;
#ifdef _DEBUG
	std::cout << "ret, changed to " << local_root->getValue() << "\n";

	this->printTree();
	std::cout << "\n---------------------------------------------------------\n";
#endif
}
void AST::goDown()
{
	local_roots.push_back(local_root);
	local_root = cursor;

#ifdef _DEBUG
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

#ifdef _DEBUG
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
#ifdef _DEBUG
	std::cout << "Check for full of " << cursor->getValue() << " : Next token is " << next->value << "\n";
#endif
	while (cursor->isFull() && 
			(((cursor->getValue() == "if" || cursor->getValue() == "elif") && (cursor->getChilds().size() >= 3 || next->value != "elif" && next->value != "else")) ||
			(cursor->getValue() == "while" || cursor->getValue() == "for" || cursor->getValue() == "else" ||
			 cursor->getValue() == "print" || cursor->getValue() == "input" ||
			 cursor->getValue() == "break" || cursor->getValue() == "continue")))
	{
#ifdef _DEBUG
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

#ifdef _DEBUG
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
				std::cout << "\nExpected name instead of rounds!!!";
				return 0;
			}
			if (newNode->getRang() == 2 || newNode->getRang() == 3)
			{
				newNode->setParent(cursor);
				if (cursor->getChilds().empty())
					cursor = cursor->addChild(newNode);
				else
				{
					newNode->addChild(cursor->getLastChild());
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
				cursor = cursor->getParent()->replaceLastChild(newNode);
			}
			else if (cursor->isFull())
			{
				newNode->setParent(cursor);
				newNode->addChild(cursor->getLastChild());
				cursor = cursor->replaceLastChild(newNode);
			}
			else
				cursor = cursor->addChild(newNode);
		}
		else if ((type == TokenType::IDENTIFIER || type == TokenType::CONSTANT))
		{
			while (cursor->isFull())
				cursor = cursor->getLastChild();
			cursor = cursor->addChild(newNode);
		}
		break;
	case TokenType::IDENTIFIER:
		if (type == TokenType::OPERATION)
		{
			while (cursor->getParent() != local_root && newNode->getRang() >= cursor->getParent()->getRang())
				cursor = cursor->getParent();
			newNode->addChild(cursor);
			newNode->setParent(cursor->getParent());
			cursor = cursor->getParent()->replaceLastChild(newNode);

			if (token.value == "(" || token.value == "[")
				goDown();
		}
		else
		{
			std::cout << "\nExpected operation instead of: " + token.str();
			return 0;
		}
		break;
	case TokenType::CONSTANT:
		if (type == TokenType::OPERATION)
		{
			if (cursor->getValue() == "." || cursor->getValue() == "(" || (token.type != TokensEnum::STRING && cursor->getValue() == "["))
			{
				std::cout << "\nExpected name instead of: " << cursor->getToken().str();
				return 0;
			}
			while (cursor->getParent() != local_root && newNode->getRang() >= cursor->getParent()->getRang())
				cursor = cursor->getParent();

			newNode->setParent(cursor->getParent());
			newNode->addChild(cursor);
			cursor = cursor->getParent()->replaceLastChild(newNode);
		}
		else
		{
			std::cout << "\nExpected operation instead of: " + token.str();
			return 0;
		}
		break;
	case TokenType::SPECIAL:
		if (token.value == "(")
			goDown();
		else if (type == TokenType::SPECIAL || type == TokenType::TYPE)
		{
			cursor = cursor->addChild(newNode);
			goDown();
			if (token.value == "break" || token.value == "continue")
				goUp();
		}
		else
			cursor = cursor->addChild(newNode);
		break;
	case TokenType::TYPE:
		if (type == TokenType::IDENTIFIER)
			cursor = cursor->addChild(newNode);
		else
		{
			std::cout << "\nExpected identifier instead of: " + token.str();
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
				newNode->addChild(cursor->getLastChild());
			newNode->setParent(cursor->getParent());
			cursor = cursor->getParent()->replaceLastChild(newNode);
		}
		break;
	}

#ifdef _DEBUG
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
	auto out = this->root->RPN(root, 0, true);
	this->rpn_ok = root->getState();
	return out;
}

void AST::showVars()
{
	root->showVars();
}
