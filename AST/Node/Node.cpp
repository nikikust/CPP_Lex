#include "Node.h"
#include "../../profile.h"
#include "../../Memory/Operations.h"


Node::~Node()
{
	childs.clear();
}
void Node::clearMemory()
{
	variables.clear();
	functions.getTable().clear();
	classes.getTable().clear();
}

std::map<size_t, std::shared_ptr<CoinTable>>& Node::getVariables()
{
	return variables;
}
FunctionTable& Node::getFunctions()
{
	return functions;
}
ClassTable& Node::getClasses()
{
	return classes;
}

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
uint32_t Node::child_adr(std::shared_ptr<Node> child)
{
	uint32_t cnt = 0;
	for (auto& obj : childs)
	{
		if (obj == child)
			return cnt;
		++cnt;
	}
	return cnt;
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
void Node::setType(TokenType type)
{
	this->type = type;
}
void Node::setTokenType(TokensEnum type)
{
	this->token.type = type;
}
void Node::setSubValue(std::string value)
{
	this->sub_value = value;
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
	return this->parent.lock();
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
Token& Node::getToken()
{
	return this->token;
}
bool Node::getState()
{
	return this->StateOK;
}
std::string Node::getPosition()
{
	return "(" + std::to_string(this->token.line) + ", " + std::to_string(this->token.position) + ")";
}
std::string Node::getSubValue()
{
	return this->sub_value;
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
			this->token.value == "."||
			this->token.value == "->")
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

		if (val == "node" || val == "{" || val == "<")
			return 0;
		else if (val == "if" || val == "elif")
			return amount >= 2;
		else if (val == "while" || val == "class")
			return amount >= 2;
		else if (val == "for")
			return amount >= 4;
		else if (val == "function_declaration")
			return amount >= 3;
		else if (val == "else" || val == "print" || val == "input" || val == "return" || val == "template")
			return amount >= 1;
		else if (val == "break" || val == "continue" || val == "exit")
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
	std::string buf = offset_string + ((is_last && newView) ? " `- " : "`- ") +
					  ((this->getType() == TokenType::CONSTANT && this->getToken().type == TokensEnum::STRING) ? "\"" : "") +
					  this->getValue() +
					  ((this->getType() == TokenType::CONSTANT && this->getToken().type == TokensEnum::STRING) ? "\"" : "") +
					  " " + this->getPosition() + " : " + getName(this->getType());
	if (me == local_root)
		buf += " <- local root";
	if (me == cursor)
		buf += " <- cursor";

	buf += '\n';

	nodeVect::iterator it = this->getChilds().begin();
	while (it != this->getChilds().end())
	{
		buf += (*it)->str(*it, local_root, cursor, newView, offset_string + ((it == this->getChilds().end() - 1 && newView)? "\t" : "\t|"), it == this->getChilds().end() - 1);
		++it;
	}
	return buf;
}

bool Node::fixLinks(std::shared_ptr<Node> me)
{
	nodeVect::iterator it = childs.begin();
	bool fixed = false;

	while (it != childs.end())
	{
		if ((*it)->getParent() != me)
		{
			(*it)->setParent(me);
			fixed = true;
		}
		fixed = (*it)->fixLinks(*it) || fixed;
		++it;
	}
	return fixed;
}
void Node::clearProbes(std::shared_ptr<Node> me)
{
	nodeVect::iterator it = childs.begin();
	std::vector<std::shared_ptr<Node>> out;

	while (it != childs.end())
	{
		if ((*it)->getType() == TokenType::PROBE)
		{
			if (!(*it)->isEmpty())
			{
				std::shared_ptr<Node> buf = (*it)->getFirstChild();
				buf->setParent(me);
				buf->clearProbes(buf);
				out.push_back(buf);
				(*it)->childs.clear();
			}
		}
		else
		{
			(*it)->clearProbes(*it);
			out.push_back(*it);
		}
		++it;
	}
	childs = out;
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

///
nodeVect Node::RPN(std::shared_ptr<Node> me, size_t current_size, bool clean)
{
	if (!StateOK)
		return {};

	if (this->getType() == TokenType::CONSTANT)
		addConstant(me);

	nodeVect output = {};
	nodeVect::iterator it = this->childs.begin();

	if (this->getType() == TokenType::_EOF)
	{
		output.push_back(me);
		current_size += 1;
	}
	else if (this->getValue() == "(")
	{
		if (checkTypes(*it, true, "function") == nullptr || !functionAttributesCheck(me))
		{
			breakCode();
			return {};
		}
		auto buf = it++;

		while (it != this->getChilds().end())
		{
			nodeVect out = (*it)->RPN(*it, current_size);
			current_size += out.size();
			output.insert(output.cend(), out.begin(), out.end());
			++it;
		}

		if ((*buf)->getValue() == "." || (*buf)->getValue() == "->")
		{
			last_THIS.push(checkTypes(*buf));

			nodeVect out = (*buf)->getFirstChild()->RPN((*buf)->getFirstChild(), current_size);
			output.insert(output.cend(), out.begin(), out.end());
			current_size += out.size();

			auto me2 = *this;
			me2.setName(".(");
			me2.setSubValue((*buf)->getLastChild()->getValue());

			output.push_back(std::make_shared<Node>(me2));
			current_size += 1;

			last_THIS.pop();
		}
		else
		{
			auto me2 = *this;
			me2.setSubValue((*buf)->getValue());

			output.push_back(std::make_shared<Node>(me2));
			current_size += 1;
		}
	}
	else if (this->getValue() == "if" || this->getValue() == "elif")
	{
		nextNamespace();

		nodeVect condition = (*it)->RPN(*it, current_size); ++it;
		current_size += condition.size();

		output.insert(output.cend(), condition.begin(), condition.end());
		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "CJM", this->getToken().line, this->getToken().position), TokenType::CJM, output.back()));
		auto nextElse = output.back();

		current_size += 1;

		bool cleanIt = (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "{" ||
					   (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "node" || 
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

			nodeVect else_section = (*it)->RPN(*it, current_size, true);
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

		size_t begin = current_size;

		nodeVect condition = (*it)->RPN(*it, current_size); ++it;
		current_size += condition.size();

		output.insert(output.cend(), condition.begin(), condition.end());
		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "CJM", this->getToken().line, this->getToken().position), TokenType::CJM, output.back()));
		auto getFromCycle = output.size() - 1;
		current_size += 1;

		bool cleanIt = (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "{" ||
					   (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "node" ||
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
		output.at(getFromCycle)->setJumper(current_size);

		loadCycleRequests(current_size, begin);
		lastLoopNamespace.pop_back();
	}
	else if (this->getValue() == "for")
	{
		nextNamespace();
		lastLoopNamespace.push_back(currentNamespace);

		nodeVect init_section = (*it)->RPN(*it, current_size, true); ++it;
		output.insert(output.cend(), init_section.begin(), init_section.end());
		current_size += init_section.size();

		size_t begin = current_size;

		nodeVect condition = (*it)->RPN(*it, current_size); ++it;
		current_size += condition.size();

		output.insert(output.cend(), condition.begin(), condition.end());

		size_t getFromCycle;
		if (!condition.empty())
		{
			output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "CJM", this->getToken().line, this->getToken().position), TokenType::CJM, output.back()));
			getFromCycle = output.size() - 1;
			current_size += 1;
		}

		++it;
		bool cleanIt = (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "{" ||
					   (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "node" ||
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

		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, std::shared_ptr<Node>()));
		current_size += 1;

		if (!condition.empty())
			output.at(getFromCycle)->setJumper(current_size);

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
		auto it2 = variables.rbegin();
		do
		{
			if (it2->first <= lastLoopNamespace.back())
				break;
			for (auto obj : it2->second->getTable())
				destr.push_back(std::make_shared<Node>(Token(obj.first), TokenType::DESTRUCTOR, std::shared_ptr<Node>()));
		} while ((*it2++).first > lastLoopNamespace.back());

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
		auto it2 = variables.rbegin();
		do
		{
			if (it2->first <= lastLoopNamespace.back())
				break;
			for (auto obj : it2->second->getTable())
				destr.push_back(std::make_shared<Node>(Token(obj.first), TokenType::DESTRUCTOR, std::shared_ptr<Node>()));
		} while ((*it2++).first > lastLoopNamespace.back());

		output.insert(output.cend(), destr.begin(), destr.end());
		current_size += destr.size();

		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, std::shared_ptr<Node>()));
		current_size += 1;
		
		requestContinue(output.back());
	}
	else if (this->getValue() == "return")
	{
		if (functionDeclaration.empty() || functionDeclaration.top() == "")
		{
			std::cout << colorText(31) << "ERR: 'return' must be inside function. " << this->token.str() << colorText() << "\n";
			breakCode();
			return {};
		}

		if (functionDeclaration.top() != "void")
		{
			if (this->getChilds().empty())
			{
				std::cout << colorText(31) << "ERR: function must return '" << functionDeclaration.top() << "' at " << this->token.str() << colorText() << "\n";
				breakCode();
				return {};
			}

			std::string val = this->getFirstChild()->getOpType();
			if (val != functionDeclaration.top() && !areNumerics(val, functionDeclaration.top()))
			{
				std::cout << colorText(31) << "ERR: function returns '" << functionDeclaration.top() << "', provided '" << val << "'" << colorText() << "\n";
				breakCode();
				return {};
			}

			nodeVect out = this->getFirstChild()->RPN(this->getFirstChild(), current_size);
			output.insert(output.cend(), out.begin(), out.end());
			current_size += out.size();

			output.push_back(me);
			current_size += 1;
		}
		else
		{
			if (!this->getChilds().empty())
			{
				std::cout << colorText(31) << "ERR: function doesn't return values, after " << this->token.str() << colorText() << "\n";
				breakCode();
				return {};
			}
			auto me2 = *this;
			me2.setName("return_void");
			output.push_back(std::make_shared<Node>(me2));
			current_size += 1;
		}

		nodeVect destr;
		auto it2 = variables.rbegin();
		do
		{
			if (it2->first <= functionNamespace)
				break;
			for (auto obj : it2->second->getTable())
				destr.push_back(std::make_shared<Node>(Token(obj.first), TokenType::DESTRUCTOR, std::shared_ptr<Node>()));
		} while ((*it2++).first > functionNamespace);

		output.insert(output.cend(), destr.begin(), destr.end());
		current_size += destr.size();

		output.push_back(std::make_shared<Node>(Token(TokensEnum::KEYWORD, "JMP", this->getToken().line, this->getToken().position), TokenType::JMP, std::shared_ptr<Node>()));
		current_size += 1;

		requestReturn(output.back());
	}
	else if (this->getValue() == "print")
	{
		if (this->getChilds().empty())
		{
			auto newNode = std::make_shared<Node>(Token(TokensEnum::STRING, "", 0, 0), TokenType::CONSTANT, me);
			addConstant(newNode);

			output.push_back(newNode);
			current_size += 1;
			output.push_back(me);
			current_size += 1;
		}
		else
		{
			std::string type = this->getFirstChild()->getOpType();
			if (areNumerics(type, "int") || type == "string")
			{
				nodeVect out = this->getFirstChild()->RPN(this->getFirstChild(), current_size);
				output.insert(output.cend(), out.begin(), out.end());
				current_size += out.size();

				output.push_back(me);
				current_size += 1;
			}
			else
			{
				std::cout << colorText(31) << "ERR: expected string or numeric, provided '" << type << "' at "
						  << this->getPosition() << colorText() << "\n";
				breakCode();
				return {};
			}
		}
	}
	else if (this->getValue() == "input")
	{
		std::shared_ptr<Coin> res = checkTypes(this->getFirstChild());
		if (res == nullptr || res->isConst())
		{
			std::cout << colorText(31) << "ERR: expression must be l_value '" << this->getFirstChild()->getValue()
					  << "' at " << this->getPosition() << colorText() << "\n";
			breakCode();
			return {};
		}

		if (areNumerics(res->getType(), "int") || res->getType() == "string")
		{
			nodeVect out = this->getFirstChild()->RPN(this->getFirstChild(), current_size);
			output.insert(output.cend(), out.begin(), out.end());
			current_size += out.size();

			output.push_back(me);
			current_size += 1;
		}
		else
		{
			std::cout << colorText(31) << "ERR: expected string or numeric, provided '" << res->getType() << "' at "
				<< this->getFirstChild()->getPosition() << colorText() << "\n";
			breakCode();
			return {};
		}
	}
	else if (this->getValue() == "class")
	{
		nextNamespace();
		classDeclaration.push((*it)->getValue());
		
		++it;

		if (!((*it)->getValue() == "template" && replace_with.empty()))
		{
			if ((*it)->getValue() == "template")
			{
				replace_what.push((*it)->getFirstChild()->getValue());
				++it;
			}

			auto& body = *it;

			nodeVect::iterator it2 = body->childs.begin();
			nodeVect fieldsVect;
			while (it2 != body->childs.end())
			{
				if ((*it2)->getType() == TokenType::TYPE)	// FIELDS
				{
					auto field = (*it2)->RPN(*it2, 0);
					fieldsVect.insert(fieldsVect.cend(), field.begin(), field.end());
				}
				++it2;
			}
			CoinTable fields = *variables[currentNamespace];

			RPNVect fieldsRPN;
			for (auto& obj : fieldsVect)
				fieldsRPN.push_back(std::make_shared<RPN_Element>(
					obj->getValue(), obj->getType(), obj->getToken().line, obj->getToken().position, obj->getJumper(), obj->getSubValue()));

			classes.putClass(classDeclaration.top(), std::make_shared<CoinTable>(fields),
				std::make_shared<RPNVect>(fieldsRPN), std::make_shared<FunctionTable>());
			
			variables[currentNamespace]->putDelayedObjectCoin("this", classDeclaration.top());
			last_THIS.push(variables[currentNamespace]->getCoin("this"));
			std::dynamic_pointer_cast<Object, Coin>(last_THIS.top())->upgradeToFull(classes.getClass(classDeclaration.top()));

			it2 = body->childs.begin();
			while (it2 != body->childs.end())
			{
				if ((*it2)->getType() == TokenType::SPECIAL)	// METHODS
					(*it2)->RPN(*it2, current_size);
				++it2;
			}

			for (auto& obj : classes.getClass(classDeclaration.top())->getFields()->getTable())
			{
				if (obj.second->getCoinType() == CoinType::OBJECT)
				{
					std::shared_ptr<Object> object = std::dynamic_pointer_cast<Object, Coin>(obj.second);
					if (object->getDelayedClass() == classDeclaration.top())
						object->upgradeToFull(classes.getClass(classDeclaration.top()));
				}
			}

			last_THIS.pop();

			replace_what.pop();
		}
		else
		{
			templated_classes.insert({ classDeclaration.top() , this->getParent()->child_adr(me) });
		}

		classDeclaration.pop();
		previousNamespace();
	}
	else if (this->getValue() == "function_declaration")
	{
		nextNamespace();
		/*
		Abstract Syntax Tree:
		o
		 `- node : Special <- local root <- cursor
				|`- function_declaration : Special
				|       |`- void : Type ---------------------------------------------------- Return type
				|       |        `- main : Identifier -------------------------------------- Name
				|       |`- node : Special ------------------------------------------------- Attributes
				|       |       |`- int : Type
				|       |       |        `- a : Identifier
				|       |        `- ptr : Type
				|       |               |`- < : Special
				|       |               |        `- int : Type
				|       |                `- b : Identifier
				|        `- { : Special ---------------------------------------------------- Body
				|               |`- int : Type
				|               |        `- = : Operation
				|               |               |`- c : Identifier
				|               |                `- + : Operation
				|               |                       |`- a : Identifier
				|               |                        `- * : Operation
				|               |                                `- b : Identifier
				|                `- return : Special
				|                        `- c : Identifier
				 `- EOF : EOF
		*/

		nodeVect typeAndName = (*it)->RPN(*it, current_size);
		std::string functionName = typeAndName.front()->getSubValue();
		std::string functionReturnType = typeAndName.front()->getValue();

		functionDeclaration.push(functionReturnType);

		++it;

		if ((classDeclaration.empty() || classDeclaration.top() == "") && functions.functionExists(functionName))
		{
			std::cout << colorText(31) << "ERR: function name duplication. " << this->getFirstChild()->getFirstChild()->token.str() << colorText() << "\n";
			breakCode();
			return {};
		}
		else if (!classDeclaration.empty() && classDeclaration.top() != "" && classes.getClass(classDeclaration.top())->getMethods()->functionExists(functionName))
		{
			std::cout << colorText(31) << "ERR: method name duplication. " << this->getFirstChild()->getFirstChild()->token.str() << colorText() << "\n";
			breakCode();
			return {};
		}


		nextNamespace();
		functionNamespace = currentNamespace;
		nodeVect attributesVect = (*it)->RPN(*it, current_size);
		RPNVect attributesRPN;
		for (auto obj : attributesVect)
			attributesRPN.push_back(std::make_shared<RPN_Element>(
				obj->getValue(), obj->getType(), obj->getToken().line, obj->getToken().position, obj->getJumper(), obj->getSubValue()));

		std::vector<std::string> order;
		std::vector<std::string> orderNames;

		for (auto& attr : this->childs[1]->childs)
		{
			order.push_back(attr->getValue());
			orderNames.push_back(attr->getLastChild()->getValue());
		}
		CoinTable attributes = *variables[currentNamespace];

		++it;

		nodeVect body = (*it)->RPN(*it, 0, true);
		body.push_back(std::make_shared<Node>(Token("EOF"), TokenType::_EOF, std::shared_ptr<Node>()));
		loadReturnRequests(body.size() - 1);
		
		RPNVect bodyRPN;
		for (auto obj : body)
			bodyRPN.push_back(std::make_shared<RPN_Element>(
				obj->getValue(), obj->getType(), obj->getToken().line, obj->getToken().position, obj->getJumper(), obj->getSubValue()));

		if (!functionReturnType.starts_with("ptr<"))
			getCoin(functionName)->setConst(true);

		auto it2 = attributes.getTable().begin();
		while (it2 != attributes.getTable().end())
		{
			if (it2->first.starts_with("##alloc"))
				it2 = attributes.getTable().erase(it2);
			else
				++it2;
		}

		if (!classDeclaration.empty() && classDeclaration.top() != "")
			classes.getClass(classDeclaration.top())->getMethods()->putFunction(
				functionName, std::make_shared<CoinTable>(attributes), order, orderNames,
				std::make_shared<RPNVect>(attributesRPN), getCoin(functionName), std::make_shared<RPNVect>(bodyRPN));
		else
			functions.putFunction(
				functionName, std::make_shared<CoinTable>(attributes), order, orderNames,
				std::make_shared<RPNVect>(attributesRPN), getCoin(functionName), std::make_shared<RPNVect>(bodyRPN));

		previousNamespace();

		functionDeclaration.pop();
		previousNamespace();
	}
	else if (this->getValue() == "exit") 
	{
		output.push_back(me);
		current_size += 1;
	}
	else if (this->getType() == TokenType::PROBE)
	{
		if (!this->isEmpty())
		{
			bool cleanIt = (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "{" ||
						   (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "else" ||
						   (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "node" ||
						   (*it)->getType() == TokenType::PROBE ||
						   (*it)->getType() == TokenType::TYPE;
			output = this->getLastChild()->RPN(*it, current_size, clean && cleanIt);

			if (clean && (*it)->getType() != TokenType::SPECIAL && (*it)->getType() != TokenType::_EOF && !cleanIt)
			{
				output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, std::shared_ptr<Node>()));
				current_size += 1;
			}

			current_size += output.size();
		}
	}
	else if (this->getType() == TokenType::TYPE)
	{
		if (templated_classes.contains(this->getValue()))
		{
			if (!classes.classExists(this->concatTypes()))
			{
				replace_with.push(this->getFirstChild()->getFirstChild());

				std::shared_ptr<Node> root = this->parent.lock();
				while (root->parent.lock() != nullptr)
					root = root->parent.lock();

				std::shared_ptr<Node> class_create = root->childs.at(templated_classes.at(this->getValue()))->clone(root);
				class_create->getFirstChild()->setName(this->concatTypes());

				replace_what.push(class_create->childs.at(1)->getFirstChild()->getValue());

				class_create->getLastChild()->replace_template();

				functionDeclaration.push("");
				class_create->RPN(class_create, 0);
				functionDeclaration.pop();

				replace_what.pop();
				replace_with.pop();
			}
			++it;
		}
		else if (this->getValue() == "ptr")
		{
			++it;
		}

		while (it != this->childs.end())
		{
			if ((*it)->getType() == TokenType::OPERATION)
			{
				if (varExists((*it)->getFirstChild()->getValue()) ||
					functions.functionExists((*it)->getFirstChild()->getValue()) ||
					classes.classExists((*it)->getFirstChild()->getValue()))
				{
					std::cout << colorText(31) << "ERR: variable name duplication. " << (*it)->getFirstChild()->token.str() << colorText() << "\n";
					breakCode();
					return {};
				}
				auto me2 = *this;
				if (this->getToken().type == TokensEnum::TEMPLATEDTYPE)
					me2.setName("ptr<" + this->getFirstChild()->concatTypes() + ">");
				me2.setSubValue((*it)->getFirstChild()->getValue());
				output.push_back(std::make_shared<Node>(me2));
				current_size += 1;

				if (this->getValue() == "int")
					variables[currentNamespace]->putCoin((*it)->getFirstChild()->getValue(), NumType::INT);
				else if (this->getValue() == "double")
					variables[currentNamespace]->putCoin((*it)->getFirstChild()->getValue(), NumType::DOUBLE);
				else if (this->getValue() == "bool")
					variables[currentNamespace]->putCoin((*it)->getFirstChild()->getValue(), NumType::BOOL);
				else if (this->getValue() == "ptr")
				{
					variables[currentNamespace]->putCoin((*it)->getFirstChild()->getValue(), this->getFirstChild()->concatTypes());
					auto res = this->getFirstChild()->allocate(1);
					if (res.size() != 1)
					{
						breakCode();
						return {};
					}

					std::dynamic_pointer_cast<Pointer, Coin>(getCoin((*it)->getFirstChild()->getValue()))->set(res[0]);
				}
				else if (this->getValue() == "string")
					variables[currentNamespace]->putCoin((*it)->getFirstChild()->getValue());
				else if (classes.classExists(this->getValue()))
				{
					std::shared_ptr<CoinTable> newFields = classes.getClass(this->getValue())->getFields()->clone();
					variables[currentNamespace]->putCoin(
						(*it)->getFirstChild()->getValue(), classes.getClass(this->getValue()), newFields);
				}
				else if (classes.classExists(this->concatTypes()))
				{
					std::shared_ptr<CoinTable> newFields = classes.getClass(this->concatTypes())->getFields()->clone();
					variables[currentNamespace]->putCoin(
						(*it)->getFirstChild()->getValue(), classes.getClass(this->concatTypes()), newFields);
				}
				else if (this->getValue() == "void")
					variables[currentNamespace]->putCoin((*it)->getFirstChild()->getValue(), true, true);
				else
				{
					std::cout << colorText(31) << "ERR: unknown variable type '" << this->concatTypes()
											   << "' at " << this->getPosition() << colorText() << "\n";
					breakCode();
					return {};
				}

				nodeVect out = (*it)->RPN(*it, current_size);
				current_size += out.size();
				output.insert(output.cend(), out.begin(), out.end());

				output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, std::shared_ptr<Node>()));
				current_size += 1;
			}
			else
			{
				if (varExists((*it)->getValue()) || functions.functionExists((*it)->getValue()) ||
					classes.classExists((*it)->getValue()) || classes.classExists((*it)->concatTypes()))
				{
					std::cout << colorText(31) << "ERR: variable name duplication. " << (*it)->token.str() << colorText() << "\n";
					breakCode();
					return {};
				}
				
				auto me2 = *this;
				if (this->getToken().type == TokensEnum::TEMPLATEDTYPE)
					me2.setName(this->getValue() + "<" + this->getFirstChild()->concatTypes() + ">");
				me2.setSubValue((*it)->getValue());
				output.push_back(std::make_shared<Node>(me2));
				current_size += 1;

				if (this->getValue() == "int")
					variables[currentNamespace]->putCoin((*it)->getValue(), NumType::INT);
				else if (this->getValue() == "double")
					variables[currentNamespace]->putCoin((*it)->getValue(), NumType::DOUBLE);
				else if (this->getValue() == "bool")
					variables[currentNamespace]->putCoin((*it)->getValue(), NumType::BOOL);
				else if (this->getValue() == "ptr")
				{
					variables[currentNamespace]->putCoin((*it)->getValue(), this->getFirstChild()->concatTypes());
					auto res = this->getFirstChild()->allocate(1);
					if (res.size() != 1)
					{
						std::cout << "\nERR: wrong amount of arguments for ptr: " << res.size() << " instead of 1 at " << this->getPosition();
						breakCode();
						return {};
					}

					std::dynamic_pointer_cast<Pointer, Coin>(getCoin((*it)->getValue()))->set(res[0]);
				}
				else if (this->getValue() == "string")
					variables[currentNamespace]->putCoin((*it)->getValue());
				else if (classes.classExists(this->getValue()))
				{
					std::shared_ptr<CoinTable> newFields = classes.getClass(this->getValue())->getFields()->clone();
					variables[currentNamespace]->putCoin(
						(*it)->getValue(), classes.getClass(this->getValue()), newFields);
				}
				else if (classes.classExists(this->concatTypes()))
				{
					std::shared_ptr<CoinTable> newFields = classes.getClass(this->concatTypes())->getFields()->clone();
					variables[currentNamespace]->putCoin(
						(*it)->getValue(), classes.getClass(this->concatTypes()), newFields);
				}
				else if (this->getValue() == "void")
					variables[currentNamespace]->putCoin((*it)->getValue(), true, true);
				else
				{
					std::cout << colorText(31) << "ERR: unknown variable type '" << this->concatTypes() 
							  << "' at " << this->getPosition() << colorText() << "\n";
					breakCode();
					return {};
				}
			}
			++it;
		}

		if (this->getToken().type == TokensEnum::TEMPLATEDTYPE)
			this->setName(this->getValue() + "<" + this->getFirstChild()->concatTypes() + ">");
	}
	else if (this->getValue() == "." || this->getValue() == "->")
	{
		nodeVect out = (*it)->RPN(*it, current_size);
		output.insert(output.cend(), out.begin(), out.end());
		current_size += out.size();
		++it;

		me->setSubValue((*it)->getValue());
		output.push_back(me);
		current_size += 1;

		if (clean)
		{
			output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, std::shared_ptr<Node>()));
			current_size += 1;
		}
	}
	else 
	{
		if (this->getValue() == "{" || this->getValue() == "else")
			nextNamespace();
		else if (this->getValue() == ".")
		{
			if (checkTypes(me, true) == nullptr)
			{
				breakCode();
				return {};
			}
		}
		else if (this->getToken().type == TokensEnum::IDENTIFIER && this->getParent()->getValue() != "." &&
				 !varExists(this->getValue()) && !functions.functionExists(this->getValue()) &&
				 !classes.classExists(this->getValue()) &&
				 !(this->getValue() == "this" && !classDeclaration.empty()))
		{
			std::cout << colorText(31) << "ERR: unknown variable. " << this->token.str() << colorText() << "\n";
			breakCode();
			return {};
		}
		else if (this->getType() == TokenType::OPERATION)
		{
			if (getOpType() == "")
			{
				breakCode();
				return {};
			}
		}

		while (it != this->getChilds().end())
		{
			bool cleanIt = (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "{" ||
						   (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "else" ||
						   (*it)->getType() == TokenType::SPECIAL && (*it)->getValue() == "node" ||
						   (*it)->getType() == TokenType::PROBE ||
						   (*it)->getType() == TokenType::TYPE;

			nodeVect out = (*it)->RPN(*it, current_size, clean && cleanIt);
			output.insert(output.cend(), out.begin(), out.end());
			current_size += out.size();

			if (clean && (*it)->getType() != TokenType::SPECIAL && (*it)->getType() != TokenType::_EOF && !cleanIt)
			{
				output.push_back(std::make_shared<Node>(Token("' '"), TokenType::CLEANER, std::shared_ptr<Node>()));
				current_size += 1;
			}
			++it;
		}
		if (this->getType() != TokenType::SPECIAL)
		{
			if (this->token.type == TokensEnum::OP_UNAR_PREF &&
				(this->getValue() == "+" || this->getValue() == "-" ||
				 this->getValue() == "&" || this->getValue() == "*" ))
			{
				auto me2 = *this;
				me2.setName(this->getValue() + "1");
				output.push_back(std::make_shared<Node>(me2));
			}
			else
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
///

void Node::requestBreak(std::shared_ptr<Node> me)
{
	toBreak.push_back(me);
}
void Node::requestContinue(std::shared_ptr<Node> me)
{
	toContinue.push_back(me);
}
void Node::requestReturn(std::shared_ptr<Node> me)
{
	toReturn.push_back(me);
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
void Node::loadReturnRequests(size_t indexReturn)
{
	while (!toReturn.empty())
	{
		toReturn.back()->setJumper(indexReturn);
		toReturn.pop_back();
	}
}


void Node::nextNamespace()
{
	currentNamespace = ++maxNamespace;
	variables.insert({ currentNamespace, std::make_shared<CoinTable>() });
}
nodeVect Node::previousNamespace()
{
	nodeVect out;
	for (auto obj : variables.rbegin()->second->getTable())
		out.push_back(std::make_shared<Node>(Token(obj.first), TokenType::DESTRUCTOR, std::shared_ptr<Node>()));
	
	variables.erase(variables.rbegin()->first);
	//auto it = variables.rbegin(); ++it;
	currentNamespace = variables.rbegin()->first;
	return out;
}

void Node::addConstant(std::shared_ptr<Node> me)
{
	std::string name = "##const" + std::to_string(const_ID); 
	++const_ID;

	switch (me->getToken().type)
	{
	case TokensEnum::BOOL:
		variables[1]->putCoin(name, NumType::BOOL);
		break;
	case TokensEnum::DOUBLE:
		variables[1]->putCoin(name, NumType::DOUBLE);
		break;
	case TokensEnum::INT:
		variables[1]->putCoin(name, NumType::INT);
		break;
	case TokensEnum::STRING:
		variables[1]->putCoin(name);
		std::dynamic_pointer_cast<String, Coin>(variables[1]->getCoin(name))->set(me->getValue());
		std::dynamic_pointer_cast<String, Coin>(variables[1]->getCoin(name))->setConst(true);
		me->setName(name);
		return;
	default:
		return;
	}
	std::string data = me->getValue();

	findAndReplaceAll(data, ".", ",");
	std::dynamic_pointer_cast<Numeric, Coin>(variables[1]->getCoin(name))->set(std::stod(data));
	std::dynamic_pointer_cast<Numeric, Coin>(variables[1]->getCoin(name))->setConst(true);
	me->setName(name);
}
void Node::showVars()
{
	std::cout << "\n\nNamespaces: (size: " << variables.size() << ") ";

	for (auto& nmspce : variables)
	{
		std::cout << "\n\nNamespace[" << nmspce.first << "] Variables: (size: " << nmspce.second->getTable().size() << ") ";
		for (auto& coin : nmspce.second->getTable())
		{
			if (!coin.first.starts_with("##alloc"))
				std::cout << "\n" << coin.second->str();
		}
	}
}
void Node::showFunctions(bool methods, std::shared_ptr<FunctionTable> table)
{
	FunctionTable useForShow = (methods) ? *table : functions;

	for (auto obj : useForShow.getTable())
	{
		std::cout << "\n\n" << ((obj.second->getReturnValue()->isConst()) ? "const " : "") + obj.second->getReturnValue()->getType() << " " << obj.first << " - attributes(";
		for (auto attr : obj.second->getAttributes()->getTable())
			std::cout << " " << attr.first << ":" << attr.second->getType();
		std::cout << " )";

		RPNVect::iterator it = obj.second->getProgram()->begin();
		size_t cnt = 0;
		while (it != obj.second->getProgram()->end())
		{
			std::shared_ptr<RPN_Element> node = *it;

			std::cout << "\n~(" << cnt << "): ";
			if (node->getType() == TokenType::JMP || node->getType() == TokenType::CJM)
				std::cout << node->getValue() << "(" << node->getJumper() << ": " << obj.second->getProgram()->at(node->getJumper())->getValue() << ")";
			else if (node->getType() == TokenType::DESTRUCTOR)
				std::cout << "~" << node->getValue();
			else if (node->getType() == TokenType::TYPE)
				std::cout << node->getValue() << "( " << node->getSubValue() << " )";
			else if (node->getValue() == ".")
				std::cout << "." << node->getSubValue();
			else if (node->getValue() == "->")
				std::cout << "->" << node->getSubValue();
			else if (node->getValue() == ".(")
				std::cout << "." << node->getSubValue() << "()";
			else if (node->getValue() == "(")
				std::cout << node->getSubValue() << "()";
			else
				std::cout << node->getValue();

			++it; ++cnt;
		}
	}
}
void Node::showClasses()
{
	for (auto obj : classes.getTable())
	{
		std::cout << "\n\n# --- Class " << obj.second->getName() << ":\n\n --- Fields --- \n";
		for (auto field : obj.second->getFields()->getTable())
			std::cout << field.second->str() << "\n";
		
		std::cout << "\n --- Methods --- ";
		showFunctions(true, obj.second->getMethods());
	}
}
void Node::clearVarAllocs()
{

	for (auto& coin : variables[1]->getTable())
	{
		if (coin.first.starts_with("##alloc"))
			variables[1]->eraseCoin(coin.first);
	}
}

bool Node::varExists(std::string name)
{
	auto it = variables.rbegin();
	while (it != variables.rend())
	{
		if (it->second->coinExists(name))
			return true;
		++it;
	}
	return false;
}
std::vector<std::shared_ptr<Coin>> Node::allocate(size_t amount)
{
	std::vector<std::shared_ptr<Coin>> output = {};

	size_t attributes_left = amount;
	
	nodeVect::iterator it = this->childs.begin();
	while (it != this->childs.end() && attributes_left > 0)
	{
		std::string Temp_Name = "##alloc" + std::to_string(tmp_ID); ++tmp_ID;

		if ((*it)->getValue() == "int")
		{
			variables[1]->putCoin(Temp_Name, NumType::INT);
			output.push_back(getCoin(Temp_Name));
		}
		else if ((*it)->getValue() == "double")
		{
			variables[1]->putCoin(Temp_Name, NumType::DOUBLE);
			output.push_back(getCoin(Temp_Name));
		}
		else if ((*it)->getValue() == "bool")
		{
			variables[1]->putCoin(Temp_Name, NumType::BOOL);
			output.push_back(getCoin(Temp_Name));
		}
		else if ((*it)->getValue() == "ptr")
		{
			variables[1]->putCoin(Temp_Name, (*it)->getFirstChild()->concatTypes());
			
			output.push_back(getCoin(Temp_Name));
			auto res = (*it)->getFirstChild()->allocate(1);
			
			if (!StateOK)
				return {};

			std::dynamic_pointer_cast<Pointer, Coin>(getCoin(Temp_Name))->set(res[0]);
		}
		else if ((*it)->getValue() == "string")
		{
			variables[1]->putCoin(Temp_Name);
			output.push_back(getCoin(Temp_Name));
		}
		else if (classes.classExists((*it)->getValue()))
		{
			CoinTable newFields = *classes.getClass((*it)->getValue())->getFields();
			variables[1]->putCoin(
				Temp_Name, classes.getClass((*it)->getValue()), std::make_shared<CoinTable>(newFields));
			output.push_back(getCoin(Temp_Name));
		}
		else if (classes.classExists((*it)->concatTypes()))
		{
			CoinTable newFields = *classes.getClass((*it)->concatTypes())->getFields();
			variables[1]->putCoin(
				Temp_Name, classes.getClass((*it)->concatTypes()), std::make_shared<CoinTable>(newFields));
			output.push_back(getCoin(Temp_Name));
		}
		else if (!classDeclaration.empty() && (*it)->getValue() == classDeclaration.top())
		{
			//std::cout << "WARN: not full class type\n";
			
			variables[1]->putDelayedObjectCoin(Temp_Name, classDeclaration.top());
			output.push_back(getCoin(Temp_Name));
		}
		else if (!classDeclaration.empty() && (*it)->concatTypes() == classDeclaration.top())
		{
			//std::cout << "WARN: not full class type\n";
			
			variables[1]->putDelayedObjectCoin(Temp_Name, classDeclaration.top());
			output.push_back(getCoin(Temp_Name));
		}
		else if (templated_classes.contains((*it)->getValue()))
		{
			replace_with.push((*it)->getFirstChild()->getFirstChild());

			std::shared_ptr<Node> root = this->parent.lock();
			while (root->parent.lock() != nullptr)
				root = root->parent.lock();

			std::shared_ptr<Node> class_create = root->childs.at(templated_classes.at((*it)->getValue()))->clone(root);
			class_create->getFirstChild()->setName((*it)->concatTypes());

			replace_what.push(class_create->childs.at(1)->getFirstChild()->getValue());

			class_create->getLastChild()->replace_template();

			functionDeclaration.push("");
			class_create->RPN(class_create, 0);
			functionDeclaration.pop();

			replace_what.pop();
			replace_with.pop();

			CoinTable newFields = *classes.getClass((*it)->concatTypes())->getFields();

			variables[1]->putCoin(
				Temp_Name, classes.getClass((*it)->concatTypes()), std::make_shared<CoinTable>(newFields));
			output.push_back(getCoin(Temp_Name));
		}
		else
		{
			std::cout << "\nERR: can't allocate! " << (*it)->concatTypes();
		}
		++it; --attributes_left;
	}

	if (!(it == this->getChilds().end() && attributes_left == 0))
	{
		std::cout << colorText(31) << "\nWrong amount of arguments, expected: " << amount << ", provided: " << this->childs.size()
			<< " at " << this->getPosition() << colorText();
		breakCode();
		return {};
	}
	else
		return output;
}
std::shared_ptr<Coin> Node::getCoin(std::string name)
{
	auto it = variables.rbegin();
	while (it != variables.rend())
	{
		if (it->second->coinExists(name))
			return it->second->getTable().at(name);
		++it;
	}
	return std::shared_ptr<Coin>();
}
std::shared_ptr<Function> Node::getMethodFromDot(std::shared_ptr<Node> node)
{
	std::shared_ptr<Coin> from = checkTypes(node->getFirstChild(), true);

	if (from == nullptr)
		return nullptr;

	if (!std::dynamic_pointer_cast<Object, Coin>(from)->getMethods()->functionExists(node->getLastChild()->getValue()))
	{
		std::cout << colorText(31) << "\nUnknown member" << node->getLastChild()->getToken().str() << colorText();
		return nullptr;
	}
	return std::dynamic_pointer_cast<Object, Coin>(from)->getMethods()->getFunction(node->getLastChild()->getValue());
}


std::string Node::getOpType()
{
	if (this->getType() == TokenType::IDENTIFIER)
	{
		if (this->getValue() == "this")
		{
			if (last_THIS.top() == nullptr)
			{
				std::cout << colorText(31) << "ERR: 'this' must be inside member function. " << this->token.str() << colorText() << "\n";
				breakCode();
				return "";
			}
			else
				return last_THIS.top()->getType();
		}
		else if (varExists(this->getValue()))
			return getCoin(this->getValue())->getType();
		else
			return "";
	}
	else if (this->getType() == TokenType::CONSTANT)
	{
		switch (this->getToken().type)
		{
		case TokensEnum::BOOL:		return "bool";
		case TokensEnum::DOUBLE:	return "double";
		case TokensEnum::INT:		return "int";
		case TokensEnum::STRING:	return "string";
		default:					return "";
		}
	}
	else if (this->getValue() == "." || this->getValue() == "->")
	{
		auto buf = checkTypes(this->getFirstChild()->getParent(), true);

		if (buf != nullptr)
			return buf->getType();
		else
			return "";
	}
	else if (this->getValue() == "++<" || this->getValue() == "--<")
	{
		if (this->getFirstChild()->getToken().type == TokensEnum::IDENTIFIER &&
			checkTypes(this->getFirstChild(), true, "variable")->isConst())
		{
			std::cout << colorText(31) << "ERR: can't modify constant with " << this->getToken().str() << colorText() << "\n";
			breakCode();
			return "";
		}

		if (this->getFirstChild()->getToken().type == TokensEnum::OP_UNAR_PREF ||
			this->getFirstChild()->getType() == TokenType::IDENTIFIER)
		{
			std::string operand = this->getFirstChild()->getOpType();
			if (operand == "double" || operand == "int")
				return operand;
			else
			{
				std::cout << colorText(31) << "ERR: variable (" << operand << ") isn't iteratable after " << this->getToken().str() << colorText() << "\n";
				breakCode();
				return "";
			}
		}
		else
		{
			std::cout << colorText(31) << "ERR: can't modify constant with " << this->getToken().str() << colorText() << "\n";
			breakCode();
			return "";
		}
	}
	else if (this->getValue() == ">++" || this->getValue() == ">--")
	{
		if (this->getFirstChild()->getToken().type == TokensEnum::IDENTIFIER &&
			checkTypes(this->getFirstChild(), true, "variable")->isConst())
		{
			std::cout << colorText(31) << "ERR: can't modify constant with " << this->getToken().str() << colorText() << "\n";
			breakCode();
			return "";
		}

		if (this->getFirstChild()->getToken().type == TokensEnum::IDENTIFIER ||
			this->getFirstChild()->getToken().type == TokensEnum::OP_UNAR_PREF)
		{
			std::string operand = this->getFirstChild()->getOpType();
			if (operand == "double" || operand == "int")
				return operand;
			else
			{
				std::cout << colorText(31) << "ERR: variable (" << operand << ") isn't iteratable, before " << this->getToken().str() << colorText() << "\n";
				breakCode();
				return "";
			}
		}
		else
		{
			std::cout << colorText(31) << "ERR: can't modify constant with " << this->getToken().str() << colorText() << "\n";
			breakCode();
			return "";
		}
	}
	else if (this->token.type != TokensEnum::OP_BINAR && (this->getValue() == "+" || this->getValue() == "-"))
	{
		std::string operand = this->getFirstChild()->getOpType();
		if (operand == "bool" || operand == "int" || operand == "double")
			return operand;
		else
		{
			std::cout << colorText(31) << "ERR: can't perform operation - '" << this->getValue() << "' for " << operand << " at "
				<< this->token.str() << colorText() << "\n";
			breakCode();
			return "";
		}
	}
	else if (this->token.type == TokensEnum::OP_UNAR_PREF && this->getValue() == "*")
	{
		std::string operand = this->getFirstChild()->getOpType();
		
		if (operand.starts_with("ptr<"))
			return operand.substr(4, operand.size() - 5);
		else
		{
			std::cout << colorText(31) << "ERR: variable (" << operand << ") isn't pointer after " << this->getToken().str() << colorText() << "\n";
			breakCode();
			return "";
		}
	}
	else if (this->getValue() == "!")
	{
		std::string operand = this->getFirstChild()->getOpType();
		if (operand == "bool" || operand == "int")
			return operand;
		else
		{
			std::cout << colorText(31) << "ERR: variable (" << operand << ") isn't boolean after " << this->getToken().str() << colorText() << "\n";
			breakCode();
			return "";
		}
	}
	else if (this->token.type == TokensEnum::OP_UNAR_PREF && this->getValue() == "&")
	{
		std::string operand = this->getFirstChild()->getOpType();
		return "ptr<" + operand + ">";
	}
	else if (this->getValue() == "(")
	{
		auto operand = checkTypes(this->getFirstChild(), true, "function");
		if (operand == nullptr)
			return "";

		return operand->getType();
	}
	else if (this->getType() == TokenType::OPERATION && this->getRang() == 15)
	{
		std::shared_ptr<Coin> res = checkTypes(this->getFirstChild(), true);
		if (res == nullptr || res->isConst())
		{
			std::cout << colorText(31) << "ERR: expression must be l_value '" << this->getFirstChild()->getValue()
				<< "' at " << this->getPosition() << colorText() << "\n";
			breakCode();
			return "";
		}
		if (this->getValue() == "=")
		{
			std::string lval = this->getFirstChild()->getOpType();
			std::string rval = this->getLastChild()->getOpType();
			if (lval == rval || areNumerics(lval, rval))
				return rval;
			else
			{
				std::cout << colorText(31) << "ERR: can't assign '" << rval << "' to '" << lval << "' at " << this->getPosition() << colorText() << "\n";
				breakCode();
				return "";
			}
		}
		else
		{
			Node newNode = *this;
			newNode.setName(newNode.getValue().substr(0, newNode.getValue().size() - 1));
			std::string outType = newNode.getOpType();

			return outType;
		}
	}

	std::string operand_1 = this->getFirstChild()->getOpType();
	std::string buf_operand_1 = operand_1;
	if (operand_1 == "")
	{
		if (this->getFirstChild()->getType() == TokenType::IDENTIFIER)
			std::cout << colorText(31) << "ERR: unknown variable. " << this->getFirstChild()->getToken().str() << colorText() << "\n";
		breakCode();
		return "";
	}
	if (operand_1.starts_with("ptr<"))
		operand_1 = "ptr";

	std::string op;
	if (BinarOperations.contains(operand_1))
	{
		if (BinarOperations[operand_1].contains(this->getValue()))
			op = this->getValue();
		else if (BinarOperations[operand_1].contains(convertOpToType(this->getRang())))
			op = convertOpToType(this->getRang());
		else
		{
			std::cout << colorText(31) << "ERR: can't perform operation - '" << this->getValue() << "' for " << buf_operand_1 << " at "
				<< this->token.str() << colorText() << "\n";
			breakCode();
			return "";
		}
	}
	else
	{
		std::cout << colorText(31) << "ERR: wrong type of first operand - " << buf_operand_1 << ". for " << this->token.str() << colorText() << "\n";
		breakCode();
		return "";
	}

	std::string operand_2 = this->getLastChild()->getOpType();
	std::string buf_operand_2 = operand_2;
	if (operand_2 == "")
	{
		if (this->getLastChild()->getType() == TokenType::IDENTIFIER) 
			std::cout << colorText(31) << "ERR: unknown variable. " << this->getLastChild()->getToken().str() << colorText() << "\n";
		breakCode();
		return "";
	}
	if (operand_2.starts_with("ptr<"))
		operand_2 = "ptr";

	if (BinarOperations[operand_1][op].contains(operand_2))
		return BinarOperations[operand_1][op][operand_2];
	else
	{
		std::cout << colorText(31) << "ERR: wrong type of second operand - " << buf_operand_2 << ". for " << this->token.str() << colorText() << "\n";
		breakCode();
		return "";
	}
}
bool Node::areNumerics(std::string a, std::string b)
{
	if (a == "bool" || a == "int" || a == "double")
		if (b == "bool" || b == "int" || b == "double")
			return true;
	return false;
}
std::shared_ptr<Coin> Node::checkTypes(std::shared_ptr<Node> node, bool throwError, std::string expectedType)
{
	if (node->getValue() == "(")
	{	
		return checkTypes(node->getFirstChild(), throwError, "function");
	}
	else if (node->getValue() == "*" && node->token.type == TokensEnum::OP_UNAR_PREF)
	{
		std::shared_ptr<Coin> obj = checkTypes(node->getFirstChild(), throwError);
		if (obj == nullptr)
			return nullptr;

		if (obj->getCoinType() == CoinType::POINTER)
			return std::dynamic_pointer_cast<Pointer, Coin>(obj)->get();
		else
		{
			std::cout << colorText(31) << "ERR: variable isn't pointer at " << node->getPosition() << colorText() << "\n";
			breakCode();
			return {};
		}
	}
	else if (node->getValue() == "&" && node->token.type == TokensEnum::OP_UNAR_PREF)
	{
		std::shared_ptr<Coin> obj = checkTypes(node->getFirstChild(), throwError);
		if (obj == nullptr)
			return nullptr;

		std::string Temp_Name = "##alloc" + std::to_string(tmp_ID); ++tmp_ID;
		Pointer newPtr = Pointer(Temp_Name, obj->getType(), true);
		newPtr.set(obj);

		variables[1]->putCoin(std::make_shared<Pointer>(newPtr));

		return getCoin(Temp_Name);
	}
	else if (node->getValue() == "++<" || node->getValue() == "--<")
	{
		return checkTypes(node->getFirstChild(), throwError);
	}
	else if (node->getValue() == ".")
	{
		std::shared_ptr<Coin> obj = checkTypes(node->getFirstChild(), throwError);

		if (obj == nullptr)
			return nullptr;

		std::shared_ptr<Node> previousObj;
		if (node->getFirstChild()->getValue() == "." ||
			node->getFirstChild()->getValue() == "->")
			previousObj = node->getFirstChild()->getLastChild();
		else if (node->getFirstChild()->getValue() == "(")
		{
			if (node->getFirstChild()->getFirstChild()->getValue() == "." ||
				node->getFirstChild()->getFirstChild()->getValue() == "->")
				previousObj = node->getFirstChild()->getFirstChild()->getLastChild();
			else
				previousObj = node->getFirstChild()->getFirstChild();
		}
		else
			previousObj = node->getFirstChild();

		if (obj->getCoinType() != CoinType::OBJECT)
		{
			if (throwError)
				std::cout << colorText(31) << "\nVariable:" << previousObj->getToken().str() + " - isn't an Object!" << colorText();
			return nullptr;
		}

		// --- //

		std::shared_ptr<Object> object = std::dynamic_pointer_cast<Object, Coin>(obj);

		if (expectedType == "var")
		{
			if (object->getDelayedClass() != "")
			{
				if (classes.getClass(object->getDelayedClass())->getFields()->coinExists(node->getLastChild()->getValue()))
				{
					return classes.getClass(object->getDelayedClass())->getFields()->getCoin(node->getLastChild()->getValue());
				}
				else
				{
					if (throwError)
						std::cout << colorText(31) << "\nUnknown field: "
						<< node->getLastChild()->getToken().str() << " - of '" << node->getLastChild()->getValue() << "'" << colorText();
					return nullptr;
				}
			}
			else if (object->getFields()->coinExists(node->getLastChild()->getValue()))
			{
				return object->getFields()->getCoin(node->getLastChild()->getValue());
			}
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown field: "
					<< node->getLastChild()->getToken().str() << " - of '" << node->getLastChild()->getValue() << "'" << colorText();
				return nullptr;
			}
		}
		else if (expectedType == "function")
		{
			if (object->getDelayedClass() != "")
			{
				if (classes.getClass(object->getDelayedClass())->getMethods()->functionExists(node->getLastChild()->getValue()))
				{
					return classes.getClass(object->getDelayedClass())->getMethods()->getFunction(node->getLastChild()->getValue())->getReturnValue();
				}
				else
				{
					if (throwError)
						std::cout << colorText(31) << "\nUnknown field: "
						<< node->getLastChild()->getToken().str() << " - of '" << node->getLastChild()->getValue() << "'" << colorText();
					return nullptr;
				}
			}
			else if (object->getMethods()->functionExists(node->getLastChild()->getValue()))
				return object->getMethods()->getFunction(node->getLastChild()->getValue())->getReturnValue();
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown method: "
					<< node->getLastChild()->getToken().str() << " - of '" << previousObj->getValue() << "'" << colorText();
				return nullptr;
			}
		}
		else if (object->getDelayedClass() != "")
		{
			if (classes.getClass(object->getDelayedClass())->getFields()->coinExists(node->getLastChild()->getValue()))
			{
				return classes.getClass(object->getDelayedClass())->getFields()->getCoin(node->getLastChild()->getValue());
			}
			else if (classes.getClass(object->getDelayedClass())->getMethods()->functionExists(node->getLastChild()->getValue()))
			{
				return classes.getClass(object->getDelayedClass())->getMethods()->getFunction(node->getLastChild()->getValue())->getReturnValue();
			}
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown member '" << node->getLastChild()->getToken().str() << "'" << colorText();
				return nullptr;
			}
		}
		else
		{
			if (object->getFields()->coinExists(node->getLastChild()->getValue()))
			{
				return object->getFields()->getCoin(node->getLastChild()->getValue());
			}
			else if (object->getMethods()->functionExists(node->getLastChild()->getValue()))
			{
				std::shared_ptr<Function> func = object->getMethods()->getFunction(node->getLastChild()->getValue());
				auto coin(func->getReturnValue());
				return coin;
			}
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown member '" << node->getLastChild()->getToken().str() << "'" << colorText();
				return nullptr;
			}
		}
	}
	else if (node->getValue() == "->")
	{
		std::shared_ptr<Coin> obj;
		std::shared_ptr<Coin> obj2 = checkTypes(node->getFirstChild(), throwError);
		if (obj2 == nullptr)
			return nullptr;

		if (obj2->getCoinType() == CoinType::POINTER)
			obj = std::dynamic_pointer_cast<Pointer, Coin>(obj2)->get();
		else
		{
			std::cout << colorText(31) << "ERR: variable isn't pointer at " << node->getPosition() << colorText() << "\n";
			breakCode();
			return nullptr;
		}

		if (obj == nullptr)
			return nullptr;

		std::shared_ptr<Node> previousObj;
		if (node->getFirstChild()->getValue() == "." || 
			node->getFirstChild()->getValue() == "->")
			previousObj = node->getFirstChild()->getLastChild();
		else if (node->getFirstChild()->getValue() == "(")
		{
			if (node->getFirstChild()->getFirstChild()->getValue() == "." || 
				node->getFirstChild()->getFirstChild()->getValue() == "->")
				previousObj = node->getFirstChild()->getFirstChild()->getLastChild();
			else
				previousObj = node->getFirstChild()->getFirstChild();
		}
		else
			previousObj = node->getFirstChild();

		if (obj->getCoinType() != CoinType::OBJECT)
		{
			if (throwError)
				std::cout << colorText(31) << "\nVariable:" << previousObj->getToken().str() + " - isn't an Object!" << colorText();
			return nullptr;
		}

		// --- //

		std::shared_ptr<Object> object = std::dynamic_pointer_cast<Object, Coin>(obj);

		if (expectedType == "var")
		{
			if (object->getDelayedClass() != "")
			{
				if (classes.getClass(object->getDelayedClass())->getFields()->coinExists(node->getLastChild()->getValue()))
				{
					return classes.getClass(object->getDelayedClass())->getFields()->getCoin(node->getLastChild()->getValue());
				}
				else
				{
					if (throwError)
						std::cout << colorText(31) << "\nUnknown field: "
						<< node->getLastChild()->getToken().str() << " - of '" << node->getLastChild()->getValue() << "'" << colorText();
					return nullptr;
				}
			}
			else if (object->getFields()->coinExists(node->getLastChild()->getValue()))
			{
				return object->getFields()->getCoin(node->getLastChild()->getValue());
			}
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown field: "
					<< node->getLastChild()->getToken().str() << " - of '" << node->getLastChild()->getValue() << "'" << colorText();
				return nullptr;
			}
		}
		else if (expectedType == "function")
		{
			if (object->getDelayedClass() != "")
			{
				if (classes.getClass(object->getDelayedClass())->getMethods()->functionExists(node->getLastChild()->getValue()))
				{
					return classes.getClass(object->getDelayedClass())->getMethods()->getFunction(node->getLastChild()->getValue())->getReturnValue();
				}
				else
				{
					if (throwError)
						std::cout << colorText(31) << "\nUnknown field: "
						<< node->getLastChild()->getToken().str() << " - of '" << node->getLastChild()->getValue() << "'" << colorText();
					return nullptr;
				}
			}
			else if (object->getMethods()->functionExists(node->getLastChild()->getValue()))
				return object->getMethods()->getFunction(node->getLastChild()->getValue())->getReturnValue();
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown method: "
					<< node->getLastChild()->getToken().str() << " - of '" << previousObj->getValue() << "'" << colorText();
				return nullptr;
			}
		}
		else if (object->getDelayedClass() != "")
		{
			if (classes.getClass(object->getDelayedClass())->getFields()->coinExists(node->getLastChild()->getValue()))
			{
				return classes.getClass(object->getDelayedClass())->getFields()->getCoin(node->getLastChild()->getValue());
			}
			else if (classes.getClass(object->getDelayedClass())->getMethods()->functionExists(node->getLastChild()->getValue()))
			{
				return classes.getClass(object->getDelayedClass())->getMethods()->getFunction(node->getLastChild()->getValue())->getReturnValue();
			}
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown member '" << node->getLastChild()->getToken().str() << "'" << colorText();
				return nullptr;
			}
		}
		else
		{
			if (object->getFields()->coinExists(node->getLastChild()->getValue()))
			{
				return object->getFields()->getCoin(node->getLastChild()->getValue());
			}
			else if (object->getMethods()->functionExists(node->getLastChild()->getValue()))
			{
				std::shared_ptr<Function> func = object->getMethods()->getFunction(node->getLastChild()->getValue());
				auto coin(func->getReturnValue());
				return coin;
			}
			else
			{
				if (throwError)
					std::cout << colorText(31) << "\nUnknown member '" << node->getLastChild()->getToken().str() << "'" << colorText();
				return nullptr;
			}
		}
	}
	else if (node->getRang() == 15)
		return checkTypes(node->getFirstChild(), throwError);
	else if (node->getType() == TokenType::OPERATION)
	{
		std::string type = node->getOpType();
		std::string Temp_Name = "##alloc" + std::to_string(tmp_ID); ++tmp_ID;

		if (type == "int")
			variables[1]->putCoin(Temp_Name, NumType::INT);
		else if (type == "double")
			variables[1]->putCoin(Temp_Name, NumType::DOUBLE);
		else if (type == "bool")
			variables[1]->putCoin(Temp_Name, NumType::BOOL);
		else if (type == "string")
			variables[1]->putCoin(Temp_Name);
		return getCoin(Temp_Name);
	}
	
	if (expectedType == "var")
	{
		if (this->getValue() == "this")
		{
			if (last_THIS.top() == nullptr)
			{
				std::cout << colorText(31) << "ERR: 'this' must be inside member function. " << this->token.str() << colorText() << "\n";
				breakCode();
				return {};
			}
			else
				return last_THIS.top();
		}
		else if (varExists(node->getValue()))
			return getCoin(node->getValue());
		else
		{
			if (throwError)
				std::cout << colorText(31) << "\nUnknown variable:" << node->getToken().str() << colorText();
			return nullptr;
		}
	}
	else if (expectedType == "function")
	{
		if (functions.getTable().contains(node->getValue()))
			return functions.getFunction(node->getValue())->getReturnValue();
		else
		{
			if (throwError)
				std::cout << colorText(31) << "Unknown function: " << node->getToken().str() << colorText() << "\n";
			return nullptr;
		}
	}
	else
	{
		if (this->getValue() == "this")
		{
			if (last_THIS.top() == nullptr)
			{
				std::cout << colorText(31) << "ERR: 'this' must be inside member function. " << this->token.str() << colorText() << "\n";
				breakCode();
				return {};
			}
			else
				return last_THIS.top();
		}
		else if (varExists(node->getValue()))
			return getCoin(node->getValue());
		else if (functions.getTable().contains(node->getValue()))
			return getCoin(node->getValue());
		else
		{
			if (throwError)
				std::cout << colorText(31) << "\nUnknown variable: " + node->getToken().str() << colorText();
			return nullptr;
		}
	}
}
bool Node::functionAttributesCheck(std::shared_ptr<Node> node)
{
	nodeVect attributes = node->getChilds();
	nodeVect::iterator attribute = attributes.begin();
	std::shared_ptr<Function> func;
	if ((*attribute)->getValue() == ".")
		func = getMethodFromDot(*attribute);
	else
		func = functions.getFunction((*attribute)->getValue());
	++attribute;

	std::vector<std::string> attrOrder = func->getAttributesOrder();
	std::vector<std::string> ::iterator order = attrOrder.begin();
	
	while (order != attrOrder.end() && attribute != attributes.end())
	{
		std::string result;

		if ((*attribute)->getType() == TokenType::CONSTANT)
		{
			switch ((*attribute)->getToken().type)
			{
			case TokensEnum::BOOL:		result = "bool";	break;
			case TokensEnum::DOUBLE:	result = "double";	break;
			case TokensEnum::INT:		result = "int";		break;
			case TokensEnum::STRING:	result = "string";	break;
			default:					result = "";		break;
			}
		}
		else
			result = checkTypes(*attribute)->getType();

		if (*order != result && !areNumerics(*order, result))
		{
			std::cout << colorText(31) << "\nWrong attribute type '" << result << "' instead of '" << *order << "'. Argument "
					  << (order - attrOrder.begin() + 1) << " of " << func->getName() << "() at ("
					  << node->getFirstChild()->getToken().line << "," << node->getFirstChild()->getToken().position << ") " << colorText();
			return false;
		}
		++order; ++attribute;
	}
	if (order != attrOrder.end() || attribute != attributes.end())
	{
		std::cout << colorText(31) << "\nWrong amount of arguments, expected: " << attrOrder.size() << ", provided: " << (attributes.size() - 1) 
				  << " to call of " << func->getName() << "() at " << node->getFirstChild()->getPosition() << colorText();
		return false;
	}
	return true;
}

