#include "Token.h"

std::string getName(TokensEnum token)
{
	switch (token)
	{
	case TokensEnum::PUNCTUATOR:		return "PUNCTUATOR";
	case TokensEnum::COMMENT:			return " COMMENT  ";
	case TokensEnum::IDENTIFIER:		return "IDENTIFIER";
	case TokensEnum::DOUBLE:			return "  DOUBLE  ";
	case TokensEnum::INT:				return "   INT    ";
	case TokensEnum::BOOL:				return "   BOOL   ";
	case TokensEnum::STRING:			return "  STRING  ";
	case TokensEnum::OP_BINAR:			return " OP_BINAR ";
	case TokensEnum::NEWLINE:			return " NEW_LINE ";
	case TokensEnum::OP_UNAR_PREF:		return " OP_UNAR  ";
	case TokensEnum::OP_UNAR_POST:		return " OP_UNAR  ";
	case TokensEnum::KEYWORD:			return " KEYWORD  ";
	case TokensEnum::SIMPLETYPE:		return " SMP_TYPE ";
	case TokensEnum::TEMPLATEDTYPE:		return "TEMPL_TYPE";
	case TokensEnum::_EOF:				return "   EOF    ";
	default:							return " UNKNOWN  ";
	}
}
