#include <iostream>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#include "profile.h"


int main(void)
{
	setlocale(LC_ALL, "Rus");
	std::vector<std::shared_ptr<Token>> Tokens;
	
	{
		auto a = LogDuration("Lexing time: ");
		if (!getTokens(Tokens, "sample_code.yapl"))
		{
			std::cout << "\x1B[31m!Lexer error!\033[0m\n";
			return 1;
		}
	}

	// --- //
	/*
	std::cout << "\nTokens list (amount: " << Tokens.size() << "):\n";
	for (auto& token : Tokens)
	{
		std::cout << "< " << getName(token->type) << " >\t< "
						  << ((token->type != TokensEnum::NEWLINE) ? token->value : "\\n") << " >\t | Line:Pos - "
						  << token->line << ":" << token->position << std::endl;
	}
	std::cout << "\nTotal: " << Tokens.size() << "\n---------------------------------------------------------\n";
	*/
	// --- //
	
	//	   __________.
	//	  /         /|
	//	 /         / |
	//	/_________/  |
	//	|         |  |
	//	|         |  /
	//	|         | /
	//	|_________|/
	//	

	Parser parser;
	bool parse_result;
	bool RPN_result;
	{
		auto a = LogDuration("Parsing time: ");
		parse_result = parser.lang(Tokens);
	}   // Average time for Lex+Parse - 131.34mks, 190mks

	std::cout << "\n\nTokens list after parse (amount: " << Tokens.size() << "):";
	for (auto& token : Tokens)
	{
		std::cout << "\n< " << getName(token->type) << " >\t< ";
		std::cout << ((token->type != TokensEnum::NEWLINE)? token->value : "\\n") << " >\t";
		std::cout << token->gotIn;
	}
	
	std::cout << "\n\nParse result: " << (parse_result ? colorText(32) + "code is correct" : colorText(31) + "mistake found") << colorText();
	
	if (parse_result)
	{
		std::cout << "\n\n---------------------------------------------------------\nAbstract Syntax Tree:\n";
		parser.printTree();

		std::cout << "\n---------------------------------------------------------\nReverse Polish Notation:\n\n";
		RPN_result = parser.formRPN();
		if (RPN_result)
		{
			parser.printRPN();
			parser.showVars();
		}
	}

	if (parse_result && RPN_result)
		std::cout << colorText(32) << "\n\nCode generated successfuly!" << colorText();
	else
		std::cout << colorText(31) << "\n\nCode generation is failed!" << colorText();


	std::cout << "\n\nSend any key: "; int PressAnyKey;
	std::cin >> PressAnyKey; // Press Any Key :)
	return 0; 
}
