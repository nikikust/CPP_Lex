#pragma once
#include <stack>
#include <boost/function.hpp>

#include "../Memory/Memory.h"


class Interpreter
{
	ClassTable classes;
	FunctionTable functions;

	CoinTable const_table;
	std::map<uint32_t, std::shared_ptr<CoinTable>> variables = { {0, std::make_shared<CoinTable>()} };
	std::map<uint32_t, std::shared_ptr<std::stack<std::shared_ptr<Coin>>>> Stacks = 
		{ {0, std::make_shared<std::stack<std::shared_ptr<Coin>>>()} };
	uint32_t stackDepth = 0;

	RPNVect program;
	std::stack<RPNVect::iterator> IC;

	uint32_t local_const_ID = 0;

public:
	Interpreter();
	bool load_links();

	void PUT(std::string name);
	void PUT(std::shared_ptr<Coin> coin);
	std::shared_ptr<Coin> TOP();
	std::shared_ptr<Coin> POP();

	std::shared_ptr<Coin> turnToConst(std::shared_ptr<Coin> coin);
	std::shared_ptr<Coin> newPointer(std::shared_ptr<Coin> coin);
	NumType getNumericReturnType(NumType a, NumType b);
	bool isComplexAssign(std::string op);

	std::shared_ptr<Coin> getCoin(std::string name);
	void putCoin(std::shared_ptr<Coin> coin);
	void eraseCoin(std::string name);

	void newScope();
	void previousScope();

	void run();					// finaly!!!
	void run(RPNVect& prog);	// finaly!!!

	void op_post_incr();
	void op_pref_incr();
	void op_post_decr();
	void op_pref_decr();
	void op_and();
	void op_or();
	void op_shift_left();
	void op_shift_right();
	void op_mod();
	void op_minus();
	void op_plus();
	void op_mul();
	void op_div();
	void op_bit_and();
	void op_bit_or();
	void op_bit_xor();
	void op_less();
	void op_more();
	void op_assign();
	void op_not();
	void op_less_equal();
	void op_more_equal();
	void op_equal();
	void op_not_equal();
	void op_negate();
	void op_positive();
	void op_reference();
	void op_get_val();
	void op_member(std::string name);
	void op_call(std::string name);
	void op_member_call(std::string name);
	
private:
	enum class OP_ENUM
	{
		OP_post_incr,
		OP_pref_incr,
		OP_post_decr,
		OP_pref_decr,
		OP_and,
		OP_or,
		OP_shift_left,
		OP_shift_right,
		OP_mod,
		OP_minus,
		OP_plus,
		OP_mul,
		OP_div,
		OP_bit_and,
		OP_bit_or,
		OP_bit_xor,
		OP_less,
		OP_more,
		OP_assign,
		OP_not,
		OP_less_equal,
		OP_more_equal,
		OP_equal,
		OP_not_equal,
		OP_negate,
		OP_positive,
		OP_reference,
		OP_get_val,
		OP_member,
		OP_call,
		OP_member_call
	};
	
	inline static std::unordered_map<std::string, OP_ENUM>
		operations =
	{
		{ ">++", OP_ENUM::OP_post_incr},
		{ "++<", OP_ENUM::OP_pref_incr},
		{ ">--", OP_ENUM::OP_post_decr},
		{ "--<", OP_ENUM::OP_pref_decr},
		{ "&&", OP_ENUM::OP_and},
		{ "||", OP_ENUM::OP_or},
		{ "<<", OP_ENUM::OP_shift_left},
		{ ">>", OP_ENUM::OP_shift_right},
		{ "%", OP_ENUM::OP_mod},
		{ "-", OP_ENUM::OP_minus},
		{ "+", OP_ENUM::OP_plus},
		{ "*", OP_ENUM::OP_mul},
		{ "/", OP_ENUM::OP_div},
		{ "&", OP_ENUM::OP_bit_and},
		{ "|", OP_ENUM::OP_bit_or},
		{ "^", OP_ENUM::OP_bit_xor},
		{ "<", OP_ENUM::OP_less},
		{ ">", OP_ENUM::OP_more},
		{ "=", OP_ENUM::OP_assign},
		{ "!", OP_ENUM::OP_not},
		{ "<=", OP_ENUM::OP_less_equal},
		{ ">=", OP_ENUM::OP_more_equal},
		{ "==", OP_ENUM::OP_equal},
		{ "!=", OP_ENUM::OP_not_equal},
		{ "-1", OP_ENUM::OP_negate},
		{ "+1", OP_ENUM::OP_positive},
		{ "&1", OP_ENUM::OP_reference},
		{ "*1", OP_ENUM::OP_get_val},
		{ ".", OP_ENUM::OP_member},
		{ "(", OP_ENUM::OP_call},
		{ ".(", OP_ENUM::OP_member_call}
	};
};
