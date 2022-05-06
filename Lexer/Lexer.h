#pragma once

#include <vector>
#include <memory>

#include "../Token/Token.h"


typedef std::vector<std::shared_ptr<Token>> tokenVect;

std::string operator*(std::string a, unsigned int b);
std::string& operator*=(std::string& a, unsigned int b);


int getTokens(tokenVect& Tokens, std::string fileName = "prog1.prog");