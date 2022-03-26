#include <iostream>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#define _DEBUG 1

#if _DEBUG
#include <chrono>
#include <windows.h>
#endif

int main(void)
{
	std::vector<std::shared_ptr<Token>> Tokens;
	
	auto t0 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	if (!getTokens(Tokens, "sample_code.yapl"))
	{
		std::cout << "\nLexer error.";
		return 1;
	}
	auto t1 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::cout << "Lex TIME: " << (t1 - t0) / 1000.;
	//Sleep(2000);
	
	std::cout << "\nTokens list (" << Tokens.size() << "):\n";

	for (auto& token : Tokens)
	{
		std::cout << "< " << getName(token->type) << " >\t< ";
		std::cout << token->value << " >\n";
	}
	
	std::cout << "\nTotal: " << Tokens.size();

	Parser parser;
	
	t0 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	int res = parser.lang(Tokens);
	t1 = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	std::cout << "\nParse result: " << res;
	std::cout << "\nParse TIME: " << (t1 - t0) / 1000.;

	std::cout << "\n\nTokens list 2 (" << Tokens.size() << "):";

	for (auto& token : Tokens)
	{
		std::cout << "\n< " << getName(token->type) << " >\t< ";
		std::cout << token->value << " >\t";
		std::cout << token->gotIn;
	}
	Sleep(2000);
	return 0;
}