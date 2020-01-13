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
	switch (type_)
	{
	case calc::OperatorType::Plus:
		return left_->eval() + right_->eval();
	case calc::OperatorType::Minus:
		return left_->eval() - right_->eval();
	case calc::OperatorType::Prod:
		return left_->eval() * right_->eval();
	case calc::OperatorType::Div:
		return left_->eval() / right_->eval();
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