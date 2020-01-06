#pragma once
#include <string>
#include <vector>

namespace calc
{

enum class TokenType
{
	NUMBER,
	OPERATION,
	LEFT_BRACKET,
	RIGHT_BRACKET
};

struct Token
{
	std::string value;
	TokenType type;
};

std::vector<Token> tokenize(std::string);

}
