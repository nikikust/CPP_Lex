#include <iostream>

#include "DSL_Lexer.h"
#include "DSL_Parser.h"

#include <chrono>

int main(void)
{
	TokenNode Tree;

	std::vector<std::unique_ptr<Token>> Tokens;
	
	auto t0 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	for (int i = 0; i < 50; i++)
	{
		std::vector<std::unique_ptr<Token>> Tokenss;
		getTokens(Tokenss, "sample_code.yapl");
	}
	float end = clock();

	std::cout << "Time: " << (std::chrono::high_resolution_clock::now().time_since_epoch().count() - t0) / 1000000.0 / 50.;

	/*
	if (!getTokens(Tokens, "prog1.prog"))
	{
		std::cout << "\nLexer error.";
		return 1;
	}

	std::cout << "\nTokens list (" << Tokens.size() << "):\n";
	for (auto& token : Tokens)
		std::cout << "< " << token->type << " >\t< " << token->value << " >\n";

	if (!buildTree(Tokens, Tree))
	{
		std::cout << "\nParser error.";
		return 2;
	}
	*/

	
	return 0;
}