#include "token.h"
#include <sstream>
#include <cctype>
#include <stdexcept>

using namespace calc;
using namespace std;

namespace {
	bool isDigitSeparator(char c)
	{
		return c == '.' || c == ',';
	}

	bool isOperation(char c)
	{
		return c == '+' || c == '*' || c == '/';
	}

	Token tokenizeNumber(istream& is, char first)
	{
		string number(first == '+' ? "" : string({first}));
		while (isdigit(is.peek()) || isDigitSeparator(is.peek()))
		{
			number += is.get();
		}
		return { number, TokenType::NUMBER };
	}
} // anonymous

vector<Token> calc::tokenize(string str)
{
	vector<Token> result;

	istringstream iss(move(str));
	bool unary = true;
	char c;
	while (iss >> c)
	{
		if (isspace(c))
		{
			continue;
		}
		else if (c == '(')
		{
			result.push_back({ {c}, TokenType::LEFT_BRACKET });
			unary = true;
		}
		else if (c == ')')
		{
			result.push_back({ {c}, TokenType::RIGHT_BRACKET });
			unary = false;
		}
		else if (c == '-' || c == '+')
		{
			if (unary)
			{
				result.push_back(tokenizeNumber(iss, c));
				unary = false;
			}
			else
			{
				result.push_back({ {c}, TokenType::OPERATION });
				unary = true;
			}
		}
		else if (isdigit(c))
		{
			result.push_back(tokenizeNumber(iss, c));
			unary = false;
		}
		else if (c == '/' || c == '*')
		{
			result.push_back({ {c}, TokenType::OPERATION });
			unary = true;
		}
	}

	return result;
}
