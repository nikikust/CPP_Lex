#include "String.h"


std::string String::getType()
{
	return "string";
}
std::string String::str()
{
	return  ((Const) ? "const string " : "string ") + name + " = " + data;
}

void String::set(std::string new_data)
{
	this->data = new_data;
}
std::string String::get()
{
	return data;
}

json String::to_json_edt()
{
	json j =
	{
		{"CoinType", type},
		{"name", name},
		{"ID", ID},
		{"Const", Const},
		{"data", 
			{
				{"data", data}
			}
		}
	};

	return j;
}