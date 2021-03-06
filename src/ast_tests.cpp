#ifdef RUN_TESTS

#include "catch.hpp"
#include "ast.h"
#include <cstring>

using namespace calc;
using namespace std;

TEST_CASE("OperandNode", "[node]")
{
	REQUIRE(OperandNode().eval() == 0);
	OperandNode node(2.5);
	REQUIRE(node.eval() == 2.5);
}

TEST_CASE("OperatorNode", "[node]")
{
	shared_ptr<Node> left = make_shared<OperandNode>(2.5);
	shared_ptr<Node> right1 = make_shared<OperandNode>(2);
	shared_ptr<Node> right2 = make_shared<OperandNode>(2);

	shared_ptr<Node> sum = 
		make_shared<OperatorNode>(OperatorType::Plus, right1, right2);

	shared_ptr<Node> prod = 
		make_shared<OperatorNode>(OperatorType::Prod, left, sum);

	shared_ptr<Node> divZero = 
		make_shared<OperatorNode>(OperatorType::Div, prod, make_shared<OperandNode>());

	REQUIRE(prod->eval() == 10);

	REQUIRE_THROWS_AS(divZero->eval(), logic_error);
}

TEST_CASE("Build AST", "[ast]")
{
	SECTION("Simple")
	{
		auto tokens = tokenize("2 + 2 * 2");
		auto ast = buildAST(tokens);
		REQUIRE(ast->eval() == 6);
	}

	SECTION("Substruction")
	{
		auto tokens = tokenize("2 + 2 - 2");
		auto ast = buildAST(tokens);
		REQUIRE(ast->eval() == 2);
	}

	SECTION("Production")
	{
		auto tokens = tokenize("25 + 5 * 3 - 5 + 3");
		auto ast = buildAST(tokens);
		REQUIRE(ast->eval() == 38);
	}

	SECTION("Division")
	{
		auto tokens = tokenize("25 / 2.5 / 2,5");
		auto ast = buildAST(tokens);
		REQUIRE(ast->eval() == 4);
	}

	SECTION("Division zero")
	{
		auto tokens = tokenize("25 / 0");
		auto ast = buildAST(tokens);
		REQUIRE_THROWS_AS(ast->eval() == 10, logic_error);
	}
}

TEST_CASE("Build AST with brackets", "[ast]")
{
	SECTION("Brackets simple")
	{
		auto tokens = tokenize("(2 + 2) * 2");
		auto ast = buildAST(tokens);
		REQUIRE(ast->eval() == 8);
	}

	SECTION("Brackets harder")
	{
		auto tokens = tokenize("1 + (2 * (2.5 + 2.5 + (3 - 2))) - (3 / 1.5)");
		auto ast = buildAST(tokens);
		REQUIRE(ast->eval() == 11);
	}

	SECTION("Empty brackets")
	{
		auto tokens = tokenize("()");
		try {
			auto ast = buildAST(tokens);
		}
		catch (logic_error & e)
		{
			REQUIRE(std::strcmp(e.what(), "Incorrect expression") == 0);
		}
	}

	SECTION("Empty brackets inside")
	{
		auto tokens = tokenize("(2 + 2)() * (2)");
		auto ast = buildAST(tokens);
		REQUIRE(ast->eval() == 8);
	}

	SECTION("Wrong brackets")
	{
		auto tokensLeft = tokenize("2 + 2) * 2");
		try {
			auto ast = buildAST(tokensLeft);
		}
		catch (logic_error & e)
		{
			REQUIRE(std::strcmp(e.what(), "Incorrect expression") == 0);
		}

		auto tokensRight = tokenize("(2 + 2 * 2");
		try {
			auto ast = buildAST(tokensRight);
		}
		catch (logic_error & e)
		{
			REQUIRE(std::strcmp(e.what(), "Incorrect expression") == 0);
		}
	}
}

#endif // RUN_TESTS