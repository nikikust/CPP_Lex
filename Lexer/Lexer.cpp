#include <boost/regex.hpp>
#include <iostream>
#include <fstream>

#include "Lexer.h"
#include "../profile.h"

bool USE_COLORS = true;


static boost::regex REGEX_3000(
	"([\\[\\]\\{\\}\\(\\);,])|\
(\\/\\/[^\\n]*)|\
((?:[0-9]+\\.[0-9]*)|(?:\\.[0-9]+))|\
([0-9]+)|\
([A-Za-z_][A-Za-z0-9_]*)|\
(\"(?:\\\\.|\\\\\\n|[^\"\\\\\\n])*\")|\
(>(?:\\+\\+|\\-\\-)|(?:\\+\\+|\\-\\-)<|&&|(?:\\|\\|)|(?:<<|>>)=?|[.]|->|[\\-+*\\/&|\\\\^<>=!%]=?)|\
(\\n)|\
([^\\s]+)"
);

static std::string Keywords("|for|while|if|else|elif|continue|break|return|class|print|input|exit|template|");
static std::string SimpleTypes("|int|double|bool|string|void|");
static std::string TemplatedTypes("|ptr|list|map|");

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

	size_t pos = 0;
	boost::smatch match;
	for (boost::sregex_iterator it = boost::sregex_iterator(data.begin(), data.end(), REGEX_3000); it != boost::sregex_iterator(); it++) {
		match = *it;
		if (!match.str((int)match.size() - 1).empty())
		{
			std::cout << colorText(31) << "Unknown lexem at (" << curent_line << ", " << ((match.position() ? match.position() : 2) - curent_line_pos) << "): '" << match.str() << "'" << colorText() << std::endl;
			Tokens.clear();
			return 0;
		}

		for (int i = 1; i < match.size(); i++)
		{
			if (!match.str(i).empty())
			{
				pos = (match.position() ? match.position() : 2) - curent_line_pos;
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
					if(Keywords.find("|" + match.str(i) + "|") != std::string::npos)
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::KEYWORD, match.str(i), curent_line, pos)));
					else if (SimpleTypes.find("|" + match.str(i) + "|") != std::string::npos)
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::SIMPLETYPE, match.str(i), curent_line, pos)));
					else if (TemplatedTypes.find("|" + match.str(i) + "|") != std::string::npos)
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::TEMPLATEDTYPE, match.str(i), curent_line, pos)));
					else if (match.str(i) == "true" || match.str(i) == "false")
						Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::BOOL, (match.str(i) == "true")? "1" : "0", curent_line, pos)));
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
	if (pos == 0)
		std::cout << colorText(30, 43) << "File is empty!\n" << colorText();

	Tokens.push_back(std::move(std::make_shared<Token>(TokensEnum::_EOF, "EOF", curent_line, pos)));
	return 1;
}

void findAndReplaceAll(std::string& data, std::string toSearch, std::string replaceStr)
{
	size_t pos = data.find(toSearch);

	while (pos != std::string::npos)
	{
		data.replace(pos, toSearch.size(), replaceStr);
		pos = data.find(toSearch, pos + replaceStr.size());
	}
}


void enable_colors(bool x)
{
	USE_COLORS = x;
}
std::string colorText(int color, int background) // black, red, green, yellow, blue, purple, lightblue, white; 
{
	if (USE_COLORS)
		return "\x1B[" + std::to_string(color) + ";" + std::to_string(background) + "m";
	return "";
}
std::string colorText()
{
	if (USE_COLORS)
		return "\x1B[0m";
	return "";
}
