#include <iostream>

#include "Parser.h"
#include "../profile.h"


Parser::Parser() {}

void Parser::printTree(bool newView)
{
	tree.printTree(newView);
}
void Parser::printRPN(bool full)
{
	if (!tree.rpn_ok)
		return;

	RPNVect::iterator it = RPN.begin();
	size_t cnt = 0;
	while (it != RPN.end())
	{
		auto node = *it;

		std::cout << "(" << cnt << "): ";
		if (node->getType() == TokenType::JMP || node->getType() == TokenType::CJM)
			std::cout << node->getValue() << "(" << node->getJumper() << ": " << RPN.at(node->getJumper())->getValue() << ")\n";
		else if (node->getType() == TokenType::DESTRUCTOR)
			std::cout << "~" << node->getValue() << "\n";
		else
			std::cout << node->getValue() << "\n";

		++it; ++cnt;
	}
}
bool Parser::formRPN()
{
	nodeVect out = tree.RPN();

	if (tree.rpn_ok)
		for (auto& obj : out)
			this->RPN.push_back(std::make_shared<RPN_Element>(*obj));
	return tree.rpn_ok;
}
void Parser::showVars()
{
	if (tree.rpn_ok)
		tree.showVars();
}

bool Parser::checkToken(bool x, std::string type, bool moveIter)
{
	if (x)
	{
		(*TokenIterator)->gotIn = type;
		last_token = *(*TokenIterator);
		if (moveIter)
			TokenIterator++;
	}
	return x;
}
bool Parser::breakCode()
{
	std::cout << colorText(31, 1) << "--------------------------- Break ---------------------------\nat " << (*TokenIterator)->str() << colorText() << "\n\n";
	StateOK = false;
	return 1;
}

bool Parser::TRY(bool x)
{
	return StateOK && x;
}
bool Parser::USE(bool x)
{
	if (StateOK && !x)
		breakCode();
	return x;
}


bool Parser::lang(tokenVect& TokenVect)
{
	Tokens = TokenVect;
	TokenIterator = Tokens.begin();

	while (TokenIterator != Tokens.end())
		if (!TRY(statement()))
			return 0;
	return 1;
}

bool Parser::statement()
{
	int a = 1;
	
	bool x = StateOK && 
		     ( block() 
		    || declaration()
		    || cycle_statement()
		    || selection_statement()
		    || jump_statements()
		    || arifmetic_expression_list()
		    || _EOF()
		     );
	
	if (x)
		tree.ret();
	return x;
}

bool Parser::block()
{
	if (!TRY(BRACKET_L_FIG()))
		return 0;

	while(TRY(statement())) {}
	
	USE(BRACKET_R_FIG());
	
	return StateOK;
}
bool Parser::declaration()
{
	return StateOK && 
		  (variable_declaration() || function_declaration());
}
bool Parser::cycle_statement()
{
	if (TRY(FOR()))
	{
		USE(for_condition_expr());
		USE(after_keyword());
		tree.checkForFull(*TokenIterator);
	}
	else if (TRY(WHILE()))
	{
		USE(condition_expr());
		USE(after_keyword());
		tree.checkForFull(*TokenIterator);
	}/*	--- Нужен ли?
	else if (TRY(DO()))
	{
		USE(after_keyword());
		USE(WHILE());
		USE(condition_expr());
	}*/
	else
		return 0;
	return StateOK;
}
bool Parser::selection_statement()
{
	if (!TRY(IF()))
		return 0;

	USE(condition_expr());
	USE(after_keyword());
	tree.checkForFull(*TokenIterator);

	while (TRY(ELIF()))
	{
		USE(condition_expr());
		USE(after_keyword());
		tree.checkForFull(*TokenIterator);
	}

	if (TRY(ELSE()))
	{
		USE(after_keyword());
		tree.checkForFull(*TokenIterator);
	}
	return StateOK;
}

bool Parser::after_keyword()
{
	return StateOK &&
		  (block() || statement());
}
bool Parser::jump_statements()
{
	return StateOK && 
		  (RETURN() || BREAK() || CONTINUE());
}
bool Parser::arifmetic_expression_list()
{
	if (!TRY(arifmetic_expression()))
		return 0;
	
	while (TRY(COMMA()))
		USE(arifmetic_expression());

	return StateOK;
}

