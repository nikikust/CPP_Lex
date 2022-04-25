#include "Node.h"


std::shared_ptr<Node> Node::addChild(std::shared_ptr<Node> child)
{
	this->childs.push_back(child);
	return this->getLastChild();
}
std::shared_ptr<Node> Node::replaceLastChild(std::shared_ptr<Node> child)
{
	this->childs.pop_back();
	return this->addChild(child);
}

void Node::setParent(std::shared_ptr<Node> i_parent)
{
	this->parent = i_parent;
}
void Node::setName(std::string name)
{
	this->token.value = name;
}

nodeVect& Node::getChilds()
{
	return this->childs;
}
std::shared_ptr<Node> Node::getLastChild()
{
	return this->childs.back();
}
std::shared_ptr<Node> Node::getParent()
{
	return this->parent;
}
TokenType Node::getType()
{
	return this->type;
}
std::string Node::getValue()
{
	return this->token.value;
}

int Node::getRang()
{
	switch (this->getType())
	{
	case TokenType::PROBE:
	case TokenType::SPECIAL:
	case TokenType::ROUNDS:
		return 0;
	case TokenType::OPERATION:
		if (this->token.value == "()" ||
			this->token.value == "[]" ||
			this->token.value == ".")
			return 1;
		else if (this->token.type == TokensEnum::OP_UNAR_POST)
			return 2;
		else if (this->token.type == TokensEnum::OP_UNAR_PREF)
			return 3;
		else if (this->token.value == "*" ||
				 this->token.value == "/" ||
				 this->token.value == "%")
			return 4;
		else if (this->token.value == "+" ||
				 this->token.value == "-")
			return 5;
		else if (this->token.value == "<<" ||
				 this->token.value == ">>")
			return 6;
		else if (this->token.value == "<" ||
				 this->token.value == "<=" ||
				 this->token.value == ">=" ||
				 this->token.value == ">")
			return 7;
		else if (this->token.value == "==" ||
				 this->token.value == "!=")
			return 8;
		else if (this->token.value == "&")
			return 9;
		else if (this->token.value == "^")
			return 10;
		else if (this->token.value == "|")
			return 11;
		else if (this->token.value == "&&")
			return 12;
		else if (this->token.value == "||")
			return 13;
		//else if ("?:")
		//	return 14;
		else if (this->token.value == "="  ||
				 this->token.value == "*=" ||
				 this->token.value == "/=" ||
				 this->token.value == "%=" ||
				 this->token.value == "+=" ||
				 this->token.value == "-=" ||
				 this->token.value == "&=" ||
				 this->token.value == "^=" ||
				 this->token.value == "|=" ||
				 this->token.value == "<<="||
				 this->token.value == ">>=")
			return 15;
		//else if (",")
		//	return 16;
	case TokenType::CONSTANT:
	case TokenType::IDENTIFIER:
		return 17;
	}
}
bool Node::isEmpty()
{
	return this->childs.empty();
}
bool Node::isFull()
{
	int op_type = this->getRang();
	if (op_type == 1)
		return 0;
	else if (op_type <= 3)
		return 1;
	else if (op_type <= 15)
		return 2;
	else
		return 0;
}

std::string Node::str(int level, std::shared_ptr<Node> me, std::shared_ptr<Node> local_root, std::shared_ptr<Node> cursor)
{
	std::string buf("|\t");
	buf *= level;

	buf += "|`- " + this->getValue() + " : " + getName(this->getType());
	if(me == local_root)
		buf += " <- local root";
	if (me == cursor)
		buf += " <- cursor";

	std::vector<std::shared_ptr<Node>> ::iterator it = this->getChilds().begin();
	while (it != this->getChilds().end())
	{
		buf += "\n" + (*it)->str(level + 1, *it, local_root, cursor);
		it++;
	}

	return buf;
}
std::string Node::RPN_str(bool full)
{
	if (this->getType() == TokenType::PROBE)
	{
		if (!this->isEmpty())
			return this->getLastChild()->RPN_str(full);
		return "";
	}

	std::string buf(""), buf2("");

	std::vector<std::shared_ptr<Node>> ::iterator it = this->getChilds().begin();
	if (this->getType() == TokenType::ROUNDS)
		buf2 = (*it++)->RPN_str(full);

	if (full)
	{
		while (it != this->getChilds().end())
			buf += (*it++)->RPN_str(full);

		if (this->getType() == TokenType::ROUNDS)
			buf += buf2;

		if (this->getType() != TokenType::PROBE)
			buf += this->token.str();
		buf += '\n';
	}
	else
	{
		while (it != this->getChilds().end())
		{
			buf += (*it)->RPN_str(full);
			if ((*it++)->getType() != TokenType::PROBE)
				buf += " ";
		}
		if (this->getType() == TokenType::ROUNDS)
			buf += buf2 + " ";

		buf += this->getValue();
	}
	return buf;
}

nodeVect Node::RPN()
{
	std::vector<std::shared_ptr<Node>> ::iterator it = this->getChilds().begin();
	std::vector<std::shared_ptr<Node>> output;
	
	if (this->getType() == TokenType::ROUNDS)
	{
		auto buf = it++;

		while (it != this->getChilds().end())
		{
			nodeVect out = (*it)->RPN();
			output.insert(output.cend(), out.begin(), out.end());
			if ((*it)->getType() != TokenType::PROBE)
				output.push_back(*it);
			it++;
		}
		nodeVect out = (*buf)->RPN();
		output.insert(output.cend(), out.begin(), out.end());
		output.push_back(*buf);
	}
	else if (this->getType() == TokenType::PROBE)
	{
		if (!this->isEmpty())
			output = this->getLastChild()->RPN();
	}
	else
	{
		while (it != this->getChilds().end())
		{
			nodeVect out = (*it)->RPN();
			output.insert(output.cend(), out.begin(), out.end());
			if((*it)->getType() != TokenType::PROBE)
				output.push_back(*it);
			it++;
		}
	}

	return output;
}

std::string getName(TokenType token)
{
	switch (token)
	{
	case TokenType::PROBE:		return "Probe";
	case TokenType::IDENTIFIER:	return "Identifier";
	case TokenType::CONSTANT:	return "Constant";
	case TokenType::OPERATION:	return "Operation";
	case TokenType::ROUNDS:		return "FunctionCall";
	case TokenType::SQUARES:	return "ArrayAccess";
	case TokenType::SPECIAL:	return "Special";
	}
}