#include <iostream>
#include <fstream>
#include <memory>

#include <chrono>
#include <boost/regex.hpp>

#include "DSL_Lexer.h"

boost::regex REGEX_3000(
	"([\\[\\]\\{\\}\\(\\)])|\
((?:0|(?:[1-9][0-9]*))[A-Za-z_][A-Za-z0-9_]*)|\
(0|(?:[1-9][0-9]*))|\
([A-Za-z_][A-Za-z0-9_]*)|\
([,;])|\
(\\+\\+|\\-\\-)|\
(\\*\\*|[!<>=]=|[=\\-+*\\/\\^.]|<<|>>|[<>])|\
([\\-+*\\/\\^]=)|\
(\"(?:\\\\.|[^\"\\\\])*\")|\
([^\\s]+)"
);
std::string Keywords("|print|input|for|while|do|if|else|elif|continue|break|");
std::string Types("|void|int|float|double|long|string|");

int getTokens(std::vector<std::unique_ptr<Token>>& Tokens, std::string fileName)
{
	std::ifstream prog(fileName);
	std::string data((std::istreambuf_iterator<char>(prog)),
					  std::istreambuf_iterator<char>());
	prog.close();

	for (boost::sregex_iterator it = boost::sregex_iterator(data.begin(), data.end(), REGEX_3000); it != boost::sregex_iterator(); it++) {
		boost::smatch match = *it;

		if (!match.str(match.size() - 1).empty())
		{
			std::ifstream prog_wrong_line(fileName);
			long line = 1, pos = 0; 
			std::string buf;
			while (pos < match.position())
			{
				std::getline(prog_wrong_line, buf);
				pos += buf.size() + 1;
				line++;
			}
			prog_wrong_line.close();

			std::cout << "\nUnknown lexem at (" << line << "," << (match.position() + buf.size() - pos + 1) << "): '" << match.str() << "'" << std::endl;
			Tokens.clear();
			return 0;
		}

		for (int i = 1; i < match.size(); i++)
		{
			if (!match.str(i).empty())
			{
				if (i == 4 && Keywords.find("|" + match.str(i) + "|") != std::string::npos)
					Tokens.push_back(std::move(std::make_unique<Token>(TokensEnum::KEYWORD, match.str(i))));
				else if (i == 4 && Types.find("|" + match.str(i) + "|") != std::string::npos)
					Tokens.push_back(std::move(std::make_unique<Token>(TokensEnum::TYPE, match.str(i))));
				else
					Tokens.push_back(std::move(std::make_unique<Token>(static_cast<TokensEnum>(i), match.str(i))));
				break;
			}
		}
	}

	return 1;
}