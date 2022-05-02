#include "RPN_Element.h"


std::string RPN_Element::getValue()
{
	return this->value;
}
size_t RPN_Element::getLine()
{
	return this->line;
}
size_t RPN_Element::getPosition()
{
	return this->position;
}
TokenType RPN_Element::getType()
{
	return this->type;
}
size_t RPN_Element::getJumper()
{
	return this->jumper;
}
