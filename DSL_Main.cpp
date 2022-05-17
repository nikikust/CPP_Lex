#include <iostream>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

#include "profile.h"

#define _counters
//#define _output

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Rus");
	
	std::string input = "sample_code.yapl";
	if (argc > 1)
	{
		input = static_cast<std::string>(argv[1]);

		if (strlen(argv[1]) <= 5
			|| (strlen(argv[1]) >= 6
				&& argv[1][strlen(argv[1]) - 1] != 'l'
				&& argv[1][strlen(argv[1]) - 2] != 'p'
				&& argv[1][strlen(argv[1]) - 3] != 'a'
				&& argv[1][strlen(argv[1]) - 4] != 'y'
				&& argv[1][strlen(argv[1]) - 5] != '.'
				)
			)
		{
			std::cout << colorText(31) << "\n<< Usage: DSL [source].yapl >>\n" << colorText();
			return 1;
		}
	}


	std::vector<std::shared_ptr<Token>> Tokens;
	{
#ifdef _counters
		auto a = LogDuration("Lexing time: ");
#endif
		if (!getTokens(Tokens, input))
		{
			std::cout << colorText(31) << "!Lexer error!\n" << colorText();
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
#ifdef _counters
		auto a = LogDuration("Parsing time: ");
#endif
		parse_result = parser.lang(Tokens);
	}   // Average time for Lex+Parse - 131.34mks, 190mks

#ifdef _output
	std::cout << "\n\nTokens list after parse (amount: " << Tokens.size() << "):";
	for (auto& token : Tokens)
	{
		std::cout << "\n< " << getName(token->type) << " >\t< ";
		std::cout << ((token->type != TokensEnum::NEWLINE) ? token->value : "\\n") << " >\t";
		std::cout << token->gotIn;
	}
	std::cout << "\n\nParse result: " << (parse_result ? colorText(32) + "code is correct" : colorText(31) + "mistake found") << colorText();
#endif

	if (parse_result)
	{
#ifdef _output
		std::cout << "\n\n---------------------------------------------------------\nAbstract Syntax Tree:\n";
		parser.printTree();

		std::cout << "\n---------------------------------------------------------\nReverse Polish Notation:\n\n";
#endif
		RPN_result = parser.formRPN();
		if (RPN_result)
		{
#ifdef _output
			parser.printRPN();
			parser.showVars();
			std::cout << "\n\n\nFunctions:";
			parser.showFunctions();
			std::cout << "\n\n\nClasses:";
			parser.showClasses();
#endif
			{
#ifdef _counters
				std::cout << "\n\n";
				auto a = LogDuration("Saving time: ");
#endif
				parser.saveData();
			}
		}
	}

	if (parse_result && RPN_result)
		std::cout << "\n\n" << colorText(32) << "Code generated successfuly!" << colorText();
	else
		std::cout << "\n\n" << colorText(31) << "Code generation is failed!" << colorText();


	std::cout << "\n\nSend any key: "; int PressAnyKey;
	std::cin >> PressAnyKey; // Press Any Key :)

	return 0; 
}