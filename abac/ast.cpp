#include "ast.h"
#include <stdexcept>
#include <stack>
#include <map>

using namespace calc;
using namespace std;

namespace {
	OperatorType createOperatorType(const string& type)
	{
		if (type == "+")
		{
			return OperatorType::Plus;
		}
		else if (type == "-")
		{
			return OperatorType::Minus;
		}
		else if (type == "*")
		{
			return OperatorType::Prod;
		}
		else if (type == "/")
		{
			return OperatorType::Div;
		}
		else
		{
			throw std::logic_error("Unknown operator");
		}
	}

	double createValue(const string& value)
	{
		return stod(value);
	}
}

const map<OperatorType, int> OperatorNode::precedence_ =
{
	{ OperatorType::Plus, 1 },
	{ OperatorType::Minus, 1 },
	{ OperatorType::Prod, 2 },
	{ OperatorType::Div, 2 }
};

OperatorNode::OperatorNode(OperatorType type,
						   std::shared_ptr<Node> left,
						   std::shared_ptr<Node> right) :
	type_(type), left_(left), right_(right)
{}

OperatorNode::OperatorNode(OperatorType type) :
	OperatorNode(type, nullptr, nullptr)
{}

int OperatorNode::precedence() const
{
	return precedence_.at(type_);
}

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

void OperatorNode::setLeftNode(std::shared_ptr<Node> node)
{
	left_ = move(node);
}

void OperatorNode::setRightNode(std::shared_ptr<Node> node)
{
	right_ = move(node);
}

OperandNode::OperandNode(double value) : value_(value)
{}

double OperandNode::eval() const
{
	return value_;
}

std::shared_ptr<Node> calc::buildAST(const std::vector<Token>& tokens)
{
	stack<shared_ptr<Node>> operands;
	stack<shared_ptr<OperatorNode>> operators;

	for (const Token& token : tokens)
	{
		switch (token.type)
		{
		case TokenType::NUMBER:
		{
			double value = createValue(token.value);
			operands.emplace(make_shared<OperandNode>(value));
			break;
		}
		case TokenType::OPERATION:
		{
			OperatorType opType = createOperatorType(token.value);
			auto opNode = make_shared<OperatorNode>(opType);
			while (!operators.empty() &&
					operators.top()->precedence() >= opNode->precedence())
			{
				if (operands.size() < 2)
				{
					throw std::logic_error("Incorrect exression");
				}
				auto op = move(operators.top());
				operators.pop();
				op->setRightNode(move(operands.top()));
				operands.pop();
				op->setLeftNode(move(operands.top()));
				operands.pop();

				operands.push(move(op));
			}
			operators.push(move(opNode));
			break;
		}
		default:
			break;
		}
	}

	while (!operators.empty())
	{
		if (operands.size() < 2)
		{
			throw std::logic_error("Incorrect exression");
		}
		auto op = move(operators.top());
		operators.pop();
		op->setRightNode(move(operands.top()));
		operands.pop();
		op->setLeftNode(move(operands.top()));
		operands.pop();

		operands.push(move(op));
	}

	if (operands.size() != 1)
	{
		throw std::logic_error("Incorrect exression");
	}

	return operands.top();
}