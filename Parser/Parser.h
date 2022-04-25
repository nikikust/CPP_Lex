#pragma once
#include <string>
#include <vector>
#include <list>

#include "../VarTable/VarTable.h"
#include "../AST/AST.h"


class Parser
{
	tokenVect Tokens;
	tokenVect ::iterator TokenIterator;
	Token last_token;

	bool StateOK = true;
	bool WriteToVarTable = false;

	VarTable variables;
	AST tree;
	nodeVect RPN;

public:
	Parser();
	tokenVect ::iterator getLastToken();
	void printTree();
	void printRPN(bool full = false);
	void formRPN();

	bool checkToken(bool x, std::string type);
	bool breakCode();

	bool TRY(bool x);
	void USE(bool x);


	bool lang(tokenVect& TokenVect);

	bool statement();

	bool declaration();
	bool variable_declaration();
	bool function_declaration();

	bool declarator();
	bool extended_declaration();
	bool attributes_list();
	bool attribute();

	bool cycle_statement();
	bool for_condition_expr();

	bool selection_statement();

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
	bool FLOAT();
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
	bool DEF();
	bool RETURN();
	bool BREAK();
	bool CONTINUE();

	bool TYPE_SPECIFIER();
	
	bool BRACKET_L_FIG(); // {
	bool BRACKET_R_FIG(); // }
	bool BRACKET_L_RND(); // (
	bool BRACKET_R_RND(); // )
	bool BRACKET_L_SQR(); // [
	bool BRACKET_R_SQR(); // ]

	bool SEMICOLON();
	bool COMMA();
	bool NEWLINE();

	bool _EOF();
};