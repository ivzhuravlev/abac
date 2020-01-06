#include "catch.hpp"
#include "token.h"

using namespace calc;

TEST_CASE("Tokenize empty string", "[token]")
{
	auto vec = tokenize("");
	REQUIRE(vec.empty());
}

TEST_CASE("Tokenize spaces only", "[token]")
{
	auto vec = tokenize(" \t  ");
	REQUIRE(vec.empty());
}

TEST_CASE("Tokenize brackets only", "[token]")
{
	auto vec = tokenize(" ( ) ");
	REQUIRE(vec.size() == 2);
	REQUIRE(vec[0].value == "(");
	REQUIRE(vec[1].value == ")");
}

TEST_CASE("Tokenize negative number", "[token]")
{
	SECTION("Integer number") {
		auto vec = tokenize("-256");
		REQUIRE(vec.size() == 1);
		REQUIRE(vec[0].value == "-256");
	}
	SECTION("Real number with dot") {
		auto vec = tokenize("-256.25");
		REQUIRE(vec.size() == 1);
		REQUIRE(vec[0].value == "-256.25");
	}
	SECTION("Real number with comma") {
		auto vec = tokenize("-256,25");
		REQUIRE(vec.size() == 1);
		REQUIRE(vec[0].value == "-256,25");
	}
}

TEST_CASE("Tokenize positive numbers", "[token]")
{
	SECTION("Integer number") {
		auto vec = tokenize("256");
		REQUIRE(vec.size() == 1);
		REQUIRE(vec[0].value == "256");
	}
	SECTION("Real number with dot") {
		auto vec = tokenize("256.25");
		REQUIRE(vec.size() == 1);
		REQUIRE(vec[0].value == "256.25");
	}
	SECTION("Real number with comma") {
		auto vec = tokenize("256,25");
		REQUIRE(vec.size() == 1);
		REQUIRE(vec[0].value == "256,25");
	}
}

TEST_CASE("Tokenize expression", "[token]")
{
	SECTION("Expression: -1 + 5 - 3") {
		auto vec = tokenize("-1 + 5 - 3");
		REQUIRE(vec.size() == 5);
		REQUIRE(vec[0].value == "-1");
		REQUIRE(vec[1].value == "+");
		REQUIRE(vec[2].value == "5");
		REQUIRE(vec[3].value == "-");
		REQUIRE(vec[4].value == "3");
	}

	SECTION("Expression: -10 + (8 * 2.5) - (3/1,5)") {
		auto vec = tokenize("-10 + (8 * 2.5) - (3/1,5)");
		REQUIRE(vec.size() == 13);
		REQUIRE(vec[0].value == "-10");
		REQUIRE(vec[1].value == "+");
		REQUIRE(vec[2].value == "(");
		REQUIRE(vec[3].value == "8");
		REQUIRE(vec[4].value == "*");
		REQUIRE(vec[5].value == "2.5");
		REQUIRE(vec[6].value == ")");
		REQUIRE(vec[7].value == "-");
		REQUIRE(vec[8].value == "(");
		REQUIRE(vec[9].value == "3");
		REQUIRE(vec[10].value == "/");
		REQUIRE(vec[11].value == "1,5");
		REQUIRE(vec[12].value == ")");
	}
}