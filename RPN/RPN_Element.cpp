#include "RPN_Element.h"


std::string RPN_Element::getValue()
{
	return this->value;
}
std::string RPN_Element::getSubValue()
{
	return this->sub_value;
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



void to_json(json& j, const RPN_Element& p)
{
	j = json{ 
		{"value", p.value},
		{"sub_value", p.sub_value},
		{"line", p.line},
		{"position", p.position},
		{"type", p.type},
		{"jumper", p.jumper}
	};
}
void from_json(const json& j, RPN_Element& p)
{
	j.at("value").get_to(p.value);
	j.at("sub_value").get_to(p.sub_value);
	j.at("line").get_to(p.line);
	j.at("position").get_to(p.position);
	j.at("type").get_to(p.type);
	j.at("jumper").get_to(p.jumper);
}