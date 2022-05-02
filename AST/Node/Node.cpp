#include "Node.h"
#include "../../profile.h"

void Node::breakCode()
{
	this->StateOK = false;
}

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
void Node::setJumper(size_t me)
{
	this->jumper = me;
}

nodeVect& Node::getChilds()
{
	return this->childs;
}
std::shared_ptr<Node> Node::getFirstChild()
{
	return this->childs.front();
}
std::shared_ptr<Node> Node::getLastChild()
{
	return this->childs.back();
}
std::shared_ptr<Node> Node::getParent()
{
	return this->parent;
}
size_t Node::getJumper()
{
	return this->jumper;
}
TokenType Node::getType()
{
	return this->type;
}
std::string Node::getValue()
{
	return this->token.value;
}
Token Node::getToken()
{
	return this->token;
}
bool Node::getState()
{
	return this->StateOK;
}

int Node::getRang()
{
	switch (this->getType())
	{
	case TokenType::PROBE:
	case TokenType::SPECIAL:
		return 0;
	case TokenType::OPERATION:
		if (this->token.value == "(" ||
			this->token.value == "[" ||
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
	return 18;
}
bool Node::isEmpty()
{
	return this->childs.empty();
}
bool Node::isFull()
{
	if (this->getType() == TokenType::SPECIAL)
	{
		std::string val = this->getValue();
		size_t amount = this->childs.size();

		if (val == "node" || val == "{")
			return 0;
		else if (val == "if" || val == "elif")
			return amount >= 2;
		else if (val == "while" || val == "class")
			return amount >= 2;
		else if (val == "for")
			return amount >= 4;
		else if (val == "else" || val == "print" || val == "input")
			return amount >= 1;
		else if (val == "break" || val == "continue")
			return 1;
		else
			return 0;
	}
	else
	{
		int op_type = this->getRang();
		if (op_type <= 1)
			return 0;
		else if (op_type <= 3)
			return (this->childs.size() >= 1);
		else if (op_type <= 15)
			return (this->childs.size() >= 2);
		else
			return 0;
	}
}

std::string Node::str(std::shared_ptr<Node> me, std::shared_ptr<Node> local_root, std::shared_ptr<Node> cursor, bool newView, std::string offset_string, bool is_last)
{
	std::string buf = offset_string + ((is_last && newView) ? " `- " : "`- ") + this->getValue() + " : " + getName(this->getType());
	if (me == local_root)
		buf += " <- local root";
	if (me == cursor)
		buf += " <- cursor";

	buf += '\n';

	nodeVect::iterator it = this->getChilds().begin();
	while (it != this->getChilds().end())
	{
		buf += (*it)->str(*it, local_root, cursor, newView, offset_string + ((it == this->getChilds().end() - 1 && newView)? "\t" : "\t|"), it == this->getChilds().end() - 1);
		it++;
	}

	// --- //

	

	return buf;
}

std::string Node::RPN_str(bool full)
{

	if (this->getType() == TokenType::PROBE)
	{
		if (!this->isEmpty())
			return this->getLastChild()->RPN_str(full);
		else
			return "";
	}

	std::string buf(""), buf2("");

	std::vector<std::shared_ptr<Node>> ::iterator it = this->getChilds().begin();
	if (this->getValue() == "(")
		buf2 = (*it++)->RPN_str(full);

	if (full)
	{
		while (it != this->getChilds().end())
			buf += (*it++)->RPN_str(full);

		if (this->getValue() == "(")
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
		if (this->getValue() == "(")
			buf += buf2 + " ";

		buf += this->getValue();
	}
	return buf;
}
nodeVect Node::RPN(std::shared_ptr<Node> me, size_t current_size, bool clean)
{
	if (!StateOK)
		return {};

	nodeVect output;
	nodeVect::iterator it = this->getChilds().begin();

	if (this->getType() == TokenType::_EOF)
	{
		output.push_back(me);
		current_size += 1;
	}
	else if (this->getValue() == "(")
	{
		auto buf = it++;

		while (it != this->getChilds().end())
		{
			nodeVect out = (*it)->RPN(*it, current_size);
			current_size += out.size();
			output.insert(output.cend(), out.begin(), out.end());
			it++;
		}
		nodeVect out = (*buf)->RPN(*buf, current_size);
		current_size += out.size();
		output.insert(output.cend(), out.begin(), out.end());
		output.push_back(me);
	}
	else if (this->getValue() == "if" || this->getValue() == "elif")
	{
		nextNamespace();
		nodeVect::iterator it = this->getChilds().begin();

		nodeVect condition = (*it)->RPN(*it, current_size); ++it;
		current_size += condition.size();

		output.insert(output.cend(), condition.begin(), condition.end());
		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "CJM", this->getToken().line, this->getToken().position), TokenType::CJM, output.back()));
		auto nextElse = output.back();

		current_size += 1;

		bool cleanIt = (*it)->getValue() == "{" ||
					   (*it)->getValue() == "node" || 
					   (*it)->getType() == TokenType::PROBE ||
					   (*it)->getType() == TokenType::TYPE;

		nodeVect body = (*it)->RPN(*it, current_size, cleanIt);

		output.insert(output.cend(), body.begin(), body.end());
		current_size += body.size();

		if ((*it)->getType() != TokenType::SPECIAL && (*it)->getType() != TokenType::_EOF && !cleanIt)
		{
			output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, output.back()));
			current_size += 1;
		}
		++it;

		auto destr = previousNamespace();
		output.insert(output.cend(), destr.begin(), destr.end());
		current_size += destr.size();

		if (it != this->getChilds().end())
		{
			output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, output.back()));
			current_size += 1;

			nextElse->setJumper(current_size);

			nodeVect else_section = (*it)->RPN(*it, current_size);
			current_size += else_section.size();
			output.back()->setJumper(current_size);

			output.insert(output.cend(), else_section.begin(), else_section.end());
		}
		else
			nextElse->setJumper(current_size);
	}
	else if (this->getValue() == "while")
	{
		nextNamespace();
		lastLoopNamespace.push_back(currentNamespace);

		nodeVect::iterator it = this->getChilds().begin();
		size_t begin = current_size;

		nodeVect condition = (*it)->RPN(*it, current_size); ++it;
		current_size += condition.size();

		output.insert(output.cend(), condition.begin(), condition.end());
		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "CJM", this->getToken().line, this->getToken().position), TokenType::CJM, output.back()));
		auto getFromCycle = output.back();
		current_size += 1;

		bool cleanIt = (*it)->getValue() == "{" ||
					   (*it)->getValue() == "node" ||
					   (*it)->getType() == TokenType::PROBE ||
					   (*it)->getType() == TokenType::TYPE;

		if ((*it)->getValue() != "{")
			nextNamespace();

		nodeVect body = (*it)->RPN(*it, current_size, cleanIt);
		output.insert(output.cend(), body.begin(), body.end());
		current_size += body.size();

		if ((*it)->getType() != TokenType::SPECIAL && (*it)->getType() != TokenType::_EOF && !cleanIt)
		{
			output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, output.back()));
			current_size += 1;
		}
		if ((*it)->getValue() != "{")
		{
			auto destr = previousNamespace();
			output.insert(output.cend(), destr.begin(), destr.end());
			current_size += destr.size();
		}

		auto destr = previousNamespace();
		output.insert(output.cend(), destr.begin(), destr.end());
		current_size += destr.size();

		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, output.back()));
		current_size += 1;

		output.back()->setJumper(begin);
		getFromCycle->setJumper(current_size);

		loadCycleRequests(current_size, begin);
		lastLoopNamespace.pop_back();
	}
	else if (this->getValue() == "for")
	{
		nextNamespace();
		lastLoopNamespace.push_back(currentNamespace);

		nodeVect::iterator it = this->getChilds().begin();

		nodeVect init_section = (*it)->RPN(*it, current_size, true); ++it;
		output.insert(output.cend(), init_section.begin(), init_section.end());
		current_size += init_section.size();

		size_t begin = current_size;

		nodeVect condition = (*it)->RPN(*it, current_size); ++it;
		current_size += condition.size();

		output.insert(output.cend(), condition.begin(), condition.end());

		std::shared_ptr<Node> getFromCycle;
		if (!condition.empty())
		{
			output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "CJM", this->getToken().line, this->getToken().position), TokenType::CJM, output.back()));
			getFromCycle = output.back();
			current_size += 1;
		}

		++it;
		bool cleanIt = (*it)->getValue() == "{" ||
					   (*it)->getValue() == "node" ||
					   (*it)->getType() == TokenType::PROBE ||
					   (*it)->getType() == TokenType::TYPE;
		
		if ((*it)->getValue() != "{")
			nextNamespace();

		nodeVect body = (*it)->RPN(*it, current_size, cleanIt); 
		output.insert(output.cend(), body.begin(), body.end());
		current_size += body.size();

		if ((*it)->getType() != TokenType::SPECIAL && (*it)->getType() != TokenType::_EOF && !cleanIt)
		{
			output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, output.back()));
			current_size += 1;
		}

		if ((*it)->getValue() != "{")
		{
			auto destr = previousNamespace();
			output.insert(output.cend(), destr.begin(), destr.end());
			current_size += destr.size();
		}
		--it;

		nodeVect after_body = (*it)->RPN(*it, current_size, true);
		current_size += after_body.size();
		output.insert(output.cend(), after_body.begin(), after_body.end());

		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, output.back()));
		current_size += 1;

		if (!condition.empty())
			getFromCycle->setJumper(current_size);

		output.back()->setJumper(begin);

		loadCycleRequests(current_size, begin);

		auto destr = previousNamespace();
		output.insert(output.cend(), destr.begin(), destr.end());
		current_size += destr.size();
		lastLoopNamespace.pop_back();
	}
	else if (this->getValue() == "break")
	{
		if (lastLoopNamespace.empty())
		{
			std::cout << colorText(31) << "ERR: 'break' must be inside loop. " << this->token.str() << colorText() << "\n";
			breakCode();
			return {};
		}

		nodeVect destr;
		auto it = variables.rbegin();
		do
		{
			if (it->first <= lastLoopNamespace.back())
				break;
			for (auto obj : it->second->getTable())
				destr.push_back(std::make_shared<Node>(Token(obj.second->getCoinName()), TokenType::DESTRUCTOR, std::shared_ptr<Node>()));
		} while ((*it++).first > lastLoopNamespace.back());

		output.insert(output.cend(), destr.begin(), destr.end());
		current_size += destr.size();

		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, std::shared_ptr<Node>()));
		current_size += 1;
		requestBreak(output.back());
	}
	else if (this->getValue() == "continue")
	{
		if (lastLoopNamespace.empty())
		{
			std::cout << colorText(31) << "ERR: 'continue' must be inside loop. " << this->token.str() << colorText() << "\n";
			breakCode();
			return {};
		}
		nodeVect destr;
		auto it = variables.rbegin();
		do
		{
			for (auto obj : it->second->getTable())
				destr.push_back(std::make_shared<Node>(Token(obj.second->getCoinName()), TokenType::DESTRUCTOR, std::shared_ptr<Node>()));
		} while ((*it++).first > lastLoopNamespace.back());

		output.insert(output.cend(), destr.begin(), destr.end());
		current_size += destr.size();

		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, std::shared_ptr<Node>()));
		current_size += 1;
		
		requestContinue(output.back());
	}
	else if (this->getValue() == "class")
	{
		
	}
	else if (this->getType() == TokenType::PROBE)
	{
		if (!this->isEmpty())
			output = this->getLastChild()->RPN(*it, current_size, clean);
		current_size += output.size();
	}
	else if (this->getType() == TokenType::TYPE)
	{
		while (it != this->getChilds().end())
		{
			if ((*it)->getType() == TokenType::OPERATION)
			{
				if (varExists((*it)->getFirstChild()->getValue()))
				{
					std::cout << colorText(31) << "ERR: variable duplication. " << (*it)->getFirstChild()->token.str() << colorText() << "\n";
					breakCode();
					return {};
				}

				output.push_back((*it)->getFirstChild());
				output.push_back(me);
				variables[currentNamespace]->putVar((*it)->getFirstChild()->getValue(), toVarType[this->getValue()], currentNamespace);

				current_size += 2;

				nodeVect out = (*it)->RPN(*it, current_size);
				current_size += out.size();
				output.insert(output.cend(), out.begin(), out.end());

				output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, std::shared_ptr<Node>()));
				current_size += 1;
			}
			else
			{
				if (varExists((*it)->getValue()))
				{
					std::cout << colorText(31) << "ERR: variable duplication. " << (*it)->token.str() << colorText() << "\n";
					breakCode();
					return {};
				}
				output.push_back(*it);
				output.push_back(me);
				variables[currentNamespace]->putVar((*it)->getValue(), toVarType[this->getValue()], currentNamespace);
				current_size += 2;
			}
			it++;
		}
	}
	else 
	{
		if (this->getValue() == "{" || this->getValue() == "else")
			nextNamespace();
		else if (this->getToken().type == TokensEnum::IDENTIFIER && !varExists(this->getValue()) && this->getParent()->getValue() != "class")
		{
			std::cout << colorText(31) << "ERR: unknown variable. " << this->token.str() << colorText() << "\n";
			breakCode();
			return {};
		}

		while (it != this->getChilds().end())
		{
			bool cleanIt = (*it)->getValue() == "{" ||
						   (*it)->getValue() == "node" ||
						   (*it)->getType() == TokenType::PROBE ||
						   (*it)->getType() == TokenType::TYPE;

			nodeVect out = (*it)->RPN(*it, current_size, clean && cleanIt);
			current_size += out.size();
			output.insert(output.cend(), out.begin(), out.end());
			if (clean && (*it)->getType() != TokenType::SPECIAL && (*it)->getType() != TokenType::_EOF && !cleanIt)
			{
				output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, std::shared_ptr<Node>()));
				current_size += 1;
			}
			it++;
		}
		if (this->getType() != TokenType::SPECIAL)
		{
			output.push_back(me);
			current_size += 1;
		}
		if (this->getValue() == "{" || this->getValue() == "else")
		{
			auto destr = previousNamespace();
			output.insert(output.cend(), destr.begin(), destr.end());
			current_size += destr.size();
		}
	}

	return output;
}


