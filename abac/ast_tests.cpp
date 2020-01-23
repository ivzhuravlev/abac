#include "catch.hpp"
#include "ast.h"

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
}