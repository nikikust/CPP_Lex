#include <fstream>
#include <iostream>
#include <cmath>

#include "Interpreter.h"

constexpr bool _output = 0;
constexpr bool ___debug = 0;


std::string operator*(std::string a, int b)
{
	std::string output = "";
	while (--b >= 0)
		output += a;
	return output;
}
std::string operator+(std::string a, int b)
{
	return a + std::to_string(b);
}
std::string operator+(std::string a, bool b)
{
	return a + std::to_string(b);
}
std::string operator+(std::string a, double b)
{
	return a + std::to_string(b);
}


Interpreter::Interpreter()
{
	std::ifstream fin("prog.json", std::ios::in | std::ios::binary);
	
	std::vector<uint8_t> v_ubjson((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
	fin.close();

	try
	{
		json ALL;
		ALL = json::from_ubjson(v_ubjson);

		ALL.at("Classes").get_to(classes);
		if (_output)
			std::cout << "Classes Loaded successfuly!\n";

		ALL.at("Functions").get_to(functions);
		if (_output)
			std::cout << "Functions Loaded successfuly!\n";

		ALL.at("Variables").get_to(const_table);
		if (_output)
			std::cout << "Variables Loaded successfuly!\n";

		ALL.at("RPN").get_to(program);
		if (_output)
			std::cout << "RPN Loaded successfuly!\n";
		
		std::cout << "\nJSON Loaded successfuly!\n";

		if (!load_links())
		{
			std::cout << "\nCan't load links!\n";
			return;
		}
		else if (_output)
			std::cout << "\nLinks loaded successfuly!\n";

	}
	catch (const std::exception& err)
	{
		std::cout << err.what();
	}
}
bool Interpreter::load_links()
{
	// --- Classes --- //
	for (auto& obj : classes.getTable())
	{
		obj.second->fields = std::make_shared<CoinTable>();
		for (RPNVect::iterator it = obj.second->fieldsInit->begin(); it != obj.second->fieldsInit->end(); ++it)
		{
			std::string name = (*it)->getSubValue();
			std::string type = (*it)->getValue();

			if (type == "int")
				obj.second->fields->putCoin(name, NumType::INT);
			else if (type == "double")
				obj.second->fields->putCoin(name, NumType::DOUBLE);
			else if (type == "bool")
				obj.second->fields->putCoin(name, NumType::BOOL);
			else if (type.starts_with("ptr<"))
				obj.second->fields->putCoin(name, type.substr(4, type.size() - 5));
			else if (type == "string")
				obj.second->fields->putCoin(name);
			else if (classes.classExists(type))
			{
				std::shared_ptr<CoinTable> newFields = classes.getClass(type)->getFields()->clone();
				obj.second->fields->putCoin(name, classes.getClass(type), newFields);
			}
			else
			{
				std::cout << "ERR: 003a. error-type '" << type << "\n";
				exit(30);
			}
		}

		for (auto& func : obj.second->methods->getTable())
		{
			func.second->attributes = std::make_shared<CoinTable>();
			for (RPNVect::iterator it = func.second->attributesInit->begin(); it != func.second->attributesInit->end(); ++it)
			{
				std::string name = (*it)->getSubValue();
				std::string type = (*it)->getValue();

				if (type == "int")
					func.second->attributes->putCoin(name, NumType::INT);
				else if (type == "double")
					func.second->attributes->putCoin(name, NumType::DOUBLE);
				else if (type == "bool")
					func.second->attributes->putCoin(name, NumType::BOOL);
				else if (type.starts_with("ptr<"))
					func.second->attributes->putCoin(name, type.substr(4, type.size() - 5));
				else if (type == "string")
					func.second->attributes->putCoin(name);
				else if(classes.classExists(type))
				{
					std::shared_ptr<CoinTable> newFields = classes.getClass(type)->getFields()->clone();
					func.second->attributes->putCoin(name, classes.getClass(type), newFields);
				}
				else
				{
					std::cout << "ERR: 003b. error-type '" << type << "\n";
					exit(31);
				}
			}
		}
	}

	// --- Functions --- //
	for (auto& func : functions.getTable())
	{
		func.second->attributes = std::make_shared<CoinTable>();
		for (RPNVect::iterator it = func.second->attributesInit->begin(); it != func.second->attributesInit->end(); ++it)
		{
			std::string name = (*it)->getSubValue();
			std::string type = (*it)->getValue();

			if (type == "int")
				func.second->attributes->putCoin(name, NumType::INT);
			else if (type == "double")
				func.second->attributes->putCoin(name, NumType::DOUBLE);
			else if (type == "bool")
				func.second->attributes->putCoin(name, NumType::BOOL);
			else if (type.starts_with("ptr<"))
				func.second->attributes->putCoin(name, type.substr(4, type.size() - 5));
			else if (type == "string")
				func.second->attributes->putCoin(name);
			else if (classes.classExists(type))
			{
				std::shared_ptr<CoinTable> newFields = classes.getClass(type)->getFields()->clone();
				func.second->attributes->putCoin(name, classes.getClass(type), newFields);
			}
			else
			{
				std::cout << "ERR: 003c. error-type '" << type << "\n";
				exit(32);
			}
		}
	}

	return 1;
}


void Interpreter::PUT(std::string name)
{
	Stacks[stackDepth]->push(getCoin(name));
}
void Interpreter::PUT(std::shared_ptr<Coin> coin)
{
	Stacks[stackDepth]->push(coin);
}

std::shared_ptr<Coin> Interpreter::TOP()
{
	return Stacks[stackDepth]->top();
}
std::shared_ptr<Coin> Interpreter::POP()
{
	std::shared_ptr<Coin> buf = Stacks[stackDepth]->top();
	Stacks[stackDepth]->pop();

	if (buf == nullptr)
	{
		std::cout << "ERR: 004a. [POP] Value on stack is nullptr\n";
		exit(40);
	}

	if (buf->getName().starts_with("##local_const"))
		eraseCoin(buf->getName());
	return buf;
}

std::shared_ptr<Coin> Interpreter::turnToConst(std::shared_ptr<Coin> coin)
{
	std::string name = "##local_const" + std::to_string(local_const_ID);
	++local_const_ID;

	switch (coin->getCoinType())
	{
	case CoinType::NUMERIC:
	{
		Numeric buf = *std::dynamic_pointer_cast<Numeric, Coin>(coin);
		buf.setName(name); buf.setConst(true);
		putCoin(std::make_shared<Numeric>(buf));
	}
		break;
	case CoinType::STRING:
	{
		String buf = *std::dynamic_pointer_cast<String, Coin>(coin);
		buf.setName(name); buf.setConst(true);
		putCoin(std::make_shared<String>(buf));
	}
		break;
	case CoinType::POINTER:
	{
		Pointer buf = *std::dynamic_pointer_cast<Pointer, Coin>(coin);
		buf.setName(name); buf.setConst(true);
		putCoin(std::make_shared<Pointer>(buf));
	}
		break;
	case CoinType::OBJECT:
	{
		Object buf = *std::dynamic_pointer_cast<Object, Coin>(coin);
		buf.setName(name); buf.setConst(true);
		putCoin(std::make_shared<Object>(buf));
	}
		break;
	}
	return getCoin(name);
}
std::shared_ptr<Coin> Interpreter::newPointer(std::shared_ptr<Coin> coin)
{
	std::string name = "##local_const" + std::to_string(local_const_ID);
	++local_const_ID;

	Pointer newPtr = Pointer(name, coin->getType(), true);
	newPtr.set(coin);
	putCoin(std::make_shared<Pointer>(newPtr));

	return getCoin(name);
}
NumType Interpreter::getNumericReturnType(NumType a, NumType b)
{
	if (a == NumType::DOUBLE || b == NumType::DOUBLE)
		return NumType::DOUBLE;
	else if (a == NumType::INT || b == NumType::INT)
		return NumType::INT;
	else
		return NumType::BOOL;
}
bool Interpreter::isComplexAssign(std::string op)
{
	return
		op == "*="  ||
		op == "/="  ||
		op == "%="  ||
		op == "+="  ||
		op == "-="  ||
		op == "&="  ||
		op == "^="  ||
		op == "|="  ||
		op == "<<=" ||
		op == ">>=";
}

std::shared_ptr<Coin> Interpreter::getCoin(std::string name)
{
	if (name.starts_with("##const"))
		return const_table.getCoin(name);
	else
		return variables[stackDepth]->getCoin(name);
}
void Interpreter::putCoin(std::shared_ptr<Coin> coin)
{
	variables[stackDepth]->putCoin(coin);
}
void Interpreter::eraseCoin(std::string name)
{
	variables[stackDepth]->eraseCoin(name);
}

void Interpreter::newScope()
{
	++stackDepth;
	variables[stackDepth] = std::make_shared<CoinTable>();
	Stacks[stackDepth] = std::make_shared<std::stack<std::shared_ptr<Coin>>>();
}
void Interpreter::previousScope()
{
	if (___debug)
	{
		std::cout << "\n[DEBUG] Variables:";
		for (auto& obj : variables.rbegin()->second->getTable())
			std::cout << " " << obj.first;
	}

	variables.erase(variables.rbegin()->first);

	if (___debug)
		std::cout << "\n[DEBUG] Stack size before cleaning: " << Stacks.rbegin()->second->size() << "\n";

	Stacks.erase(Stacks.rbegin()->first);
	--stackDepth;
}

//
void Interpreter::run()
{
	std::cout << "-----\n\n";
	newScope();
	run(program);
}
void Interpreter::run(RPNVect& prog)
{
	std::shared_ptr<Coin> value_to_return = std::make_shared<Void>("void");

	RPNVect::iterator beg = prog.begin();
	IC.push(prog.begin());

	while (true)
	{
		std::shared_ptr<RPN_Element> element = *IC.top();

		switch (element->getType())
		{
		case TokenType::_EOF:
			previousScope();
			PUT(value_to_return);
			IC.pop();
			return;
		case TokenType::IDENTIFIER:
			PUT(element->getValue());
			break;
		case TokenType::CONSTANT:
			PUT(element->getValue());
			break;
		case TokenType::OPERATION:
		{
			std::string val = element->getValue();
			Interpreter::OP_ENUM op;
			if (isComplexAssign(val))
			{
				std::shared_ptr<Coin> what = POP();
				std::shared_ptr<Coin> to = POP();

				PUT(to);	// a 2 += 
				PUT(to);	//    V
				PUT(what);	// a a 2 + =

				op = operations.at(val.substr(0, val.size() - 1));
			}
			else
				op = operations.at(val);

			switch (op)
			{
			case Interpreter::OP_ENUM::OP_post_incr:
				op_post_incr();
				break;
			case Interpreter::OP_ENUM::OP_pref_incr:
				op_pref_incr();
				break;
			case Interpreter::OP_ENUM::OP_post_decr:
				op_post_decr();
				break;
			case Interpreter::OP_ENUM::OP_pref_decr:
				op_pref_decr();
				break;
			case Interpreter::OP_ENUM::OP_and:
				op_and();
				break;
			case Interpreter::OP_ENUM::OP_or:
				op_or();
				break;
			case Interpreter::OP_ENUM::OP_shift_left:
				op_shift_left();
				break;
			case Interpreter::OP_ENUM::OP_shift_right:
				op_shift_right();
				break;
			case Interpreter::OP_ENUM::OP_mod:
				op_mod();
				break;
			case Interpreter::OP_ENUM::OP_minus:
				op_minus();
				break;
			case Interpreter::OP_ENUM::OP_plus:
				op_plus();
				break;
			case Interpreter::OP_ENUM::OP_mul:
				op_mul();
				break;
			case Interpreter::OP_ENUM::OP_div:
				op_div();
				break;
			case Interpreter::OP_ENUM::OP_bit_and:
				op_bit_and();
				break;
			case Interpreter::OP_ENUM::OP_bit_or:
				op_bit_or();
				break;
			case Interpreter::OP_ENUM::OP_bit_xor:
				op_bit_xor();
				break;
			case Interpreter::OP_ENUM::OP_less:
				op_less();
				break;
			case Interpreter::OP_ENUM::OP_more:
				op_more();
				break;
			case Interpreter::OP_ENUM::OP_assign:
				op_assign();
				break;
			case Interpreter::OP_ENUM::OP_not:
				op_not();
				break;
			case Interpreter::OP_ENUM::OP_less_equal:
				op_less_equal();
				break;
			case Interpreter::OP_ENUM::OP_more_equal:
				op_more_equal();
				break;
			case Interpreter::OP_ENUM::OP_equal:
				op_equal();
				break;
			case Interpreter::OP_ENUM::OP_not_equal:
				op_not_equal();
				break;
			case Interpreter::OP_ENUM::OP_negate:
				op_negate();
				break;
			case Interpreter::OP_ENUM::OP_positive:
				op_positive();
				break;
			case Interpreter::OP_ENUM::OP_reference:
				op_reference();
				break;
			case Interpreter::OP_ENUM::OP_get_val:
				op_get_val();
				break;
			case Interpreter::OP_ENUM::OP_member:
				op_member(element->getSubValue());
				break;
			case Interpreter::OP_ENUM::OP_ptr_member:
				op_ptr_member(element->getSubValue());
				break;
			case Interpreter::OP_ENUM::OP_call:
				op_call(element->getSubValue());
				break;
			case Interpreter::OP_ENUM::OP_member_call:
				op_member_call(element->getSubValue());
				break;
			}
			if (isComplexAssign(val))
				op_assign();
		}
			break;
		case TokenType::TYPE:
		{
			std::string name = element->getSubValue();
			std::string type = element->getValue();

			if (type == "int")
				variables[stackDepth]->putCoin(name, NumType::INT);
			else if (type == "double")
				variables[stackDepth]->putCoin(name, NumType::DOUBLE);
			else if (type == "bool")
				variables[stackDepth]->putCoin(name, NumType::BOOL);
			else if (type.starts_with("ptr<"))
				variables[stackDepth]->putCoin(name, type.substr(4, type.size() - 5));
			else if (type == "string")
				variables[stackDepth]->putCoin(name);
			else if (classes.classExists(type))
			{
				std::shared_ptr<CoinTable> newFields = classes.getClass(type)->getFields()->clone();
				variables[stackDepth]->putCoin(name, classes.getClass(type), newFields);
			}
			else
			{
				std::cout << "ERR: 003d. error-type '" << type << "\n";
				exit(33);
			}
		}
			break;
		case TokenType::JMP:
			IC.top() = beg + element->getJumper();
			continue;
			break;
		case TokenType::CJM:
			if (!std::dynamic_pointer_cast<Numeric, Coin>(TOP())->getInt())
			{
				IC.top() = beg + element->getJumper();
				POP();
				continue;
			}
			else
				POP();
			break;
		case TokenType::CLEANER:
			POP();
			break;
		case TokenType::DESTRUCTOR:
			eraseCoin(element->getValue());
			break;
		case TokenType::SPECIAL:
		{
			std::string code = element->getValue();
			if (code == "input")
			{
				std::shared_ptr<Coin> coin = POP();
				if (coin->type == CoinType::NUMERIC)
				{
					double buf;
					std::cin >> buf;
					std::dynamic_pointer_cast<Numeric, Coin>(coin)->set(buf);
				}
				else if (coin->type == CoinType::STRING)
				{
					std::string buf;
					std::getline(std::cin, buf);
					std::dynamic_pointer_cast<String, Coin>(coin)->set(buf);
				}
			}
			else if (code == "print")
			{
				std::shared_ptr<Coin> coin = POP();
				if (coin->type == CoinType::NUMERIC)
				{
					if (std::dynamic_pointer_cast<Numeric, Coin>(coin)->numericType == NumType::DOUBLE)
						std::cout << std::dynamic_pointer_cast<Numeric, Coin>(coin)->get();
					else
						std::cout << std::dynamic_pointer_cast<Numeric, Coin>(coin)->getInt();
				}
				else if (coin->type == CoinType::STRING)
					std::cout << std::dynamic_pointer_cast<String, Coin>(coin)->get();
			}
			else if (code == "return")
			{
				value_to_return = POP();
			}
			else if (code == "return_void")
			{
				// value_to_return = std::make_shared<Void>("void");
			}
			else if (code == "exit")
			{
				exit(0);
			}
		}
			break;
		}
		++IC.top();
	}

	std::cout << "\n\n!!!SOMETHING WENT WRONG!!!\n\n";
	previousScope();
	PUT(value_to_return);
	IC.pop();
}
//

void Interpreter::op_post_incr()
{
	std::shared_ptr<Coin> val = POP();
	PUT(turnToConst(val));

	++(std::dynamic_pointer_cast<Numeric, Coin>(val)->data);
}
void Interpreter::op_pref_incr()
{
	std::shared_ptr<Coin> val = POP();
	++(std::dynamic_pointer_cast<Numeric, Coin>(val)->data);
	PUT(val);
}
void Interpreter::op_post_decr()
{
	std::shared_ptr<Coin> val = POP();
	PUT(turnToConst(val));

	--(std::dynamic_pointer_cast<Numeric, Coin>(val)->data);
}
void Interpreter::op_pref_decr()
{
	std::shared_ptr<Coin> val = POP();
	--(std::dynamic_pointer_cast<Numeric, Coin>(val)->data);
	PUT(val);
}
void Interpreter::op_and()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	bool result = val_left->get() && val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_or()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	bool result = val_left->get() || val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_shift_left()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	int result = val_left->getInt() << val_right->getInt();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::INT, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_shift_right()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	int result = val_left->getInt() >> val_right->getInt();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::INT, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_mod()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	double result = fmod(val_left->getInt(), val_right->getInt());
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", getNumericReturnType(val_right->numericType, val_right->numericType), true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_minus()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	double result = val_left->get() - val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", getNumericReturnType(val_right->numericType, val_right->numericType), true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_plus()
{
	if (TOP()->type == CoinType::STRING)
	{
		std::shared_ptr<String> val_right = std::dynamic_pointer_cast<String, Coin>(POP());
		std::shared_ptr<String> val_left = std::dynamic_pointer_cast<String, Coin>(POP());

		std::string result = val_left->get() + val_right->get();
		std::shared_ptr<String> res = std::make_shared<String>("", true);
		res->set(result);
		PUT(turnToConst(res));
	}
	else
	{
		std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
		
		if (TOP()->type == CoinType::STRING)
		{
			std::shared_ptr<String> val_left = std::dynamic_pointer_cast<String, Coin>(POP());
			
			std::string result;
			if (val_right->numericType == NumType::DOUBLE)
				result = val_left->get() + val_right->get();
			else
				result = val_left->get() + val_right->getInt();

			std::shared_ptr<String> res = std::make_shared<String>("", true);
			res->set(result);
			PUT(turnToConst(res));
		}
		else
		{
			std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

			double result = val_left->get() + val_right->get();
			std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", getNumericReturnType(val_right->numericType, val_right->numericType), true);
			res->set(result);
			PUT(turnToConst(res));
		}
	}
}
void Interpreter::op_mul()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	if (TOP()->type == CoinType::STRING)
	{
		std::shared_ptr<String> val_left = std::dynamic_pointer_cast<String, Coin>(POP());

		std::string result = val_left->get() * val_right->getInt();
		std::shared_ptr<String> res = std::make_shared<String>("", true);
		res->set(result);
		PUT(turnToConst(res));
	}
	else
	{
		std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

		double result = val_left->get() * val_right->get();
		std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", getNumericReturnType(val_right->numericType, val_right->numericType), true);
		res->set(result);
		PUT(turnToConst(res));
	}
}
void Interpreter::op_div()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	double result = val_left->get() / val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", getNumericReturnType(val_right->numericType, val_right->numericType), true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_bit_and()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	int result = val_left->getInt() & val_right->getInt();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::INT, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_bit_or()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	int result = val_left->getInt() | val_right->getInt();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::INT, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_bit_xor()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	int result = val_left->getInt() ^ val_right->getInt();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::INT, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_less()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	bool result = val_left->get() < val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_more()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	bool result = val_left->get() > val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_assign()
{
	std::shared_ptr<Coin> val_right = POP();
	std::shared_ptr<Coin> val_left = TOP();

	if (val_left == nullptr)
	{
		std::cout << "ERR: 002b. l_value was nullptr\n";
		exit(21);
	}

	switch (val_left->type)
	{
	case CoinType::NUMERIC:
		std::dynamic_pointer_cast<Numeric, Coin>(val_left)->set(
			std::dynamic_pointer_cast<Numeric, Coin>(val_right)->get()
		);
		break;
	case CoinType::STRING:
		std::dynamic_pointer_cast<String, Coin>(val_left)->set(
			std::dynamic_pointer_cast<String, Coin>(val_right)->get()
		);
		break;
	case CoinType::POINTER:
		std::dynamic_pointer_cast<Pointer, Coin>(val_left)->set(
			std::dynamic_pointer_cast<Pointer, Coin>(val_right)->get(),
			std::dynamic_pointer_cast<Pointer, Coin>(val_right)->getStoringType()
		);
		break;
	case CoinType::OBJECT:
		std::dynamic_pointer_cast<Object, Coin>(val_left)->set(
			std::dynamic_pointer_cast<Object, Coin>(val_right)
		);
		break;
	default:
		break;
	}
}
void Interpreter::op_not()
{
	std::shared_ptr<Numeric> value = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	bool result = !value->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_less_equal()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	bool result = val_left->get() <= val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_more_equal()
{
	std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
	std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	bool result = val_left->get() >= val_right->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_equal()
{
	if (TOP()->type == CoinType::NUMERIC)
	{
		std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
		std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

		bool result = val_left->get() == val_right->get();
		std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
		res->set(result);

		PUT(turnToConst(res));
	}
	else if (TOP()->type == CoinType::STRING)
	{
		std::shared_ptr<String> val_right = std::dynamic_pointer_cast<String, Coin>(POP());
		std::shared_ptr<String> val_left = std::dynamic_pointer_cast<String, Coin>(POP());

		bool result = val_left->get() == val_right->get();
		std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
		res->set(result);

		PUT(turnToConst(res));
	}
	else if (TOP()->type == CoinType::POINTER)
	{
		std::shared_ptr<Pointer> val_right = std::dynamic_pointer_cast<Pointer, Coin>(POP());
		std::shared_ptr<Pointer> val_left = std::dynamic_pointer_cast<Pointer, Coin>(POP());

		bool result = val_left->get() == val_right->get();
		std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
		res->set(result);

		PUT(turnToConst(res));
	}
	else
	{
		std::cout << "ERR: 001a. can't peform operation ==\n";
		exit(10);
	}
}
void Interpreter::op_not_equal()
{
	if (TOP()->type == CoinType::NUMERIC)
	{
		std::shared_ptr<Numeric> val_right = std::dynamic_pointer_cast<Numeric, Coin>(POP());
		std::shared_ptr<Numeric> val_left = std::dynamic_pointer_cast<Numeric, Coin>(POP());

		bool result = val_left->get() != val_right->get();
		std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
		res->set(result);

		PUT(turnToConst(res));
	}
	else if (TOP()->type == CoinType::STRING)
	{
		std::shared_ptr<String> val_right = std::dynamic_pointer_cast<String, Coin>(POP());
		std::shared_ptr<String> val_left = std::dynamic_pointer_cast<String, Coin>(POP());

		bool result = val_left->get() != val_right->get();
		std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
		res->set(result);

		PUT(turnToConst(res));
	}
	else if (TOP()->type == CoinType::POINTER)
	{
		std::shared_ptr<Pointer> val_right = std::dynamic_pointer_cast<Pointer, Coin>(POP());
		std::shared_ptr<Pointer> val_left = std::dynamic_pointer_cast<Pointer, Coin>(POP());

		bool result = val_left->get() != val_right->get();
		std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", NumType::BOOL, true);
		res->set(result);

		PUT(turnToConst(res));
	}
	else
	{
		std::cout << "ERR: 001b. can't peform operation !=\n";
		exit(11);
	}
}
void Interpreter::op_negate()
{
	std::shared_ptr<Numeric> value = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	double result = -value->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", value->numericType, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_positive()
{
	std::shared_ptr<Numeric> value = std::dynamic_pointer_cast<Numeric, Coin>(POP());

	double result = +value->get();
	std::shared_ptr<Numeric> res = std::make_shared<Numeric>("", value->numericType, true);
	res->set(result);

	PUT(turnToConst(res));
}
void Interpreter::op_reference()
{
	std::shared_ptr<Coin> value = POP();

	PUT(newPointer(value));
}
void Interpreter::op_get_val()
{
	if (TOP() == nullptr)
	{
		std::cout << "ERR: 002a. when tried to get value via '*', pointer was nullptr\n";
		exit(20);
	}

	std::shared_ptr<Coin> value = std::dynamic_pointer_cast<Pointer, Coin>(POP())->data;
	PUT(value);
}
void Interpreter::op_member(std::string name)
{
	std::shared_ptr<Object> value = std::dynamic_pointer_cast<Object, Coin>(POP());
	
	PUT(value->fields->getCoin(name));
}
void Interpreter::op_ptr_member(std::string name)
{
	if (TOP() == nullptr)
	{
		std::cout << "ERR: 002b. when tried to get value via '->', pointer was nullptr\n";
		exit(21);
	}

	std::shared_ptr<Object> value = std::dynamic_pointer_cast<Object, Coin>(
			std::dynamic_pointer_cast<Pointer, Coin>(POP())->data);
	
	PUT(value->fields->getCoin(name));
}
void Interpreter::op_call(std::string name)
{
	std::shared_ptr<Function> func = functions.getFunction(name);

	std::shared_ptr<CoinTable> attrInput = func->getAttributes();
	for (auto& attribute : func->attributesNamesOrder)
	{
		std::shared_ptr<Coin> inp = attrInput->getCoin(attribute);
		std::shared_ptr<Coin> buf = POP();
		switch (buf->type)
		{
		case CoinType::NUMERIC:
			std::dynamic_pointer_cast<Numeric, Coin>(inp)->set(
				std::dynamic_pointer_cast<Numeric, Coin>(buf)->get()
			);
			break;
		case CoinType::STRING:
			std::dynamic_pointer_cast<String, Coin>(inp)->set(
				std::dynamic_pointer_cast<String, Coin>(buf)->get()
			);
			break;
		case CoinType::POINTER:
			std::dynamic_pointer_cast<Pointer, Coin>(inp)->set(
				std::dynamic_pointer_cast<Pointer, Coin>(buf)->get(),
				std::dynamic_pointer_cast<Pointer, Coin>(buf)->getStoringType()
			);
			break;
		case CoinType::OBJECT:
			std::dynamic_pointer_cast<Object, Coin>(inp)->set(
				std::dynamic_pointer_cast<Object, Coin>(buf)
			);
			break;
		default:
			break;
		}
	}

	newScope();
	
	for (auto& attribute : func->attributes->getTable())
		putCoin(attribute.second);

	run(*func->body);
}
void Interpreter::op_member_call(std::string name)
{
	std::shared_ptr<Object> obj = std::dynamic_pointer_cast<Object, Coin>(POP());
	std::shared_ptr<Function> func = obj->methods.lock()->getFunction(name);

	std::shared_ptr<CoinTable> attrInput = func->getAttributes();

	std::vector<std::string>::reverse_iterator it = func->attributesNamesOrder.rbegin();
	for (; it != func->attributesNamesOrder.rend(); ++it)
	{
		std::shared_ptr<Coin> inp = attrInput->getCoin(*it);
		std::shared_ptr<Coin> buf = POP();
		switch (buf->type)
		{
		case CoinType::NUMERIC:
			std::dynamic_pointer_cast<Numeric, Coin>(inp)->set(
				std::dynamic_pointer_cast<Numeric, Coin>(buf)->get()
			);
			break;
		case CoinType::STRING:
			std::dynamic_pointer_cast<String, Coin>(inp)->set(
				std::dynamic_pointer_cast<String, Coin>(buf)->get()
			);
			break;
		case CoinType::POINTER:
			std::dynamic_pointer_cast<Pointer, Coin>(inp)->set(
				std::dynamic_pointer_cast<Pointer, Coin>(buf)->get(),
				std::dynamic_pointer_cast<Pointer, Coin>(buf)->getStoringType()
			);
			break;
		case CoinType::OBJECT:
			std::dynamic_pointer_cast<Object, Coin>(inp)->set(
				std::dynamic_pointer_cast<Object, Coin>(buf)
			);
			break;
		default:
			break;
		}
	}

	newScope();

	for (auto& attribute : func->attributes->getTable())
		putCoin(attribute.second);
	for (auto& attribute : obj->fields->getTable())
		putCoin(attribute.second);

	std::string buf_name = obj->getName();
	obj->setName("this");
	putCoin(obj);

	run(*func->body);

	obj->setName(buf_name);
}
