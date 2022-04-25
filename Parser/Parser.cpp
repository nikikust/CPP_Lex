#include "Parser.h"

#include <iostream>

Parser::Parser() {}
tokenVect ::iterator Parser::getLastToken()
{
	return TokenIterator;
}

void Parser::printTree()
{
	tree.printTree();
}
void Parser::printRPN(bool full)
{
	//std::cout << tree.genRPN_str(full) << std::endl;
	
	nodeVect::iterator it = RPN.begin();
	std::cout << RPN.size() << "\n";

	while (it != RPN.end())
	{
		std::cout << (*it++)->getValue() << " ";
	}
}
void Parser::formRPN()
{
	this->RPN = tree.RPN();
}

bool Parser::checkToken(bool x, std::string type)
{
	if (x)
	{
		(*TokenIterator)->gotIn = type;
		last_token = *(*TokenIterator++);
	}
	return x;
}
bool Parser::breakCode()
{
	StateOK = false;
	return 1;
}

bool Parser::TRY(bool x)
{
	return StateOK && x;
}
void Parser::USE(bool x)
{
	if (StateOK && !x)
		breakCode();
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
		    || SEMICOLON()
		    || _EOF()
		     );
	if (x)
		tree.nextStatement();
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
		USE(block());
	}
	else if (TRY(WHILE()))
	{
		USE(condition_expr());
		USE(block());
	}
	else if (TRY(DO()))
	{
		USE(block());
		USE(WHILE());
		USE(condition_expr());
	}
	else
		return 0;
	return StateOK;
}
bool Parser::selection_statement()
{
	if (!TRY(IF()))
		return 0;

	USE(condition_expr());
	USE(block());

	while (TRY(ELIF()))
	{
		USE(condition_expr());
		USE(block());
	}

	if (TRY(ELSE()))
		USE(block());

	return StateOK;
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
	if (!TRY(TYPE_SPECIFIER()))
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

	TRY(variable_declaration());
	USE(SEMICOLON());
	TRY(condition());
	USE(SEMICOLON());
	TRY(arifmetic_expression_list());
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
		tree.addToken(last_token, TokenType::IDENTIFIER);
	return x;
}
bool Parser::STRING()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::STRING, "STRING");
	if (x)
		tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}
bool Parser::INT()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::INT, "INT");
	if (x)
		tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}
bool Parser::FLOAT()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::FLOAT, "FLOAT");
	if (x)
		tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}
bool Parser::BOOL()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::BOOL, "BOOL");
	if (x)
		tree.addToken(last_token, TokenType::CONSTANT);
	return x;
}

bool Parser::OP_BINAR()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::OP_BINAR, "OP_BINAR");
	if (x)
		tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::OP_UNAR_PREF()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::OP_UNAR_PREF, "OP_UNAR_PREF");
	if (x)
		tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::OP_UNAR_POST()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::OP_UNAR_POST, "OP_UNAR_POST");
	if (x)
		tree.addToken(last_token, TokenType::OPERATION);
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
		tree.addToken(last_token, TokenType::OPERATION);
	}
	return x;
}
bool Parser::STRONG_ASSIGN_OP()
{
	bool x = checkToken((*TokenIterator)->value == "=", "STRONG_ASSIGN_OP");
	if (x)
		tree.addToken(last_token, TokenType::OPERATION);
	return x;
}
bool Parser::POINTER()
{
	bool x = checkToken((*TokenIterator)->value == "*", "POINTER");
	if (x)
	{
		last_token.type = TokensEnum::OP_UNAR_PREF;
		tree.addToken(last_token, TokenType::OPERATION);
	}
	return x;
}

bool Parser::IF()
{
	return checkToken((*TokenIterator)->value == "if", "IF");
}
bool Parser::ELIF()
{
	return checkToken((*TokenIterator)->value == "elif", "ELIF");
}
bool Parser::ELSE()
{
	return checkToken((*TokenIterator)->value == "else", "ELSE");
}
bool Parser::FOR()
{
	return checkToken((*TokenIterator)->value == "for", "FOR");
}
bool Parser::WHILE()
{
	return checkToken((*TokenIterator)->value == "while", "WHILE");
}
bool Parser::DO()
{
	return checkToken((*TokenIterator)->value == "do", "DO");
}
bool Parser::DEF()
{
	return checkToken((*TokenIterator)->value == "def", "DEF");
}
bool Parser::RETURN()
{
	return checkToken((*TokenIterator)->value == "return", "RETURN");
}
bool Parser::BREAK()
{
	return checkToken((*TokenIterator)->value == "break", "BREAK");
}
bool Parser::CONTINUE()
{
	return checkToken((*TokenIterator)->value == "continue", "CONTINUE");
}
bool Parser::TYPE_SPECIFIER()
{
	bool x = checkToken((*TokenIterator)->type == TokensEnum::SIMPLETYPE, "SIMPLE_TYPE");
	if (x)
		tree.addToken(last_token, TokenType::TYPE);
	return x;
}

bool Parser::BRACKET_L_FIG()
{
	return checkToken((*TokenIterator)->value == "{", "punc");
}
bool Parser::BRACKET_R_FIG()
{
	return checkToken((*TokenIterator)->value == "}", "punc");
}
bool Parser::BRACKET_L_RND()
{
	bool x = checkToken((*TokenIterator)->value == "(", "punc");
	if (x)
		tree.addToken(last_token, TokenType::ROUNDS);
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
		tree.addToken(last_token, TokenType::SQUARES);
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
	{
		tree.goUp();
		tree.goDown();
	}
	return x;
}
bool Parser::NEWLINE()
{
	return checkToken((*TokenIterator)->type == TokensEnum::NEWLINE, "new_line");
}

bool Parser::_EOF()
{
	return checkToken((*TokenIterator)->type == TokensEnum::_EOF, "EOF");
}