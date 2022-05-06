#pragma once

#include "../AST/AST.h"


class Parser
{
	tokenVect Tokens;
	tokenVect ::iterator TokenIterator;
	Token last_token;

	bool StateOK = true;
	bool WriteToVarTable = false;

	AST tree;
	size_t trig = 0;
	RPNVect RPN;

	std::vector<std::string> knownClasses;
	std::vector<std::string> knownFunctions;

public:
	Parser();

	void printTree(bool newView = true);
	void printRPN(bool full = false);
	bool formRPN();
	void showVars();
	void showFunctions();

	bool checkToken(bool x, std::string type, bool moveIter = true);
	bool breakCode(std::string message = "");

	bool TRY(bool x);
	bool USE(bool x);


	bool lang(tokenVect& TokenVect);

	bool statement();

	bool simple_specificator();
	bool declaration();
	bool class_declaration();
	bool class_block();
	bool full_variable_declaration();
	bool variable_declaration();
	bool function_declaration();
	bool return_statement();
	
	bool specificator();
	bool templated_specificator();
	bool type_specification();
	bool declarator();
	bool extended_declaration();
	bool attributes_list();

	bool cycle_statement();
	bool for_condition_expr();

	bool selection_statement();

	bool after_keyword();
	bool jump_statements();
	bool condition_expr();
	bool condition();

	bool arifmetic_expression_list();
	bool arifmetic_expression();
	bool unary_expression();
	bool postfix_expression();
	bool postfix_operations();

	bool literal();

	bool block();
	bool array_declatation_part();

	bool IDENTIFIER();
	bool INT();
	bool DOUBLE();
	bool STRING();
	bool BOOL();

	bool OP_BINAR();
	bool OP_UNAR_PREF();
	bool OP_UNAR_POST();
	bool OP_UNAR_BINAR();
	bool STRONG_ASSIGN_OP();
	bool POINTER();

	bool IF();
	bool ELIF();
	bool ELSE();
	bool WHILE();
	bool FOR();
	bool DO();
	bool RETURN();
	bool BREAK();
	bool CONTINUE();
	bool PRINT();
	bool INPUT();
	bool VOID();
	bool CLASS();

	bool CLASS_TYPE();
	bool SIMPLE_TYPE();
	bool TEMPLATED_TYPE();
	
	bool BRACKET_L_FIG(); // {
	bool BRACKET_R_FIG(); // }
	bool BRACKET_L_RND(); // (
	bool BRACKET_R_RND(); // )
	bool BRACKET_L_TRG(); // <
	bool BRACKET_R_TRG(); // >
	bool BRACKET_L_SQR(); // [
	bool BRACKET_R_SQR(); // ]

	bool SEMICOLON();
	bool COMMA();
	bool NEWLINE();

	bool _EOF();
};