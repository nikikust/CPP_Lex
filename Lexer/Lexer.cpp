#include <boost/regex.hpp>
#include <iostream>
#include <fstream>
#include <memory>

#include "Lexer.h"

boost::regex REGEX_3000(
	"([\\[\\]\\{\\}\\(\\)])|\
([0-9]+[A-Za-z_][A-Za-z0-9_]*)|\
((?:[0-9]+\\.[0-9]*)|(?:\\.[0-9]+))|\
([0-9]+)|\
([A-Za-z_][A-Za-z0-9_]*)|\
([,;])|\
([!<>=]=)|\
([\\-+*\\/\\^]=|=)|\
(\\+\\+|\\-\\-)|\
(\\*\\*|[\\-+*\\/\\^.]|<<|>>|[<>])|\
(\"(?:\\\\.|[^\"\\\\])*\")|\
([^\\s]+)"
);

int getTokens(std::vector<std::shared_ptr<Token>>& Tokens, std::string fileName)
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
				++line;
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
				Tokens.push_back(std::move(std::make_shared<Token>(static_cast<TokensEnum>(i), match.str(i))));
				break;
			}
		}
	}
	return 1;
}

std::string getName(TokensEnum token)
{
	switch (token)
	{
	case TokensEnum::BRACKET:			return "BRACKET";
	case TokensEnum::NAME:				return "NAME";
	case TokensEnum::NUMBER_WITH_LIT:	return "NUM_W_LIT";
	case TokensEnum::NUMBER:			return "NUMBER";
	case TokensEnum::NUMBER_F:			return "NUMBER_F";
	case TokensEnum::OP_EQUAL:			return "OP_EQUAL";
	case TokensEnum::OP_ASSIGN:			return "OP_ASSIGN";
	case TokensEnum::OP_BINAR:			return "OP_BINAR";
	case TokensEnum::OP_UNAR:			return "OP_UNAR";
	case TokensEnum::SPLITTER:			return "SPLITTER";
	case TokensEnum::STRING:			return "STRING";
	default:							return "UNKNOWN";
	}
}