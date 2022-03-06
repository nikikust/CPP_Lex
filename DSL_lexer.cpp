#include <iostream>
#include <fstream>
#include <memory>
#include <regex>
#include <map>

#include <chrono>

#include "DSL_Lexer.h"


int getTokens(std::vector<std::unique_ptr<Token>>& Tokens, std::string fileName)
{
	std::regex NAME("[A-Za-z_][A-Za-z0-9_]*");
	std::regex NUMBER("0|([1-9][0-9]*)");
	std::regex STRING("\"((\\\\.)|[^\"\\\\])*\"");
	std::regex CHAR("\\'[\\s\\d\\w]\\'"); // ?

	std::regex OP_UNAR("(\\+\\+)|(\\-\\-)");
	std::regex OP_BINAR("[=\\-+*\\/\\^]|(\\*\\*)|([!<>=]=)|(<<)|(>>)|[<>]");
	std::regex OP_FAST("[\\-+*\\/\\^]=");

	std::regex BRAKET("[\\[\\]\\{\\}\\(\\)]");
	std::regex SPLITTER("[,;]");

	//std::cmatch matches;

	std::map<std::string, std::regex> regexps
	{
		{"NAME", NAME},
		{"NUMBER", NUMBER},
		{"STRING", STRING},
		{"CHAR", CHAR},
		{"OP_UNAR", OP_UNAR},
		{"OP_BINAR", OP_BINAR},
		{"OP_FAST", OP_FAST},
		{"BRAKET", BRAKET},
		{"SPLITTER", SPLITTER}
	};

	// --- //

	std::ifstream programm(fileName);
	if (!programm.is_open())
	{
		std::cout << "\nUnable to open file.";
		Tokens.clear();
		return 1;
	}

	std::string line;
	unsigned short line_pos = 1;
	std::string candidate = "";
	bool findFirst = false, noCandidate = true;

	while(std::getline(programm, line))
	{
		for (int i = 0; i < line.size(); )
		{
			for (int k = i + 1; k <= line.size(); k++)
			{
				if (!findFirst)
				{
					if (k == i + 1 && (line.substr(i, 1) == " " || line.substr(i, 1) == "\t"))
					{
						i++;
						break;
					}
					else if (k > i + 1 && k == line.length())
					{
						programm.close();
						Tokens.clear();
						return 2;
					}
					else
					{
						for (auto& regexp : regexps)
						{
							if (std::regex_match(line.substr(i, k - i).c_str(), regexp.second))
							{
								candidate = regexp.first;
								findFirst = true;
								break;
							}
						}
					}
				}

				if (findFirst)
				{
					noCandidate = true;

					for (auto& regexp : regexps)
					{
						if (std::regex_match(line.substr(i, k - i).c_str(), regexp.second))
						{
							candidate = regexp.first;
							noCandidate = false;
							break;
						}
					}
					
					if (noCandidate)
					{
						Tokens.push_back(std::move(std::make_unique<Token>(candidate, std::move(line.substr(i, k - i - 1)))));
						i = k - 1;
						break;
					} 
					else if(k == line.length())
					{
						Tokens.push_back(std::move(std::make_unique<Token>(candidate, std::move(line.substr(i, k - i)))));
						i = k;
						break;
					}
				}
			}
			findFirst = false;
		}
		line_pos++;
		
	}

	programm.close();
	return 0;
}