bool Parser::variable_declaration()
{
	if (!TRY(declarator()))
		return 0;

	if (TRY(STRONG_ASSIGN_OP()))
		USE(arifmetic_expression());

	while(TRY(extended_declaration())) {}
	tree.goUp();

	return StateOK;
}
bool Parser::function_declaration()
{
	if (!TRY(DEF()))
		return 0;

	USE(IDENTIFIER());
	USE(BRACKET_L_RND());
	USE(attributes_list());
	USE(BRACKET_R_RND());
	USE(block());

	return StateOK;
}

bool Parser::declarator()
{
	if (!( TRY(TYPE_SPECIFIER()) || TRY(CLASS()) ))
		return 0;

	TRY(array_declatation_part());
	USE(IDENTIFIER());

	return StateOK;
}
bool Parser::extended_declaration()
{
	if (!TRY(COMMA()))
		return 0;

	USE(IDENTIFIER());
	if (TRY(STRONG_ASSIGN_OP()))
		USE(arifmetic_expression());

	return StateOK;
}
bool Parser::attributes_list()
{
	if (!TRY(attribute()))
		return 0;

	while (TRY(COMMA()))
		USE(attribute());

	return StateOK;
}
bool Parser::attribute()
{
	if (!TRY(TYPE_SPECIFIER()))
		return 0;
	while (TRY(POINTER())) {}
	USE(IDENTIFIER());

	return StateOK;
}

bool Parser::array_declatation_part()
{
	if (!TRY(BRACKET_L_SQR()))
		return 0;
	USE(INT());
	USE(BRACKET_R_SQR());

	return StateOK;
}

bool Parser::for_condition_expr()
{
	if (!TRY(BRACKET_L_RND()))
		return 0;

	tree.addNode(); tree.goDown();
	if (TRY(variable_declaration()))
		tree.goUp();
	else
		TRY(arifmetic_expression_list());
	tree.goUp();

	USE(SEMICOLON());

	tree.addNode(); tree.goDown();
	TRY(condition());
	tree.goUp(); tree.ret();

	USE(SEMICOLON());

	tree.addNode(); tree.goDown();
	TRY(arifmetic_expression_list());
	//tree.goUp();

	USE(BRACKET_R_RND());

	return StateOK;
}

bool Parser::condition_expr()
{
	if (!TRY(BRACKET_L_RND()))
		return 0;

	USE(condition());
	USE(BRACKET_R_RND());

	return StateOK;
}
bool Parser::condition()
{
	return StateOK && arifmetic_expression();
}

bool Parser::arifmetic_expression()
{
	if (!TRY(unary_expression()))
		return 0;

	while (TRY(OP_BINAR()))
		USE(arifmetic_expression());
	
	return StateOK;
}
bool Parser::unary_expression()
{
	if (TRY(OP_UNAR_PREF()))
		USE(unary_expression());
	else if (TRY(OP_UNAR_BINAR()))
		USE(unary_expression());
	else if (TRY(postfix_expression()))
		while(TRY(postfix_operations())) {}
	else 
		return 0;
	return StateOK;
}
bool Parser::postfix_expression()
{
	if (TRY(literal())) {}
	else if (TRY(BRACKET_L_RND()))
	{
		USE(arifmetic_expression());
		USE(BRACKET_R_RND());
	}
	else
		return 0;
	return StateOK;
}
bool Parser::postfix_operations()
{
	if (TRY(BRACKET_L_RND()))
	{
		TRY(arifmetic_expression_list());
		USE(BRACKET_R_RND());
	}
	else if (TRY(BRACKET_L_SQR()))
	{
		USE(arifmetic_expression());
		USE(BRACKET_R_SQR());
	}
	else if (TRY(OP_UNAR_POST())) {}
	else
		return 0;
	return StateOK;
}

bool Parser::literal()
{
	return StateOK &&
		  (IDENTIFIER() || STRING() || INT() || FLOAT() || BOOL());
}


bool Parser::IDENTIFIER()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::IDENTIFIER, "VAR");
	if (x)
		return tree.addToken(last_token, TokenType::IDENTIFIER);
	return x;
}
bool Parser::CLASS()
{
	checkToken((*TokenIterator)->type == TokensEnum::IDENTIFIER, "VAR", false);
	return 0;


	bool x = checkToken((*TokenIterator)->type == TokensEnum::IDENTIFIER, "VAR", false);
	if (x)
	{
		tree.addToken(last_token, TokenType::TYPE);
		return checkToken((*TokenIterator)->type == TokensEnum::IDENTIFIER, "USER_TYPE");
	}
	return x;
}
bool Parser::STRING()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::STRING, "STRING");
	if (x)
		return tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}
bool Parser::INT()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::INT, "INT");
	if (x)
		return tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}
bool Parser::FLOAT()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::FLOAT, "FLOAT");
	if (x)
		return tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}
