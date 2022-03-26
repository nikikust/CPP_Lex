#include "Parser.h"

#include <iostream>

Parser::Parser() {}
std::vector<std::shared_ptr<Token>> ::iterator Parser::getLastToken()
{
	return TokenIterator;
}

std::vector<std::shared_ptr<Token>> ::iterator Parser::storeIter()
{
	return TokenIterator;
}
void Parser::restoreIter(std::vector<std::shared_ptr<Token>> ::iterator iter)
{
	TokenIterator = iter;
}
int Parser::checkToken(bool x, std::string type)
{
	if (x)
	{
		(*TokenIterator)->gotIn = type;
		TokenIterator++;
		return 1;
	}
	return 0;
}


int Parser::lang(std::vector<std::shared_ptr<Token>>& TokenVect)
{
	Tokens = TokenVect;
	TokenIterator = Tokens.begin();
	auto store = storeIter();

	while (TokenIterator != Tokens.end())
		if (!expr())
			return 0;
	return 1;
}

int Parser::expr()
{
	if (assign_op() || if_op() || while_loop() || SEMICOLON() || COMMA()){}
	else
		return 0;
	return 1;
}

int Parser::assign_op()
{
	auto store = storeIter();
	if (VAR() && ASSIGN() && arifmetic_op()) {}
	else
	{
		restoreIter(store);
		return 0;
	}

	while (true)
	{
		store = storeIter();
		if (COMMA() && VAR() && ASSIGN() && arifmetic_op()) {}
		else
		{
			restoreIter(store);
			break;
		}
	}
	return 1;
}
int Parser::if_op()
{
	auto store = storeIter();
	// --- if()...
	if (IF() && BRACKET_L_RND() && arifmetic_op() && BRACKET_R_RND() && block()) {}
	else
	{
		restoreIter(store);
		return 0;
	}

	// --- elif()...
	while (true)
	{
		store = storeIter();
		if (ELIF() && BRACKET_L_RND() && arifmetic_op() && BRACKET_R_RND() && block()) {}
		else
		{
			restoreIter(store);
			break;
		}
	}
	// --- else...

	store = storeIter();
	if (ELSE() && block()) {}
	else
		restoreIter(store);

	return 1;
}
int Parser::while_loop()
{
	auto store = storeIter();
	if (WHILE() && BRACKET_L_RND() && arifmetic_part() && BRACKET_R_RND() && block()) {}
	else
	{
		restoreIter(store);
		return 0;
	}

	return 1;
}


int Parser::arifmetic_part()
{
	auto store = storeIter();

	while (UNAR_OP() || UNAR_BINAR_OP()) {}

	if (NUMBER() || STRING() || VAR())
	{
		while (true)
		{
			if (UNAR_OP()) {}
			else
				break;
		}
		return 1;
	}


	// --- //

	if (BRACKET_L_RND() && arifmetic_op() && BRACKET_R_RND()) 
	{
		while (true)
		{
			if (UNAR_OP()) {}
			else
				break;
		}
	}
	else
	{
		restoreIter(store);
		return 0;
	}
	return 1;
}
int Parser::arifmetic_op()
{
	auto store = storeIter();
	if (arifmetic_part()) {}
	else
	{
		restoreIter(store);
		return 0;
	}
	while (true)
	{
		store = storeIter();
		if ((BINAR_OP() || EQUAL_OP()) && arifmetic_part()) {}
		else
		{
			restoreIter(store);
			std::cout << "!-" << (*TokenIterator)->value;
			break;
		}
	}
	return 1;
}

int Parser::block()
{
	auto store = storeIter();
	if (BRACKET_L_FIG()) {}
	else
		return 0;

	while (true)
	{
		if (BRACKET_R_FIG())
			break;

		if (expr()) {}
		else
		{
			restoreIter(store);
			return 0;
		}
	}
	return 1;
}


int Parser::VAR()
{
	return checkToken((*TokenIterator)->type == TokensEnum::NAME, "VAR");
}
int Parser::ASSIGN()
{
	return checkToken((*TokenIterator)->type == TokensEnum::OP_ASSIGN, "ASSIGN");
}
int Parser::NUMBER()
{
	return checkToken((*TokenIterator)->type == TokensEnum::NUMBER ||
					  (*TokenIterator)->type == TokensEnum::NUMBER_F ||
					  (*TokenIterator)->type == TokensEnum::NUMBER_WITH_LIT, "NUMBER");
}
int Parser::STRING()
{
	return checkToken((*TokenIterator)->type == TokensEnum::STRING, "STRING");
}

int Parser::BINAR_OP()
{
	return checkToken((*TokenIterator)->type == TokensEnum::OP_BINAR, "BINAR_OP");
}
int Parser::EQUAL_OP()
{
	return checkToken((*TokenIterator)->type == TokensEnum::OP_EQUAL, "EQUAL_OP");
}
int Parser::UNAR_OP()
{
	std::cout << "\n" << (*TokenIterator)->value;
	return checkToken((*TokenIterator)->type == TokensEnum::OP_UNAR, "UNAR_OP");
}
int Parser::UNAR_BINAR_OP()
{
	return checkToken((*TokenIterator)->type == TokensEnum::OP_BINAR && ((*TokenIterator)->value == "-" ||
																		 (*TokenIterator)->value == "+"), "UN_BIN_OP");
}


int Parser::IF()
{
	return checkToken((*TokenIterator)->type == TokensEnum::KEYWORD &&
					  (*TokenIterator)->value == "if", "IF");
}
int Parser::ELIF()
{
	return checkToken((*TokenIterator)->type == TokensEnum::KEYWORD &&
					  (*TokenIterator)->value == "elif", "ELIF");
}
int Parser::ELSE()
{
	return checkToken((*TokenIterator)->type == TokensEnum::KEYWORD &&
					  (*TokenIterator)->value == "else", "ELSE");
}

int Parser::WHILE()
{
	return checkToken((*TokenIterator)->type == TokensEnum::KEYWORD &&
					  (*TokenIterator)->value == "while", "WHILE");
}

int Parser::BRACKET_L_FIG()
{
	return checkToken((*TokenIterator)->type == TokensEnum::BRACKET &&
					  (*TokenIterator)->value == "{", "");
}
int Parser::BRACKET_R_FIG()
{
	return checkToken((*TokenIterator)->type == TokensEnum::BRACKET &&
					  (*TokenIterator)->value == "}", "");
}
int Parser::BRACKET_L_RND()
{
	return checkToken((*TokenIterator)->type == TokensEnum::BRACKET &&
					  (*TokenIterator)->value == "(", "");
}
int Parser::BRACKET_R_RND()
{
	return checkToken((*TokenIterator)->type == TokensEnum::BRACKET &&
					  (*TokenIterator)->value == ")", "");
}
int Parser::BRACKET_L_SQR()
{
	return checkToken((*TokenIterator)->type == TokensEnum::BRACKET &&
					  (*TokenIterator)->value == "[", "");
}
int Parser::BRACKET_R_SQR()
{
	return checkToken((*TokenIterator)->type == TokensEnum::BRACKET &&
					  (*TokenIterator)->value == "]", "");
}

int Parser::SEMICOLON()
{
	return checkToken((*TokenIterator)->type == TokensEnum::SPLITTER &&
					  (*TokenIterator)->value == ";", "");
}
int Parser::COMMA()
{
	return checkToken((*TokenIterator)->type == TokensEnum::SPLITTER &&
					  (*TokenIterator)->value == ",", "");
}