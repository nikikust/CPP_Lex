#include "Coin.h"


bool CoinVar::getINT(int& a)
{
	switch (this->getType())
	{
	case CoinType::INT:
		a = INT;
		return true;
	case CoinType::DOUBLE:
		a = DOUBLE;
		return true;
	case CoinType::BOOL:
		a = BOOL;
		return true;
	case CoinType::STRING:
		a = std::stoi(STRING);
		return true;
	}
	return false;
}
bool CoinVar::getDOUBLE(double& a)
{
	switch (this->getType())
	{
	case CoinType::INT:
		a = INT;
		return true;
	case CoinType::DOUBLE:
		a = DOUBLE;
		return true;
	case CoinType::BOOL:
		a = BOOL;
		return true;
	case CoinType::STRING:
		a = std::stod(STRING);
		return true;
	}
	return false;
}
bool CoinVar::getBOOL(bool& a)
{
	switch (this->getType())
	{
	case CoinType::INT:
		a = INT;
		return true;
	case CoinType::DOUBLE:
		a = DOUBLE;
		return true;
	case CoinType::BOOL:
		a = BOOL;
		return true;
	case CoinType::STRING:
		a = (bool)std::stoi(STRING);
		return true;
	}
	return false;
}
bool CoinVar::getSTRING(std::string& a)
{
	switch (this->getType())
	{
	case CoinType::INT:
		a = std::to_string(INT);
		return true;
	case CoinType::DOUBLE:
		a = std::to_string(DOUBLE);
		return true;
	case CoinType::BOOL:
		a = std::to_string(BOOL);
		return true;
	case CoinType::STRING:
		a = STRING;
		return true;
	}
	return false;
}
bool CoinVar::getPOINTER(std::shared_ptr<Coin>& a)
{
	if (this->getType() == CoinType::POINTER && POINTER != nullptr)
	{
		a = POINTER;
		return true;
	}
	return false;
}

bool CoinFunction::call(std::vector<Coin> attributes)
{
	return false;
}

