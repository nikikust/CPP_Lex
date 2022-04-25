#include <boost/regex.hpp>
#include <iostream>
#include <fstream>
#include <memory>

#include "Lexer.h"

static boost::regex REGEX_3000(
	"([\\[\\]\\{\\}\\(\\);,])|\
(\\/\\/[^\\n]*)|\
((?:[0-9]+\\.[0-9]*)|(?:\\.[0-9]+))|\
([0-9]+)|\
(true|false)|\
([A-Za-z_][A-Za-z0-9_]*)|\
(\"(?:\\\\.|\\\\\\n|[^\"\\\\\\n])*\")|\
(>(?:\\+\\+|\\-\\-)|(?:\\+\\+|\\-\\-)<|&&|(?:\\|\\|)|(?:<<|>>)=?|[.]|[\\-+*\\/&|\\\\^<>=!%]=?)|\
(\\n)|\
([^\\s]+)"
);

static std::string Keywords("|print|input|for|while|do|if|else|elif|continue|break|return|def|");
static std::string SimpleTypes("|int|float|double|long|bool|ptr|");

std::string operator*(std::string a, unsigned int b)
{
	std::string output = "";
	while (b--) 
		output += a;
	return output;
}
std::string& operator*=(std::string& a, unsigned int b)
{
	std::string output = "";
	while (b--) 
		output += a;
	a = output;
	return a;
}

int getTokens(tokenVect& Tokens, std::string fileName)
{
	size_t curent_line = 1, curent_line_pos = 1;

	std::ifstream prog(fileName);
	std::string data((std::istreambuf_iterator<char>(prog)),
					  std::istreambuf_iterator<char>());
	prog.close();

	boost::smatch match;
	for (boost::sregex_iterator it = boost::sregex_iterator(data.begin(), data.end(), REGEX_3000); it != boost::sregex_iterator(); it++) {
		match = *it;
		if (!match.str(match.size() - 1).empty())
		{
			std::cout << "\nUnknown lexem at (" << curent_line << ", " << (match.position() - curent_line_pos) << "): '" << match.str() << "'"<< std::endl;
			Tokens.clear();
			return 0;
		}

		for (int i = 1; i < match.size(); i++)
		{
			if (!match.str(i).empty())
			{
				size_t pos = match.position() - curent_line_pos + 1; 
				TokensEnum type = static_cast<TokensEnum>(i);

				if (type == TokensEnum::NEWLINE)
				{
					++curent_line;
					curent_line_pos = match.position();
				}
				else if (type == TokensEnum::STRING)
				{
					std::string buf(match.str(i));
					curent_line += std::count(buf.begin(), buf.end(), '\n');
					Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::STRING, buf, curent_line, pos)));
				}
				else if (type == TokensEnum::IDENTIFIER)
				{
					if(Keywords.find("|" + match.str(i) + "|") != std::string::npos || match.str(i) == "void")
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::KEYWORD, match.str(i), curent_line, pos)));
					else if (SimpleTypes.find("|" + match.str(i) + "|") != std::string::npos)
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::SIMPLETYPE, match.str(i), curent_line, pos)));
					else
						Tokens.push_back(std::move(std::make_shared<Token>(static_cast<TokensEnum>(i), match.str(i), curent_line, pos)));
				}
				else if (type == TokensEnum::OP_BINAR)
				{
					if (std::string("|++<|--<|").find("|" + match.str(i) + "|") != std::string::npos)
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::OP_UNAR_PREF, match.str(i), curent_line, pos))); 
					else if (std::string("|>++|>--|").find("|" + match.str(i) + "|") != std::string::npos)
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::OP_UNAR_POST, match.str(i), curent_line, pos)));
					else
						Tokens.push_back(std::move(std::make_shared<Token>(static_cast<TokensEnum>(i), match.str(i), curent_line, pos)));
				}
				else if (type != TokensEnum::COMMENT)
				{
					Tokens.push_back(std::move(std::make_shared<Token>(static_cast<TokensEnum>(i), match.str(i), curent_line, pos)));
				}
				break;
			}
		}
	}
	Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::_EOF, "EOF", curent_line, match.position() - curent_line_pos + 1)));
	return 1;
}

std::string getName(TokensEnum token)
{
	switch (token)
	{
	case TokensEnum::PUNCTUATOR:		return "PUNCTUATOR";
	case TokensEnum::COMMENT:			return " COMMENT  ";
	case TokensEnum::IDENTIFIER:		return "IDENTIFIER";
	case TokensEnum::FLOAT:				return "  FLOAT   ";
	case TokensEnum::INT:				return "   INT    ";
	case TokensEnum::BOOL:				return "   BOOL   ";
	case TokensEnum::STRING:			return "  STRING  ";
	case TokensEnum::OP_BINAR:			return " OP_BINAR ";
	case TokensEnum::NEWLINE:			return " NEW_LINE ";
	case TokensEnum::OP_UNAR_PREF:		return " OP_UNAR  ";
	case TokensEnum::OP_UNAR_POST:		return " OP_UNAR  ";
	case TokensEnum::KEYWORD:			return " KEYWORD  ";
	case TokensEnum::SIMPLETYPE:		return " SMP_TYPE ";
	case TokensEnum::_EOF:				return "   EOF    ";
	default:							return " UNKNOWN  ";
	}
}