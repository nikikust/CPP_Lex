#include "Numeric.h"


void Numeric::set(double new_data)
{
	switch (numericType)
	{
	case NumType::INT:
		this->data = (int)new_data;
		break;
	case NumType::DOUBLE:
		this->data = new_data;
		break;
	case NumType::BOOL:
		this->data = (bool)new_data;
		break;
	default:
		this->data = new_data;
	}
}

std::string Numeric::getType()
{
	switch (numericType)
	{
	case NumType::INT:		return "int";
	case NumType::DOUBLE:	return "double";
	case NumType::BOOL:		return "bool";
	default:				return "error-type";
	};
}
std::string Numeric::str()
{
	switch (numericType)
	{
	case NumType::INT:		return ((Const) ? "const int " : "int " ) + name + " = " + std::to_string((int)data);
	case NumType::DOUBLE:	return ((Const) ? "const double " : "double ") + name + " = " + std::to_string(data);
	case NumType::BOOL:		return ((Const) ? "const bool " : "bool ") + name + " = " + std::to_string((bool)data);
	default:				return "error-type " + name + " = " + std::to_string(data);
	}
}
int Numeric::getInt()
{
	return (int)data;
}
double Numeric::get()
{
	return data;
}

json Numeric::to_json_edt()
{
	json j =
	{
		{"CoinType", type},
		{"name", name},
		{"ID", ID},
		{"Const", Const},
		{"data", 
			{
				{"data", data},
				{"NumType", numericType}
			}
		}
	};
	return j;
}

std::shared_ptr<Coin> Numeric::clone()
{
	Numeric res(name, numericType, Const);
	res.data = data;

	return std::make_shared<Numeric>(res);
}
