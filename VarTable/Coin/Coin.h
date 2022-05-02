#pragma once
#include <unordered_map>
#include <memory>

#include "../../Token/Token.h"


class Coin
{
public:
	enum class CoinType
	{
		VOID,
		VAR,
		FUNCTION,
		CLASS,
		OBJECT
	};

	Coin(std::string i_name, CoinType i_type, size_t i_namespaceID) : name(i_name), type(i_type), namespaceID(i_namespaceID) {}
	virtual ~Coin() = default;

	std::string getType();
	std::string getCoinName();

protected:
	std::string name;
	CoinType type;
	size_t namespaceID;
	size_t linksToMe = 0;	// Unused now

};


class CoinVar : public Coin
{
public:
	enum class VarType
	{
		INT,
		DOUBLE,
		BOOL,
		STRING,
		POINTER
	};

	CoinVar(std::string name, VarType type, size_t namespaceID) : Coin(name, CoinType::VAR, namespaceID), varType(type) {}

	void set(int a) { INT = a; DOUBLE = a; BOOL = !!a; }
	void set(double a) { INT = a; DOUBLE = a; BOOL = !!a; }
	void set(bool a) { INT = a; DOUBLE = a; BOOL = a; }
	void set(std::string a) { STRING = a; }
	void set(std::shared_ptr<Coin> a) { POINTER = a; }

	bool get(int& a);
	bool get(double& a);
	bool get(bool& a);
	bool get(std::string& a);
	bool get(std::shared_ptr<Coin>& a);

	VarType getType();
	std::string str();

private:
	int INT = 0;
	double DOUBLE = 0;
	bool BOOL = false;
	std::string STRING = "";
	std::shared_ptr<Coin> POINTER = nullptr;

	VarType varType;
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
	std::string typeName = "class1";

public:
	std::string getType();
};


std::string getName(Coin::CoinType coin);
