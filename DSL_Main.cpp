#include <iostream>

#include "DSL_Lexer.h"
#include "DSL_Parser.h"

int main(void)
{
	TokenNode Tree;

	std::vector<std::unique_ptr<Token>> Tokens;
	
	
	if (!getTokens(Tokens, "prog1.prog"))
	{
		std::cout << "\nLexer error.";
		return 1;
	}

	std::cout << "\nTokens list (" << Tokens.size() << "):\n";
	for (auto token : Tokens)
		std::cout << "< " << token.type << " >\t< " << token.value << " >\n";

	if (!buildTree(Tokens, Tree))
	{
		std::cout << "\nParser error.";
		return 2;
	}
	

	
	return 0;
}