bool Parser::BOOL()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::BOOL, "BOOL");
	if (x)
		return tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}

bool Parser::OP_BINAR()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::OP_BINAR, "OP_BINAR");
	if (x)
		return tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::OP_UNAR_PREF()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::OP_UNAR_PREF, "OP_UNAR_PREF");
	if (x)
		return tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::OP_UNAR_POST()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::OP_UNAR_POST, "OP_UNAR_POST");
	if (x)
		return tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::OP_UNAR_BINAR()
{
	bool x = checkToken((*TokenIterator)->value == "-" ||
					    (*TokenIterator)->value == "+" ||
					    (*TokenIterator)->value == "*" ||
					    (*TokenIterator)->value == "!" ||
					    (*TokenIterator)->value == "&", "OP_UNAR_BINAR");
	if (x)
	{
		last_token.type = TokensEnum::OP_UNAR_PREF;
		return tree.addToken(last_token, TokenType::OPERATION);
	}
	return x;
}
bool Parser::STRONG_ASSIGN_OP()
{
	bool x = checkToken((*TokenIterator)->value == "=", "STRONG_ASSIGN_OP");
	if (x)
		return tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::POINTER()
{
	bool x = checkToken((*TokenIterator)->value == "*", "POINTER");
	if (x)
	{
		last_token.type = TokensEnum::OP_UNAR_PREF;
		return tree.addToken(last_token, TokenType::OPERATION);
	}
	return x;
}

bool Parser::IF()
{
	bool x = checkToken((*TokenIterator)->value == "if", "IF");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::ELIF()
{
	bool x = checkToken((*TokenIterator)->value == "elif", "ELIF");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::ELSE()
{
	bool x = checkToken((*TokenIterator)->value == "else", "ELSE");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::FOR()
{
	bool x = checkToken((*TokenIterator)->value == "for", "FOR");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::WHILE()
{
	bool x = checkToken((*TokenIterator)->value == "while", "WHILE");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::DO()
{
	bool x = checkToken((*TokenIterator)->value == "do", "DO");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::DEF()
{
	bool x = checkToken((*TokenIterator)->value == "def", "DEF");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::RETURN()
{
	bool x = checkToken((*TokenIterator)->value == "return", "RETURN");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::BREAK()
{
	bool x = checkToken((*TokenIterator)->value == "break", "BREAK");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::CONTINUE()
{
	bool x = checkToken((*TokenIterator)->value == "continue", "CONTINUE");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::PRINT()
{
	bool x = checkToken((*TokenIterator)->value == "print", "PRINT");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::INPUT()
{
	bool x = checkToken((*TokenIterator)->value == "input", "INPUT");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}

bool Parser::TYPE_SPECIFIER()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::SIMPLETYPE, "SIMPLE_TYPE");
	if (x)
		return tree.addToken(last_token, TokenType::TYPE);
	return x;
}

bool Parser::BRACKET_L_FIG()
{
	bool x = checkToken((*TokenIterator)->value == "{", "punc");
	if (x)
		return tree.addToken(last_token, TokenType::SPECIAL);
	return x;
}
bool Parser::BRACKET_R_FIG()
{
	bool x = checkToken((*TokenIterator)->value == "}", "punc");
	if (x)
	{
		tree.goUp();
		tree.checkForFull(*TokenIterator);
	}
	return x;
}
bool Parser::BRACKET_L_RND()
{
	bool x = checkToken((*TokenIterator)->value == "(", "punc");
	if (x)
		return tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::BRACKET_R_RND()
{
	bool x = checkToken((*TokenIterator)->value == ")", "punc");
	if (x)
		tree.goUp();
	return x;
}
bool Parser::BRACKET_L_SQR()
{
	bool x = checkToken((*TokenIterator)->value == "[", "punc");
	if (x)
		return tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::BRACKET_R_SQR()
{
	bool x = checkToken((*TokenIterator)->value == "]", "punc");
	if (x)
		tree.goUp();
	return x;
}

bool Parser::SEMICOLON()
{
	return checkToken((*TokenIterator)->value == ";", "punc");
}
bool Parser::COMMA()
{
	bool x = checkToken((*TokenIterator)->value == ",", "punc");
	if (x)
		tree.ret();
	return x;
}
bool Parser::NEWLINE()
{
	return checkToken((*TokenIterator)->type == TokensEnum::NEWLINE, "new_line");
}

bool Parser::_EOF()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::_EOF, "EOF");
	if (x)
		return tree.addToken(last_token, TokenType::_EOF);
	return x;
}
