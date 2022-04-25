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
	setlocale(LC_ALL, "Rus");
	std::vector<std::shared_ptr<Token>> Tokens;
	
	auto t0 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	if (!getTokens(Tokens, "sample_code.yapl"))
	{
		std::cout << "\nLexer error.";
		return 1;
	}
	auto t1 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::cout << "Lex TIME: " << (t1 - t0) / 1000.;

	// --- //
	
	std::cout << "\nTokens list (" << Tokens.size() << "):\n";
	for (auto& token : Tokens)
	{
		std::cout << "< " << getName(token->type) << " >\t< "
						  << ((token->type != TokensEnum::NEWLINE) ? token->value : "\\n") << " >\t | Line:Pos - "
						  << token->line << ":" << token->position << std::endl;
	}
	std::cout << "\nTotal: " << Tokens.size() << "\n---------------------------------------------------------\n";
	
	///
	
	Parser parser;
	
	t0 = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	int res = parser.lang(Tokens);
	t1 = std::chrono::high_resolution_clock::now().time_since_epoch().count(); // Average time for Lex+Parse - 131.34mks

	std::cout << "\nParse result: " << res;
	std::cout << "\nParse TIME: " << (t1 - t0) / 1000.;

	std::cout << "\n\nTokens list 2 (" << Tokens.size() << "):";
	for (auto& token : Tokens)
	{
		std::cout << "\n< " << getName(token->type) << " >\t< ";
		std::cout << ((token->type != TokensEnum::NEWLINE)? token->value : "\\n") << " >\t";
		std::cout << token->gotIn;
	}
	
	std::cout << "\n\n---------------------------------------------------------\nAbstract Syntax Tree:\n|";
	parser.printTree();

	std::cout << "\n---------------------------------------------------------\nReverse Polish Notation:\n\n";
	parser.formRPN();
	parser.printRPN();

	std::cout << "\n\nSend any key: ";
	std::cin >> t0; // Press Any Key :)
	return 0; 
}