std::string Node::concatTypes()
{
	if (!replace_what.empty() && this->getValue() == replace_what.top())
		return replace_with.top()->concatTypes();
	else if (this->getChilds().empty())
		return getValue();
	else if (this->getValue() != "<")
		return getValue() + "<" + this->getFirstChild()->concatTypes() + ">";

	nodeVect::iterator it = childs.begin();
	std::string buf = (*it)->concatTypes();

	++it;
	while (it != childs.end())
	{
		buf += ", " + (*it)->concatTypes();
		++it;
	}

	return buf;
}

std::shared_ptr<Node> Node::clone(std::shared_ptr<Node> parent)
{
	std::shared_ptr<Node> res = std::make_shared<Node>(token, type, parent, jumper);
	res->setSubValue(sub_value);

	nodeVect::iterator it = childs.begin();
	while (it != childs.end())
	{
		res->addChild((*it)->clone(res));
		++it;
	}

	return res;
}
void Node::replace_template()
{
	nodeVect::iterator it = childs.begin();
	while (it != childs.end())
	{
		if (!replace_what.empty() && (*it)->getValue() == replace_what.top())
		{
			
			std::shared_ptr<Node> replaced = replace_with.top()->clone((*it)->getParent());

			nodeVect::iterator it2 = (*it)->childs.begin();
			while (it2 != (*it)->childs.end())
			{
				replaced->addChild(*it2);
				++it2;
			}
			*it = replaced;
		}
		else
			(*it)->replace_template();
		++it;
	}
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
