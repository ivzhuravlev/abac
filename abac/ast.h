#pragma once
#include <memory>
#include <map>

#include "token.h"

namespace calc
{

enum class OperatorType
{
	Plus,
	Minus,
	Prod,
	Div
};

class Node
{
public:
	virtual double eval() const = 0;
	~Node() = default;
};

class OperatorNode : public Node
{
public:
	OperatorNode(OperatorType);
	OperatorNode(OperatorType, std::shared_ptr<Node>, std::shared_ptr<Node>);
	void setLeftNode(std::shared_ptr<Node>);
	void setRightNode(std::shared_ptr<Node>);

	int precedence() const;
	double eval() const override;

private:
	static const std::map<OperatorType, int> precedence_;
	OperatorType type_;
	std::shared_ptr<Node> left_;
	std::shared_ptr<Node> right_;
};

class OperandNode : public Node
{
public:
	OperandNode() = default;
	explicit OperandNode(double value);
	double eval() const override;

private:
	double value_ = 0;
};

std::shared_ptr<Node> buildAST(const std::vector<Token>& tokens);

} // calc