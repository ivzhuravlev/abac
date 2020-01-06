#ifndef RUN_TESTS
int main(int argc, char** argv)
{
	return 0;
}
#else
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#endif