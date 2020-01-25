#ifndef RUN_TESTS
#include <iostream>

#include "token.h"
#include "ast.h"

using namespace std;

int main(int argc, char** argv)
{
	cout << "Enter arithmetic expression: " << endl;
	cout << ":> ";
	string expression;
	while (getline(cin, expression))
	{
		try
		{
			const auto tokens = calc::tokenize(expression);
			const auto ast = calc::buildAST(tokens);
			cout << ast->eval() << endl;
		}
		catch (logic_error & e)
		{
			cout << "Error: " << e.what() << endl;
		}
		cout << ":> ";
	}

	return 0;
}
#else
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif