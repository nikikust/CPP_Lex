#include <iostream>

#include "DSL_Lexer.h"
#include "DSL_Parser.h"

#include <chrono>
#include <windows.h>

int main(void)
{
	std::vector<std::unique_ptr<Token>> Tokens;

	int counts = 1;
	auto t0 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	for (int i = 0; i < counts; i++)
	{
		Tokens.clear();
		Tokens.reserve(250);
		getTokens(Tokens, "sample_code.yapl");
	}
	auto t1 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::cout << "Time: " << (t1 - t0) / 1000.0 / counts << std::endl;
	Sleep(2000);

	
	int abc[11] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	/*
	if (!getTokens(Tokens, "sample_code.yapl"))
	{
		std::cout << "\nLexer error.";
		return 1;
	}
	*/
	std::cout << "\nTokens list (" << Tokens.size() << "):\n";
	for (auto& token : Tokens)
	{
		switch (token->type)
		{
		case TokensEnum::BRACKET:
			std::cout << "< BRACKET >\t< ";
			abc[0]++;
			break;
		case TokensEnum::NAME:
			std::cout << "< NAME >\t< ";
			abc[1]++;
			break;
		case TokensEnum::KEYWORD:
			std::cout << "< KEYWORD >\t< ";
			abc[9]++;
			break;
		case TokensEnum::TYPE:
			std::cout << "< TYPE >\t< ";
			abc[10]++;
			break;
		case TokensEnum::NUMBER_WITH_LIT:
			std::cout << "< NUM_W_LIT >\t< ";
			abc[2]++;
			break;
		case TokensEnum::NUMBER:
			std::cout << "< NUMBER >\t< ";
			abc[3]++;
			break;
		case TokensEnum::OP_BINAR:
			std::cout << "< OP_BINAR >\t< ";
			abc[4]++;
			break;
		case TokensEnum::OP_FAST:
			std::cout << "< OP_FAST >\t< ";
			abc[5]++;
			break;
		case TokensEnum::OP_UNAR:
			std::cout << "< OP_UNAR >\t< ";
			abc[6]++;
			break;
		case TokensEnum::SPLITTER:
			std::cout << "< SPLITTER >\t< ";
			abc[7]++;
			break;
		case TokensEnum::STRING:
			std::cout << "< STRING >\t< ";
			abc[8]++;
			break;
		default:
			std::cout << "< UNKNOWN >\t< ";
			break;
		}
		std::cout << token->value << " >\n";
	}
	std::cout << "Total: " << Tokens.size();
	
	for (int i = 0; i < 11; i++)
		std::cout << "\nType: " << i << " : " << abc[i];

	return 0;
}