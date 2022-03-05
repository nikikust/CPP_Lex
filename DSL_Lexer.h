#pragma once
#include <string>
#include <vector>
#include <memory>

struct Token {
	std::string type;
	std::string value;

	Token(std::string i_type, std::string i_value) : type(i_type), value(i_value) {}
};

std::vector<std::unique_ptr<Token>> getTokens(std::string fileName = "prog1.prog");