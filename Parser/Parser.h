#pragma once
#include <string>
#include <vector>
#include <list>

#include "../Lexer/Lexer.h"


class Parser
{
	std::vector<std::shared_ptr<Token>> Tokens;
	std::vector<std::shared_ptr<Token>> ::iterator TokenIterator;

public:
	Parser();
	std::vector<std::shared_ptr<Token>> ::iterator getLastToken();

	std::vector<std::shared_ptr<Token>> ::iterator storeIter();
	void restoreIter(std::vector<std::shared_ptr<Token>> ::iterator iter);
	int checkToken(bool x, std::string type);

	int lang(std::vector<std::shared_ptr<Token>>& TokenVect);

	int expr();

	int assign_op();
	int if_op();
	int while_loop();

	int arifmetic_part();
	int arifmetic_op();

	int block();

	int VAR();
	int ASSIGN();
	int NUMBER();
	int STRING();

	int BINAR_OP();
	int EQUAL_OP();
	int UNAR_OP();
	int UNAR_BINAR_OP();

	int IF();
	int ELIF();
	int ELSE();
	int WHILE();
	
	int BRACKET_L_FIG(); // {
	int BRACKET_R_FIG(); // }
	int BRACKET_L_RND(); // (
	int BRACKET_R_RND(); // )
	int BRACKET_L_SQR(); // [
	int BRACKET_R_SQR(); // ]

	int SEMICOLON();
	int COMMA();
};