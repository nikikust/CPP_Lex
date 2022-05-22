#include "Void.h"


std::string Void::getType()
{
	return "void";
}
std::string Void::str()
{
	return "void";
}

std::shared_ptr<Coin> Void::clone()
{
	Void res(name, Const);

	return std::make_shared<Void>(res);
}
