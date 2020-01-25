#include "ast.h"
#include <stdexcept>
#include <stack>
#include <map>
#include <algorithm>

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

	double createValue(string value)
	{
		replace(begin(value), end(value), ',', '.');
		return stod(value);
	}

	void combineOperatorWithOperands(stack<shared_ptr<Node>>& operands,
		stack<shared_ptr<OperatorNode>>& operators)
	{
		if (operands.size() < 2)
		{
			throw std::logic_error("Incorrect expression");
		}
		auto op = move(operators.top());
		operators.pop();

		op->setRightNode(move(operands.top()));
		operands.pop();
		op->setLeftNode(move(operands.top()));
		operands.pop();

		operands.push(move(op));
	}
}

const map<OperatorType, int> OperatorNode::precedence_ =
{
	{ OperatorType::Plus, 1 },
	{ OperatorType::Minus, 1 },
	{ OperatorType::Prod, 2 },
	{ OperatorType::Div, 2 },
	{ OperatorType::LeftBracket, 0 },
	{ OperatorType::RightBracket, 0 }
};

OperatorNode::OperatorNode(OperatorType type,
						   std::shared_ptr<Node> left,
						   std::shared_ptr<Node> right) :
	type_(type), left_(left), right_(right)
{}

OperatorNode::OperatorNode(OperatorType type) :
	OperatorNode(type, nullptr, nullptr)
{}

void OperatorNode::setLeftNode(std::shared_ptr<Node> node)
{
	left_ = move(node);
}

void OperatorNode::setRightNode(std::shared_ptr<Node> node)
{
	right_ = move(node);
}

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
	case calc::OperatorType::LeftBracket:
		return 0;
	case calc::OperatorType::RightBracket:
		return 0;
	default:
		throw std::logic_error("Unknown operator");
	}
}

OperatorType OperatorNode::type() const
{
	return type_;
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
				combineOperatorWithOperands(operands, operators);
			}
			operators.push(move(opNode));
			break;
		}
		case TokenType::LEFT_BRACKET:
		{
			operators.emplace(make_shared<OperatorNode>(OperatorType::LeftBracket));
			break;
		}
		case TokenType::RIGHT_BRACKET:
		{
			if (operators.top()->type() == OperatorType::LeftBracket)
			{
				operators.pop();
				break;
			}

			while (!operators.empty() && operators.top()->type() != OperatorType::LeftBracket)
			{
				combineOperatorWithOperands(operands, operators);
			}

			if (operators.empty())
			{
				throw logic_error("Incorrect expression");
			}

			operators.pop();
		}

		default:
			break;
		}
	}

	while (!operators.empty())
	{
		combineOperatorWithOperands(operands, operators);
	}

	if (operands.size() != 1)
	{
		throw logic_error("Incorrect expression");
	}

	return operands.top();
}