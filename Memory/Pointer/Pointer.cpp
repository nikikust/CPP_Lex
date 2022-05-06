#include "Pointer.h"


std::string Pointer::getType()
{
	return "pointer<" + this->storingType + ">";
}
std::string Pointer::str()
{
	return "ptr<" + storingType + "> " + name;// +" -> " + data->getName();
}

void Pointer::set(std::shared_ptr<Coin> new_data, std::string storingType)
{
	this->data = new_data;
	this->storingType = storingType;
}
std::shared_ptr<Coin> Pointer::get()
{
	return data;
}
std::string Pointer::getStoringType()
{
	return storingType;
}