void Node::requestBreak(std::shared_ptr<Node> me)
{
	toBreak.push_back(me);
}
void Node::requestContinue(std::shared_ptr<Node> me)
{
	toContinue.push_back(me);
}
void Node::loadCycleRequests(size_t indexBreak, size_t indexContinue)
{
	while (!toBreak.empty())
	{
		toBreak.back()->setJumper(indexBreak);
		toBreak.pop_back();
	}
	while (!toContinue.empty())
	{
		toContinue.back()->setJumper(indexContinue);
		toContinue.pop_back();
	}
}

void Node::nextNamespace()
{
	currentNamespace = ++maxNamespace;
	variables.insert(std::pair<size_t, std::shared_ptr<VarTable>>(currentNamespace, std::make_shared<VarTable>()));
}
nodeVect Node::previousNamespace()
{
	nodeVect out;
	for (auto obj : variables.rbegin()->second->getTable())
		out.push_back(std::make_shared<Node>(Token(obj.second->getCoinName()), TokenType::DESTRUCTOR, std::shared_ptr<Node>()));
	
	variables.erase(variables.rbegin()->first);
	//auto it = variables.rbegin(); it++;
	currentNamespace = variables.rbegin()->first;
	return out;
}

void Node::showVars()
{
	std::cout << "\n\nNamespaces: (size: " << variables.size() << ") ";

	for (auto& nmspce : variables)
	{
		std::cout << "\n\nNamespace[" << nmspce.first << "] Variables: (size: " << nmspce.second->getTable().size() << ") ";
		for (auto& coin : nmspce.second->getTable())
		{
			auto obj = std::dynamic_pointer_cast<CoinVar>(coin.second);
			std::cout << "\n" << obj->str();
		}
	}
}
bool Node::varExists(std::string name)
{
	auto it = variables.rbegin();
	while (it != variables.rend())
	{
		if (it->second->varExists(name))
		{
			return true;
			break;
		}
		it++;
	}
	return false;
}


std::string getName(TokenType token)
{
	switch (token)
	{
	case TokenType::PROBE:			return "Probe";
	case TokenType::IDENTIFIER:		return "Identifier";
	case TokenType::CONSTANT:		return "Constant";
	case TokenType::OPERATION:		return "Operation";
	case TokenType::TYPE:			return "Type";
	case TokenType::JMP:			return "Jump";
	case TokenType::CJM:			return "CondJump";
	case TokenType::SPECIAL:		return "Special";
	case TokenType::CLEANER:		return "Cleaner";
	case TokenType::DESTRUCTOR:		return "Destructor";
	case TokenType::_EOF:			return "EOF";
	default:						return "UNKNOWN TokenType";
	}
}
