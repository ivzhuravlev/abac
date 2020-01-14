#include "ast.h"
#include <stdexcept>

using namespace calc;
using namespace std;

OperatorNode::OperatorNode(OperatorType type,
						   std::shared_ptr<Node> left,
						   std::shared_ptr<Node> right) :
	type_(type), left_(left), right_(right)
{}

double OperatorNode::eval() const
{
	const double left = left_->eval();
	const double right = right_->eval();
	switch (type_)
	{
	case calc::OperatorType::Plus:
		return left + right;
	case calc::OperatorType::Minus:
		return left - right;
	case calc::OperatorType::Prod:
		return left * right;
	case calc::OperatorType::Div:
		if (right == 0)
		{
			throw std::logic_error("Zero division");
		}
		return left / right;
	default:
		throw std::logic_error("Unknown operator");
	}
}

OperandNode::OperandNode(double value) : value_(value)
{}

double OperandNode::eval() const
{
	return value_;
}