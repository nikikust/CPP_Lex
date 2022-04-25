#pragma once
#include <unordered_map>

#include "../../Lexer/Lexer.h"

enum class CoinType
{
	INT,
	DOUBLE,
	BOOL,
	STRING,
	POINTER,
	VOID,
	FUNCTION,
	CLASS,
	OBJECT
};

class Coin
{
protected:
	std::string name;
	CoinType type;
	long namespaceID = 0; // Unused now

public:
	Coin(std::string i_name, CoinType i_type) : name(i_name), type(i_type) {}

	CoinType getType() { return type; }
};


class CoinVar : protected Coin
{
	int INT = 0;
	double DOUBLE = 0.;
	bool BOOL = false;
	std::string STRING = "";
	std::shared_ptr<Coin> POINTER = nullptr;

public:
	CoinVar(std::string name, CoinType type) : Coin(name, type) {}

	void setINT(int a) { INT = a; DOUBLE = a; BOOL = !!a; }
	void setDOUBLE (double a) { INT = a; DOUBLE = a; BOOL = !!a; }
	void setBOOL(bool a) { INT = a; DOUBLE = a; BOOL = a; }
	void setSTRING(std::string a) { STRING = a; }
	void setPOINTER(std::shared_ptr<Coin> a) { POINTER = a; }

	bool getINT(int& a);
	bool getDOUBLE(double& a);
	bool getBOOL(bool& a);
	bool getSTRING(std::string& a);
	bool getPOINTER(std::shared_ptr<Coin>& a);
};

class CoinFunction : protected Coin
{
	CoinType returnValue;
	std::unordered_map<std::string, CoinType> attributesSpec;

public:
	bool call(std::vector<Coin> attributes);
};

class CoinClass : protected Coin
{

};

class CoinObject : protected Coin
{

};

