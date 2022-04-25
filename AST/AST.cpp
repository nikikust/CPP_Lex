#include <iostream>

#include "AST.h"


AST::AST()
{
	root = std::make_shared<Node>(Node(Token(), TokenType::SPECIAL, nullptr));
	local_root = root;
	cursor = root;
}

void AST::nextStatement()
{
	local_roots.clear();
	local_root = root;
	cursor = root;
}

void AST::goDown()
{
	auto newProbe = std::make_shared<Node>(Node(Token("goDownProbe"), TokenType::PROBE, cursor));
	cursor = cursor->addChild(newProbe);

	local_roots.push_back(local_root);
	local_root = cursor->getParent();
}
bool AST::goUp()
{
	if (!local_roots.empty()) // && local_root != cursor->getParent() ???
	{
		cursor = local_root;
		local_root = local_roots.back();
		local_roots.pop_back();
		return 1;
	}
	return 0;
}

bool AST::addToken(Token& token, TokenType type)
{
	auto newNode = std::make_shared<Node>(Node(token, type, cursor));
	//std::cout << "\n" << token.str() << "\n";

	switch (cursor->getType())
	{
	case TokenType::OPERATION:
		if (type == TokenType::OPERATION)
		{
			if (newNode->getRang() >= cursor->getRang())
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
		else if (type == TokenType::ROUNDS)
		{
			if (cursor->getValue() == ".")
			{
				while (cursor->getParent()->getValue() == ".")
					cursor = cursor->getParent();
				newNode->addChild(cursor);
				newNode->setParent(cursor->getParent());
				cursor = cursor->getParent()->replaceLastChild(newNode);
			}
			goDown();
		}
		else
			cursor = cursor->addChild(newNode);
		break;
	case TokenType::IDENTIFIER:
		if (type == TokenType::OPERATION)
		{
			while (cursor->getParent() != local_root && newNode->getRang() >= cursor->getParent()->getRang())
				cursor = cursor->getParent();

			newNode->setParent(cursor->getParent());
			newNode->addChild(cursor);
			cursor = cursor->getParent()->replaceLastChild(newNode);
		}
		else if (type == TokenType::ROUNDS || type == TokenType::SQUARES)
		{
			while (cursor->getParent()->getValue() == ".")
				cursor = cursor->getParent();

			newNode->addChild(cursor);
			newNode->setParent(cursor->getParent());
			cursor = cursor->getParent()->replaceLastChild(newNode);
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
		if (type == TokenType::ROUNDS)
			goDown();
		else
			cursor = cursor->addChild(newNode);
		break;
	case TokenType::PROBE:
		if (newNode->getType() == TokenType::ROUNDS)
			goDown();
		else
		{
			if (!cursor->isEmpty())
				newNode->addChild(cursor->getLastChild());
			newNode->setParent(cursor->getParent());
			cursor = cursor->getParent()->replaceLastChild(newNode);
		}
		break;
	case TokenType::ROUNDS:
	case TokenType::SQUARES:
		if (cursor != local_root)
		{
			newNode->setParent(cursor->getParent());
			newNode->addChild(cursor);
			cursor = cursor->getParent()->replaceLastChild(newNode);
		}
		else
			cursor = cursor->addChild(newNode);

		if (type == TokenType::ROUNDS || type == TokenType::SQUARES)
			goDown();
		break;
	}

	//this->printTree();
	//std::cout << "\n---------------------------------------------------------\n";

	return 1;
}

void AST::printTree()
{
	std::cout << std::endl << root->str(0, root, local_root, cursor);
}
std::string AST::genRPN_str(bool full)
{
	return root->RPN_str(full);
}
nodeVect AST::RPN()
{
	auto rpn = this->root->RPN();
	rpn.push_back(root);
	return rpn;
}