
#include <boost/program_options.hpp>
#include <iostream>

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Interpreter/Interpreter.h"

#include "profile.h"


namespace opt = boost::program_options;


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Rus");

	opt::options_description desc("All options");

	desc.add_options()
		("file_path,fp", opt::value<std::string>()->default_value("sample_code.yapl"), "path to compile file")
		("output_path,o", opt::value<std::string>()->default_value("prog.json"), "path to output file")
		("lexer_res,l", opt::value<bool>()->default_value(false), "show lexing results")
		("parser_res,p", opt::value<bool>()->default_value(false), "show parsing results")
		("counters,c", opt::value<bool>()->default_value(false), "show counters")
		("usecolors,u", opt::value<bool>()->default_value(false), "use colors for output")
		("help", "produce help message");

	opt::variables_map vm;

	opt::store(opt::parse_command_line(argc, argv, desc), vm);
	opt::notify(vm);

	if (vm.count("help")) {
		std::cout << desc << "\n";
		return 1;
	}

	enable_colors(vm["usecolors"].as<bool>());

	std::vector<std::shared_ptr<Token>> Tokens;

	if (vm["counters"].as<bool>())
	{
		auto a = LogDuration("Lexing time: ");
		if (!getTokens(Tokens, vm["file_path"].as<std::string>()))
		{
			std::cout << colorText(31) << "!Lexer error!\n" << colorText();
			return 1;
		}
	}
	else
	{
		if (!getTokens(Tokens, vm["file_path"].as<std::string>()))
		{
			std::cout << colorText(31) << "!Lexer error!\n" << colorText();
			return 1;
		}
	}

	// --- //
	if (vm["lexer_res"].as<bool>())
	{
		std::cout << "\nTokens list (amount: " << Tokens.size() << "):\n";
		for (auto& token : Tokens)
		{
			std::cout << "< " << getName(token->type) << " >\t< "
				<< ((token->type != TokensEnum::NEWLINE) ? token->value : "\\n") << " >\t | Line:Pos - "
				<< token->line << ":" << token->position << std::endl;
		}
		std::cout << "\nTotal: " << Tokens.size() << "\n---------------------------------------------------------\n";
	}
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

	if (vm["counters"].as<bool>())
	{
		auto a = LogDuration("Parsing time: ");
		parse_result = parser.lang(Tokens);
	}
	else
		parse_result = parser.lang(Tokens);

	if (vm["parser_res"].as<bool>())
	{
		std::cout << "\n\nTokens list after parse (amount: " << Tokens.size() << "):";
		for (auto& token : Tokens)
		{
			std::cout << "\n< " << getName(token->type) << " >\t< ";
			std::cout << ((token->type != TokensEnum::NEWLINE) ? token->value : "\\n") << " >\t";
			std::cout << token->gotIn;
		}
		std::cout << "\n\nParse result: " << (parse_result ? colorText(32) + "code is correct" : colorText(31) + "mistake found") << colorText();

		std::cout << "\n\n---------------------------------------------------------\nAbstract Syntax Tree:\n";
		parser.printTree();

		std::cout << "\n---------------------------------------------------------\nReverse Polish Notation:\n\n";
	}

	if (parse_result)
	{
		RPN_result = parser.formRPN();
		if (RPN_result)
		{
			if (vm["parser_res"].as<bool>())
			{
				parser.printRPN();
				parser.showVars();
				std::cout << "\n\n\nFunctions:";
				parser.showFunctions();
				std::cout << "\n\n\nClasses:";
				parser.showClasses();
			}
			if (vm["counters"].as<bool>())
			{
				std::cout << "\n\n";
				auto a = LogDuration("Saving time: ");
				parser.saveData(vm["output_path"].as<std::string>());
			}
			else
				parser.saveData(vm["output_path"].as<std::string>());
		}
	}

	if (parse_result && RPN_result)
	{
		std::cout << "\n\n" << colorText(32) << "Code generated successfuly!\n\n" << colorText();

		std::cout << "--- Interpreter ---\n";

		Interpreter interpreter;

		std::cout << "\nReady to start, send any key: "; std::string PressAnyKey;
		std::cin >> PressAnyKey; // Press Any Key :)
		std::cin.ignore();
		std::cout << std::string("\n") * 50;

		std::cout << colorText(36) << "\n--- !!! RUN !!! ---\n" << colorText();
		{
#ifdef _counters
			auto a = LogDuration("\n\n-----\nIntrepret time: ", true);
#endif
			interpreter.run();
		}
	}
	else
		std::cout << "\n\n" << colorText(31) << "Code generation is failed!\n\n" << colorText();


	std::cout << "\n\nSend any key: "; std::string PressAnyKey;
	std::cin >> PressAnyKey; // Press Any Key :)
	std::cin.ignore();

	return 0; 
}