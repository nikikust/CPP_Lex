#include <boost/regex.hpp>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <memory>

#include "Lexer.h"

static boost::regex REGEX_3000(
	"([\\[\\]\\{\\}\\(\\);,])|\
([0-9]+[A-Za-z_][A-Za-z0-9_]*)|\
((?:[0-9]+\\.[0-9]*)|(?:\\.[0-9]+))|\
([0-9]+)|\
([A-Za-z_][A-Za-z0-9_]*)|\
([!<>=]=)|\
([\\-+*\\/\\^]=|=)|\
(\\+\\+|\\-\\-)|\
(\\*\\*|[\\-+*\\/\\^.]|<<|>>|[<>])|\
(\"(?:\\\\.|[^\"\\\\])*\")|\
(\\n)|\
([^\\s]+)"
);

std::string Keywords("|print|input|for|while|do|if|else|elif|continue|break|");

static std::unordered_set<std::string> Keywords2 = { "while", "do", "for", "if", "elif", "else", "input", "print" };


int getTokens(std::vector<std::shared_ptr<Token>>& Tokens, std::string fileName)
{
	size_t curent_line = 1, curent_line_pos = 1;

	std::ifstream prog(fileName);
	std::string data((std::istreambuf_iterator<char>(prog)),
					  std::istreambuf_iterator<char>());
	prog.close();

	for (boost::sregex_iterator it = boost::sregex_iterator(data.begin(), data.end(), REGEX_3000); it != boost::sregex_iterator(); it++) {
		boost::smatch match = *it;
		if (!match.str(match.size() - 1).empty())
		{
			std::cout << "\nUnknown lexem at (" << curent_line << ", " << (match.position() - curent_line_pos + 1) << "): '" << match.str() << "'"<< std::endl;
			Tokens.clear();
			return 0;
		}

		for (int i = 1; i < match.size(); i++)
		{
			if (!match.str(i).empty())
			{
				size_t pos = match.position() - curent_line_pos + 1;
				if (static_cast<TokensEnum>(i) == TokensEnum::NEWLINE)
				{
					++curent_line;
					curent_line_pos = match.position() + 1;
				}
				else if (static_cast<TokensEnum>(i) == TokensEnum::NAME &&
						Keywords.find("|" + match.str(i) + "|") != std::string::npos)
					Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::KEYWORD, match.str(i), curent_line, pos)));
				else
					Tokens.push_back(std::move(std::make_shared<Token>(static_cast<TokensEnum>(i), match.str(i), curent_line, pos)));
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
	case TokensEnum::PUNCTUATOR:		return "PUNCTUATOR";
	case TokensEnum::NAME:				return "   NAME   ";
	case TokensEnum::KEYWORD:			return " KEYWORD  ";
	case TokensEnum::NUMBER_WITH_LIT:	return "NUM_W_LIT ";
	case TokensEnum::NUMBER:			return "  NUMBER  ";
	case TokensEnum::NUMBER_F:			return " NUMBER_F ";
	case TokensEnum::OP_EQUAL:			return " OP_EQUAL ";
	case TokensEnum::OP_ASSIGN:			return "OP_ASSIGN ";
	case TokensEnum::OP_BINAR:			return " OP_BINAR ";
	case TokensEnum::OP_UNAR:			return " OP_UNAR  ";
	case TokensEnum::STRING:			return "  STRING  ";
	default:							return " UNKNOWN  ";
	}
}