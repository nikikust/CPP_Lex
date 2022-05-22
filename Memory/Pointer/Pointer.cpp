#include "Pointer.h"


Pointer::~Pointer()
{
	data = nullptr;
}

std::string Pointer::getType()
{
	return "ptr<" + this->storingType + ">";
}
std::string Pointer::getStoringType()
{
	return storingType;
}
std::string Pointer::str()
{
	return ((Const) ? "const ptr<" : "ptr<") + storingType + "> " + name;// +" -> " + data->getName();
}

void Pointer::set(std::shared_ptr<Coin> new_data, std::string storingType)
{
	this->data = new_data;
	if (storingType != "")
		this->storingType = storingType;
}
std::shared_ptr<Coin> Pointer::get()
{
	return data;
}

json Pointer::to_json_edt()
{
	json j =
	{
		{"CoinType", type},
		{"name", name},
		{"ID", ID},
		{"Const", Const},
		{"data",
			{
				{"storingType", storingType}
			}
		}
	};

	return j;
}

std::shared_ptr<Coin> Pointer::clone()
{
	Pointer res(name, storingType, Const);

	return std::make_shared<Pointer>(res);
}

